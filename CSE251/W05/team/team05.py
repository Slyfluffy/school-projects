"""
Course: CSE 251
Lesson Week: 05
File: team05.py
Author: Brother Comeau (modified by Brother Foushee)

Purpose: Team Activity

Instructions:

- See in Canvas

"""

import threading
import queue
import time
import requests
import json

RETRIEVE_THREADS = 4     # Number of retrieve_threads
NO_MORE_VALUES = 'No more'  # Special value to indicate no more items in the queue


def retrieve_thread(q: queue.Queue, number):  # TODO add arguments
    """ Process values from the data_queue """

    while True:
        # TODO check to see if anything is in the queue
        url = q.get()

        if (url == None):
            q.put(None)
            break

        response = requests.get(url)
        if response.status_code == 200:
            response = response.json()
            print(f"Thread {number}" + ": " + response["name"] + "\n", "")


def file_reader(url_queue: queue.Queue):  # TODO add arguments
    """ This thread reads the data file and places the values in the data_queue """

    file = open("urls.txt")
    for line in file:
        url_queue.put(line.strip())

    file.close()
    url_queue.put(None)


def main():
    """ Main function """

    # Start a timer
    begin_time = time.perf_counter()

    # TODO create queue (if you use the queue module, then you won't need semaphores/locks)
    url_queue = queue.Queue()

    # TODO create the threads. 1 filereader() and RETRIEVE_THREADS retrieve_thread()s
    # Pass any arguments to these thread needed to do their jobs
    file_thread = threading.Thread(target=file_reader, args= [url_queue])
    request_threads = []
    for i in range(RETRIEVE_THREADS):
        request_threads.append(threading.Thread(target= retrieve_thread, args= [url_queue, i]))

    # TODO Get them going
    file_thread.start()
    for thread in request_threads:
        thread.start()

    # TODO Wait for them to finish
    file_thread.join()
    for thread in request_threads:
        thread.join()

    total_time = "{:.2f}".format(time.perf_counter() - begin_time)
    print(f'Total time to process all URLS = {total_time} sec')


if __name__ == '__main__':
    main()
