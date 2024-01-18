'''
Requirements
1. Write a multithreaded program that calls a local web server. The web server is 
   provided to you. It will return data about the Star Wars movies.
2. You will make 94 calls to the web server, using 94 threads to get the data.
3. Using a new thread each time, obtain a list of the characters, planets, 
   starships, vehicles, and species of the sixth Star War movie.
3. Use the provided print_film_details function to print out the data 
   (should look exactly like the "sample_output.txt file).
   
Questions:
1. Is this assignment an IO Bound or CPU Bound problem (see https://stackoverflow.com/questions/868568/what-do-the-terms-cpu-bound-and-i-o-bound-mean)?
    > IO Bound. This is because the data is small and the logic isn't complex. Most of the work is occuring through api calls. This would be more apparant
    > if we were using the swapi website (which is where all the data in the dat.txt file is from)
2. Review dictionaries (see https://isaaccomputerscience.org/concepts/dsa_datastruct_dictionary). How could a dictionary be used on this assignment to improve performance?
    > Well I used a dictionary to handle everything in this assignment. Dictionaries are focused on speed when you know the keys. 
    > In this case we know the keys! Dictionaries also convert to and from json easily as they practically are the same format.
'''


from datetime import datetime, timedelta
import time
import requests
import json
import threading
import multiprocessing as mp

# Const Values
TOP_API_URL = 'http://127.0.0.1:8790'
CPU_COUNT = mp.cpu_count()

# globals
call_count = 0
characters = []
planets = []
starships = []
vehicles = []
species = []

class Request_Thread(threading.Thread):
    def __init__(self, url, lock: threading.Lock):
        threading.Thread.__init__(self)
        self.url = url
        self.response = {}
        self.lock = lock

    def run(self):
        global call_count

        # lock the global variable adjustment
        # so we don't have any issues
        self.lock.acquire()
        call_count += 1
        self.lock.release()

        response = requests.get(self.url)
        if response.status_code == 200:
            self.response = response.json()
        else:
            print('url call response = ', response.status_code)


def get_details(url: str):
    response = requests.get(url)

    if response.status_code == 200:
        return response.json()
    else:
        print('url call response = ', response.status_code)

def add_character(c):
    characters.append(c)
    global call_count
    call_count += 1

def add_planet(p):
    planets.append(p)
    global call_count
    call_count += 1

def add_starship(s):
    starships.append(s)
    global call_count
    call_count += 1

def add_vehicle(v):
    vehicles.append(v)
    global call_count
    call_count += 1

def add_species(s):
    species.append(s)
    global call_count
    call_count += 1


def print_film_details(film, chars, planets, starships, vehicles, species):
    '''
    Print out the film details in a formatted way
    '''

    def display_names(title, name_list):
        print('')
        print(f'{title}: {len(name_list)}')
        names = sorted([item["name"] for item in name_list])
        print(str(names)[1:-1].replace("'", ""))

    print('-' * 40)
    print(f'Title   : {film["title"]}')
    print(f'Director: {film["director"]}')
    print(f'Producer: {film["producer"]}')
    print(f'Released: {film["release_date"]}')

    display_names('Characters', chars)
    display_names('Planets', planets)
    display_names('Starships', starships)
    display_names('Vehicles', vehicles)
    display_names('Species', species)


def main():
    # Start a timer
    begin_time = time.perf_counter()

    print('Starting to retrieve data from the server')

    # Need the lock for our global counter
    lock = threading.Lock()

    # Start off by getting the directory
    directory_request = Request_Thread(TOP_API_URL, lock)

    directory_request.start()
    directory_request.join()

    # Use this directory to get details
    # dictionary containing the url according the topic
    # topics: people, planets, films, species, vehicles, starships
    directory_urls = directory_request.response

    # I had to do this as there is a difference between the directory and film
    # returns. Directory returns "people" as a key while films return "characters"
    # This is an issue that could be easily resolved in the .txt file
    directory_urls["characters"] = directory_urls["people"]
    directory_urls.pop("people")

    # Get the movie details
    movie_directory_request = Request_Thread(directory_urls["films"] + '6', lock)
    movie_directory_request.start()

    movie_directory_request.join()
    film = movie_directory_request.response

    # Iterates through each key and determines if it is one of the sections that needs
    # additional info (meaning the key also has a matching one in the directory)
    pool = mp.Pool(CPU_COUNT)

    for url in film["characters"]:
        pool.apply_async(get_details, args= (url,), callback=add_character)
    
    for url in film["planets"]:
        pool.apply_async(get_details, args= (url,), callback=add_planet)

    for url in film["starships"]:
        pool.apply_async(get_details, args= (url,), callback=add_starship)

    for url in film["vehicles"]:
        pool.apply_async(get_details, args= (url,), callback=add_vehicle)

    for url in film["species"]:
        pool.apply_async(get_details, args= (url,), callback=add_species)

    # finish pool before going on
    pool.close()
    pool.join()

    print(film)
    # display the film details
    print_film_details(film, characters, planets, starships, vehicles, species)

    print(f'There were {call_count} calls to the server')
    total_time = time.perf_counter() - begin_time
    total_time_str = "{:.2f}".format(total_time)
    print(f'Total time = {total_time_str} sec')

    # If you do have a slow computer, then put a comment in your code about why you are changing
    # the total_time limit. Note: 90+ seconds means that you are not doing multithreading
    assert total_time < 15, "Unless you have a super slow computer, it should not take more than 15 seconds to get all the data."
    assert call_count == 94, "It should take exactly 94 threads to get all the data"


if __name__ == "__main__":
    main()
