#ifndef bullet_h
#define bullet_h

#define BULLET_SPEED 5
#define BULLET_LIFE 40

#include "flyingObject.h"
#include "point.h"
#include "velocity.h"
#include "uiDraw.h"

// Put your Bullet class here
/********************************************
 * This is the Bullet Class. It inherits from
 * the flyingObjects class. It will have 
 * everything that is required to fire in 
 * asteroids.
********************************************/
class Bullet : public FlyingObjects
{
private:
   int bulletLife;
   float angle;
   
public:
   Bullet();
   
   //simple draw
   const void draw() { drawDot(FlyingObjects::getPoint()); }
   
   //These are the specific methods required to use bullets!
   void fire(Point point, float angle, Velocity velocity);
   void advance(Point tl, Point br);
   void kill() { FlyingObjects::setAlive(false); }
};

#endif /* bullet_h */
