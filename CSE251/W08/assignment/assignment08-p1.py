'''
Requirements
1. Create a recursive program that finds the solution path for each of the provided mazes.
'''

import math
from screen import Screen
from maze import Maze
import cv2
import sys

SCREEN_SIZE = 800
COLOR = (0, 0, 255)


def is_end_found(maze: Maze, solution_path: list[tuple[int, int]]):
    # We only need the x and y of the last item in the path
    if len(solution_path) > 0:
        x = solution_path[len(solution_path) - 1][0]
        y = solution_path[len(solution_path) - 1][1]
        if maze.at_end(x, y):
            return True  # We have found a path to the end

    return False  # Not the finished path


def find_maze_end(maze: Maze, solution_path: list[tuple[int, int]], row, col):
    # Only go through all the work if we have not found the path
    possible = maze.get_possible_moves(row, col)

    # Go through possible options if they exist
    if len(possible) > 0 and not is_end_found(maze, solution_path):
        for x, y in possible:
            # For every possible move, move and add position to path if end is not found
            if maze.can_move_here(x, y) and not is_end_found(maze, solution_path):
                maze.move(x, y, COLOR)
                solution_path.append((x, y))

                # Only continue recursion if the end has not been found
                if not is_end_found(maze, solution_path):
                    find_maze_end(maze, solution_path, x, y)
    else:
        # Go through the work of more recursion if the end has not been found
        if not is_end_found(maze, solution_path):
            solution_path.remove((row, col))
            maze.restore(row, col)
            prev_row = solution_path[len(solution_path) - 1][0]
            prev_col = solution_path[len(solution_path) - 1][1]
            find_maze_end(maze, solution_path, prev_row, prev_col)


def solve(maze: Maze):
    """ Solve the maze. The path object should be a list (x, y) of the positions 
        that solves the maze, from the start position to the end position. """

    solution_path = []

    # Remember that an object is passed by reference, so you can pass in the
    # solution_path object, modify it, and you won't need to return it from
    # your recursion function
    start_pos = maze.get_start_pos()
    maze.move(start_pos[0], start_pos[1], COLOR)
    find_maze_end(maze, solution_path, start_pos[0], start_pos[1])

    return solution_path


def get_solution_path(filename):
    # create a Screen Object that will contain all of the drawing commands
    screen = Screen(SCREEN_SIZE, SCREEN_SIZE)
    screen.background((255, 255, 0))

    maze = Maze(screen, SCREEN_SIZE, SCREEN_SIZE, filename)

    solution_path = solve(maze)

    print(f'Number of drawing commands for = {screen.get_command_count()}')

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

    return solution_path


def find_paths():
    files = ('verysmall.bmp', 'verysmall-loops.bmp',
             'small.bmp', 'small-loops.bmp',
             'small-odd.bmp', 'small-open.bmp', 'large.bmp', 'large-loops.bmp')

    print('*' * 40)
    print('Part 1')
    for filename in files:
        print()
        print(f'File: {filename}')
        solution_path = get_solution_path(filename)
        print(f'Found path has length          = {len(solution_path)}')
    print('*' * 40)


def main():
    # prevent crashing in case of infinite recursion
    sys.setrecursionlimit(50000)
    find_paths()


if __name__ == "__main__":
    main()
