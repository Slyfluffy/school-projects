'''
Requirements
1. Create a multiprocessing program that reads in files with defined tasks to perform.
2. The program should use a process pool per task type and use apply_async calls with callback functions.
3. The callback functions will store the results in global lists based on the task to perform.
4. Once all 4034 tasks are done, the code should print out each list and a breakdown of 
   the number of each task performed.
   
Questions:
1. How many processes did you specify for each pool:
   > Finding primes: 4
   > Finding words in a file: 6
   > Changing text to uppercase: 2
   > Finding the sum of numbers: 4
   > Web request to get names of Star Wars people: 10
   
   > How do you determine these numbers: I placed more processes on the IO bound problems as they would take the longest.
   > Having more processors working on them would decrease the amount of time used. I used less processes for CPU bound problems as
   > the speed of the processors would easily be faster than the IO bound problems. It also cost more time to split up the work for the CPU
   > bound work than to use less processors.
   
2. Specify whether each of the tasks is IO Bound or CPU Bound?
   > Finding primes: CPU
   > Finding words in a file: IO
   > Changing text to uppercase: CPU 
   > Finding the sum of numbers: CPU
   > Web request to get names of Star Wars people: IO
   
3. What was your overall time, with:
   > one process in each of your five pools:  34.97 seconds
   > with the number of processes you show in question one:  6.34 seconds
'''

import glob
import json
import math
import multiprocessing as mp
import os
import time
from datetime import datetime, timedelta

import numpy as np
import requests

TYPE_PRIME = 'prime'
TYPE_WORD = 'word'
TYPE_UPPER = 'upper'
TYPE_SUM = 'sum'
TYPE_NAME = 'name'

# Global lists to collect the task results
result_primes = []
result_words = []
result_upper = []
result_sums = []
result_names = []


def is_prime(n: int):
    """Primality test using 6k+-1 optimization.
    From: https://en.wikipedia.org/wiki/Primality_test
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


def task_prime(value):
    """
    Add a message stating whether the numer is a prime or not
    BOUND TYPE: CPU because of is_prime and how fast this can run
    """
    if is_prime(value):
        return f'{value} is prime'
    else:
        return f'{value} is not prime'


def add_prime_result(r):
    """Callback function to add the word result"""
    result_primes.append(r)


def task_word(word):
    """
    Add a message stating whether the word was found or not
    BOUND TYPE: IO because it is how long it takes to read the file. CPU bound after that
    """
    file = open('words.txt', 'r')
    for w in file:
        if w == word:
            return f'{word} found'

    return f'{word} not found'


def add_word_result(r):
    """Callback function to add the word result"""
    result_words.append(r)


def task_upper(text: str):
    """
    Add a comparison message between text and an uppercase version
    BOUND TYPE: CPU because it is how fast it can get through this
    """
    return f'{text} ==>  uppercase version of {text.upper()}'


def add_upper_result(r):
    """Callback function to add the upper result"""
    result_upper.append(r)


def task_sum(start_value, end_value):
    """
    Add all values from start to finish and add message to results
    BOUND TYPE: CPU because it is how fast it can get through this
    """
    sum = 0
    for i in range(start_value, end_value + 1):
        sum += i

    return f'sum of {start_value} to {end_value} = {sum}'


def add_sum_result(r):
    """Callback function to add the sum result"""
    result_sums.append(r)


def task_name(url):
    """
    Add a message to results_names whether the response has a name or not
    BOUND TYPE: IO as it has to wait for a request
    """
    response = requests.get(url)
    if response.status_code == 200:
        response = response.json()
        return f'{url} has name <{response["name"]}>'

    return f'{url} had an error receiving the information'


def add_name_result(r):
    """Callback function to add the name result"""
    result_names.append(r)


def load_json_file(filename):
    if os.path.exists(filename):
        with open(filename) as json_file:
            data = json.load(json_file)
        return data
    else:
        return {}


def main():
    begin_time = time.time()

    # Swimming party for primes, words, text, sums, and names!
    prime_pool = mp.Pool(4)
    word_pool = mp.Pool(6)
    text_pool = mp.Pool(2)
    sum_pool = mp.Pool(4)
    name_pool = mp.Pool(10)

    count = 0
    task_files = glob.glob("tasks/*.task")
    for filename in task_files:
        task = load_json_file(filename)
        print(task)
        count += 1
        task_type = task['task']
        if task_type == TYPE_PRIME:
            prime_pool.apply_async(task_prime, args=(task['value'],), callback=add_prime_result)
        elif task_type == TYPE_WORD:
            word_pool.apply_async(task_word, args=(task['word'],), callback=add_word_result)
        elif task_type == TYPE_UPPER:
            text_pool.apply_async(task_upper, args=(task['text'],), callback=add_upper_result)
        elif task_type == TYPE_SUM:
            sum_pool.apply_async(task_sum, args=(task['start'], task['end']), callback=add_sum_result)
        elif task_type == TYPE_NAME:
            name_pool.apply_async(task_name, args=(task['url'],), callback=add_name_result)
        else:
            print(f'Error: unknown task type {task_type}')

    # Tell all pools that there will be no more work added
    prime_pool.close()
    word_pool.close()
    text_pool.close()
    sum_pool.close()
    name_pool.close()

    # Block until all pools are done
    prime_pool.join()
    word_pool.join()
    text_pool.join()
    sum_pool.join()
    name_pool.join()

    def print_list(lst):
        for item in lst:
            print(item)
        print(' ')

    print('-' * 80)
    print(f'Primes: {len(result_primes)}')
    print_list(result_primes)

    print('-' * 80)
    print(f'Words: {len(result_words)}')
    print_list(result_words)

    print('-' * 80)
    print(f'Uppercase: {len(result_upper)}')
    print_list(result_upper)

    print('-' * 80)
    print(f'Sums: {len(result_sums)}')
    print_list(result_sums)

    print('-' * 80)
    print(f'Names: {len(result_names)}')
    print_list(result_names)

    print(f'Number of Primes tasks: {len(result_primes)}')
    print(f'Number of Words tasks: {len(result_words)}')
    print(f'Number of Uppercase tasks: {len(result_upper)}')
    print(f'Number of Sums tasks: {len(result_sums)}')
    print(f'Number of Names tasks: {len(result_names)}')
    print(f'Finished processes {count} tasks = {time.time() - begin_time}')


if __name__ == '__main__':
    main()
