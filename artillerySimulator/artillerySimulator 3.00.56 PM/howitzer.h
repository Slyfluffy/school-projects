//
//  howitzer.h
//  Lab08
//
//  Created by Journey Curtis on 3/5/22.
//

#define _USE_MATH_DEFINES
#include "math.h"

#ifndef howitzer_h
#define howitzer_h

#include "position.h"
#include "uiDraw.h"


/***********************************************
 * ARTILLERY :: HOWITZER CLASS
 * This class contains all the data necessary
 * to run a howitzer object in the Artillery
 * Simulator. This includes the position, angle,
 * and drawing of it.
 **********************************************/
class Howitzer {
private:
   Position p;
   double angle;
   float age;
   
public:
   // Constructors
   Howitzer() : p(Position()), angle(0), age(-0.1)  { }
   Howitzer(Position p) : p(p), angle(0), age(-0.1) { }
   
   // Getters
   const float getAge()    const { return age;   }
   const double getAngle() const { return angle; }
   const float  getAngleDegrees() const { return angle * (180/M_PI); }
   Position * getPosition()      { return &p;    }
   
   // Setters
   void setAngle(double angle) { this->angle = angle; }
   
   // Rotation methods
   void rotateLeft();
   void rotateRight();
   void rotateUp();
   void rotateDown();
   
   // Other
   void fire() { age = 2; }
   void reset(Position p);
   void draw(ogstream & gout, float tInterval) { gout.drawHowitzer(p, angle, age); age -= tInterval; }
};

#endif /* howitzer_h */
