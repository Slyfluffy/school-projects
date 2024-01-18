"""
Course: CSE 251
Lesson Week: 11
File: Assignment.py
"""

import time
import random
import multiprocessing as mp

# number of cleaning staff and hotel guests
CLEANING_STAFF = 2
HOTEL_GUESTS = 5

# Run program for this number of seconds
TIME = 60

STARTING_PARTY_MESSAGE = 'Turning on the lights for the party vvvvvvvvvvvvvv'
STOPPING_PARTY_MESSAGE = 'Turning off the lights  ^^^^^^^^^^^^^^^^^^^^^^^^^^'

STARTING_CLEANING_MESSAGE = 'Starting to clean the room >>>>>>>>>>>>>>>>>>>>>>>>>>>>>'
STOPPING_CLEANING_MESSAGE = 'Finish cleaning the room <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<'


def cleaner_waiting():
    time.sleep(random.uniform(0, 2))


def cleaner_cleaning(id):
    print(f'Cleaner {id}')
    time.sleep(random.uniform(0, 2))


def guest_waiting():
    time.sleep(random.uniform(0, 2))


def guest_partying(id):
    print(f'Guest {id}')
    time.sleep(random.uniform(0, 1))


def cleaner(id: int, start_time: float, room_lock, cleaned_count):
    """
    do the following for TIME seconds
        cleaner will wait to try to clean the room (cleaner_waiting())
        get access to the room
        display message STARTING_CLEANING_MESSAGE
        Take some time cleaning (cleaner_cleaning())
        display message STOPPING_CLEANING_MESSAGE
    """
    while (time.time() - start_time) < TIME:
        cleaner_waiting()
        if room_lock.acquire():  # Clean the room when empty
            print(STARTING_CLEANING_MESSAGE)
            cleaner_cleaning(id)
            print(STOPPING_CLEANING_MESSAGE)
            cleaned_count.value += 1
            room_lock.release()


def guest(id: int, start_time: float, room_lock, light_lock, guest_number, party_count):
    """
    do the following for TIME seconds
        guest will wait to try to get access to the room (guest_waiting())
        get access to the room
        display message STARTING_PARTY_MESSAGE if this guest is the first one in the room
        Take some time partying (guest_partying())
        display message STOPPING_PARTY_MESSAGE if the guest is the last one leaving in the room
    """
    while (time.time() - start_time) < TIME:
        guest_waiting()
        # If the room is empty, start a party!
        if room_lock.acquire(blocking=False):
            print(STARTING_PARTY_MESSAGE)
            light_lock.acquire()  # Turn on the lights, letting everyone know a party is on!
            party_count.value += 1

        # IF lights are on (meaning party), join party!
        # otherwise, it is being cleaned
        if not light_lock.acquire(blocking=False):
            guest_number.value += 1
            guest_partying(id)
            guest_number.value -= 1

            if guest_number.value == 1:  # Am I the last one?
                light_lock.release()
                print(STOPPING_PARTY_MESSAGE)
                room_lock.release()
        else:
            light_lock.release()  # Room is still being cleaned


def main():
    # Start time of the running of the program.
    start_time = time.time()

    # Locks control room access and when the light is one
    # staff "cleans" in darkness
    room_lock = mp.Manager().Lock()
    light_lock = mp.Manager().Lock()

    # Keep track of the guest number so that we may know
    # who the last guest is
    guest_number = mp.Manager().Value('int', 0)

    # I don't know why the Rubric says "lists used to count ..."
    # when Manager().Value can do the counting for us.
    # It is a process-safe int so it will work better than a list
    cleaned_count = mp.Manager().Value('int', 0)
    party_count = mp.Manager().Value('int', 0)

    # Create Processes
    processes: list[mp.Process] = []
    for i in range(CLEANING_STAFF):
        processes.append(mp.Process(target=cleaner,
                                    args=(i + 1, start_time, room_lock, cleaned_count)))

    for i in range(HOTEL_GUESTS):
        processes.append(mp.Process(target=guest,
                                    args=(i + 1, start_time, room_lock, light_lock,
                                          guest_number, party_count)))

    # Start processes
    for p in processes:
        p.start()

    # Wait for everything to finish before moving on
    for p in processes:
        p.join()

    # Results
    print(
        f'Room was cleaned {cleaned_count.value} times, there were {party_count.value} parties')


if __name__ == '__main__':
    main()
