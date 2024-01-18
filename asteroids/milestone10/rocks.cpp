//
//  rocks.cpp
//  Asteroids
//
//  Created by Journey Curtis on 6/28/20.
//  Copyright © 2020 Journey Curtis. All rights reserved.
//

#include "rocks.h"

// Put your Rock methods here
//Rock Class

/*******************************************
 * Default Constructor for Rock parent class
*******************************************/
Rock::Rock()
{
   rotation = 0;
   FlyingObjects::setRadius(0);
}

void Rock::hit()
{
   setAlive(false);
}

/*************************
 * Advances the asteroids!
*************************/
void Rock::advance(Point tl, Point br)
{
   setRotation(getRotation() + getRotationSpeed()); //rotation speed!
   FlyingObjects::advance(tl, br);
}

//LargeRock Class
/***************************************
 * Non-Default Constructor for LargeRock
 **************************************/
LargeRock::LargeRock(Point point, Velocity velocity, int rotationSpeed)
{
   setRotationSpeed(rotationSpeed);
   setRadius(16);
   setPoint(point);
   setVelocity(velocity);
}

//MediumRock Class
/************************
 * MediumRock constructor
************************/
MediumRock::MediumRock(Point point, Velocity velocity, int rotationSpeed)
{
   setRotationSpeed(rotationSpeed);
   setPoint(point);
   setVelocity(velocity);
   setRadius(8);
}

//SmallRock Class
/************************
 * SmallRock Constructor!
************************/
SmallRock::SmallRock(Point point, Velocity velocity, int rotationSpeed)
{
   setRotationSpeed(rotationSpeed);
   setVelocity(velocity);
   setRadius(4);
   setPoint(point);
}
