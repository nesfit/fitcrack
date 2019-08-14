package cmd

import (
	"github.com/dasio/pcfg-manager/manager"
	"github.com/dasio/pcfg-manager/server"
	"github.com/spf13/cobra"
	"time"
)

var (
	serverArgs manager.InputArgs
)

func init() {
	rootCmd.AddCommand(serverCmd)
	serverCmd.Flags().StringVar(&serverArgs.HashFile, "hashlist", "", "hash list to crack")
	serverCmd.Flags().StringVar(&serverArgs.HashcatMode, "hashcat-mode", "0", "hashcat mode of hash")
	serverCmd.Flags().StringVarP(&serverArgs.Port, "port", "p", "50051", "server port")
	serverCmd.Flags().Uint64VarP(&serverArgs.MaxGuesses, "max-guesses", "m", 0, "max guesses before exit")
	serverCmd.Flags().IntVar(&serverArgs.TerminalsQueSize, "term-que-size", 100000, "how many pre-terminals structure leads to terminals can be in que ")
	serverCmd.Flags().Uint64Var(&serverArgs.ChunkStartSize, "chunk-start-size", 10000, "how many pre-terminals will be sent at connected client")
	serverCmd.Flags().DurationVar(&serverArgs.ChunkDuration, "chunk-duration", time.Second*30, "how long should each chunk take")
	serverCmd.Flags().BoolVar(&serverArgs.GenerateTerminals, "generate-terminals", false, "server will generate terminals from preterminals structure and send them")
	serverCmd.Flags().BoolVar(&serverArgs.SaveStats, "stats", false, "save stats after end")

}

var serverCmd = &cobra.Command{
	Use:   "server",
	Short: "run server",
	Long:  "run server",
	RunE: func(cmd *cobra.Command, args []string) error {
		svc := server.NewService()
		serverArgs.RulesFolder = rulesFolder
		if err := svc.Load(serverArgs); err != nil {
			return err
		}
		/*go func() {
			for {
				_, _ = bufio.NewReader(os.Stdin).ReadBytes('\n')
				svc.DebugClients()
			}
		}()*/
		if err := svc.Run(); err != nil {
			return err
		}
		if serverArgs.SaveStats {
			if err := svc.SaveStats(); err != nil {
				return err
			}
		}
		return nil
	},
}
