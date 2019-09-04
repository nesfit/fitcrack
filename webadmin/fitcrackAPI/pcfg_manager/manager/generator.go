package manager

import (
	"github.com/sirupsen/logrus"
	"sync"
	"sync/atomic"
)

type Generator struct {
	Generated  uint64
	Pcfg       *Pcfg
	pQue       *PcfqQueue
	goRoutines int
	args       *InputArgs
}

func NewGenerator(pcfg *Pcfg) *Generator {
	que, err := NewPcfgQueue(pcfg)
	if err != nil {
		panic(err)
	}
	return &Generator{
		Pcfg: pcfg,
		pQue: que,
	}
}

func (g *Generator) worker(jobs <-chan *TreeItem) {
	for j := range jobs {
		guesses, _, _ := g.Pcfg.ListTerminals(j)
		atomic.AddUint64(&g.Generated, guesses)
	}

}

func (g *Generator) debugger() {

}

type PreTerminalItem struct {
	Item  *TreeItem
	Count uint64
}

func (g *Generator) RunForServer(args *InputArgs) <-chan PreTerminalItem {
	var ch chan PreTerminalItem
	if args.TerminalsQueSize > 0 {
		ch = make(chan PreTerminalItem, args.TerminalsQueSize)
	} else {
		ch = make(chan PreTerminalItem)
	}
	go func() {
		var err error
		var item *QueueItem
		for err != ErrPriorirtyQueEmpty {
			if args.MaxGuesses > 0 && atomic.LoadUint64(&g.Generated) >= args.MaxGuesses {
				break
			}
			item, err = g.pQue.Next()
			if err != nil {
				if err != ErrPriorirtyQueEmpty {
					logrus.Warn(err)
				}
				break
			}
			guessGeneration := NewGuessGeneration(g.Pcfg.Grammar, item.Tree)
			count := guessGeneration.Count()
			atomic.AddUint64(&g.Generated, count)
			ch <- PreTerminalItem{Item: item.Tree, Count: count}
		}
		close(ch)
	}()
	return ch
}

func (g *Generator) Run(args *InputArgs) error {
	g.args = args

	var err error
	var item *QueueItem
	jobs := make(chan *TreeItem, args.GoRoutines)
	wg := sync.WaitGroup{}
	wg.Add(int(args.GoRoutines))

	if args.Debug {
		wg.Add(1)
		go func() {
			g.debugger()
			wg.Done()
		}()
	}
	for w := uint(1); w <= args.GoRoutines; w++ {
		go func() {
			g.worker(jobs)
			wg.Done()
		}()
	}
	for err != ErrPriorirtyQueEmpty {
		if args.MaxGuesses > 0 && atomic.LoadUint64(&g.Generated) >= args.MaxGuesses {
			break
		}
		item, err = g.pQue.Next()
		if err != nil {
			if err == ErrPriorirtyQueEmpty {
				break
			}
			close(jobs)
			return err
		}
		//guesses, _, _ := g.Pcfg.ListTerminals(item.Tree)
		//g.Generated += guesses
		jobs <- item.Tree
	}
	close(jobs)
	wg.Wait()

	return nil
}
