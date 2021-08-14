package main

import (
	"fmt"
	"os"
	"strings"
)

func main() {
	if len(os.Args) == 1 {
		fmt.Println("Error: You must send at least one parameter")
		return
	}
	name := strings.Join(os.Args[1:], " ")
	fmt.Printf("Hello %v, Welcome to the jungle \n", name)
}
