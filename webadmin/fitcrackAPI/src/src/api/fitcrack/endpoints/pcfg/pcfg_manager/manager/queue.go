package manager

import (
	"container/heap"
	"errors"
	"fmt"
	log "github.com/sirupsen/logrus"
)

type QueueItem struct {
	IsTerminal  bool
	Probability float64
	Tree        *TreeItem
}

func debugItem(item *TreeItem) string {
	if item == nil {
		return "[]"
	}
	res := fmt.Sprintf("[%d, %d, ", item.Index, item.Transition)
	for _, child := range item.Childrens {
		res += debugItem(child)
	}
	return res + "]"
}
func (q *QueueItem) Print() {
	fmt.Println("isTerminal = ", q.IsTerminal)
	fmt.Printf("Probability = %f\n", q.Probability)
	fmt.Printf("ParseTree = %s\n", debugItem(q.Tree))
}

type PriorityQueue []*QueueItem

func (pq PriorityQueue) Len() int {
	return len(pq)
}

func (pq PriorityQueue) Less(i, j int) bool {
	return pq[i].Probability > pq[j].Probability
}

func (pq *PriorityQueue) Pop() interface{} {
	old := *pq
	n := len(old)
	item := old[n-1]
	*pq = old[0 : n-1]
	return item
}

func (pq *PriorityQueue) Push(x interface{}) {
	item := x.(*QueueItem)
	*pq = append(*pq, item)
}

func (pq PriorityQueue) Swap(i, j int) {
	pq[i], pq[j] = pq[j], pq[i]
}

type PcfqQueue struct {
	pQue    PriorityQueue
	pcfg    *Pcfg
	minProb float64
	maxProb float64
}

func NewPcfgQueue(pcfg *Pcfg) (*PcfqQueue, error) {
	q := &PcfqQueue{
		pcfg:    pcfg,
		minProb: 0.0,
		maxProb: 1.0,
	}
	heap.Init(&q.pQue)
	index := q.pcfg.StartIndex()
	if index == -1 {
		return nil, errors.New("could not find starting position for the Pcfg")
	}
	tree := &TreeItem{
		Index:      index,
		Transition: 0,
	}
	qItem := &QueueItem{
		IsTerminal:  false,
		Probability: pcfg.FindProbability(tree),
		Tree:        tree,
	}
	heap.Push(&q.pQue, qItem)
	return q, nil
}

func (q *PcfqQueue) Next() (*QueueItem, error) {
	for {
		for q.pQue.Len() == 0 {
			// Rebuild
			return nil, ErrPriorirtyQueEmpty
		}
		item := heap.Pop(&q.pQue).(*QueueItem)
		q.maxProb = item.Probability

		q.AddChildrenToQueue(item)

		if item.IsTerminal {
			return item, nil
		}
	}
}

func (q *PcfqQueue) AddChildrenToQueue(item *QueueItem) {

	childrenList := q.pcfg.DeadbeatDad(item.Tree)
	for _, child := range childrenList {
		childNode := &QueueItem{
			IsTerminal:  q.pcfg.FindIsTerminal(child),
			Probability: q.pcfg.FindProbability(child),
			Tree:        child,
		}
		if childNode.Probability <= item.Probability {
			heap.Push(&q.pQue, childNode)
		} else {
			log.Warnf("trying to push a parent and not a child on the list")
		}
	}
}
