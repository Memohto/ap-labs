// Clock Server is a concurrent TCP server that periodically writes the time.
package main

import (
	"flag"
	"io"
	"log"
	"net"
	"os"
	"time"
)

func handleConn(c net.Conn, l *time.Location) {
	defer c.Close()
	for {
		_, err := io.WriteString(c, l.String()+": "+time.Now().In(l).Format("15:04:05")+"\n")
		if err != nil {
			return // e.g., client disconnected
		}
		time.Sleep(1 * time.Second)
	}
}

func main() {
	var port string
	flag.StringVar(&port, "port", "8080", "Port of the time server")
	flag.Parse()

	location, err := time.LoadLocation(os.Getenv("TZ"))
	if err != nil {
		log.Fatal(err)
	}

	listener, err := net.Listen("tcp", "localhost:"+port)
	if err != nil {
		log.Fatal(err)
	}
	for {
		conn, err := listener.Accept()
		if err != nil {
			log.Print(err) // e.g., connection aborted
			continue
		}
		go handleConn(conn, location) // handle connections concurrently
	}
}
