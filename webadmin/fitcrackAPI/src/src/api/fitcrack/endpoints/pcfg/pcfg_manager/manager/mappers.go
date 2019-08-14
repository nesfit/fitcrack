package manager

import (
	"github.com/dasio/pcfg-manager/proto"
)

func GrammarToProto(g *Grammar) *proto.Grammar {
	return &proto.Grammar{
		RulesFolder: g.RulesFolder,
		Sections:    sectionsToProto(g.Sections),
		Mapping:     mappingToProto(g.Mapping),
	}
}

func GrammarFromProto(g *proto.Grammar) *Grammar {
	return &Grammar{
		RulesFolder: g.RulesFolder,
		Sections:    sectionsFromProto(g.Sections),
		Mapping:     mappingFromProto(g.Mapping),
	}
}

func TreeItemToProto(i *TreeItem) *proto.TreeItem {
	if i == nil {
		return nil
	}
	childrens := make([]*proto.TreeItem, 0, len(i.Childrens))
	for _, ch := range i.Childrens {
		childrens = append(childrens, TreeItemToProto(ch))
	}
	return &proto.TreeItem{
		Index:      i.Index,
		Transition: i.Transition,
		Id:         i.Id,
		Childrens:  childrens,
	}
}

func TreeItemFromProto(i *proto.TreeItem) *TreeItem {
	if i == nil {
		return nil
	}
	childrens := make([]*TreeItem, 0, len(i.Childrens))
	for _, ch := range i.Childrens {
		childrens = append(childrens, TreeItemFromProto(ch))
	}
	return &TreeItem{
		Index:      i.Index,
		Transition: i.Transition,
		Id:         i.Id,
		Childrens:  childrens,
	}
}
func mappingToProto(m GrammarMapping) map[string]*proto.IntMap {
	res := make(map[string]*proto.IntMap)
	for k, v := range m {
		res[k] = &proto.IntMap{
			Value: v,
		}
	}
	return res
}

func mappingFromProto(m map[string]*proto.IntMap) GrammarMapping {
	res := make(GrammarMapping)
	for k, v := range m {
		if v != nil {
			res[k] = v.Value
		}
	}
	return res
}

func sectionToProto(s *Section) *proto.Section {
	return &proto.Section{
		Type:         s.Type,
		Name:         s.Name,
		Replacements: replacementsToProto(s.Replacements),
	}
}

func sectionFromProto(s *proto.Section) *Section {
	return &Section{
		Type:         s.Type,
		Name:         s.Name,
		Replacements: replacementsFromProto(s.Replacements),
	}
}

func sectionsFromProto(s []*proto.Section) []*Section {
	res := make([]*Section, 0, len(s))
	for _, val := range s {
		res = append(res, sectionFromProto(val))
	}
	return res
}

func sectionsToProto(s []*Section) []*proto.Section {
	res := make([]*proto.Section, 0, len(s))
	for _, val := range s {
		res = append(res, sectionToProto(val))
	}
	return res
}
func replacementToProto(r *Replacement) *proto.Replacement {
	return &proto.Replacement{
		Probability: r.Probability,
		IsTerminal:  r.IsTerminal,
		Values:      r.Values,
		Function:    r.Function,
		Pos:         r.Pos,
	}
}

func replacementFromProto(r *proto.Replacement) *Replacement {
	return &Replacement{
		Probability: r.Probability,
		IsTerminal:  r.IsTerminal,
		Values:      r.Values,
		Function:    r.Function,
		Pos:         r.Pos,
	}
}

func replacementsToProto(r []*Replacement) []*proto.Replacement {
	res := make([]*proto.Replacement, 0, len(r))
	for _, val := range r {
		res = append(res, replacementToProto(val))
	}
	return res
}

func replacementsFromProto(r []*proto.Replacement) []*Replacement {
	res := make([]*Replacement, 0, len(r))
	for _, val := range r {
		res = append(res, replacementFromProto(val))
	}
	return res
}
