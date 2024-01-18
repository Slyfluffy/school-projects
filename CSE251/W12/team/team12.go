/*
	---------------------------------------

Course: CSE 251
Lesson Week: 12
File: team.go
Author: Brother Comeau

Purpose: team activity - finding primes

Instructions:

- Process the array of numbers, find the prime numbers using goroutines

worker()

This goroutine will take in a list/array/channel of numbers.  It will place
prime numbers on another channel

readValue()

This goroutine will display the contents of the channel containing
the prime numbers

---------------------------------------
*/
package main

import (
	"fmt"
	"math/rand"
	"sync"
	"time"
)

func isPrime(n int) bool {
	// Primality test using 6k+-1 optimization.
	// From: https://en.wikipedia.org/wiki/Primality_test

	if n <= 3 {
		return n > 1
	}

	if n%2 == 0 || n%3 == 0 {
		return false
	}

	i := 5
	for (i * i) <= n {
		if n%i == 0 || n%(i+2) == 0 {
			return false
		}
		i += 6
	}
	return true
}

func worker(ch chan int, primech chan int, wg *sync.WaitGroup) {
	// TODO - process numbers on one channel and place prime number on another
	for val := range ch {
		fmt.Printf("Checking %d to see if it is prime\n", val)
		if isPrime(val) {
			primech <- val
		}
		wg.Done()
	}
}

func readValues(ch chan int, wg *sync.WaitGroup) {
	// TODO -Display prime numbers from a channel
	for prime := range ch {
		fmt.Printf("Prime: %d\n", prime)
	}
}

func main() {

	workers := 10
	numberValues := 100
	// start := 1
	// end := 100

	// Create any channels that you need
	ch := make(chan int, workers)
	primech := make(chan int, workers)

	// Create any other "things" that you need to get the workers to finish(join)
	var wg sync.WaitGroup
	
	// create workers
	for w := 1; w <= workers; w++ {
		go worker(ch, primech, &wg) // Add any arguments
	}

	rand.Seed(time.Now().UnixNano())
	for i := 0; i < numberValues; i++ {
		ch <- rand.Int()
		wg.Add(1)
	}

	go readValues(primech, &wg) // Add any arguments

	wg.Wait()
	fmt.Println("All Done!")
}
