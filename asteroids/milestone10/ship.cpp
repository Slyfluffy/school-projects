//
//  ship.cpp
//  Asteroids
//
//  Created by Journey Curtis on 6/28/20.
//  Copyright © 2020 Journey Curtis. All rights reserved.
//

#include "ship.h"

// Put your ship methods here

/*********************
 * Default Constructor
*********************/
Ship::Ship()
{
   FlyingObjects::setPoint(Point());
   FlyingObjects::setRadius(10);
   shipOrientation = 0;
   angle = 90;
}

/*****************************
 * Applies thrust to our ship!
 ****************************/
void Ship::applyThrust()
{
   float dx = THRUST_AMOUNT * (-cos(M_PI / 180.0 * angle));
   float dy = THRUST_AMOUNT * (sin(M_PI / 180.0 * angle));
   
   Velocity addV = Velocity(dx, dy);
   Velocity tempV = getVelocity();
   
   tempV.add(addV);
   setVelocity(tempV);
}

/*******************************
 * Turns our ship to port (left)
 ******************************/
void Ship::turnLeft()
{
   shipOrientation += ROTATE_AMOUNT;
   angle -= ROTATE_AMOUNT;
   
   if (shipOrientation >= 360)
      shipOrientation = 0;
}

/*************************************
 * Turns our ship to starboard (right)
*************************************/
void Ship::turnRight()
{
   shipOrientation -= ROTATE_AMOUNT;
   angle += ROTATE_AMOUNT;
   
   if (shipOrientation == 0)
      shipOrientation = 360;
}

/********************
 * Advances our ship!
 *******************/
void Ship::advance(Point tl, Point br)
{
   Velocity tempV = getVelocity();
   Point tempP = getPoint();
   tempV.advancePoint(tempP);
   setPoint(tempP);
   checkPoint(tl, br);
}
