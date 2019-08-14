package manager

type TreeItem struct {
	Index      int32
	Transition int32
	Childrens  []*TreeItem
	Id         bool
}

func (i *TreeItem) Append(item *TreeItem) {
	if item == nil {
		return
	}
	i.Childrens = append(i.Childrens, item)
}

func (i *TreeItem) Pop() *TreeItem {
	var item *TreeItem
	if len(i.Childrens) < 1 {
		return nil
	}
	item, i.Childrens = i.Childrens[len(i.Childrens)-1], i.Childrens[:len(i.Childrens)-1]
	return item
}

func (i *TreeItem) Copy() *TreeItem {
	item := &TreeItem{}
	item.Index = i.Index
	item.Transition = i.Transition
	if len(i.Childrens) == 0 {
		return item
	}
	for _, children := range i.Childrens {
		item.Childrens = append(item.Childrens, children.Copy())
	}
	return item
}
