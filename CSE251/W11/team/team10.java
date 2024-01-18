
/************************************
Course: cse 251
File: team1.java
Week: week 11 - team activity 1

Instructions:

- Main contains an array of 1,000 random values.  You will be creating
  threads to process this array.  If you find a prime number, display
  it to the console.

- DON'T copy/slice the array in main() for each thread.

Part 1:
- Create a class that is a sub-class of Thread.
- create 4 threads based on this class you created.
- Divide the array among the threads.

Part 2:
- Create a class on an interface or Runnable
- create 4 threads based on this class you created.
- Divide the array among the threads.

Part 3:
- Modify part1 or part 2 to handle any size array and any number
  of threads.

************************************/
import java.util.Random;
import java.lang.Math;

class Main {

  static boolean isPrime(int n) {
    // Corner cases
    if (n <= 1)
      return false;
    if (n <= 3)
      return true;

    // This is checked so that we can skip
    // middle five numbers in below loop
    if (n % 2 == 0 || n % 3 == 0)
      return false;

    for (int i = 5; i * i <= n; i = i + 6)
      if (n % i == 0 || n % (i + 2) == 0)
        return false;

    return true;
  }

  public static void main(String[] args) {
    System.out.println("Hello world!");

    // create instance of Random class
    Random rand = new Random();

    int count = 1000;
    int[] array = new int[count];
    for (int i = 0; i < count; i++) {
      array[i] = Math.abs(rand.nextInt());
    }

    int threadCount = 8;
    int remainder = count % threadCount;

    int[] todo = new int[threadCount];
    for (int i = 0; i < threadCount; i++) {
      todo[i] = count / threadCount;
    }

    int index = 0;
    while (remainder > 0) {
      todo[index++] += 1;
      remainder -= 1;
      if (index > todo.length) {
        index = 0;
      }
    }

    class FindPrime1 extends Thread {
      int[] array;
      int start;
      int end;

      FindPrime1(int[] array, int start, int end) {
        super();
        this.array = array;
        this.start = start;
        this.end = end;
      }

      @Override
      public void run() {
        for (int i = start; i < end; i++) {
          if (isPrime(array[i])) {
            System.out.println("FindPrime1: " + Integer.toString(array[i]) + " is prime");
          }
        }
      }
    }

    class FindPrime2 implements Runnable {
      int[] array;
      int start;
      int end;

      FindPrime2(int[] array, int start, int end) {
        this.array = array;
        this.start = start;
        this.end = end;
      }

      public void run() {
        for (int i = start; i < end; i++) {
          if (isPrime(array[i])) {
            System.out.println("FindPrime2: " + Integer.toString(array[i]) + " is prime");
          }
        }
      }
    }

    FindPrime1[] threads1 = new FindPrime1[threadCount];
    Thread[] threads2 = new Thread[threadCount];
    int start = 0;
    for (int i = 0; i < threadCount; i++) {
      int end = start + todo[index];
      threads1[i] = new FindPrime1(array, start, end);
      threads2[i] = new Thread(new FindPrime2(array, start, end));
      threads1[i].start();
      threads2[i].start();
      start = end;
    }

    for (FindPrime1 t : threads1) {
      try {
        t.join();
      } catch (Exception e) {
        System.out.println(e);
      }
    }

    for (Thread t : threads2) {
      try {
        t.join();
      } catch (Exception e) {
        System.out.println(e);
      }
    }
  }
}