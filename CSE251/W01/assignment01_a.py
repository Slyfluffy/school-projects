import threading

# Calculates the sum of all numbers from 1 up to the passed number
def calculate_sum(number, list, index):
    sum = 0
    for i in range(1, number):
        sum += i

    list[index] = sum

def main():
    results = [0] * 3

    # Thread 1 testing 10
    t1 = threading.Thread(calculate_sum(10, results, 0))
    t1.start()

    # Thread 2 testing 13
    t2 = threading.Thread(calculate_sum(13, results, 1))
    t2.start()

    # Thread 3 testing 17
    t3 = threading.Thread(calculate_sum(17, results, 2))
    t3.start()

    # Join first so that we can ensure all work is done before we use assert
    t1.join()
    assert results[0] == 45, f'The sum should equal 45 but instead was {results[0]}'

    t2.join()
    assert results[1] == 78, f'The sum should equal 78 but instead was {results[1]}'

    t3.join()
    assert results[2] == 136, f'The sum should equal 136 but instead was {results[2]}'

if __name__ == '__main__':
    main()
    print("DONE")