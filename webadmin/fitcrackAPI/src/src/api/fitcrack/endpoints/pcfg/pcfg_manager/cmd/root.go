package cmd

import (
	"github.com/dasio/pcfg-manager/manager"
	"github.com/spf13/cobra"
)

var (
	rulesFolder      string
	grammarFile      string
	preTerminalsFile string
	inputArgs        manager.InputArgs
)

var rootCmd = &cobra.Command{
	Use:   "pcfg-manager",
	Short: "Password generator",
	Long:  `Password generator`,
	RunE: func(cmd *cobra.Command, args []string) error {
		mng := manager.NewManager(rulesFolder)
		if grammarFile != "" {
			if err := mng.LoadFromFile(grammarFile); err != nil {
				return err
			}
		} else {
			if err := mng.Load(); err != nil {
				return err
			}
		}
		if preTerminalsFile != "" {
			return mng.ListTerminals(preTerminalsFile)
		}
		if err := mng.Start(&inputArgs); err != nil {
			return err
		}
		return nil
	},
}

func Execute() error {
	if err := rootCmd.Execute(); err != nil {
		return err
	}
	return nil
}

func init() {
	cobra.OnInitialize(initConfig)
	rootCmd.PersistentFlags().StringVarP(&rulesFolder, "rules", "r", "Rules/Default", "specifies rule")
	rootCmd.Flags().StringVarP(&grammarFile, "grammar-file", "", "", "it uses marshaled grammar file instead of parsing")
	rootCmd.Flags().StringVarP(&preTerminalsFile, "preterminals-file", "", "", "generates terminals from specified preterminals")

	rootCmd.Flags().UintVarP(&inputArgs.GoRoutines, "go-routines", "g", 1, "how many go routines will be used")
	rootCmd.Flags().Uint64VarP(&inputArgs.MaxGuesses, "max-guesses", "m", 0, "max guesses before exit (generates at least m terminals, could be more)")
	rootCmd.Flags().BoolVarP(&inputArgs.Debug, "debug", "d", false, "")
}

func initConfig() {
}
