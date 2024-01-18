//
//  projectile.h
//  Lab08
//
//  Created by Journey Curtis on 3/5/22.
//

#ifndef projectile_h
#define projectile_h

#define _USE_MATH_DEFINES
#include "math.h"

#include <list>

#include "position.h"
#include "velocity.h"
#include "uiDraw.h"


using namespace std;

/********************************************
 * ARTILLERY :: MAPPING STRUCT
 * Simple struct that stores domain and range
 *******************************************/
struct Mapping {
   float domain;
   float range;
};

/************************************************
 * ARTILLERY :: PROJECTILE CLASS
 * Contains everything needed for the projectile.
 * Includes methods that allow movement, drawing,
 * and computations.
 ***********************************************/
class Projectile {
private:
   Position p;
   Velocity v;
   list<Position> tail;
   
   float angle;
   float hangTime;
   bool alive;
   
protected:
   // Linear Interpolation Methods
	double linearInterpolation(float x, float x0, float x1, float y0, float y1);
	double linearInterpolation(const Mapping& zero, const Mapping& one, float d);
	double linearInterpolation(const Mapping map[], int numMapping, float domain);
   
   // Computational Methods
	double computeAirDensity(float altitude);
	float computeVelocitySound(float altitude);
	float computeGravity(float altitude);
	float computeCoefficient(float v, float vSound);
   float computeSurfaceArea(float radius) { return M_PI * (radius * radius); }
   float computeForceDrag(float density, float dragCoefficient, float radius, float velocity);
   float calculateAcceleration(float force, float mass) { return force / mass;        }
   
public:
   // Constructors
	Projectile();
   
   // Getters
   // getAltitude returns altitude from sea level.
   const float getAltitude()    { return p.getMetersY(); }
   const float getDistance()    { return p.getMetersX(); }
   const float getSpeed()       { return v.getSpeed(); }
   const float getHangTime()    { return hangTime; }
   const bool  isAlive()        { return alive; }
   const Position getPosition() { return p; }
   const Velocity getVelocity() { return v; }
   
   // Setters
   void setAlive() { alive = true; }
   void kill()     { alive = false; tail.clear(); }
   
   // Other methods
   void fire(float angle, Position p, float tInverval);
   void move(const float tInverval);
   void reset();
   void draw(ogstream & gout);
};

#endif /* projectile_h */
