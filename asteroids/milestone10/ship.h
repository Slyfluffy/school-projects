//
//  ship.h
//  Asteroids
//
//  Created by Journey Curtis on 6/28/20.
//  Copyright © 2020 Journey Curtis. All rights reserved.
//

#ifndef ship_h
#define ship_h

#define SHIP_SIZE 10

#define ROTATE_AMOUNT 6
#define THRUST_AMOUNT 0.5
#define MAX_SPEED 10

#include "flyingObject.h"
#include "velocity.h"
#include "point.h"
#include "uiDraw.h"

#include <cmath>

// Put your Ship class here
/***************************************************
 * This is the ship class. It will handle everything
 * around the ship!
 **************************************************/
class Ship : public FlyingObjects
{
private:
   float shipOrientation;
   float angle;
   
public:
   Ship();
   
   //simple getters and setters for orientation
   int getShipOrientation() { return shipOrientation; }
   void setShipOrientation(int sOr) { shipOrientation = sOr; }
   
   //only need a get angle for when we apply thrust!
   float getAngle() { return angle; }
   void setAngle(float angle) {this->angle = angle; }
   
   void draw() { drawShip(FlyingObjects::getPoint(), shipOrientation); }
   void advance(Point tl, Point br);
   void crash() { setAlive(false); }
   
   void turnLeft();
   void turnRight();
   void applyThrust();
};

#endif /* ship_h */
