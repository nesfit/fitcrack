package main

import (
	"github.com/dasio/pcfg-manager/cmd"
	log "github.com/sirupsen/logrus"
	//_ "net/http/pprof"
	"os"
)

func init() {
	//log.SetFormatter(&log.JSONFormatter{})
	log.SetOutput(os.Stderr)
}

func main() {
	/*go func() {
		log.Println(http.ListenAndServe("localhost:6060", nil))
	}()*/
	if err := cmd.Execute(); err != nil {
		log.Error(err)
	}
}
