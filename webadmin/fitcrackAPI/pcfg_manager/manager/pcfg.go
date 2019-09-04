package manager

import (
	"bufio"
	"fmt"
	"io"
	"os"
	"strings"
)

type Pcfg struct {
	Grammar *Grammar
}

func NewPcfg(g *Grammar) *Pcfg {
	return &Pcfg{
		Grammar: g,
	}
}

func (p *Pcfg) StartIndex() int32 {
	if len(p.Grammar.Sections) == 0 {
		return -1
	}
	if p.Grammar.Sections[len(p.Grammar.Sections)-1].Type == "START" {
		return int32(len(p.Grammar.Sections) - 1)
	}
	for i := range p.Grammar.Sections {
		if p.Grammar.Sections[i].Type == "START" {
			return int32(i)
		}
	}
	return -1
}

func (p *Pcfg) ListTerminals(preTerminal *TreeItem) (uint64, string, string) {
	guessGeneration := NewGuessGeneration(p.Grammar, preTerminal)
	guess := guessGeneration.First()
	guesses := uint64(0)
	first := guess
	last := guess
	buf := bufio.NewWriter(os.Stdout)
	for guess != "" {
		//fmt.Println(guess)
		fmt.Fprintln(buf, guess)
		guess = guessGeneration.Next()
		last = guess
		guesses++
	}
	buf.Flush()
	//fmt.Println(guesses)
	return guesses, first, last
}

func (p *Pcfg) ListTerminalsToWriter(preTerminal *TreeItem, w io.Writer) error {
	guessGeneration := NewGuessGeneration(p.Grammar, preTerminal)
	guess := guessGeneration.First()
	buf := bufio.NewWriter(w)
	for guess != "" {
		if _, err := fmt.Fprintln(buf, guess); err != nil {
			return err
		}
		guess = guessGeneration.Next()
	}
	return buf.Flush()
}

func (p *Pcfg) ListTerminalsToSlice(preTerminal *TreeItem, capacity uint64) []string {
	guessGeneration := NewGuessGeneration(p.Grammar, preTerminal)
	guess := guessGeneration.First()
	var guesses []string
	if capacity > 0 {
		guesses = make([]string, 0, capacity)
	}
	for guess != "" {
		guesses = append(guesses, guess)
		guess = guessGeneration.Next()
	}
	return guesses
}

func PrintChildren(chs []*TreeItem, depth int) {
	if len(chs) < 1 {
		return
	}
	for _, ch := range chs {
		fmt.Fprintln(os.Stderr, strings.Repeat("\t", depth), *ch)
		PrintChildren(ch.Childrens, depth+1)
	}

}
func (p *Pcfg) FindProbability(tree *TreeItem) float64 {
	if len(p.Grammar.Sections) <= int(tree.Index) || len(p.Grammar.Sections[tree.Index].Replacements) <= int(tree.Transition) {
		panic("wrong indexing in find probability")
	}
	prob := p.Grammar.Sections[tree.Index].Replacements[tree.Transition].Probability
	if len(tree.Childrens) > 0 {
		for _, children := range tree.Childrens {
			childProb := p.FindProbability(children)
			prob *= childProb
		}
	}
	return prob
}

func (p *Pcfg) FindIsTerminal(tree *TreeItem) bool {
	if len(tree.Childrens) == 0 {
		if !p.Grammar.Sections[tree.Index].Replacements[tree.Transition].IsTerminal {
			return false
		}
	} else {
		for _, children := range tree.Childrens {
			if !p.FindIsTerminal(children) {
				return false
			}
		}
	}
	return true
}

func (p *Pcfg) DeadbeatDad(tree *TreeItem) []*TreeItem {
	childrenList := make([]*TreeItem, 0, 4)
	childrenList = p.DDFindChildren(tree, tree, childrenList)
	return childrenList
}

func (p *Pcfg) DDFindChildren(node, parent *TreeItem, childrenList []*TreeItem) []*TreeItem {
	if len(node.Childrens) == 0 {
		numReplacements := len(p.Grammar.Sections[node.Index].Replacements)
		// Takes care of the incrementing if there are children for the current node. Aka(1,2,[]) => (1,3,[])
		if numReplacements > int(node.Transition)+1 {
			// Make this a child node
			node.Transition++
			// An id to identify the calling node as the parent
			node.Id = true
			if p.DDIsMyParent(parent, false) {
				node.Id = false
				childrenList = append(childrenList, parent.Copy())
			} else {
				node.Id = false
			}
			// Replace the parent's value
			node.Transition--
		}
		if !p.Grammar.Sections[node.Index].Replacements[0].IsTerminal {
			var newExpansion []*TreeItem
			for _, pos := range p.Grammar.Sections[node.Index].Replacements[node.Transition].Pos {
				newExpansion = append(newExpansion, &TreeItem{
					Index:      pos,
					Transition: 0,
				})
			}
			// Make this a child node
			node.Childrens = newExpansion
			node.Id = true
			if p.DDIsMyParent(parent, true) {
				node.Id = false
				childrenList = append(childrenList, parent.Copy())
			} else {
				node.Id = false
			}
			// Replace the parent's value
			node.Childrens = []*TreeItem{}
		}
	} else {
		for _, children := range node.Childrens {
			childrenList = p.DDFindChildren(children, parent, childrenList)
		}
	}
	return childrenList
}

func (p *Pcfg) DDIsMyParent(child *TreeItem, isExpansion bool) bool {
	var curNode *TreeItem
	curParseTree := []*TreeItem{child}
	minDiff := 2.0
	foundOrigParent := false
	emptyListParent := false
	for len(curParseTree) > 0 {
		// Pop
		curNode, curParseTree = curParseTree[len(curParseTree)-1], curParseTree[:len(curParseTree)-1]
		if len(curNode.Childrens) == 0 {
			if curNode.Transition != 0 {
				parentProbDiff := p.Grammar.Sections[curNode.Index].Replacements[curNode.Transition-1].Probability -
					p.Grammar.Sections[curNode.Index].Replacements[curNode.Transition].Probability
				if parentProbDiff < minDiff {
					if curNode.Id && !isExpansion {
						foundOrigParent = true
					} else if foundOrigParent {
						return false
					}
					minDiff = parentProbDiff
				} else if curNode.Id && !isExpansion {
					return false
				}
			}
		} else {
			emptyListParent = true
			for _, children := range curNode.Childrens {
				if children.Transition != 0 || len(children.Childrens) != 0 {
					emptyListParent = false
					curParseTree = append(curParseTree, children)
				}
			}
			if emptyListParent {
				newExpansionProb := 1.0
				for _, children := range curNode.Childrens {
					newExpansionProb *= p.Grammar.Sections[children.Index].Replacements[0].Probability
				}
				parentProbDiff := 1.0 - newExpansionProb
				if parentProbDiff < minDiff {
					if curNode.Id && isExpansion {
						foundOrigParent = true
					} else if foundOrigParent {
						return false
					}
					minDiff = parentProbDiff
				} else if curNode.Id && isExpansion {
					return false
				}
			}
		}
	}
	return true
}
