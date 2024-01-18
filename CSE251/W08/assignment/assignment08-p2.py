'''
Requirements
1. Create a recursive, multithreaded program that finds the exit of each maze.
   
Questions:
1. It is not required to save the solution path of each maze, but what would
   be your strategy if you needed to do so?
   > I would have each thread have it's own path as a list. Once it reached a point it can 
   > no longer move, the list would then be placed on a queue. Once the end has been reached,
   > I would have a function that processes the queue and compile one list containing the
   > solution path.
2. Is using threads to solve the maze a depth-first search (DFS) or breadth-first search (BFS)?
   Which search is "better" in your opinion? You might need to define better. 
   (see https://stackoverflow.com/questions/20192445/which-procedure-we-can-use-for-maze-exploration-bfs-or-dfs)
   > Threads are better for a BFS approach.
   > Better in terms of finding the end the fastest? DFS
   > Better in terms of shortest route? BFS
   > My opinion? I tend to do mazes in races so DFS is better for me as it allows me to find
   > the end faster.
'''

import math
import threading
from screen import Screen
from maze import Maze
import sys
import cv2

SCREEN_SIZE = 800
COLOR = (0, 0, 255)
COLORS = (
    (0, 0, 255),
    (0, 255, 0),
    (255, 0, 0),
    (255, 150, 0),  # This color is difficult to see so I changed it
    (0, 255, 255),
    (255, 0, 255),
    (128, 0, 0),
    (128, 128, 0),
    (0, 128, 0),
    (128, 0, 128),
    (0, 128, 128),
    (0, 0, 128),
    (72, 61, 139),
    (143, 143, 188),
    (226, 138, 43),
    (128, 114, 250)
)

# Globals
current_color_index = 0
thread_count = 1
stop = False


def get_color():
    """ Returns a different color when called """
    global current_color_index
    if current_color_index >= len(COLORS):
        current_color_index = 0
    color = COLORS[current_color_index]
    current_color_index += 1
    return color


def solve_find_end(maze: Maze, pos: tuple[int, int], color):
    """ finds the end position using threads.  Nothing is returned """
    # When one of the threads finds the end position, stop all of them
    # TODO - add code here
    global stop
    global thread_count

    # Continue until the end is finished
    if not stop:
        # Move first before doing anything else (if we can)
        if maze.can_move_here(pos[0], pos[1]):
            maze.move(pos[0], pos[1], color)
        if maze.at_end(pos[0], pos[1]):
            stop = True # Let all the other threads know we are done

        possible = maze.get_possible_moves(pos[0], pos[1])

        # IF size == 1
        #    recurse normally
        # ELSE IF size > 1
        #    FOR every possible after 2
        #       recurse using a new thread and color
        if len(possible) == 1:
            solve_find_end(maze, possible[0], color)
        elif len(possible) > 1:
            threads = list[threading.Thread]()
            for i in range(1, len(possible)):
                t = threading.Thread(target=solve_find_end, args=[
                                     maze, possible[i], get_color()])
                threads.append(t)
                thread_count += 1

            for t in threads:
                t.start()

            solve_find_end(maze, possible[0], color)


def find_end(filename, delay):

    global current_color_index
    global thread_count
    global stop

    # create a Screen Object that will contain all of the drawing commands
    screen = Screen(SCREEN_SIZE, SCREEN_SIZE)
    screen.background((255, 255, 0))

    maze = Maze(screen, SCREEN_SIZE, SCREEN_SIZE, filename, delay=delay)

    solve_find_end(maze, maze.get_start_pos(), get_color())

    print(f'Number of drawing commands = {screen.get_command_count()}')
    print(f'Number of threads created  = {thread_count}')

    done = False
    speed = 1
    while not done:
        if screen.play_commands(speed):
            key = cv2.waitKey(0)
            if key == ord('+'):
                speed = max(0, speed - 1)
            elif key == ord('-'):
                speed += 1
            elif key != ord('p'):
                done = True
        else:
            done = True

    # reset globals
    current_color_index = 0
    thread_count = 1
    stop = False


def find_ends():
    files = (
        ('verysmall.bmp', True),
        ('verysmall-loops.bmp', True),
        ('small.bmp', True),
        ('small-loops.bmp', True),
        ('small-odd.bmp', True),
        ('small-open.bmp', False),
        ('large.bmp', False),
        ('large-loops.bmp', False)
    )

    print('*' * 40)
    print('Part 2')
    for filename, delay in files:
        print()
        print(f'File: {filename}')
        find_end(filename, delay)
    print('*' * 40)


def main():
    # prevent crashing in case of infinite recursion
    sys.setrecursionlimit(5000)
    find_ends()


if __name__ == "__main__":
    main()
