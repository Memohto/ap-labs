package main

import (
	"fmt"
	"io"
	"log"
	"net"
	"os"
	"strings"
)

func main() {
	if len(os.Args) < 2 {
		fmt.Println("Incorrect usage:")
		fmt.Println("go run clockWall.go <city_1>=<server_1> ... <city_n>=<server_n>")
		return
	}

	state := make(chan int)
	for i, s := range os.Args[1:] {
		arr := strings.Split(s, "=")
		if len(arr) != 2 {
			fmt.Printf("Wrong argument: %s\n", s)
			fmt.Println("Example: NewYork=localhost:8010")
			return
		}
		go func(current int) {
			conn, err := net.Dial("tcp", arr[1])
			if err != nil {
				log.Fatal(err)
			}
			defer conn.Close()
			mustCopy(os.Stdout, conn)
			state <- 2
		}(i)
	}

	<-state
	fmt.Println("Lost connection to one of the servers")
}

func mustCopy(dst io.Writer, src io.Reader) {
	if _, err := io.Copy(dst, src); err != nil {
		log.Fatal(err)
	}
}