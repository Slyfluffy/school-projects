'''
Purpose: Dining philosophers problem

Problem statement

Five silent philosophers sit at a round table with bowls of spaghetti. Forks
are placed between each pair of adjacent philosophers.

Each philosopher must alternately think and eat. However, a philosopher can
only eat spaghetti when they have both left and right forks. Each fork can be
held by only one philosopher and so a philosopher can use the fork only if it
is not being used by another philosopher. After an individual philosopher
finishes eating, they need to put down both forks so that the forks become
available to others. A philosopher can only take the fork on their right or
the one on their left as they become available and they cannot start eating
before getting both forks.  When a philosopher is finished eating, they think 
for a little while.

Eating is not limited by the remaining amounts of spaghetti or stomach space;
an infinite supply and an infinite demand are assumed.

The problem is how to design a discipline of behavior (a concurrent algorithm)
such that no philosopher will starve

Instructions:

        ***************************************************
        ** DO NOT search for a solution on the Internet, **
        ** your goal is not to copy a solution, but to   **
        ** work out this problem.                        **
        ***************************************************

- When a philosopher wants to eat, it will ask the waiter if it can.  If the waiter 
  indicates that a philosopher can eat, the philosopher will pick up each fork and eat.  
  There must not be an issue picking up the two forks since the waiter is in control of 
  the forks. When a philosopher is finished eating, it will inform the waiter that they
  are finished.  If the waiter indicates to a philosopher that they can not eat, the 
  philosopher will wait between 1 to 3 seconds and try again.

- You have Locks and Semaphores that you can use.
- Remember that lock.acquire() has an argument called timeout. This can be useful to not
  block when trying to acquire a lock.
- Philosophers need to eat for 1 to 3 seconds when they get both forks.  
  When the number of philosophers has eaten MAX_MEALS times, stop the philosophers
  from trying to eat and any philosophers eating will put down their forks when finished.
- Philosophers need to think (digest?) for 1 to 3 seconds when they are finished eating.  
- You want as many philosophers to eat and think concurrently.
- Design your program to handle N philosophers and N forks (minimum of 5 philosophers).
- Use threads for this problem.
- Provide a way to "prove" that each philosophers will not starve. This can be counting
  how many times each philosophers eat and display a summary at the end. Or, keeping track
  how long each philosopher is eating and thinking.
- Using lists for philosophers and forks will help you in this program.
  for example: philosophers[i] needs forks[i] and forks[i+1] to eat. Hint, they are
  sitting in a circle.
'''

import time
import threading
import random

PHILOSOPHERS = 5
MAX_MEALS = PHILOSOPHERS * 5

class Waiter:
    def __init__(self, forks: list[bool], lock: threading.Lock):
        self.forks = forks
        self.lock = lock
        self.meals_eaten = 0

    def is_dinner_over(self):
        """Determines if dinner is over (hit the maximum amount of meals)"""
        if self.meals_eaten < MAX_MEALS:
            return False
        
        return True
    
    def can_eat(self, p_num: int):
        """Determines if a philosopher (p_num) can eat or not"""
        fork_2_pos = 0
        if p_num + 1 < len(self.forks):
            fork_2_pos = p_num + 1

        self.lock.acquire()
        # At least half of the philosophers won't be able to eat
        # This should be the most efficient option, hence why it is first
        if not (self.forks[p_num] and self.forks[fork_2_pos] and not self.is_dinner_over()):
            self.lock.release()
            return False
        # Second most common option is the philosopher can eat. Second most optimized one
        elif self.forks[p_num] and self.forks[fork_2_pos] and not self.is_dinner_over():
            self.forks[p_num] = False
            self.forks[fork_2_pos] = False
            self.meals_eaten += 1
            self.lock.release()
            return True
        # Rare case when we have hit the MAX_MEALS
        else:
            self.lock.release()
            return False
        
    def done_eating(self, p_num: int):
        """Notifies the waiter that the philosopher (p_num) is done eating"""
        fork_2_pos = 0
        if p_num + 1 < len(self.forks):
            fork_2_pos = p_num + 1

        self.lock.acquire()
        self.forks[p_num] = True
        self.forks[fork_2_pos] = True
        self.lock.release()
    

class Philosopher(threading.Thread):
    def __init__(self, waiter: Waiter, p_num: int):
        threading.Thread.__init__(self)
        self.waiter = waiter
        self.p_num = p_num

        # Eating trackers
        self.meals_eaten = 0
        self.total_eat_time = 0

        # Think trackers
        self.think_count = 0
        self.total_think_time = 0
    
    def run(self):
        while not self.waiter.is_dinner_over():
            if self.waiter.can_eat(self.p_num):
                print(f'philosopher {self.p_num} is eating\n', sep="")
                eat_time = random.randint(1,3)
                time.sleep(eat_time) # Munch, Munch
                print(f'philosopher {self.p_num} is done eating\n', sep="")
                self.waiter.done_eating(self.p_num) # Philosopher is done!
                
                # Record time and increase meals_eating
                self.total_eat_time += eat_time
                self.meals_eaten += 1

            # Waiting/think time
            think_time = random.randint(1, 3)
            time.sleep(think_time) # I'm thinking...
            self.total_think_time += 1
            self.think_count += 1


def main():
    forks = [True] * PHILOSOPHERS
    lock = threading.Lock()
    waiter = Waiter(forks, lock)

    philosophers: list[Philosopher] = []
    for i in range(PHILOSOPHERS):
        philosophers.append(Philosopher(waiter, i))

    # Start all the philosophers
    for i in range(PHILOSOPHERS):
        philosophers[i].start()

    # Wait for them to all eat
    for i in range(PHILOSOPHERS):
        philosophers[i].join()

    # Print out statistics
    for i in range(PHILOSOPHERS):
        print(f'\nPhilosopher number: {philosophers[i].p_num}')
        print(f'       meals eaten: {philosophers[i].meals_eaten}')
        print(f'   total meal time: {philosophers[i].total_eat_time}')
        print(f'        think time: {philosophers[i].total_think_time}')

if __name__ == '__main__':
    main()
