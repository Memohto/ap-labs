// Copyright © 2016 Alan A. A. Donovan & Brian W. Kernighan.
// License: https://creativecommons.org/licenses/by-nc-sa/4.0/

// See page 156.

// Package geometry defines simple types for plane geometry.
//!+point
package main

import (
	"fmt"
	"math"
	"math/rand"
	"os"
	"strconv"
	"time"
)

type Point struct{ x, y float64 }

// traditional function
// func Distance(p, q Point) float64 {
// 	return math.Hypot(q.X()-p.X(), q.Y()-p.Y())
// }

// same thing, but as a method of the Point type
func (p Point) Distance(q Point) float64 {
	return math.Hypot(q.X()-p.X(), q.Y()-p.Y())
}

func (p Point) X() float64 {
	return p.x
}

func (p Point) Y() float64 {
	return p.y
}

//!-point

//!+path

// A Path is a journey connecting the points with straight lines.
type Path []Point

// Distance returns the distance traveled along the path.
func (path Path) Distance() float64 {
	fmt.Print("   - ")
	sum := 0.0
	dist := 0.0
	for i := range path {
		if i > 0 {
			dist = path[i-1].Distance(path[i])
			sum += dist
			fmt.Printf("%.2f + ", dist)
		}
	}
	dist = path[len(path)-1].Distance(path[0])
	sum += dist
	fmt.Printf("%.2f = %.2f\n", dist, sum)
	return sum
}

//!-path

// Given three colinear points p, q, r, the function checks if
// point q lies on line segment 'pr'
func onSegment(p, q, r Point) bool {
	if q.x <= math.Max(p.X(), r.X()) && q.X() >= math.Min(p.X(), r.X()) &&
		q.y <= math.Max(p.Y(), r.Y()) && q.Y() >= math.Min(p.Y(), r.Y()) {
		return true
	}

	return false
}

// To find orientation of ordered triplet (p, q, r).
// The function returns following values
// 0 --> p, q and r are colinear
// 1 --> Clockwise
// 2 --> Counterclockwise
func orientation(p, q, r Point) int {
	val := (q.y-p.y)*(r.x-q.x) - (q.x-p.x)*(r.y-q.y)

	if val == 0 {
		return 0
	}

	if val > 0 {
		return 1
	} else {
		return 2
	}
}

// The main function that returns true if line segment 'p1q1'
// and 'p2q2' intersect.
func doIntersect(p1, q1, p2, q2 Point) bool {
	o1 := orientation(p1, q1, p2)
	o2 := orientation(p1, q1, q2)
	o3 := orientation(p2, q2, p1)
	o4 := orientation(p2, q2, q1)

	// General case
	if o1 != o2 && o3 != o4 {
		return true
	}

	// Special Cases
	// p1, q1 and p2 are colinear and p2 lies on segment p1q1
	if o1 == 0 && onSegment(p1, p2, q1) {
		return true
	}

	// p1, q1 and q2 are colinear and q2 lies on segment p1q1
	if o2 == 0 && onSegment(p1, q2, q1) {
		return true
	}

	// p2, q2 and p1 are colinear and p1 lies on segment p2q2
	if o3 == 0 && onSegment(p2, p1, q2) {
		return true
	}

	// p2, q2 and q1 are colinear and q1 lies on segment p2q2
	if o4 == 0 && onSegment(p2, q1, q2) {
		return true
	}

	// Doesn't fall in any of the above cases
	return false
}

func main() {
	rand.Seed(time.Now().UnixNano())

	if len(os.Args) < 2 {
		fmt.Printf("Missing argument\n")
		fmt.Println("Usage: go run geometry.go <sides>")
		return
	}

	sides, err := strconv.Atoi(os.Args[1])
	if err != nil {
		fmt.Println(err)
		return
	}

	if sides < 3 {
		fmt.Println("You can't have a figure with less than 3 sides")
		return
	} else if sides > 50 {
		fmt.Println("The sides limit is 50, chose a smaller number")
		return
	}

	fmt.Printf(" - Generating a [%d] sides figure\n", sides)
	figure := make(Path, sides)
	for i := range figure {
		if i > 2 {
			generated := true
			for generated {
				flag := false
				figure[i].x = -100 + rand.Float64()*200
				figure[i].y = -100 + rand.Float64()*200
				for j := range figure[:i-2] {
					if doIntersect(figure[i-1], figure[i], figure[j], figure[j+1]) {
						flag = true
					}
				}

				if flag {
					continue
				}

				for j := range figure[1 : i-1] {
					if doIntersect(figure[i], figure[0], figure[j+1], figure[j+2]) {
						flag = true
					}
				}
				generated = flag
			}
		} else {
			figure[i].x = -100 + rand.Float64()*200
			figure[i].y = -100 + rand.Float64()*200
		}
	}

	fmt.Println(" - Figure's vertices")
	for _, p := range figure {
		fmt.Printf("   - (%6.2f, %6.2f)\n", p.X(), p.Y())
	}

	fmt.Println(" - Figure's Perimeter")
	figure.Distance()
}
