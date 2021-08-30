package main

import (
	"fmt"
	"os"
	"strconv"
)

// adds/subtracts/multiplies all values that are in the *values array.
// nValues is the number of values you're reading from the array
// operator will indicate if it's an addition (1), subtraction (2) or
// multiplication (3)
func calc(operator int, values []int) int {
	result := values[0]
	if(len(values) == 1) {
		fmt.Printf("%d = %d\n", result, result);
		return result
	}
	if operator == 1 {
		fmt.Printf("%d + ", result)
		for i, n := range values[1:] {
			if i != len(values)-2 {
				fmt.Printf("%d + ", n)
			} else {
				fmt.Printf("%d = ", n)
			}
			result += n
		}
		fmt.Printf("%d\n", result)
	} else if operator == 2 {
		fmt.Printf("%d - ", result)
		for i, n := range values[1:] {
			if i != len(values)-2 {
				fmt.Printf("%d - ", n)
			} else {
				fmt.Printf("%d = ", n)
			}
			result -= n
		}
		fmt.Printf("%d\n", result)
	} else {
		fmt.Printf("%d * ", result)
		for i, n := range values[1:] {
			if i != len(values)-2 {
				fmt.Printf("%d * ", n)
			} else {
				fmt.Printf("%d = ", n)
			}
			result *= n
		}
		fmt.Printf("%d\n", result)
	}

	return result
}

func main() {
	if len(os.Args) < 3 {
		fmt.Printf("Missing argument\n")
		fmt.Println("Usage: go run calculator.go <operator> <numbers separated by space>")
	}

	strOperator := os.Args[1]
	var operator int
	if strOperator == "add" {
		operator = 1
	} else if strOperator == "sub" {
		operator = 2
	} else if strOperator == "mult" {
		operator = 3
	} else {
		fmt.Println("Invalid opartor (Acepted values: add, sub, mult)")
		return
	}

	var values = []int{}
	for _, i := range os.Args[2:] {
		j, err := strconv.Atoi(i)
		if err != nil {
			fmt.Println("All numbers must be integers")
			return
		}
		values = append(values, j)
	}

	calc(operator, values)
}
