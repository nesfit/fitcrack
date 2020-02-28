package cmd

import (
	"github.com/dasio/pcfg-manager/client"
	"github.com/sirupsen/logrus"
	"github.com/spf13/cobra"
	"os"
	"os/signal"
	"syscall"
)

var (
	clientArgs client.InputArgs
)

func init() {
	rootCmd.AddCommand(clientCmd)
	clientCmd.Flags().StringVarP(&clientArgs.ServerAddress, "server", "s", "localhost:50051", "server address")
	clientCmd.Flags().StringVar(&clientArgs.HashcatFolder, "hashcat-folder", "./hashcat", "folder in which is hashcat binary")
	clientCmd.Flags().BoolVar(&clientArgs.GenOnly, "generate-only", false, "generation guesses without cracking")
	clientCmd.Flags().BoolVar(&clientArgs.SaveStats, "stats", false, "save stats after end")

}

var clientCmd = &cobra.Command{
	Use:           "client",
	Short:         "run client",
	Long:          "run client",
	SilenceErrors: true,
	SilenceUsage:  true,
	RunE: func(cmd *cobra.Command, args []string) error {
		clientArgs.GenRoutines = inputArgs.GoRoutines
		svc, err := client.NewService(clientArgs)
		if err != nil {
			return err
		}
		if err := svc.Connect(clientArgs.ServerAddress); err != nil {
			return err
		}
		sigs := make(chan os.Signal, 1)
		done := make(chan bool, 1)
		signal.Notify(sigs, syscall.SIGINT, syscall.SIGTERM)
		go func() {
			<-sigs
			done <- true
			if clientArgs.SaveStats {
				_ = svc.SaveStats()
			}
			_ = svc.Disconnect()
			os.Exit(1)
		}()
		if err := svc.Run(done); err != nil && err != client.ErrFinished {
			logrus.Warn(err)
		}
		if clientArgs.SaveStats {
			_ = svc.SaveStats()
		}
		if err := svc.Disconnect(); err != nil {
			return err
		}
		return nil
	},
}
