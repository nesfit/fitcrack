package manager

import (
	pb "github.com/dasio/pcfg-manager/proto"
	"github.com/golang/protobuf/proto"
	"io/ioutil"
)

type Manager struct {
	Generator   *Generator
	rulesFolder string
}

func NewManager(rulesFolder string) *Manager {
	return &Manager{
		rulesFolder: rulesFolder,
	}
}

func (m *Manager) Load() error {
	grammar, err := LoadGrammar(m.rulesFolder)
	if err != nil {
		return err
	}
	pcfg := NewPcfg(grammar)
	m.Generator = NewGenerator(pcfg)
	return nil
}

func (m *Manager) LoadFromFile(file string) error {
	b, err := ioutil.ReadFile(file)
	if err != nil {
		return err
	}
	var pbGrammar pb.Grammar
	if err := proto.Unmarshal(b, &pbGrammar); err != nil {
		return err
	}
	grammar := GrammarFromProto(&pbGrammar)
	pcfg := NewPcfg(grammar)
	m.Generator = NewGenerator(pcfg)
	return nil
}

func (m *Manager) LoadWithGrammar(g *Grammar) {
	m.Generator = NewGenerator(NewPcfg(g))
}

func (m *Manager) ListTerminals(preTerminalsFile string) error {
	b, err := ioutil.ReadFile(preTerminalsFile)
	if err != nil {
		return err
	}
	var pbItems pb.Items
	if err := proto.Unmarshal(b, &pbItems); err != nil {
		return err
	}
	for _, item := range pbItems.PreTerminals {
		treeItem := TreeItemFromProto(item)
		m.Generator.Pcfg.ListTerminals(treeItem)
	}
	return nil
}
func (m *Manager) Start(input *InputArgs) error {
	//log.Infoln("Rule: ", m.ruleName)
	//log.Infoln("GoRoutines: ", input.GoRoutines)
	//log.Infoln("MaxGuesses: ", input.MaxGuesses)
	//log.Infoln("Debug: ", input.Debug)

	if err := m.Generator.Run(input); err != nil {
		return err
	}
	return nil
}
