package server

import (
	"bufio"
	"container/list"
	"context"
	"errors"
	"fmt"
	"github.com/dasio/pcfg-manager/manager"
	pb "github.com/dasio/pcfg-manager/proto"
	"github.com/sirupsen/logrus"
	"google.golang.org/grpc"
	"google.golang.org/grpc/peer"
	"net"
	"os"
	"sync/atomic"
	"time"
	"unicode"
)

type Service struct {
	mng                *manager.Manager
	args               manager.InputArgs
	remainingHashes    map[string]struct{}
	completedHashes    map[string]string
	generatorCh        <-chan manager.PreTerminalItem
	returnedChunks     *list.List
	clients            map[string]ClientInfo
	chunkId            uint32
	endCracking        chan bool
	processedTerminals uint64
	timeGeneration     time.Duration
	bandwidth          uint64
	start              time.Time
}

type Chunk struct {
	Id             uint32
	PreTerminals   []*pb.TreeItem
	Terminals      []string
	TerminalsCount uint64
	TimeGeneration time.Duration
}

func NewService() *Service {
	return &Service{
		clients: make(map[string]ClientInfo),
		chunkId: 0,
	}
}

type ClientInfo struct {
	Addr              string
	ActualChunk       Chunk
	StartTime         time.Time
	EndTime           time.Time
	PreviousTerminals uint64
	Total             uint64
	Speed             float64
}

func (s *Service) DebugClients() {
	totalSpeed := 0.0
	for _, client := range s.clients {
		totalSpeed += client.Speed
		fmt.Printf("[%s] chunk: %d total: %d, speed: %f\n",
			client.Addr, client.ActualChunk.Id, client.Total, client.Speed)
	}
	fmt.Printf("Generated: %d, processed: %d, totalSpeed: %f, generationTime: %s\n",
		s.mng.Generator.Generated, s.processedTerminals, totalSpeed, s.timeGeneration)
}

func (s *Service) SaveStats() error {
	f, err := os.Create("stats-server-" + time.Now().Format(time.RFC3339) + ".txt")
	if err != nil {
		return err
	}
	_, err = f.WriteString(fmt.Sprintf("Time: %s\n", time.Now().Sub(s.start)))
	if err != nil {
		return err
	}
	_, err = f.WriteString(fmt.Sprintf("Bandwidth: %d\n", s.bandwidth))
	if err != nil {
		return err
	}
	_, err = f.WriteString(fmt.Sprintf("Terminals: %d\n", s.processedTerminals))
	if err != nil {
		return err
	}
	_, err = f.WriteString(fmt.Sprintf("Timegen: %s\n", s.timeGeneration))
	if err != nil {
		return err
	}
	_, err = f.WriteString(fmt.Sprintf("Clients: %d\n", len(s.clients)))
	if err != nil {
		return err
	}
	for _, client := range s.clients {
		_, err = f.WriteString(fmt.Sprintf("\t [%s] total: %d, speed: %f\n", client.Addr, client.Total, client.Speed))
		if err != nil {
			return err
		}
	}
	return f.Close()
}
func (s *Service) Load(args manager.InputArgs) error {
	s.args = args
	s.remainingHashes = make(map[string]struct{})
	s.completedHashes = make(map[string]string)
	s.endCracking = make(chan bool)
	s.returnedChunks = list.New()
	if s.args.HashFile != "" {
		lines, err := readLines(s.args.HashFile)
		if err != nil {
			return err
		}
		for _, l := range lines {
			if len(l) > 0 && !unicode.IsSpace(rune(l[0])) {
				s.remainingHashes[l] = struct{}{}
			}
		}
	}

	s.mng = manager.NewManager(s.args.RulesFolder)
	if err := s.mng.Load(); err != nil {
		return err
	}
	s.generatorCh = s.mng.Generator.RunForServer(&args)
	return nil
}
func (s *Service) Run() error {
	lis, err := net.Listen("tcp", ":"+s.args.Port)
	if err != nil {
		return err
	}
	server := grpc.NewServer()
	pb.RegisterPCFGServer(server, s)
	logrus.Infof("Listening on port %s", s.args.Port)
	go func() {
		<-s.endCracking
		server.GracefulStop()
	}()
	if err := server.Serve(lis); err != nil {
		return err
	}
	for hash, pass := range s.completedHashes {
		fmt.Println(hash, " ", pass)
	}
	return nil
}

func (s *Service) Connect(ctx context.Context, req *pb.Empty) (*pb.ConnectResponse, error) {
	p, ok := peer.FromContext(ctx)
	if !ok {
		return &pb.ConnectResponse{}, errors.New("no peer")
	}
	client := ClientInfo{
		Addr: p.Addr.String(),
	}
	s.clients[client.Addr] = client
	var hashList []string
	for k := range s.remainingHashes {
		hashList = append(hashList, k)
	}
	if s.start.IsZero() {
		s.start = time.Now()
	}
	logrus.Infof("client %s connected", client.Addr)
	return &pb.ConnectResponse{
		Grammar:     manager.GrammarToProto(s.mng.Generator.Pcfg.Grammar),
		HashList:    hashList,
		HashcatMode: s.args.HashcatMode,
	}, nil
}

func (s *Service) Disconnect(ctx context.Context, req *pb.Empty) (*pb.Empty, error) {
	p, ok := peer.FromContext(ctx)
	if !ok {
		return &pb.Empty{}, errors.New("no peer")
	}
	clientInfo, ok := s.clients[p.Addr.String()]
	if !ok {
		return &pb.Empty{}, errors.New("client wasn't connected")
	}
	if clientInfo.ActualChunk.Id != 0 {
		logrus.Infof("client %s did not finished chunk[%d], sending %d preterminals back to channel",
			clientInfo.Addr, clientInfo.ActualChunk.Id, len(clientInfo.ActualChunk.PreTerminals))
		s.returnedChunks.PushBack(&clientInfo.ActualChunk)
	}
	//delete(s.clients, p.Addr.String())
	logrus.Infof("client %s disconnected", p.Addr.String())

	return &pb.Empty{}, nil
}

func (s *Service) GetNextChunk(size uint64) (Chunk, bool) {
	total := uint64(0)
	endGen := false
	for e := s.returnedChunks.Front(); e != nil; e = e.Next() {
		// do something with e.Value
		chunk := e.Value.(*Chunk)
		if chunk.TerminalsCount < uint64(float64(size)*1.1) {
			s.returnedChunks.Remove(e)
			return *chunk, false
		}
	}
	startTime := time.Now()
	var chunkItems []manager.PreTerminalItem
loop:
	for total < size {
		select {
		case it := <-s.generatorCh:
			if it.Item == nil {
				endGen = true
				break loop
			}
			total += it.Count
			chunkItems = append(chunkItems, it)
		case <-time.After(time.Second * 2):
			break loop
		}
	}
	var preTerminals []*pb.TreeItem
	var guesses []string
	if !s.args.GenerateTerminals {
		preTerminals = make([]*pb.TreeItem, 0, len(chunkItems))
		for _, ch := range chunkItems {
			preTerminals = append(preTerminals, manager.TreeItemToProto(ch.Item))
		}
	} else {
		guesses = make([]string, 0, total)
		for _, ch := range chunkItems {
			guesses = append(guesses, s.mng.Generator.Pcfg.ListTerminalsToSlice(ch.Item, ch.Count)...)
		}
	}
	timeGen := time.Now().Sub(startTime)
	s.timeGeneration += timeGen
	return Chunk{
		Id:             atomic.AddUint32(&s.chunkId, 1),
		PreTerminals:   preTerminals,
		Terminals:      guesses,
		TerminalsCount: total,
		TimeGeneration: timeGen,
	}, endGen
}

func (s *Service) GetNextItems(ctx context.Context, req *pb.NextRequest) (*pb.Items, error) {
	p, ok := peer.FromContext(ctx)
	if !ok {
		return &pb.Items{}, errors.New("no peer")
	}
	then := time.Now()
	clientInfo, ok := s.clients[p.Addr.String()]
	if !ok {
		return nil, errors.New("client is not connected")
	}
	chunkSize := s.args.ChunkStartSize
	if req.Terminals != 0 {
		chunkSize = req.Terminals
	} else {
		if !clientInfo.EndTime.IsZero() && clientInfo.PreviousTerminals != 0 {
			clientInfo.Speed = float64(clientInfo.PreviousTerminals) / clientInfo.EndTime.Sub(clientInfo.StartTime).Seconds()
			chunkSize = uint64(clientInfo.Speed * s.args.ChunkDuration.Seconds())
		}
	}
	chunk, endGen := s.GetNextChunk(chunkSize)
	if endGen && chunk.TerminalsCount == 0 {
		return &pb.Items{}, nil
	}
	clientInfo.ActualChunk = chunk
	clientInfo.StartTime = time.Now()
	s.clients[p.Addr.String()] = clientInfo
	items := &pb.Items{
		PreTerminals:   chunk.PreTerminals,
		Terminals:      chunk.Terminals,
		TerminalsCount: chunk.TerminalsCount,
	}
	msgSize := items.XXX_Size()
	s.bandwidth += uint64(msgSize)
	logrus.Infof("sending chunk[%d], preTerminals: %d, terminals: %d to %s in %s, size: %d",
		chunk.Id, len(chunk.PreTerminals), chunk.TerminalsCount, clientInfo.Addr, time.Now().Sub(then).String(), msgSize)

	return items, nil
}

func (s *Service) SendResult(ctx context.Context, in *pb.CrackingResponse) (*pb.ResultResponse, error) {
	p, ok := peer.FromContext(ctx)
	if !ok {
		return &pb.ResultResponse{End: false}, errors.New("no peer")
	}
	clientInfo, ok := s.clients[p.Addr.String()]
	if !ok {
		return nil, errors.New("client is not connected")
	}
	for hash, password := range in.Hashes {
		delete(s.remainingHashes, hash)
		s.completedHashes[hash] = password
	}
	clientInfo.EndTime = time.Now()
	s.processedTerminals += clientInfo.ActualChunk.TerminalsCount
	clientInfo.Total += clientInfo.ActualChunk.TerminalsCount
	clientInfo.PreviousTerminals = clientInfo.ActualChunk.TerminalsCount
	clientInfo.ActualChunk = Chunk{}
	s.clients[p.Addr.String()] = clientInfo

	logrus.Infof("result from %s: %d in %f seconds", clientInfo.Addr, len(in.Hashes), clientInfo.EndTime.Sub(clientInfo.StartTime).Seconds())
	if (len(s.remainingHashes) == 0 && s.args.HashFile != "") || s.processedTerminals >= s.mng.Generator.Generated {
		s.endCracking <- true
		return &pb.ResultResponse{End: true}, nil
	}
	return &pb.ResultResponse{End: false}, nil
}

func readLines(path string) ([]string, error) {
	file, err := os.Open(path)
	if err != nil {
		return nil, err
	}
	defer file.Close()

	var lines []string
	scanner := bufio.NewScanner(file)
	for scanner.Scan() {
		lines = append(lines, scanner.Text())
	}
	return lines, scanner.Err()
}
