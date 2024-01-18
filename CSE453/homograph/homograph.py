"""
   LAB 05: Homograph
   Journey Curtis, Sailor Pope, and Anita Woodford
"""

import os
import re

HOMOGRAPH = "The paths are homographs.\n"
NONHOMOGRAPH = "The paths are non-homographs.\n"


def canonize_path(path: str):
    # Canonizes the path so they can be understood easily

    # Get the cwd as it is the basis for our "canon"
    cwd = os.getcwd()
    cwd_arr = cwd.split("\\")
    for item in cwd_arr:  # restore "/" to each section
        item = item + "/"

    path = path.replace("\\", "/")
    # Save filename so we can return it
    filename_start_index = path.rfind("/") + 1
    filename = ""
    if filename_start_index != -1:
        filename = path[filename_start_index:]
    else:
        filename = path

    # As long as path_arr isn't just the name,
    # Go through each item in path_arr
    # and adjust the cwd_arr as needed
    path_arr = []
    if filename != path:
        path_arr = path[:filename_start_index-1].split("/")
    else:
        path_arr.append(filename)

    # Handle paths that start with a folder/entire path
    r = re.match("[a-zA-Z]:", path_arr[0])
    if r is not None:
        cwd_arr.clear()

    for item in path_arr:
        if item == "..":
            cwd_arr.pop(len(cwd_arr) - 1)
        elif item != "." and item != filename:
            cwd_arr.append(item)

    path_to_file = ""
    for item in cwd_arr:
        path_to_file = path_to_file + item + "/"

    return (path_to_file.lower() + filename.lower())


def compare_paths(path1: str, path2: str):
    # Homograph function, compare the two paths and determine if they are the same
    if (path1 == path2):
        return True
    return False


def read_file(path: str):
    # Read a file that contains index or test case
    # Used to read test files, nothing else
    test_list = []
    file = open(path)
    for line in file:
        test_list.append(line.strip())

    file.close()
    return test_list


def get_tests(index_file: str):
    # Get all the tests according to the index_file
    tests = []
    file_paths = read_file(index_file)
    for path in file_paths:
        tests.extend(read_file(path))

    return tests


def run_tests(index_path: str):
    # run tests according to index path
    uncanon_test_list = get_tests(index_path)
    canon_test_list = []
    for i in range(len(uncanon_test_list)):
        canon_test_list.append(canonize_path(uncanon_test_list[i]))

    for i in range(0, len(canon_test_list), 2):
        print(f"{uncanon_test_list[i]} == {uncanon_test_list[i+1]}")
        if (compare_paths(canon_test_list[i], canon_test_list[i+1])):
            print(HOMOGRAPH)
        else:
            print(NONHOMOGRAPH)


def get_paths_from_user():
    # Gets the paths from the user
    paths = []

    done = False
    while not done:
        paths.append(input("Specify the first filename: "))
        paths.append(input("Specify the second filename: "))
        if (paths[0][0] == "/" or paths[0][0] == "\\" or
            paths[1][0] == "/" or paths[1][0] == "\\"):
            print("Invalid input: Do not start with '/' or '\\'. Please try again")
        else:
            done = True

    return paths


def handle_manual_test():
    # Handles manual testing
    # render paths (input from user)
    paths = get_paths_from_user()

    # Canonize paths
    paths[0] = canonize_path(paths[0])
    paths[1] = canonize_path(paths[1])

    # Compare paths and determine if homograph or not
    if compare_paths(paths[0], paths[1]):
        print(HOMOGRAPH)
    else:
        print(NONHOMOGRAPH)


def display_menu():
    print("\n\n-- Path Homograph --")
    print("   1: Manual Test")
    print("   2: Run non-homograph tests")
    print("   3: Run homograph tests")
    print("   4: exit\n")


def main_menu():
    done = False
    while not done:  # Go until the user is done
        display_menu()
        selection = input("> ")
        if selection == "1":
            handle_manual_test()
        elif selection == "2":
            run_tests("./non_homograph/test_index.txt")
        elif selection == "3":
            run_tests("./homograph/test_index.txt")
        elif selection == "4":
            done = True
        else:
            print("\nInvalid argument. Please enter one of the valid options")


main_menu()
