#include "projectile.h"

/***********************************
 * PROJECTILE :: DEFAULT CONSTRUCTOR
 * INPUTS  :: NONE
 * OUTPUTS :: NONE
 * Basic constructor for projectile
 **********************************/
Projectile::Projectile() {
	this->p = Position();
	this->v = Velocity();
}

/********************************************************************
 * PROJECTILE :: LINEARINTERPOLATION
 * INPUTS  :: x, x0, x1, y0, y1
 * OUTPUTS :: solution
 * Bro. Helfrich design. returns the solution of linear interpolation
 *******************************************************************/
double Projectile::linearInterpolation(float x, float x0, float x1, float y0, float y1) { 
	return y0 + (x - x0) * (y1 - y0) / (x1 - x0); 
}

/********************************************************************
 * PROJECTILE :: LINEARINTERPOLATION
 * INPUTS  :: zero, one, d
 * OUTPUTS :: solution
 * Bro. Helfrich design. returns the solution of linear interpolation
 * This one takes two maps and a domain then calculates using the
 * method above.
 *******************************************************************/
double Projectile::linearInterpolation(const Mapping & zero, const Mapping & one, float d) { 
	return linearInterpolation(d, zero.domain, one.domain, zero.range, one.range);
}

/********************************************************************
 * PROJECTILE :: LINEARINTERPOLATION
 * INPUTS  :: map, numMapping, domain
 * OUTPUTS :: solution
 * Bro. Helfrich design. returns the solution of linear interpolation
 * This one takes two maps and a domain then calculates using the
 * two methods above.
 *******************************************************************/
double Projectile::linearInterpolation(const Mapping map[], int numMapping, float domain) { 
	if (domain < map[0].domain)
		return map[0].range;

	for (int i = 0; i < numMapping - 1; i++) {
		if (map[i + 0].domain <= domain && domain <= map[i + 1].domain)
			return linearInterpolation(map[i + 0], map[i + 1], domain);
	}

	return map[numMapping - 1].range;
}

/********************************************************************
 * PROJECTILE :: COMPUTEAIRDENSITY
 * INPUTS  :: altitude
 * OUTPUTS :: airDensity
 * Returns the air density according the map. If it is in between two
 * sections then linear interpolation is used.
 *******************************************************************/
double Projectile::computeAirDensity(float altitude) { 
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

   // Check for the value in the table
   for (int i = 0; i < sizeof(densityMap); i++) {
      if (densityMap[i].domain == altitude)
         return densityMap[i].range;
   }
    
   return linearInterpolation(densityMap, sizeof(densityMap) / sizeof(densityMap[0]), altitude);
}

/*****************************************************************
 * PROJECTILE :: COMPUTEVELOCITYSOUND
 * INPUTS  :: altitude
 * OUTPUTS :: vSound
 * Returns the velocity of sound according to the map. Uses linear
 * interpolation if needed.
 ****************************************************************/
float Projectile::computeVelocitySound(float altitude) {
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

   for (int i = 0; i < sizeof(sMap); i++) {
      if (sMap[i].domain == altitude)
         return sMap[i].range;
   }

   return linearInterpolation(sMap, sizeof(sMap) / sizeof(sMap[0]), altitude);
}

/**************************************
 * PROJECTILE :: COMPUTEGRAVITY
 * INPUTS  :: altitude
 * OUTPUTS :: gravity
 * Returns the gravity at the altitude.
 *************************************/
float Projectile::computeGravity(float altitude) {
   float e = (6371009 / (6371009 + altitude)); 
   return 9.807 * (e * e);
}

/***************************************************************
 * PROJECTILE :: COMPUTECOEFFICIENT
 * INPUTS  :: v, vSound
 * OUTPUTS :: c
 * Returns the coefficient according to the map. Once again uses
 * linear interpolation if it is between two of them.
 **************************************************************/
float Projectile::computeCoefficient(float v, float vSound) {
   float mach = v / vSound;

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

   for (int i = 0; i < sizeof(cMap); i++) {
      if (cMap[i].domain == mach)
         return cMap[i].range;
   }

   return linearInterpolation(cMap, sizeof(cMap) / sizeof(cMap[0]), mach);
}

/*******************************************************
 * PROJECTILE :: COMPUTEFORCEDRAG
 * INPUTS  :: desntiy, dragCoefficient, radius, velocity
 * OUTPUTS :: drag
 * Computes and returns the force of drag.
 ******************************************************/
float Projectile::computeForceDrag(float density, float dragCoefficient, float radius, float velocity) {
   // Compute Surface Area of projectile
   float area = computeSurfaceArea(radius);
   
   // Compute and return drag!
   return .5 * dragCoefficient * density * (velocity * velocity) * area;
}

/********************************************************************
 * PROJECTILE :: FIRE
 * INPUTS  :: angle, p, tInverval
 * OUTPUTS :: NONE
 * Fires the projectile. This includes setting the angle and position
 * from the howitzer.
 *******************************************************************/
void Projectile::fire(float angle, Position p, float tInverval) {
   this->angle = angle;
   this->p = p;
   alive = true;
   //827 * tInverval
   v.setVfromSpeed((827 * tInverval), angle);
   hangTime = 0;
}

/*******************************************************************
 * PROJECTILE :: MOVE
 * INPUTS  :: tInterval
 * OUTPUTS :: NONE
 * Moves the projectile. This includes using the computation methods
 * above and putting them together to solve the new velocity.
 ******************************************************************/
void Projectile::move(const float tInterval) {
   if (!isAlive())
      return;
   
   // variables that will not change during this move sequence
   const float mass = 46.7;
   const float radius = .15489 / 2;
   const float altitude = getAltitude();
   
   // Get sound velocity
   float vSound = computeVelocitySound(altitude) * tInterval;
   
   // Now figure out coefficient
   float coefficient = computeCoefficient(v.getSpeed(), vSound);
   
   // Density is up next
   float density = computeAirDensity(altitude);
   
   // Put them all together to figure out drag
   float dragForce = 0;
   if (angle != 0)
      dragForce = computeForceDrag(density, coefficient, radius, v.getSpeed());
   
   // Use drag to calculate acceleration
   float ax = calculateAcceleration(dragForce, mass);
   
   // Velocities
   v.addDx(ax);
   v.addDy(-computeGravity(altitude) * tInterval);
   
   tail.push_back(p);
   if (tail.size() >= 8) {
      tail.pop_front();
   }
   
   // Update position
   p.addMetersX(v.getDx());
   p.addMetersY(v.getDy());
   
   // Update hangtime
   hangTime += tInterval;
}

/****************************
 * PROJECTILE :: RESET
 * INPUTS  :: NONE
 * OUTPUTS :: NONE
 * Resets the bullet for use.
 ***************************/
void Projectile::reset() {
   v = Velocity();
   p = Position();
   hangTime = 0;
   angle = 0;
   tail.clear();
}

/*********************************
 * PROJECTILE :: DRAW
 * INPUTS  :: gout
 * OUTPUTS :: NONE
 * Draws the bullet and it's tail.
 ********************************/
void Projectile::draw(ogstream & gout) {
   if (!isAlive())
      return;
   
   double age = 0;
   gout.drawProjectile(p, age++);
   
   list<Position> :: reverse_iterator it = tail.rbegin();
   while (it != tail.rend()) {
      gout.drawProjectile((*it++), age++);
   }
}
