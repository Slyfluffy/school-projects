//
//  howitzer.cpp
//  Lab08
//
//  Created by Journey Curtis on 3/8/22.
//

#include "howitzer.h"

/************************************
 * HOWITZER :: ROTATELEFT
 * INPUTS  :: NONE
 * OUTPUTS :: NONE
 * Rotate the howitzer left until the 
 * left most angle is achieved.
 ***********************************/
void Howitzer::rotateLeft() {
   if ((angle > -(M_PI/2)) && (angle <= (M_PI/2)))
      angle -= .05;
   
   if (angle < -(M_PI/2)) // If it goes past left most point
      angle = -M_PI/2;    // set it to -90 degrees
};

/*************************************
 * HOWITZER :: ROTATERIGHT
 * INPUTS  :: NONE
 * OUTPUTS :: NONE
 * Rotate the howitzer right until the 
 * right most angle is achieved.
 ************************************/
void Howitzer::rotateRight() {
   if ((angle >= -(M_PI/2)) && (angle < (M_PI/2)))
      angle += .05;
   
   if (angle > (M_PI/2)) // Similar as rotateLeft, set it
      angle = M_PI/2;    // to 90 degrees if we go past
};

/********************************
 * HOWITZER :: ROTATEUP
 * INPUTS  :: NONE
 * OUTPUTS :: NONE
 * Rotate the howitzer up until 0
 * degrees is achieved
 *******************************/
void Howitzer::rotateUp() {
   if ((angle >= (-M_PI/2)) && angle < -0.003) // Pointing left
      angle += .003;
   else if ((angle > 0.003 && angle <= (M_PI/2))) // Pointing right
      angle -= .003;
   else // If it is within the margin of .003, set to 0
      angle = 0;
};

/*************************************
 * HOWITZER :: ROTATEDOWN
 * INPUTS  :: NONE
 * OUTPUTS :: NONE
 * Rotate the howitzer down depending
 * on which side it is. If it is 0 it
 * doesn't go anywhere.
 ************************************/
void Howitzer::rotateDown() {
   if ((angle < 0) && (angle >= (-(M_PI/2) + .003)))
      angle -= .003; // Go left since we are pointed left
   else if ((angle > 0) && (angle <= ((M_PI/2) - .003)))
      angle += .003; // Go right since we are pointed right
   else if (angle < (-(M_PI/2) + .003))
      angle = -M_PI/2; // .003 margin
   else if (angle > (M_PI/2) - .003)
      angle = M_PI/2; // .003 margin

   // 0 doesn't have a case because we don't want it to go anywhere.
   // We don't know if the client wants to go left or right when 
   // going down.
};

/*****************************************
 * HOWITZER :: RESET
 * INPUTS  :: RESET
 * OUTPUTS :: NONE
 * Reset the howitzer for a new simulation
 ****************************************/
void Howitzer::reset(Position p) {
   // We need a new position due to everything changing in simulation.
   this->p = p;
   // This keeps the muzzle flash from being drawn 
   age = -0.1;
   setAngle(0);
};
