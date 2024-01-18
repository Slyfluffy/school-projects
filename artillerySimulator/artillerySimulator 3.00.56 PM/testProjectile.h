//
//  testProjectile.h
//  Lab08
//
//  Created by Journey Curtis on 3/5/22.
//

#ifndef testProjectile_h
#define testProjectile_h

#include "projectile.h"
#include <cassert>

class TestProjectile {
public:
   void run() {
      testLinearInterpolation1();
      testLinearInterpolation2();
      testLinearInterpolation3();
      testComputeAirDensity();
      testComputeVelocitySound();
      testComputeGravity();
      testComputeCoefficient();
   }
   
   void testConstructor() {
       // Setup: default constructors
       Projectile* p = new Projectile();
       Position* pos = new Position();
       Velocity* v = new Velocity();
       // Exersize and verify
       assert(p->getPosition().getMetersX() == pos->getMetersX());
       assert(p->getPosition().getMetersY() == pos->getMetersY());

       assert(p->getVelocity().getDx() == v->getDx());
       assert(p->getVelocity().getDy() == v->getDy());
       
       // Teardown
      delete p;
      delete pos;
      delete v;
   }
   
   void testLinearInterpolation1() {
      Projectile p;
       // Exersize and verify
       // Positive slope
       assert(p.linearInterpolation(1, 0, 2, 0, 2) == 1.0);
       // Negative slope
       assert(p.linearInterpolation(1, 0, 2, 2, 0) == 1.0);
       // Horizontal line
       assert(p.linearInterpolation(1, 0, 2, 0, 0) == 0.0);

   }
   
   void testLinearInterpolation2() {
      Projectile p;
       // Positive slope
       Mapping zero;
       Mapping one;
       zero.domain = 0;
       zero.range = 0;
       one.domain = 2;
       one.range = 2;
       assert(p.linearInterpolation(zero, one, 1) == 1.0);
       // Negative slope
       zero.range = 2;
       one.range = 0;
       assert(p.linearInterpolation(zero, one, 1) == 1.0);
       // Horizontal line
       zero.range = 0;
       assert(p.linearInterpolation(zero, one, 1) == 0.0);
   }
   
   void testLinearInterpolation3() {
      Projectile p;
       // Positive slope
       Mapping zerozero;
       zerozero.domain = 0;
       zerozero.range = 0;
       Mapping zerotwo;
       zerotwo.domain = 0;
       zerotwo.range = 2;
       Mapping twozero;
       twozero.domain = 2;
       twozero.range = 0;
       Mapping twotwo;
       twotwo.domain = 2;
       twotwo.range = 2;
       Mapping map[] = { zerozero, twotwo };
       assert(p.linearInterpolation(map, 2 , 1) == 1.0);
       // Negative slope
       map[0] = zerotwo;
       map[1] = twozero;
       assert(p.linearInterpolation(map, 2, 1) == 1.0);
       // Horizontal line
       map[0] = zerozero;
       assert(p.linearInterpolation(map, 2, 1) == 0.0);
   }
   
   void testComputeAirDensity() {
      Projectile p;
       // Table
       assert(p.computeAirDensity(15000) == float(0.1948));
       // Low Interp 1.1685
       assert(p.computeAirDensity(500) == float(1.1685));
       // High Interp
       //assert(p.computeAirDensity(65000) == float(0.00019625));
   }
   
   void testComputeVelocitySound() {
      Projectile p;
       // Table
       assert(p.computeVelocitySound(8000) == 308);
       // Low Interp
       //assert(p.computeVelocitySound(500) == 338.0);
       // High Interp
       //assert(p.computeVelocitySound(35000) == 314.5);
      
   }
   
   void testComputeGravity() {
      Projectile p;
       // Table
       //assert(p.computeGravity(8000) == float(9.78242));
       // Low Interp
       //assert(p.computeGravity(500) == float(9.80546));
       // High Interp
       //assert(p.computeGravity(8000) == 9.7375);
   }
   
   void testComputeCoefficient() {
      Projectile p;
       // Table (mach = 1.06)
       //assert(p.computeCoefficient(326.48, 308) == float(0.4483));
       // Low Interp (mach = .4)
       assert(p.computeCoefficient(120, 300) == 0.1644);
       // High Interp (mach = 3.945)
       assert(p.computeCoefficient(1183.5, 300) == float(0.2481));
   }
   
//   void testMove() {
//      Position pos;
//      float angle;
//
//      // -90 degree scenario
//      angle = -M_PI/2;
//      Projectile p(angle, pos);
//      p.move();
//
//      // -45 degree scenario
//      angle = -M_PI/4;
//      p = Projectile(angle, pos);
//      p.move();
//
//      // 0 degree scenario
//      angle = 0;
//      p = Projectile(angle, pos);
//      p.move();
//      assert(p.getVelocity().getDx() == 0);
//      assert(p.getVelocity().getDy() == (827 - computeGravity(0)));
//
//      // 45 degree scenario
//      angle = M_PI/4;
//      p = Projectile(angle, pos);
//      p.move();
//
//
//      // 90 degree scenario
//      angle = M_PI/2;
//      p = Projectile(angle, pos);
//      p.move();
//   }
};

#endif /* testProjectile_h */
