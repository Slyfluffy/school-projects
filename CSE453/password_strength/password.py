import math
import re
# import string


def compute_num_combinations(password: str):
    """Computes the possible number of calculations"""
    # possible = n^m (size of alphabet exponented by password length)
    n = 0
    m = len(password)

    # Determine the size of the alphabet by seeing what the password is composed of
    d = re.search("[0-9]", password)
    upper = re.search("[A-Z]", password)
    lower = re.search("[a-z]", password)

    if d != None:  # If any digits exist
        n += 10  # Add digit alphabet size
    if upper != None:  # If uppercases exist
        n += 26  # Add size of uppercase alphabet
    if lower != None:  # If lowercases exist
        n += 26  # Add size of lowercase alphabet
    if not password.isalnum():  # Any special characters?
        n += 33  # Add all possible characters

    return n ** m  # n^m


def compute_bit_strength(password: str):
    """Calculates and returns the bit strength of password"""
    # bits = log2(n^m) or log2(compute_num_combinations(password))
    # Return a whole number as there are no partial bits
    return int(math.log2(compute_num_combinations(password)))


def test_password():
    password = input('\nWhat is your password? ')

    # Determine combinations and display them
    combinations = compute_num_combinations(password)
    print(f'\tThere are {combinations} combinations')

    bit_strength = compute_bit_strength(password)
    print(f'\tThat is equivalent to a key of {bit_strength} bits\n')


def menu():
    done = False
    while not done:
        print('---------- MENU -----------')
        print('Please select an option:')
        print('   1. Test password strength')
        print('   2. Exit')
        selection = input('> ')
        if selection == '1':
            test_password()
        elif selection == '2':
            done = True
        else:
            print('\nInvalid option. Please try again\n')


menu()
