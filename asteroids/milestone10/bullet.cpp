#include "bullet.h"


// Put your bullet methods here
/*********************
 * Default Constructor
*********************/
Bullet::Bullet()
{
   setAlive(false);
   bulletLife = 40;
}

/******************************************
 * Handles setup for when a bullet is fired
******************************************/
void Bullet::fire(Point point, float angle, Velocity velocity)
{
   setPoint(point);
   setAlive(true);
   
   this->angle = angle;
   float dx = (velocity.getDx() + BULLET_SPEED) * (-cos(M_PI / 180.0 * angle));
   float dy = (velocity.getDy() + BULLET_SPEED) * (sin(M_PI / 180.0 * angle));
   
   Velocity tempV = Velocity(dx, dy);
   setVelocity(tempV);
}

/*********************
 * advances the bullet
*********************/
void Bullet::advance(Point tl, Point br)
{
   Velocity tempV = getVelocity();
   Point tempP = getPoint();
   
   tempV.advancePoint(tempP);
   setPoint(tempP);
   
   checkPoint(tl, br);
   bulletLife -= 1;
   
   if (bulletLife == 0)
      kill();
}
