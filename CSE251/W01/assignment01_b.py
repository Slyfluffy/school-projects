import threading

# Object that sums up every number up to a number passed to the constructor
class Sum_Calculator(threading.Thread):
    # constructor
    def __init__(self, number):
        super().__init__()

        self.sum = 0
        self.number = number

    def run(self):
        for i in range(1, self.number):
            self.sum += i

def main():
    # Thread object 1 testing 10
    sum_calculator_1 = Sum_Calculator(10)
    sum_calculator_1.start()

    # Thread object 2 testing 13
    sum_calculator_2 = Sum_Calculator(13)
    sum_calculator_2.start()

    # Thread object 3 testing 17
    sum_calculator_3 = Sum_Calculator(17)
    sum_calculator_3.start()

    # Use join first so we can ensure everything is complete before the assert
    sum_calculator_1.join()
    assert sum_calculator_1.sum == 45, f'The sum should equal 45 but instead was {sum_calculator_1.sum}'

    sum_calculator_2.join()
    assert sum_calculator_2.sum == 78, f'The sum should equal 78 but instead was {sum_calculator_2.sum}'

    sum_calculator_3.join()
    assert sum_calculator_3.sum == 136, f'The sum should equal 136 but instead was {sum_calculator_3.sum}'


if __name__ == '__main__':
    main()
    assert threading.active_count() == 1
    print("DONE")
