//
//  testHowitzer.h
//  Lab08
//
//  Created by Journey Curtis on 3/5/22.
//

#ifndef testHowitzer_h
#define testHowitzer_h

#include "position.h"
#include "howitzer.h"
#include <cassert>

//const float M_PI = 3.14159;


class TestHowitzer {
public:
   void run() {
      std::cout << "testHowitzer started.\n";
      testDefaultCreation();
      testNonDefaultCreation();
      testRotateLeft();
      testRotateRight();
      testRotateUp();
      testRotateDown();
      testFire();
      std::cout << "testHowitzer passed!\n" << std::endl;
   }
   
   // Test 1
   void testDefaultCreation() const {
      Howitzer h;
      Position p;
      
      assert(h.getPosition()->getMetersX() == p.getMetersX());
      assert(h.getPosition()->getMetersY() == p.getMetersY());
      assert(h.getAngle() == 0);
      assert(h.getAge() == 0);
      std::cout << "Test 1 passed!" << std::endl;
   }
   
   // Test 2
   void testNonDefaultCreation() const {
      Position p(100, 100);
      Howitzer h(p);
      
      assert(h.getPosition()->getMetersX() == p.getMetersX());
      assert(h.getPosition()->getMetersY() == p.getMetersY());
      assert(h.getAngle() == 0);
      assert(h.getAge() == 0);
      std::cout << "Test 2 passed!" << std::endl;
   }
   
   // Test 3
   void testRotateLeft() {
      Howitzer h;
      
      // -90 degree boundary test
      h.setAngle(-M_PI/2);
      h.rotateLeft();
      assert(h.getAngle() == (-M_PI/2));
      
      // -45 degree test
      h.setAngle(-M_PI/4);
      h.rotateLeft();
      assert(h.getAngle() == ((-M_PI/4) - .05));
      
      // 0 degree test
      h.setAngle(0);
      h.rotateLeft();
      assert(h.getAngle() == -.05);
      
      // 45 degree boundary test
      h.setAngle(M_PI/4);
      h.rotateLeft();
      assert(h.getAngle() == ((M_PI/4) - .05));
      
      // 90 degree boundary test
      h.setAngle(M_PI/2);
      h.rotateLeft();
      assert(h.getAngle() == ((M_PI/2) - .05));
      std::cout << "Test 3 passed!" << std::endl;
   }
   
   //test 4
   void testRotateRight() {
      Howitzer h;
      
      // -90 degree boundary test
      h.setAngle(-M_PI/2);
      h.rotateRight();
      assert(h.getAngle() == ((-M_PI/2) + .05));
      
      // -45 degree test
      h.setAngle(-M_PI/4);
      h.rotateRight();
      assert(h.getAngle() == ((-M_PI/4) + .05));
      
      // 0 degree test
      h.setAngle(0);
      h.rotateRight();
      assert(h.getAngle() == .05);
      
      // 45 degree boundary test
      h.setAngle(M_PI/4);
      h.rotateRight();
      assert(h.getAngle() == ((M_PI/4) + .05));
      
      // 90 degree boundary test
      h.setAngle(M_PI/2);
      h.rotateRight();
      assert(h.getAngle() == ((M_PI/2)));
      std::cout << "Test 4 passed!" << std::endl;
   }
   
   // test 5
   void testRotateUp() {
      Howitzer h;
      
      // -90 degree boundary test
      h.setAngle(-M_PI/2);
      h.rotateUp();
      assert(h.getAngle() == ((-M_PI/2) + .003));
      
      // -45 degree test
      h.setAngle(-M_PI/4);
      h.rotateUp();
      assert(h.getAngle() == ((-M_PI/4) + .003));
      
      // -.002 degree test
      h.setAngle(-.002);
      h.rotateUp();
      assert(h.getAngle() == 0);
      
      // 0 degree test
      h.setAngle(0);
      h.rotateUp();
      assert(h.getAngle() == 0);
      
      // .002 degree test
      h.setAngle(.002);
      h.rotateUp();
      assert(h.getAngle() == 0);
      
      // 45 degree boundary test
      h.setAngle(M_PI/4);
      h.rotateUp();
      assert(h.getAngle() == ((M_PI/4) - .003));
      
      // 90 degree boundary test
      h.setAngle(M_PI/2);
      h.rotateUp();
      assert(h.getAngle() == ((M_PI/2) - .003));
      std::cout << "Test 5 passed!" << std::endl;
   }
   
   // test 6
   void testRotateDown() {
      Howitzer h;
      
      // -90 degree boundary test
      h.setAngle(-M_PI/2);
      h.rotateDown();
      assert(h.getAngle() == ((-M_PI/2)));
      
      // Near left boundary test
      h.setAngle((-M_PI/2) + .002);
      h.rotateDown();
      assert(h.getAngle() == ((-M_PI/2)));
      
      // -45 degree test
      h.setAngle(-M_PI/4);
      h.rotateDown();
      assert(h.getAngle() == ((-M_PI/4) - .003));
      
      // 0 degree test
      h.setAngle(0);
      h.rotateDown();
      assert(h.getAngle() == 0);
      
      // 45 degree boundary test
      h.setAngle(M_PI/4);
      h.rotateDown();
      assert(h.getAngle() == ((M_PI/4) + .003));
      
      // Near right boundary test
      h.setAngle((M_PI/2) - .002);
      h.rotateDown();
      assert(h.getAngle() == ((M_PI/2)));
      
      // 90 degree boundary test
      h.setAngle(M_PI/2);
      h.rotateDown();
      assert(h.getAngle() == ((M_PI/2)));
      std::cout << "Test 6 passed!" << std::endl;
   }
   
   // test 7
   void testFire() {
      Howitzer h;
      
      h.fire();
      assert(h.getAge() == 2);
      std::cout << "Test 7 passed!" << std::endl;
   }
};

#endif /* testHowitzer_h */
