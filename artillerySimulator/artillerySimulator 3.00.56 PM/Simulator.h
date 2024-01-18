//
//  Simulator.h
//  Lab08
//
//  Created by Journey Curtis on 3/5/22.
//

#ifndef Simulator_h
#define Simulator_h

#include "howitzer.h"
#include "projectile.h"
#include "uiInteract.h"
#include "ground.h"

/***********************************************************
 * ARTILLERY :: SIMULATOR CLASS
 * Main class that contains everything to run the simulation
 **********************************************************/
class Simulator {
private:
   Position ptUpperRight;
   Howitzer howitzer;
   Projectile projectile;
   Ground ground;
   float tInterval;
   
public:
   // Constructors
   Simulator(Position ptUpperRight) : ptUpperRight(ptUpperRight), ground(ptUpperRight),
                                      tInterval(.5) { reset(); }

   // driver method
   void runSimulation(Interface ui);

   // Utility methods
   void fire();
   bool isTargetHit();
   void advance();
   void reset();

   // User related methods
   void input(Interface & ui);
   void display();
};

#endif /* Simulator_h */
