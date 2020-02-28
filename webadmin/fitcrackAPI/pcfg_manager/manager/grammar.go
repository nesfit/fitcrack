package manager

import (
	"bufio"
	"encoding/json"
	"github.com/sirupsen/logrus"
	"gopkg.in/ini.v1"
	"math/big"
	"os"
	"regexp"
	"strconv"
	"strings"
	"unicode"
)

const (
	RulesFolder = ""
)

var (
	rgxPythArr = regexp.MustCompile(`"(.*?)"`)
)

type GrammarMapping map[string]map[string]int32

type Grammar struct {
	cfgFile     *ini.File
	RulesFolder string
	sectionList []string
	Sections    []*Section
	Mapping     GrammarMapping
}

type ConfigReplacement struct {
	TransitionId string `json:"Transition_id"`
	ConfigId     string `json:"Config_id"`
}

type Replacement struct {
	Probability float64
	IsTerminal  bool
	Values      []string
	Function    string
	Pos         []int32
}

type InputBase struct {
	Base        string
	Probability float64
}

type Section struct {
	Type         string
	Name         string
	Replacements []*Replacement
}

func LoadGrammar(rulesFolder string) (*Grammar, error) {
	grammar := &Grammar{
		RulesFolder: rulesFolder,
	}
	if err := grammar.Parse(); err != nil {
		return nil, err
	}
	return grammar, nil
}

func (g *Grammar) Parse() error {
	var err error
	g.sectionList = []string{}
	g.cfgFile, err = ini.Load(g.RulesFolder + "/" + "config.ini")
	if err != nil {
		return err
	}
	g.Mapping = make(GrammarMapping)
	logrus.Info("Config file loaded")
	if err := g.Build("START"); err != nil {
		return err
	}
	return nil
}

func (g *Grammar) Count() *big.Int {
	nonTerminalsCount := make(map[string]int64)
	capitalizationCount := make(map[string]int64)
	var startSection *Section
	for _, s := range g.Sections {
		if strings.HasPrefix(s.Type, "CAPITALIZATION") {
			key := strings.TrimPrefix(s.Type, "CAPITALIZATION") + s.Name
			capitalizationCount[key] += int64(len(s.Replacements))
		} else if strings.HasPrefix(s.Type, "BASE_") {
			key := strings.TrimPrefix(s.Type, "BASE_")
			if s.Name != "markov_prob" {
				key += s.Name
			}
			for _, r := range s.Replacements {
				nonTerminalsCount[key] += int64(len(r.Values))
			}
		} else if s.Type == "START" {
			startSection = s
		}
	}
	result := big.NewInt(0)
	for _, r := range startSection.Replacements {
		count := big.NewInt(1)
		nonTerminals := splitBaseToNonTerminals(r.Values[0])
		for _, nonTerminal := range nonTerminals {
			if nonTerminal[0] != 'M' {
				val, ok := capitalizationCount[nonTerminal[1:]]
				if ok {
					count.Mul(count, big.NewInt(val))
				}
			}
			count.Mul(count, big.NewInt(nonTerminalsCount[nonTerminal]))
		}
		result.Add(result, count)
	}
	return result
}
func (g *Grammar) Build(section string) error {
	for _, s := range g.sectionList {
		if s == section {
			return nil
		}
	}
	g.sectionList = append(g.sectionList, section)

	isTerminal, err := g.cfgFile.Section(section).Key("is_terminal").Bool()
	if err != nil {
		return err
	}
	if !isTerminal {
		var replacements []ConfigReplacement
		b := []byte(g.cfgFile.Section(section).Key("replacements").String())
		if err := json.Unmarshal(b, &replacements); err != nil {
			return err
		}
		for _, replace := range replacements {
			if err := g.Build(replace.ConfigId); err != nil {
				return err
			}
		}
		if err := g.FindGrammarMapping(section); err != nil {
			return err
		}
		err = g.InsertTerminal(section)

	} else {
		err = g.InsertTerminal(section)
	}
	if err != nil {
		return err
	}
	return nil
}
func (g *Grammar) GetGrammarPos(id, length string) (int32, error) {
	if val1, ok := g.Mapping[id]; ok {
		if val2, ok := val1[length]; ok {
			return val2, nil
		} else {
			return 0, ErrGrammarMapping
		}
	} else {
		return 0, ErrGrammarMapping
	}
}
func (g *Grammar) ParseBaseStructure(base string) ([]int32, error) {
	var pos []int32
	if base == "M" {
		val, err := g.GetGrammarPos("M", "markov_prob")
		if err != nil {
			return pos, err
		}
		return []int32{val}, nil
	}

	elems := splitBaseToNonTerminals(base)
	for _, e := range elems {
		value := e[:1]
		size := e[1:]
		val, err := g.GetGrammarPos(value, size)
		if err != nil {
			return pos, err
		}
		pos = append(pos, val)
	}
	return pos, nil
}

func splitBaseToNonTerminals(base string) []string {
	// base => elems
	// A111B22C1 => [A111, B22, C1]
	var elems []string
	l := 0
	for s := base; s != ""; s = s[l:] {
		l = strings.IndexFunc(s[1:], unicode.IsUpper) + 1
		if l <= 0 {
			l = len(s)
		}
		elems = append(elems, s[:l])
	}
	return elems
}

func fromPythonArray(in string) []string {
	var ret []string
	find := rgxPythArr.FindAllStringSubmatch(in, -1)
	for _, f := range find {
		if len(f) > 1 {
			ret = append(ret, f[1])
		}
	}
	return ret
}

func extractProbability(fileName string) ([]*InputBase, error) {
	file, err := os.Open(fileName)
	if err != nil {
		return []*InputBase{}, err
	}
	defer file.Close()

	scanner := bufio.NewScanner(file)
	var res []*InputBase
	for scanner.Scan() {
		splitted := strings.Split(scanner.Text(), "\t")
		if len(splitted) != 2 {
			return res, ErrExtractProbability
		}
		prob, err := strconv.ParseFloat(splitted[1], 64)
		if err != nil {
			return res, err
		}
		res = append(res, &InputBase{
			Base:        splitted[0],
			Probability: prob,
		})
	}

	if err := scanner.Err(); err != nil {
		return []*InputBase{}, err
	}
	return res, nil
}

func (g *Grammar) FindGrammarMapping(section string) error {
	var replacements []ConfigReplacement
	b := []byte(g.cfgFile.Section(section).Key("replacements").String())
	if err := json.Unmarshal(b, &replacements); err != nil {
		return err
	}
	for _, replace := range replacements {
		g.Mapping[replace.TransitionId] = make(map[string]int32)
		for i, sec := range g.Sections {
			if replace.ConfigId == sec.Type {
				g.Mapping[replace.TransitionId][sec.Name] = int32(i)
			}
		}
	}
	return nil
}
func (g *Grammar) InsertTerminal(section string) error {
	function := g.cfgFile.Section(section).Key("function").String()
	isTerminal, err := g.cfgFile.Section(section).Key("is_terminal").Bool()
	if err != nil {
		return err
	}
	filenames := fromPythonArray(g.cfgFile.Section(section).Key("filenames").String())
	directory := g.RulesFolder + "/" + g.cfgFile.Section(section).Key("directory").String()

	for _, curFile := range filenames {
		filePath := directory + "/" + curFile
		probabilities, err := extractProbability(filePath)
		if err != nil {
			return err
		}
		curSection := &Section{
			Name: strings.TrimSuffix(curFile, ".txt"),
			Type: section,
		}
		var replacementPos int32
		if function == "Capitalization" || function == "Copy" || function == "Shadow" || function == "Markov" {
			curReplacement := &Replacement{
				Function:    function,
				IsTerminal:  isTerminal,
				Probability: probabilities[0].Probability,
				Values:      []string{probabilities[0].Base},
			}
			if function == "Shadow" {
				var replacements []ConfigReplacement
				b := []byte(g.cfgFile.Section(section).Key("replacements").String())
				if err := json.Unmarshal(b, &replacements); err != nil {
					return err
				}
				val, err := g.GetGrammarPos(replacements[0].TransitionId, curSection.Name)
				if err != nil {
					return err
				}
				replacementPos = val
				curReplacement.Pos = []int32{replacementPos}
			}
			lastProb := probabilities[0].Probability
			for i := 1; i < len(probabilities); i++ {

				if probabilities[i].Probability == lastProb {
					curReplacement.Values = append(curReplacement.Values, probabilities[i].Base)
				} else if probabilities[i].Probability < lastProb {
					curSection.Replacements = append(curSection.Replacements, curReplacement)
					lastProb = probabilities[i].Probability
					curReplacement = &Replacement{
						Function:    function,
						IsTerminal:  isTerminal,
						Probability: probabilities[i].Probability,
						Values:      []string{probabilities[i].Base},
					}

					if function == "Shadow" {
						curReplacement.Pos = []int32{replacementPos}
					}
				} else {
					return ErrOrderList
				}
			}
			curSection.Replacements = append(curSection.Replacements, curReplacement)
			g.Sections = append(g.Sections, curSection)

		} else if function == "Transparent" {
			for i := range probabilities {
				curReplacement := &Replacement{
					Function:    function,
					IsTerminal:  isTerminal,
					Probability: probabilities[i].Probability,
					Values:      []string{probabilities[i].Base},
				}
				pos, err := g.ParseBaseStructure(probabilities[i].Base)
				if err != nil {
					return err
				}
				curReplacement.Pos = pos
				curSection.Replacements = append(curSection.Replacements, curReplacement)
			}
			g.Sections = append(g.Sections, curSection)
		} else {
			return ErrInvaldiGrammarType
		}
	}
	return nil
}
