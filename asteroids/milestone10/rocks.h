//
//  rocks.h
//  Asteroids
//
//  Created by Journey Curtis on 6/28/20.
//  Copyright © 2020 Journey Curtis. All rights reserved.
//

#ifndef rocks_h
#define rocks_h

#define BIG_ROCK_SIZE 16
#define MEDIUM_ROCK_SIZE 8
#define SMALL_ROCK_SIZE 4

#define BIG_ROCK_SPIN 2
#define MEDIUM_ROCK_SPIN 5
#define SMALL_ROCK_SPIN 10

#include "flyingObject.h"
#include "velocity.h"
#include "point.h"
#include "uiDraw.h"
#include <cmath>

// Define the following classes here:
//   Rock
//   BigRock
//   MediumRock
//   SmallRock


/*****************************************************************
 * Base Class for all asteroids. It's basically FlyingObjects plus
 * a rotation and a few new methods.
*****************************************************************/
class Rock : public FlyingObjects
{
private:
   int rotation = 0;
   int rotationSpeed = 0;
   
public:
   Rock();
   
   int getRotation() { return rotation; }
   void setRotation(int rotation) { this->rotation = rotation; }
   
   int getRotationSpeed() { return rotationSpeed; }
   void setRotationSpeed(int rotationSpeed)
   { this->rotationSpeed = rotationSpeed; }
   
   void hit();
   void kill() { FlyingObjects::setAlive(false); }
   void advance(Point tl, Point br);
   
   //these will change depending on the type of rock
   virtual void draw() = 0;
};

/*******************************************************************
 * Large Asteroids. Bigger, Slower, More remnants after destruction.
*******************************************************************/
class LargeRock : public Rock
{
public:
   LargeRock(Point point, Velocity velocity, int rotationSpeed);
   
   //updated base methods
   //draws large asteroid
   virtual void draw() { drawLargeAsteroid(getPoint(), getRotation()); }
};

/***********************************************
 * Medium Asteroids. About average in everything
***********************************************/
class MediumRock : public Rock
{
public:
   MediumRock(Point point, Velocity velocity, int rotationSpeed);
   
   //updated base methods
   //draws our medium asteroid
   virtual void draw() { drawMediumAsteroid(getPoint(), getRotation()); }
};

/***********************************
 * Small asteroids. Small but quick!
***********************************/
class SmallRock : public Rock
{
public:
   SmallRock(Point point, Velocity velocity, int rotationSpeed);
   
   //updated base methods
   //draws our small asteroid
   virtual void draw() { drawSmallAsteroid(getPoint(), getRotation()); }
};
#endif /* rocks_h */
