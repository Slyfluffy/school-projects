//
//  physics.h
//  Lab08
//
//  Created by Journey Curtis on 2/21/22.
//

#ifndef physics_h
#define physics_h

#include <cmath>

float convertToRadians(float degree) { return (2 * M_PI * (degree / 360));   }
float convertToDegrees(float radian) { return 360 * (radian / (2.0 * M_PI)); }

float computeSurfaceArea(float radius) { return M_PI * (radius * radius); }

float computeAngleFromComponents(float dx, float dy) { return atan2(dy, dx); }

float calculateForce(float mass, float acceleration) { return mass * acceleration; }
float calculateAcceleration(float force, float mass) { return force / mass;        }

float velocityFromAcceleration(float acceleration, float time) { return acceleration / time; }

float calculateVSpeed(float dy, float angle) { return dy * cos(angle); }
float calculateHSpeed(float dx, float angle) { return dx * sin(angle); }
float calculateTSpeed(float dx, float dy) { return sqrt((dx * dx) + (dy * dy)); }

struct Mapping {
   float domain;
   float range;
};

/*
 * PHYSICS :: LINEARINTERPOLATION
 * INPUTS  :: x, x0, x1, y0, y1
 * OUTPUTS :: solution
 * Bro. Helfrich design. returns the solution of linear interpolation
 */
float linearInterpolation(float x, float x0, float x1, float y0, float y1) {
   return y0 + (x - x0) * (y1 - y0) / (x1 - x0);
}

/*
 * PHYSICS :: LINEARINTERPOLATION
 * INPUTS  :: zero, one, d
 * OUTPUTS :: solution
 * Bro. Helfrich design. calls linearInterpolation and returns the solution.
 * Inputs are mostly a structure called Mapping.
 */
float linearInterpolation(const Mapping & zero, const Mapping & one, float d) {
   return linearInterpolation(d, zero.domain, one.domain, zero.range, one.range);
}

/*
 * PHYSICS :: LINEARINTERPOLATION
 * INPUTS  :: map[], numMapping, domain
 * OUTPUTS :: solution
 * Bro. Helfrich design. Returns interpolation from a list of ranges.
 */
float linearInterpolation(const Mapping map[], int numMapping, float domain) {
   // Off the scale on the small end
   if (domain < map[0].domain)
      return map[0].range;
   
   for (int i = 0; i < numMapping - 1; i++) {
      if (map[i+0].domain <= domain && domain <= map[i+1].domain)
         return linearInterpolation(map[i+0], map[i+1], domain);
   }
   
   return map[numMapping - 1].range;
}

float computeDragForce(float density, float dragCoefficient, float radius, float velocity) {
   // Compute Surface Area of projectile
   float area = computeSurfaceArea(radius);
   
   // Compute and return drag!
   return .5 * dragCoefficient * density * (velocity * velocity) * area;
}

/************************************************
 * ARTILLERY :: COMPUTEAIRDENSITY
 * INPUTS    :: altitude
 * OUTPUTS   :: airDensity
 * Bro. Helfrich design. Calculates the air density based on altitude.
 ***********************************************/
float computeAirDensity(float altitude) {
   const Mapping densityMap[] =
   {
      {  0.0 , 1.225    },
      {  1000, 1.112    },
      {  2000, 1.007    },
      {  3000, .9093    },
      {  4000, .8194    },
      {  5000, .7364    },
      {  6000, .6601    },
      {  7000, .5900    },
      {  8000, .5258    },
      {  9000, .4671    },
      { 10000, .4135    },
      { 15000, .1948    },
      { 20000, .08891   },
      { 25000, .04008   },
      { 30000, .01841   },
      { 40000, .003996  },
      { 50000, .001027  },
      { 60000, .0003097 },
      { 70000, .0000828 },
      { 80000, .0000185 }
   };
   
   return linearInterpolation(densityMap, sizeof(densityMap) / sizeof(densityMap[0]), altitude);
}

/**************************************************
 * ARTILLERY :: COMPUTEVELOCITYSOUND
 * INPUTS    :: altitude
 * OUTPUTS   :: velocitySound
 * Bro. Helfrich design. Returns the speed of sound at a certain altitude
 *************************************************/
float computeVelocitySound(float altitude) {
   const Mapping sMap[] =
   {
      {  0.0 , 340 },
      {  1000, 336 },
      {  2000, 332 },
      {  3000, 328 },
      {  4000, 324 },
      {  5000, 320 },
      {  6000, 316 },
      {  7000, 312 },
      {  8000, 308 },
      {  9000, 303 },
      { 10000, 299 },
      { 15000, 295 },
      { 20000, 295 },
      { 25000, 295 },
      { 30000, 305 },
      { 40000, 324 }
   };
   
   return linearInterpolation(sMap, sizeof(sMap) / sizeof(sMap[0]), altitude);
}

/********************************************
 * ARTILLERY :: COMPUTEGRAVITY
 * INPUTS    :: altitude
 * OUTPUTS   :: gravity
 * Computes the gravity at a certain altitude
 *******************************************/
float computeGravity(float altitude) {
   float e = (6371009/(6371009 + altitude));
   return 9.807 * (e * e);
}

/************************************************
 * ARTILLERY :: COMPUTECOEFFICIENT
 * INPUTS    :: velocity, vSound
 * OUTPUTS   :: cDrag
 * Bro. Helfrich design. Calculates and returns the coefficient of drag
 ***********************************************/
float computeCoefficient(float velocity, float vSound) {
   float mach = velocity / vSound;
   
   const Mapping cMap[] =
   {
      {  0.3, .1629 },
      {  0.5, .1659 },
      {  0.7, .2031 },
      { 0.89, .2597 },
      { 0.92, .3010 },
      { 0.96, .3287 },
      { 0.98, .4002 },
      { 1.00, .4258 },
      { 1.02, .4335 },
      { 1.06, .4483 },
      { 1.24, .4064 },
      { 1.53, .3663 },
      { 1.99, .2897 },
      { 2.87, .2297 },
      { 2.89, .2306 },
      { 5.00, .2656 }
   };
   
   return linearInterpolation(cMap, sizeof(cMap) / sizeof(cMap[0]), mach);
}

#endif /* physics_h */
