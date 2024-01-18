"""
   LAB 06: SQL injection
   Journey Curtis, Sailor Pope, and Anita Woodford
"""

import re


def read_file(path: str):
    # Read a file that contains index or test case
    # Used to read test files, nothing else
    list = []
    file = open(path)
    for line in file:
        list.append(line.strip())

    file.close()
    return list


def get_tests(index_file: str):
    # Get all the tests according to the index_file
    tests = []
    file_paths = read_file(index_file)
    for path in file_paths:
        tests.append(read_file(path))

    return tests


def gen_query(username: str, password: str):
    return f"SELECT {username} FROM users where password == {password}"


def gen_query_weak(username: str, password: str):
    black_list = read_file("./blacklist.txt")

    for item in black_list:
        compiled = re.compile(re.escape(item), re.IGNORECASE)
        username = compiled.sub("", username)
        password = compiled.sub("", password)

    return "SELECT " + username + " FROM users WHERE password == " + password + ";"


def gen_query_strong(username: str, password: str):
    white_list = read_file("./whitelist.txt")
    dict = {}

    for i in range(0, len(white_list), 2):
        dict[white_list[i]] = white_list[i+1]

    if username in dict:
        if password == dict[username]:
            return "SELECT " + username + " FROM users WHERE passwords == " + password + ";"

    return "empty sql statement"


def display_results(username: str, password: str, sql: str):
    print(f"Username entered: {username}")
    print(f"Password entered: {password}")
    print(f"sql result: {sql}")


def get_gen_query_type():
    while True:
        print("-- Manual test --")
        print(" Which query test do you want to run?")
        print("  1. gen_query (vulnerable)")
        print("  2. gen_query_weak")
        print("  3. gen_query_strong")

        selection = input("> ")
        if selection == "1":
            return 1
        elif selection == "2":
            return 2
        elif selection == "3":
            return 3
        else:
            print("\nInvalid argument. Please enter one of the valid options")


def run_manual_test():
    selection = get_gen_query_type()

    username = input("Please enter your username: ")
    password = input("Please enter your password: ")

    sql = "sql"
    if selection == 1:
        sql = gen_query(username, password)
    elif selection == 2:
        sql = gen_query_weak(username, password)
    else:
        sql = gen_query_strong(username, password)

    display_results(username, password, sql)


def run_automated_test(test_name: str, test_type: int, index_file: str):
    credentials = get_tests(index_file)
    usernames = credentials[0]
    passwords = credentials[1]

    print(f"{test_name} Tests")
    if test_type == 1:
        for i in range(0, len(usernames)):
            print(gen_query(usernames[i], passwords[i]))
    elif test_type == 2:
        for i in range(0, len(usernames)):
            print(gen_query_weak(usernames[i], passwords[i]))
    elif test_type == 3:
        for i in range(0, len(usernames)):
            print(gen_query_strong(usernames[i], passwords[i]))


def run_gen_query_tests():
    print("")
    run_automated_test("Valid", 1, "./valid/index.txt")
    print("")
    run_automated_test("Union", 1, "./union/index.txt")
    print("")
    run_automated_test("Tautology", 1, "./tautology/index.txt")
    print("")
    run_automated_test("Comment", 1, "./comment/index.txt")
    print("")
    run_automated_test("add_state", 1, "./add_state/index.txt")


def run_gen_query_weak_tests():
    print("")
    run_automated_test("Valid", 2, "./valid/index.txt")
    print("")
    run_automated_test("Union", 2, "./union/index.txt")
    print("")
    run_automated_test("Tautology", 2, "./tautology/index.txt")
    print("")
    run_automated_test("Comment", 2, "./comment/index.txt")
    print("")
    run_automated_test("add_state", 2, "./add_state/index.txt")


def run_gen_query_strong_tests():
    print("")
    run_automated_test("Valid", 3, "./valid/index.txt")
    print("")
    run_automated_test("Union", 3, "./union/index.txt")
    print("")
    run_automated_test("Tautology", 3, "./tautology/index.txt")
    print("")
    run_automated_test("Comment", 3, "./comment/index.txt")
    print("")
    run_automated_test("add_state", 3, "./add_state/index.txt")


def display_menu():
    print("\n\n-- SQL Injection --")
    print("   1: Manual Test")
    print("   2: Run genQuery tests")
    print("   3: Run genQueryWeak tests")
    print("   4: Run genQueryStrong tests")
    print("   5: exit\n")


def main_menu():
    done = False
    while not done:  # Go until the user is done
        display_menu()
        selection = input("> ")
        if selection == "1":
            run_manual_test()
        elif selection == "2":
            run_gen_query_tests()
        elif selection == "3":
            run_gen_query_weak_tests()
        elif selection == "4":
            run_gen_query_strong_tests()
        elif selection == "5":
            done = True
        else:
            print("\nInvalid argument. Please enter one of the valid options")


main_menu()
