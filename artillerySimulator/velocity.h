//
//  velocity.h
//  apollo_11
//
//  Created by Journey Curtis on 1/13/22.
//

#ifndef velocity_h
#define velocity_h
#include <cmath>

/********************************
 * ARTILLLERY :: VELOCITY CLASS
 * Contains everything needed for
 * a velocity.
 *******************************/
class Velocity {
private:
   float dx;
   float dy;

public:
   //Constructors
   Velocity() { dx = 0; dy = 0; }
   Velocity(float dx, float dy) { this->dx = dx; this->dy = dy; }
   Velocity& operator = (const Velocity & rhs) { this->dx = rhs.dx; this->dy = rhs.dy; return *this; }
   
   //Getters
   float getDx()    const { return dx; }
   float getDy()    const { return dy; }
   float getSpeed() const { return sqrt((dx * dx) + (dy * dy)); }
   
   //Setters
   void setDx(float dx) { this->dx = dx; }
   void setDy(float dy) { this->dy = dy; }
   void setVfromSpeed(float speed, float angle) {
      dx = speed * sin(angle);
      dy = speed * cos(angle);
   }
   
   //Adders
   void addDx(float dx) { this->dx += dx; }
   void addDy(float dy) { this->dy += dy; }
};

#endif /* velocity_h */
