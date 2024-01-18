"""
Course: CSE 251
Lesson Week: 12
File: assignment.py
Author: Journey Curtis
Purpose: Assignment 12 - Family Search
"""
import json
import threading
import time

import requests
from virusApi import *

TOP_API_URL = 'http://127.0.0.1:8129'
NUMBER_GENERATIONS = 6  # set this to 2 as you are testing your code
NUMBER_THREADS = 0

# -----------------------------------------------------------------------------
class Request_Thread(threading.Thread):
    """Handles requests to a server via a url"""

    def __init__(self, url):
        threading.Thread.__init__(self)
        self.url = url
        self.response = {}
        global NUMBER_THREADS
        NUMBER_THREADS += 1

    def run(self):
        response = requests.get(self.url)
        if response.status_code == 200:
            self.response = response.json()
        else:
            # Let us know something is wrong
            print('url call response = ', response.status_code)


def dfs_recursion(family_id, pandemic: Pandemic):
    # base case
    if family_id == None:
        return

    # add family to pandemic
    family_request = Request_Thread(f'{TOP_API_URL}/family/{family_id}')
    family_request.start()

    family_request.join()
    if ("id" not in family_request.response):
        return

    family = Family.fromResponse(family_request.response)
    pandemic.add_family(family)

    virus1 = None
    virus2 = None

    # Create and start VIRUS1 Request
    v1_request = None
    if family.virus1 != None:
        v1_request = Request_Thread(
            f'http://{hostName}:{serverPort}/virus/{family.virus1}')
        v1_request.start()

    # Create and start VIRUS2 Request
    v2_request = None
    if family.virus2 != None:
        v2_request = Request_Thread(
            f'http://{hostName}:{serverPort}/virus/{family.virus2}')
        v2_request.start()

    # Setup offspring requests
    requests: list[Request_Thread] = []
    for id in family.offspring:
        requests.append(Request_Thread(
            f'http://{hostName}:{serverPort}/virus/{id}'))

    # Start the requests!
    for r in requests:
        r.start()

    # Setup for recursion on recursive functions
    v_threads: list[threading.Thread] = []
    global NUMBER_THREADS

    # ADD VIRUS1 to Pandemic
    if v1_request != None:  # Only add if v1_request is valid
        v1_request.join()
        virus1 = v1_request.response
    if virus1 != None:
        v = Virus.createVirus(virus1)
        pandemic.add_virus(v)
        if v.parents != None:
            v_threads.append(threading.Thread(
                target=dfs_recursion, args=(v.parents, pandemic)))
            NUMBER_THREADS += 1

    # ADD VIRUS2 to Pandemic
    if v2_request != None:  # Only add if v2_request is valid
        v2_request.join()
        virus2 = v2_request.response
    if virus2 != None:
        v = Virus.createVirus(virus2)
        pandemic.add_virus(v)
        if v.parents != None:
            v_threads.append(threading.Thread(
                target=dfs_recursion, args=(v.parents, pandemic)))
            NUMBER_THREADS += 1

    # Start these other recursion methods!
    for t in v_threads:
        t.start()

    # Verify all offspring requests are done and save the response
    offspring = []
    for r in requests:
        r.join()
        offspring.append(r.response)

    # ADD offspring to Pandemic
    for o in offspring:
        v = Virus.createVirus(o)
        # don't try and add virus that we have already added
        # (happens when we add a virus and then loop over the
        # virus parent's offspring)
        if not pandemic.does_virus_exist(v.id):
            pandemic.add_virus(v)

    # Verify the recursion threads are done before finishing up
    for t in v_threads:
        t.join()


def dfs(start_id, generations):
    pandemic = Pandemic(start_id)

    # tell server we are starting a new generation of viruses
    requests.get(f'{TOP_API_URL}/start/{generations}')

    # get all the viruses in the pandemic recursively
    dfs_recursion(start_id, pandemic)

    requests.get(f'{TOP_API_URL}/end')

    print('')
    print(f'Total Viruses  : {pandemic.get_virus_count()}')
    print(f'Total Families : {pandemic.get_family_count()}')
    print(f'Generations    : {generations}')

    return pandemic.get_virus_count()


def main():
    # Start a timer
    begin_time = time.perf_counter()

    print(f'Pandemic starting...')
    print('#' * 60)

    response = requests.get(f'{TOP_API_URL}')
    jsonResponse = response.json()

    print(f'First Virus Family id: {jsonResponse["start_family_id"]}')
    start_id = jsonResponse['start_family_id']

    virus_count = dfs(start_id, NUMBER_GENERATIONS)

    total_time = time.perf_counter() - begin_time
    total_time_str = "{:.2f}".format(total_time)

    print(f'\nTotal time = {total_time_str} sec')
    print(f'Number of threads: {NUMBER_THREADS}')
    print(f'Performance: {round(virus_count / total_time, 2)} viruses/sec')


if __name__ == '__main__':
    main()
