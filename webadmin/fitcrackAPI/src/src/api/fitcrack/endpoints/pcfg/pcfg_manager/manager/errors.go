package manager

import "errors"

var (
	ErrExtractProbability   = errors.New("failed to extract probability")
	ErrOrderList            = errors.New("the list isn't in descending probability order")
	ErrInvaldiGrammarType   = errors.New("invalid Grammar type")
	ErrGrammarMapping       = errors.New("invalid keys to Grammar mapping")
	ErrParsingBaseStructure = errors.New("errors while parsing base structure")
	ErrPriorirtyQueEmpty    = errors.New("priority queue is empty")
)
