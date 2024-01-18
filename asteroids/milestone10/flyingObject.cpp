//
//  FlyingObjects.cpp
//  Asteroids
//
//  Created by Journey Curtis on 6/12/20.
//  Copyright © 2020 Journey Curtis. All rights reserved.
//

#include "flyingObject.h"

/*********************
 * Default Constructor
*********************/
FlyingObjects::FlyingObjects()
{
   point.setX(0);
   point.setY(0);
   velocity.setDx(0);
   velocity.setDy(0);
   alive = true;
   radius = 0;
}

/**********************************
 * checkPoint handles our wrapping!
**********************************/
void FlyingObjects::checkPoint(Point tl, Point br)
{
   if (point.getX() >= br.getX() || point.getX() <= tl.getX())
      point.setX(-point.getX());
   if (point.getY() >= tl.getY() || point.getY() <= br.getY())
      point.setY(-point.getY());
}

/*********************************************
 * Basic advance. This is set to virtual in .h
*********************************************/
void FlyingObjects::advance(Point tl, Point br)
{
   velocity.advancePoint(point);
   checkPoint(tl, br);
}
