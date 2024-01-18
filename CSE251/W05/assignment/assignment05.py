'''
Requirements
1. Using multiple threads, put cars onto a shared queue, with one or more thread consuming
   the items from the queue and one or more thread producing the items.
2. The size of queue should never exceed 10.
3. Do not call queue size to determine if maximum size has been reached. This means
   that you should not do something like this: 
        if q.size() < 10:
   Use the blocking semaphore function 'acquire'.
4. Produce a Plot of car count vs queue size (okay to use q.size since this is not a
   condition statement).
5. The number of cars produced by the manufacturer must equal the number of cars bought by the 
   dealership. Use necessary data objects (e.g., lists) to prove this. There is an assert in 
   main that must be used.
   
Questions:
1. How would you define a barrier in your own words?
   > A barrier is a tool that stops each thread that calls it. It only releases when a certain number
   > of threads have called it and are waiting.
2. Why is a barrier necessary in this assignment?
   > It ensured that our dealerships didn't stop selling cars until each manufacturer was done.
   > Using a barrier ensured we didn't send the "no more cars" signal until there really weren't any more.
'''

from datetime import datetime, timedelta
import time
import threading
import random

# Global Constants
MAX_QUEUE_SIZE = 10
SLEEP_REDUCE_FACTOR = 50

# NO GLOBAL VARIABLES!


class Car():
    """ This is the Car class that will be created by the manufacturers """

    # Class Variables
    car_makes = ('Ford', 'Chevrolet', 'Dodge', 'Fiat', 'Volvo', 'Infiniti', 'Jeep', 'Subaru',
                 'Buick', 'Volkswagen', 'Chrysler', 'Smart', 'Nissan', 'Toyota', 'Lexus',
                 'Mitsubishi', 'Mazda', 'Hyundai', 'Kia', 'Acura', 'Honda')

    car_models = ('A1', 'M1', 'XOX', 'XL', 'XLS', 'XLE', 'Super', 'Tall', 'Flat', 'Middle', 'Round',
                  'A2', 'M1X', 'SE', 'SXE', 'MM', 'Charger', 'Grand', 'Viper', 'F150', 'Town', 'Ranger',
                  'G35', 'Titan', 'M5', 'GX', 'Sport', 'RX')

    car_years = [i for i in range(1990, datetime.now().year)]

    def __init__(self):
        # Make a random car
        self.model = random.choice(Car.car_models)
        self.make = random.choice(Car.car_makes)
        self.year = random.choice(Car.car_years)

        # Sleep a little.  Last statement in this for loop - don't change
        time.sleep(random.random() / (SLEEP_REDUCE_FACTOR))

        # Display the car that has was just created in the terminal
        self.display()

    def display(self):
        print(f'{self.make} {self.model}, {self.year}')


class QueueTwoFiftyOne():
    """ This is the queue object to use for this assignment. Do not modify!! """

    def __init__(self):
        self.items = []
        self.max_size = 0

    def get_max_size(self):
        return self.max_size

    def put(self, item):
        self.items.append(item)
        if len(self.items) > self.max_size:
            self.max_size = len(self.items)

    def get(self):
        return self.items.pop(0)


class Manufacturer(threading.Thread):
    """ This is a manufacturer.  It will create cars and place them on the car queue """

    def __init__(self, manufacturer_id, car_count, sem_queue_size: threading.Semaphore,
                 sem_slots_available: threading.Semaphore, barrier: threading.Barrier,
                 queue: QueueTwoFiftyOne):
        threading.Thread.__init__(self)
        self.cars_to_produce = random.randint(200, 300)     # Don't change
        self.manufacturer_id = manufacturer_id
        self.car_count = car_count
        self.cars_produced = 0
        self.sem_queue_size = sem_queue_size
        self.sem_slots_available = sem_slots_available
        self.barrier = barrier
        self.queue = queue

    def run(self):
        for i in range(self.car_count):
            """
            create a car
            place the car on the queue
            signal the dealer that there is a car on the queue
            """
            self.sem_slots_available.acquire()  # acquire removes 1 from semaphore count
            self.queue.put(Car())
            self.cars_produced += 1
            # release adds 1 to the sempaphore count (adds 1 to the size)
            self.sem_queue_size.release()

        # Ensure all the other manufacturers are done first
        self.barrier.wait()

        # Manufacturer 1 signals that manufacturers are done making cars!
        if (self.manufacturer_id == 1):
            self.queue.put(None)
            self.sem_queue_size.release()


class Dealership(threading.Thread):
    """ This is a dealer that receives cars """

    def __init__(self, sem_queue_size: threading.Semaphore, sem_slots_available: threading.Semaphore,
                 lock: threading.Lock, queue: QueueTwoFiftyOne):
        threading.Thread.__init__(self)
        self.sem_queue_size = sem_queue_size
        self.sem_slots_available = sem_slots_available
        self.queue = queue
        self.cars_received = 0
        self.lock = lock

    def run(self):
        done = False
        while not done:
            """
            take the car from the queue
            signal the factory that there is an empty slot in the queue
            """
            self.sem_queue_size.acquire()  # Removes 1 from the size semaphore
            self.lock.acquire()
            if (self.queue.get() != None):
                self.cars_received += 1
            else:
                done = True
                # These will ensure the other dealerships know too
                self.queue.put(None)
                self.sem_queue_size.release()
            self.lock.release()
            self.sem_slots_available.release()  # add 1 to the slots_available semaphore

            # Sleep a little after selling a car
            # Last statement in this for loop - don't change
            time.sleep(random.random() / (SLEEP_REDUCE_FACTOR))


def run_production(manufacturer_count, dealer_count):
    """ This function will do a production run with the number of
        manufacturers and dealerships passed in as arguments.
    """

    # Start a timer
    begin_time = time.perf_counter()

    # These semaphores serve as counters.
    sem_queue_size = threading.Semaphore(0)
    sem_slots_available = threading.Semaphore(MAX_QUEUE_SIZE)

    # Custom queue to be used
    car_queue = QueueTwoFiftyOne()

    # Lock to use whenever size is being checked
    lock = threading.Lock()

    # Barrier to ensure all dealerships are done before signaling no more cars
    barrier = threading.Barrier(manufacturer_count)

    # This is used to track the number of cars received by each dealer
    dealer_stats = list([0] * dealer_count)

    # This is used to track the number of cars produced by manufacturer
    manufacturer_stats = list([0] * manufacturer_count)

    # Prep manufacturers
    manufacturers = []
    for i in range(0, manufacturer_count):
        # random amount of cars to produce
        CARS_TO_PRODUCE = random.randint(200, 300)
        manufacturers.append(Manufacturer(i + 1, CARS_TO_PRODUCE, sem_queue_size,
                                          sem_slots_available, barrier, car_queue))

    # prep dealers
    dealers = []
    for i in range(0, dealer_count):
        dealers.append(Dealership(
            sem_queue_size, sem_slots_available, lock, car_queue))

    # Start threads
    for manufacturer in manufacturers:
        manufacturer.start()

    for dealer in dealers:
        dealer.start()

    # Finish threads before moving on
    for manufacturer in manufacturers:
        manufacturer.join()

    for dealer in dealers:
        dealer.join()

    # Get statistics
    for i in range(0, manufacturer_count):
        manufacturer_stats[i] = manufacturers[i].cars_produced

    for i in range(0, dealer_count):
        dealer_stats[i] = dealers[i].cars_received

    run_time = time.perf_counter() - begin_time

    # This function must return the following - only change the variable names, if necessary.
    # manufacturer_stats: is a list of the number of cars produced by each manufacturer,
    #                collect this information after the manufacturers are finished.
    return (run_time, car_queue.get_max_size(), dealer_stats, manufacturer_stats)


def main():
    """ Main function """

    # Use 1, 1 to get your code working like the previous assignment, then
    # try adding in different run amounts. You should be able to run the
    # full 7 run amounts.
    # runs = [(1, 1)]
    runs = [(1, 1), (1, 2), (2, 1), (2, 2), (2, 5), (5, 2), (10, 10)]
    for manufacturers, dealerships in runs:
        run_time, max_queue_size, dealer_stats, manufacturer_stats = run_production(
            manufacturers, dealerships)

        print(f'Manufacturers       : {manufacturers}')
        print(f'Dealerships         : {dealerships}')
        print(f'Run Time            : {run_time:.2f} sec')
        print(f'Max queue size      : {max_queue_size}')
        print(f'Manufacturer Stats  : {manufacturer_stats}')
        print(f'Dealer Stats        : {dealer_stats}')
        print('')

        # The number of cars produces needs to match the cars sold (this should pass)
        assert sum(dealer_stats) == sum(manufacturer_stats)


if __name__ == '__main__':
    main()
