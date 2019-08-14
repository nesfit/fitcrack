package cmd

import (
	"github.com/dasio/pcfg-manager/manager"
	"github.com/golang/protobuf/proto"
	"github.com/spf13/cobra"
	"io/ioutil"
)

var (
	outFile string
)

func init() {
	rootCmd.AddCommand(marshalCmd)
	marshalCmd.Flags().StringVarP(&outFile, "out-file", "o", "grammar.bin", "out file for grammar")
}

var marshalCmd = &cobra.Command{
	Use:   "marshal",
	Short: "used for marshalling grammar",
	RunE: func(cmd *cobra.Command, args []string) error {
		g, err := manager.LoadGrammar(rulesFolder)
		if err != nil {
			return err
		}
		protoGrammar := manager.GrammarToProto(g)
		b, err := proto.Marshal(protoGrammar)
		if err != nil {
			return err
		}
		err = ioutil.WriteFile(outFile, b, 0644)
		return err
	},
}
