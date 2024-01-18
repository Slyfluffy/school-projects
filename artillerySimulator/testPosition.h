/***********************************************************************
 * Header File:
 *    Test Position : Test the Position class
 * Author:
 *    Br. Helfrich
 * Summary:
 *    All the unit tests for Position
 ************************************************************************/


#pragma once

#include <iostream>
#include "position.h"
#include <cassert>

using namespace std;

class TestPosition
{
public:
   void run()
   {
      test_Position_Default();
      test_Position_Nondefault();
      test_Position_Copy();
      test_Position_Assign();
      test_Position_SetPixels();
      test_Position_SetMeters();
      test_Position_AddPixels();
      test_Position_AddMeters();
   }
   
   void test_Position_Default() const
   {
      // setup
      // exercise
      Position pos;
      pos.setZoom(40);
      // verify
      assert(pos.getPixelsX() == 0.0);
      assert(pos.getPixelsY() == 0.0);
      assert(pos.getMetersX() == 0.0);
      assert(pos.getMetersY() == 0.0);
   }  // teardown

   void test_Position_Nondefault() const
   {
      // setup
      // exercise
      Position pos(120.0, 360.0);
      pos.setZoom(40);
      // verify
      assert(pos.getPixelsX() == 3.0);
      assert(pos.getPixelsY() == 9.0);
      assert(pos.getMetersX() == 120.0);
      assert(pos.getMetersY() == 360.0);
   }  // teardown

   void test_Position_Copy() const
   {
      // setup
      // exercise
      Position pos1(480.0, 240.0);
      pos1.setZoom(40);
      Position pos2(pos1);
      // verify
      assert(pos2.getPixelsX() == 12.0);
      assert(pos2.getPixelsY() == 6.0);
      assert(pos2.getMetersX() == 480.0);
      assert(pos2.getMetersY() == 240.0);
   }  // teardown

   void test_Position_Assign() const
   {
      // setup
      Position pos1(240.0, 480.0);
      // exercise
      Position pos2 = pos1;
      // verify
      assert(pos2.getPixelsX() == 6.0);
      assert(pos2.getPixelsY() == 12.0);
      assert(pos2.getMetersX() == 240.0);
      assert(pos2.getMetersY() == 480.0);
   }  // teardown
   
   void test_Position_SetMeters() const
   {
      // setup
      Position pos;
      // exercise
      pos.setMetersX(4800.0);
      pos.setMetersY(2400.0);
      // verify
      assert(pos.getPixelsX() == 120.0);
      assert(pos.getPixelsY() == 60.0);
      assert(pos.getMetersX() == 4800.0);
      assert(pos.getMetersY() == 2400.0);
   }  // teardown
 
   void test_Position_SetPixels() const
   {
      // setup
      Position pos;
      // exercise
      pos.setPixelsX(60.0);
      pos.setPixelsY(120.0);
      // verify
      assert(pos.getPixelsX() == 60.0);
      assert(pos.getPixelsY() == 120.0);
      assert(pos.getMetersX() == 2400.0);
      assert(pos.getMetersY() == 4800.0);
   }  // teardown
   
   void test_Position_AddMeters() const
   {
      // setup
      Position pos(80, 160);
      // exercise
      pos.addMetersX(-40.0);
      pos.addMetersY(80.0);
      // verify
      assert(pos.getPixelsX() == 1.0);
      assert(pos.getPixelsY() == 6.0);
      assert(pos.getMetersX() == 40.0);
      assert(pos.getMetersY() == 240.0);
   }  // teardown
   
   void test_Position_AddPixels() const
   {
      // setup
      Position pos(200, 400);
      // exercise
      pos.addPixelsX(2);
      pos.addPixelsY(3);
      // verify
      assert(pos.getPixelsX() == 7.0);
      assert(pos.getPixelsY() == 13.0);
      assert(pos.getMetersX() == 280.0);
      assert(pos.getMetersY() == 520.0);
   }  // teardown
};
