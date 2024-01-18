//
//  velocity.h
//  apollo_11
//
//  Created by Journey Curtis on 1/13/22.
//

#ifndef velocity_h
#define velocity_h
#include <cmath>

class Velocity {
private:
   float dx;
   float dy;

public:
   //Constructors
   Velocity() { dx = 0; dy = 0; }
   Velocity(float dx, float dy) { this->dx = dx; this->dy = dy; }
   
   //Getters
   float getDx()    const { return dx; }
   float getDy()    const { return dy; }
   float getSpeed() const { return sqrt((dx * dx) + (dy * dy)); }
   
   //Setters
   void setDx(float dx) { this->dx = dx; }
   void setDy(float dy) { this->dy = dy; }
   
   //Adders
   void addDx(float dx) { this->dx += dx; }
   void addDy(float dy) { this->dy += dy; }
};

#endif /* velocity_h */
