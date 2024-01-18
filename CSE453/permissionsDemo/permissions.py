statement = 'This is a Hello World! How are you?'

def read_write():
    global statement
    print(f'This is what the statement currently says:\n\t{statement}')
    selection = input(f'Do you want to change it Y/N?')
    if selection == "Y":
        local_statement = input('What do you want it to say? ')
        statement = local_statement
        print(f'The statement now reads: {statement}')


def options_menu():
    
    done = False
    while not done:
        print("-- MENU --")
        print("Select which app you want")
        print("   1. ReadMobile")
        print("   2. WriteABookMobile")
        print("   3. Done")
        option = input("> ")
        if option == "1":
            print(f'\n{statement}\n')
        elif option == "2":
            read_write()
        elif option == "3":
            done = True

options_menu()