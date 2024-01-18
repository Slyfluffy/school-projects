//
//  FlyingObjects.h
//  Asteroids
//
//  Created by Journey Curtis on 6/12/20.
//  Copyright © 2020 Journey Curtis. All rights reserved.
//

#include "point.h"
#include "velocity.h"
#include "uiDraw.h"

#ifndef FlyingObjects_h
#define FlyingObjects_h

/******************************************************************
 * The FlyingObjects class is the parent of all our flying classes.
 * It provides all of the information types each one needs to
 * function.
******************************************************************/
class FlyingObjects
{
private:
   Point point;
   Velocity velocity;
   bool alive;
   int radius;
   
public:
   
   FlyingObjects();
   
   //getters and setters
   Point getPoint() { return point; }
   void setPoint(Point point) { this->point = point; }
   
   Velocity getVelocity() { return velocity; }
   void setVelocity(Velocity velocity) { this->velocity = velocity; }
   
   bool isAlive() { return alive; }
   void setAlive(bool alive) { this->alive = alive; }
   
   int getRadius() { return radius; }
   void setRadius(int radius) { this->radius = radius; }
   
   //Virtual as each advance is a little different
   virtual void advance(Point tl, Point br);
   void checkPoint(Point tl, Point br);
   
};

#endif /* FlyingObjects_h */
