package client

import (
	"bufio"
	"context"
	"errors"
	"fmt"
	"github.com/dasio/pcfg-manager/manager"
	pb "github.com/dasio/pcfg-manager/proto"
	"github.com/sirupsen/logrus"
	"google.golang.org/grpc"
	"io"
	"io/ioutil"
	"math"
	"math/rand"
	"os"
	"os/exec"
	"path/filepath"
	"runtime"
	"strings"
	"sync"
	"time"
)

type Service struct {
	c           pb.PCFGClient
	mng         *manager.Manager
	grpcConn    *grpc.ClientConn
	grammar     *manager.Grammar
	genOnly     bool
	hashFile    string
	hashcatPath string
	hashcatMode string
	hashcatPipe io.WriteCloser
	// tmp
	hashes          []string
	start           time.Time
	bandwidth       uint64
	terminalsCount  uint64
	genRoutines     uint
	waitForResponse time.Duration
	genTime         time.Duration
}

type InputArgs struct {
	ServerAddress string
	HashcatFolder string
	GenOnly       bool
	GenRoutines   uint
	SaveStats     bool
}

const (
	HsCodeGpuWatchdogAlarm    = -2
	HsCodeError               = -1
	HsCodeOk                  = 0
	HsCodeExhausted           = 1
	HsCodeAborted             = 2
	HsCodeAbortedByCheckpoint = 3
	HsCodeAbortedByRune       = 4
)

var (
	ErrFinished = errors.New("server finished cracking")
)

func NewService(inArgs InputArgs) (*Service, error) {
	var path string
	var err error
	if !inArgs.GenOnly {
		path, err = filepath.Abs(inArgs.HashcatFolder + "/" + getHashcatBinary())
		if err != nil {
			return nil, err
		}
		if _, err := os.Stat(path); os.IsNotExist(err) {
			return nil, err
		}
	}
	svc := &Service{
		hashcatPath: path,
		genOnly:     inArgs.GenOnly,
		genRoutines: inArgs.GenRoutines,
	}
	return svc, nil
}

func (s *Service) SaveStats() error {
	finish := time.Now()
	f, err := os.Create(fmt.Sprintf("stats-client[%s]-%s.txt", s.grpcConn.Target(), time.Now().Format(time.RFC3339)))
	if err != nil {
		return err
	}
	_, err = f.WriteString(fmt.Sprintf("Time: %s\n", finish.Sub(s.start)))
	if err != nil {
		return err
	}
	_, err = f.WriteString(fmt.Sprintf("Bandwidth: %d\n", s.bandwidth))
	if err != nil {
		return err
	}
	_, err = f.WriteString(fmt.Sprintf("Terminals: %d\n", s.terminalsCount))
	if err != nil {
		return err
	}
	speed := float64(s.terminalsCount) / float64(finish.Second())
	_, err = f.WriteString(fmt.Sprintf("Speed: %f\n", speed))
	if err != nil {
		return err
	}
	_, err = f.WriteString(fmt.Sprintf("Waiting for responses: %s\n", s.waitForResponse))
	if err != nil {
		return err
	}
	_, err = f.WriteString(fmt.Sprintf("Generation/Cracking time %s\n", s.genTime))
	if err != nil {
		return err
	}
	return f.Close()
}
func getHashcatBinary() string {
	var ext string
	if runtime.GOOS == "windows" {
		ext = "exe"
	} else {
		ext = "bin"
	}
	arch := "32"
	if strings.HasSuffix(runtime.GOARCH, "64") {
		arch = "64"
	}
	return fmt.Sprintf("hashcat%s.%s", arch, ext)
}

func (s *Service) Connect(address string) error {
	var err error
	s.grpcConn, err = grpc.Dial(address, grpc.WithInsecure())
	if err != nil {
		return err
	}
	s.c = pb.NewPCFGClient(s.grpcConn)
	ctx, cancel := context.WithTimeout(context.Background(), time.Second*10)
	defer cancel()
	r, err := s.c.Connect(ctx, &pb.Empty{}, grpc.MaxCallRecvMsgSize(math.MaxInt32))
	if err != nil {
		return err
	}
	s.grammar = manager.GrammarFromProto(r.Grammar)
	s.mng = manager.NewManager(s.grammar.RulesFolder)
	s.mng.LoadWithGrammar(s.grammar)
	f, err := ioutil.TempFile("", "pcfg-*.hash")
	if err != nil {
		return err
	}
	// tmp
	s.hashes = r.HashList
	s.hashcatMode = r.HashcatMode
	s.hashFile = f.Name()
	if _, err := f.Write([]byte(strings.Join(r.HashList, "\n"))); err != nil {
		return err
	}
	if err := f.Close(); err != nil {
		return err
	}
	s.start = time.Now()

	return nil
}

func (s *Service) startHashcat() (*exec.Cmd, error) {
	cmd := exec.Command(s.hashcatPath, "-m", s.hashcatMode, "-o", "results.txt", "--machine-readable", "--status", s.hashFile)
	cmd.Stdout = os.Stdout
	cmd.Stderr = os.Stderr
	pipe, err := cmd.StdinPipe()
	if err != nil {
		return nil, err
	}
	s.hashcatPipe = pipe
	if err := cmd.Start(); err != nil {
		return nil, err
	}
	return cmd, nil
}

func (s *Service) Run(done <-chan bool) error {
	for {
		select {
		case <-done:
			return nil
		default:
			ctx, cancel := context.WithTimeout(context.Background(), time.Minute*5)
			then := time.Now()
			res, err := s.c.GetNextItems(ctx, &pb.NextRequest{}, grpc.MaxCallRecvMsgSize(math.MaxInt32))
			if err != nil {
				cancel()
				_, _ = s.c.SendResult(ctx, &pb.CrackingResponse{})
				return err
			}
			s.waitForResponse += time.Now().Sub(then)
			logrus.Infof("received %d preTerminals and %d terminals", len(res.PreTerminals), len(res.Terminals))
			cancel()
			if len(res.PreTerminals) == 0 && len(res.Terminals) == 0 {
				_, err = s.c.SendResult(context.Background(), &pb.CrackingResponse{})
				if err != nil {
					return err
				}
				return nil
			}
			var results map[string]string
			then = time.Now()
			if s.genOnly {
				results, err = s.generateOnly(res)
			} else {
				results, err = s.startCracking(res)
			}
			if err != nil {
				return err
			}
			s.terminalsCount += res.TerminalsCount
			s.bandwidth += uint64(res.XXX_Size())
			s.genTime += time.Now().Sub(then)

			resultRes, err := s.c.SendResult(context.Background(), &pb.CrackingResponse{
				Hashes: results,
			})
			logrus.Infof("sending %d cracked hashes", len(results))

			if err != nil {
				return err
			}
			if resultRes.End {
				return ErrFinished
			}

		}
	}
}

func (s *Service) worker(jobs <-chan *pb.TreeItem) {
	for j := range jobs {
		treeItem := manager.TreeItemFromProto(j)
		err := s.mng.Generator.Pcfg.ListTerminalsToWriter(treeItem, os.Stdout)
		if err != nil {
			logrus.Warn(err)
		}
	}

}
func (s *Service) generateOnly(items *pb.Items) (map[string]string, error) {
	jobs := make(chan *pb.TreeItem, s.genRoutines)
	wg := sync.WaitGroup{}
	wg.Add(int(s.genRoutines))
	for w := 1; w <= int(s.genRoutines); w++ {
		go func() {
			s.worker(jobs)
			wg.Done()
		}()
	}
	for _, item := range items.PreTerminals {
		jobs <- item
	}
	close(jobs)
	wg.Wait()
	buf := bufio.NewWriter(os.Stdout)
	for _, t := range items.Terminals {
		if _, err := fmt.Fprintln(buf, t); err != nil {
			return nil, err
		}
	}
	if err := buf.Flush(); err != nil {
		return nil, err
	}

	return map[string]string{}, nil
}
func (s *Service) startCracking(items *pb.Items) (map[string]string, error) {
	cmd, err := s.startHashcat()
	if err != nil {
		return nil, err
	}
	for _, item := range items.PreTerminals {
		treeItem := manager.TreeItemFromProto(item)
		err := s.mng.Generator.Pcfg.ListTerminalsToWriter(treeItem, s.hashcatPipe)
		if err != nil {
			return nil, err
		}
	}
	buf := bufio.NewWriter(s.hashcatPipe)
	for _, t := range items.Terminals {
		if _, err := fmt.Fprintln(buf, t); err != nil {
			return nil, err
		}
	}
	if err := buf.Flush(); err != nil {
		return nil, err
	}

	if err := s.hashcatPipe.Close(); err != nil {
		return nil, err
	}
	if err := cmd.Wait(); err != nil {
		if exitErr, ok := err.(*exec.ExitError); ok {
			if exitErr.ExitCode() != HsCodeOk && exitErr.ExitCode() != HsCodeExhausted {
				return nil, err
			}
		}
	}
	results, err := getResults("results.txt")
	if err != nil {
		return nil, err
	}
	return results, nil
}

func getResults(path string) (map[string]string, error) {
	file, err := os.Open(path)
	if err != nil {
		if err == os.ErrNotExist {
			return map[string]string{}, nil
		}
		return nil, err
	}
	defer file.Close()

	res := make(map[string]string)
	scanner := bufio.NewScanner(file)
	for scanner.Scan() {
		split := strings.Split(scanner.Text(), ":")
		if len(split) != 2 {
			continue
		}
		res[split[0]] = split[1]
	}
	return res, scanner.Err()
}

func (s *Service) randomResult() map[string]string {
	if rand.Float32() > 0.8 {
		return map[string]string{
			s.hashes[rand.Int()%len(s.hashes)]: "PassWord123",
		}
	}
	return map[string]string{}
}

func (s *Service) Disconnect() error {
	if s.grpcConn == nil {
		return errors.New("no active grpc connection")
	}
	ctx, cancel := context.WithTimeout(context.Background(), time.Second*2)
	defer cancel()
	if _, err := s.c.Disconnect(ctx, &pb.Empty{}); err != nil {
		return err
	}
	_ = os.Remove(s.hashFile)
	if err := s.grpcConn.Close(); err != nil {
		return err
	}
	return nil
}
