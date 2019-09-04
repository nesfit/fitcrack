package cmd

import (
	"fmt"
	"github.com/dasio/pcfg-manager/manager"
	"github.com/spf13/cobra"
)

func init() {
	rootCmd.AddCommand(countCmd)
}

var countCmd = &cobra.Command{
	Use:   "count",
	Short: "Count how many terminals can be generated from grammar - may be wrong - not tested",
	Long:  "Count how many terminals can be generated from grammar - may be wrong - not tested",
	RunE: func(cmd *cobra.Command, args []string) error {
		g, err := manager.LoadGrammar(rulesFolder)
		if err != nil {
			return err
		}
		count := g.Count()
		fmt.Println(count)
		return nil
	},
}
