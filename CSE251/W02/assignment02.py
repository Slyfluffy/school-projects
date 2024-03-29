'''
Questions:
1. Time to run using 1 thread = 7.66
2. Time to run using 5 threads = 7.63
3. Time to run using 10 threads = 7.522
4. Based on your study of the GIL (see https://realpython.com/python-gil), 
   what conclusions can you draw about the similarity of the times (short answer)?
   > The large cost of this example came from setup. Most of the time was spent in setting up the 
   > array of Threads (I checked using print statements). 
   > The threads went through their work rather quicly once they were all setup. An increase in 
   > thread count only saved us a couple milliseconds. This is due to the GIL only allowing one
   > thread to run at a time.
5. Is this assignment an IO Bound or CPU Bound problem (see https://stackoverflow.com/questions/868568/what-do-the-terms-cpu-bound-and-i-o-bound-mean)?
   > Totally CPU bound. There is no api calls to databases or the internet. Everything is set in the program
'''

from datetime import datetime, timedelta
import math
import threading
import time

# Globals used for calculations
MIN = 10_000_000_000
MAX = 10_000_110_003

# Global count of the number of primes found
prime_count = 0

# Global count of the numbers examined
numbers_processed = 0

NUMBER_THREADS = 100


def is_prime(n: int):
    """
    Primality test using 6k+-1 optimization.
    From: https://en.wikipedia.org/wiki/Primality_test

    Parameters
    ----------
    ``n`` : int
        Number to determine if prime

    Returns
    -------
    bool
        True if ``n`` is prime.
    """

    if n <= 3:
        return n > 1
    if n % 2 == 0 or n % 3 == 0:
        return False
    i = 5
    while i ** 2 <= n:
        if n % i == 0 or n % (i + 2) == 0:
            return False
        i += 6
    return True


def check_range_for_primes(start: int, end: int):
    local_processed = 0
    local_count = 0
    for i in range(start, end):
        if (is_prime(i)):
            local_count += 1
        local_processed += 1

    # Get the global variables
    global prime_count
    global numbers_processed

    # add to the global variables
    numbers_processed += local_processed
    prime_count += local_count


if __name__ == '__main__':
    # Start a timer
    begin_time = time.perf_counter()

    thread_todo_list = [(MAX - MIN) // NUMBER_THREADS] * NUMBER_THREADS
    remainder = (MAX - MIN) % NUMBER_THREADS

    # split the remainder evenly between all threads
    i = 0
    while remainder > 0:
        thread_todo_list[i] += 1
        remainder -= 1
        i += 1
        if (i >= len(thread_todo_list)):  # Reset the index if needed
            i = 0

    # Setup and start threads
    threads = []
    start = MIN  # We start at the bottom of the range
    for index in range(0, NUMBER_THREADS):
        end = start + thread_todo_list[index]
        threads.append(threading.Thread(check_range_for_primes(start, end)))
        threads[index].start()
        start = end

    # Finish threads before using asserts
    for index in range(0, NUMBER_THREADS):
        threads[index].join()

    # Use the below code to check and print your results
    assert numbers_processed == 110_003, f"Should check exactly 110,003 numbers but checked {numbers_processed}"
    assert prime_count == 4764, f"Should find exactly 4764 primes but found {prime_count}"

    print(f'Numbers processed = {numbers_processed}')
    print(f'Primes found = {prime_count}')
    total_time = "{:.2f}".format(time.perf_counter() - begin_time)
    print(f'Total time = {total_time} sec')
