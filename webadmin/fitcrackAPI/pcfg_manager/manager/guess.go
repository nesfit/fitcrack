package manager

import (
	"strings"
	"unicode"
)

type GuessIndex struct {
	replacement  *Replacement
	function     string
	topIndex     int
	guessPointer int
	Reset        func([]string, bool) ([]string, bool)
	Next         func([]string, bool) ([]string, bool)
}

func NewGuessIndex(replacement *Replacement, endOfGuess int) *GuessIndex {
	gi := &GuessIndex{
		replacement:  replacement,
		guessPointer: endOfGuess,
	}
	gi.function = gi.replacement.Function
	if gi.function == "Copy" || gi.function == "Shadow" {
		gi.Reset = gi._resetCopyShadow
		gi.Next = gi._nextCopyShadow
	} else if gi.function == "Capitalization" {
		gi.Reset = gi._resetCapitalization
		gi.Next = gi._nextCapitalization
	} else {
		return nil
	}
	return gi
}
func (g *GuessIndex) _resetCopyShadow(guess []string, new bool) ([]string, bool) {
	g.topIndex = 0
	if len(g.replacement.Values) == 0 {
		return []string{}, false
	}
	value := g.replacement.Values[0]
	if new {
		guess = append(guess, value)
	} else {
		guess[g.guessPointer] = value
	}
	return guess, true

}

func (g *GuessIndex) _resetCapitalization(guess []string, new bool) ([]string, bool) {
	g.topIndex = 0
	if len(g.replacement.Values) == 0 {
		return []string{}, false
	}
	rule := g.replacement.Values[0]
	var tmpString strings.Builder
	baseWord := guess[g.guessPointer]
	tmpString.Grow(len(baseWord))
	lPos := 0
	for _, ch := range baseWord {
		if rule[lPos] == 'U' {
			tmpString.WriteRune(unicode.ToUpper(ch))
		} else {
			tmpString.WriteRune(unicode.ToLower(ch))
		}
		lPos++
	}
	guess[g.guessPointer] = tmpString.String()
	return guess, true

}

func (g *GuessIndex) _nextCopyShadow(guess []string, new bool) ([]string, bool) {
	g.topIndex++
	if g.topIndex >= len(g.replacement.Values) {
		return guess, false
	}

	guess[g.guessPointer] = g.replacement.Values[g.topIndex]
	return guess, true

}

func (g *GuessIndex) _nextCapitalization(guess []string, new bool) ([]string, bool) {
	g.topIndex++
	if g.topIndex >= len(g.replacement.Values) {
		return []string{}, false
	}
	rule := g.replacement.Values[g.topIndex]
	var tmpString strings.Builder
	baseWord := guess[g.guessPointer]
	tmpString.Grow(len(baseWord))

	lPos := 0
	for _, ch := range baseWord {
		if rule[lPos] == 'U' {
			tmpString.WriteRune(unicode.ToUpper(ch))
		} else {
			tmpString.WriteRune(unicode.ToLower(ch))
		}
		lPos++
	}
	guess[g.guessPointer] = tmpString.String()
	return guess, true

}

type GuessGeneration struct {
	grammar    *Grammar
	guess      []string
	structures []*GuessIndex
}

func NewGuessGeneration(grammar *Grammar, preTerminal *TreeItem) *GuessGeneration {
	g := &GuessGeneration{
		grammar: grammar,
	}
	g.Init(preTerminal, 0)
	return g
}

func (g *GuessGeneration) Init(section *TreeItem, endOfGuess int) {
	replacement := g.grammar.Sections[section.Index].Replacements[section.Transition]
	if replacement.Function == "Transparent" {
		for _, rule := range section.Childrens {
			g.Init(rule, endOfGuess)
			endOfGuess = g.structures[len(g.structures)-1].guessPointer + 1

		}
	} else {
		g.structures = append(g.structures, NewGuessIndex(replacement, endOfGuess))
		if replacement.Function == "Shadow" {
			g.Init(section.Childrens[0], endOfGuess)
		}
	}
}

func (g *GuessGeneration) Count() uint64 {
	if len(g.structures) == 0 {
		return 0
	}
	r := uint64(1)
	for _, s := range g.structures {
		r *= uint64(len(s.replacement.Values))
	}
	return r

}
func (g *GuessGeneration) First() string {
	for _, item := range g.structures {
		guess, ok := item.Reset(g.guess, true)
		if ok {
			g.guess = guess
		} else {
			return ""
		}
	}
	return strings.Join(g.guess, "")

}

func (g *GuessGeneration) Next() string {
	for i := len(g.structures) - 1; i >= 0; i-- {
		guess, ok := g.structures[i].Next(g.guess, false)
		if ok {
			g.guess = guess
			for forwardIndex := i + 1; forwardIndex < len(g.structures); forwardIndex++ {
				guess, ok = g.structures[forwardIndex].Reset(g.guess, false)
				if ok {
					g.guess = guess
				}
			}
			return strings.Join(g.guess, "")
		}
	}
	return ""
}
