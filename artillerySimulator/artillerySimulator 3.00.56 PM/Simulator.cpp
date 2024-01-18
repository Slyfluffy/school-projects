//
//  Simulator.cpp
//  Lab08
//
//  Created by Journey Curtis on 3/17/22.
//

#include "Simulator.h"

/*********************************
 * SIMULATOR :: INPUT
 * INPUTS  :: ui
 * OUTPUTS :: NONE
 * Handles the input from the user
 ********************************/
void Simulator::input(Interface &ui) {
   // Movement of howitzer
   if (ui.isUp())
      howitzer.rotateUp();
   else if (ui.isDown())
      howitzer.rotateDown();
   else if (ui.isLeft())
      howitzer.rotateLeft();
   else if (ui.isRight())
      howitzer.rotateRight();
   
   // Firing of howitzer
   if (ui.isSpace() && !projectile.isAlive())
      fire();
}

/********************************
 * SIMULATOR :: FIRE
 * INPUTS  :: NONE
 * OUTPUTS :: NONE
 * Fires the projectile. Done by
 * calling the fire methods from
 * howitzer and projectile.
 *******************************/
void Simulator::fire() {
   howitzer.fire();
   projectile.fire(howitzer.getAngle(), *howitzer.getPosition(), tInterval);
}

/******************************
 * SIMULATOR :: ISTARGETHIT
 * INPUTS  :: NONE
 * OUTPUTS :: isHit
 * Method to determine if the
 * projectile was close enough
 * to count as a hit.
 ****************************/
bool Simulator::isTargetHit() {
   float targetX = ground.getTarget().getMetersX();
   float targetY = ground.getTarget().getMetersY();
   float distance = projectile.getDistance();
   float altitude = projectile.getAltitude();
   
   if ((distance >= (targetX - 200)) && (distance <= (targetX + 200)))
      if ((altitude >= (targetY - 200)) && (altitude <= (targetY + 200)))
         return true;
   
   return false;
}

/*********************************
 * SIMULATOR :: RUNSIMULATION
 * INPUTS  :: ui
 * OUTPUTS :: NONE
 * Runs the simulator by calling
 * all the appropriate methods.
 ********************************/
void Simulator::runSimulation(Interface ui) {
   input(ui);
   
   advance();
   
   if (isTargetHit())
      reset();

   display();
}

/*********************************
 * SIMULATOR :: ADVANCE
 * INPUTS  :: NONE
 * OUTPUTS :: NONE
 * Move any objects. For now it is
 * the projectile.
 ********************************/
void Simulator::advance() {
   if (projectile.isAlive())
      projectile.move(tInterval);
   
   // Figure out altitude from ground.
   // If it hits the ground, it dies
   double altitude = projectile.getAltitude() - ground.getElevationMeters(projectile.getPosition());
   if (altitude <= 0)
       projectile.kill();
   // If it goes out of the bounds of the window, it dies
   else if (projectile.getDistance() >= ptUpperRight.getMetersX() || projectile.getDistance() <= 0)
      projectile.kill();
}

/*****************************
 * SIMULATOR :: DISPLAY
 * INPUTS  :: NONE
 * OUTPUTS :: NONE
 * draw all the objects in the 
 * simulation
 ****************************/
void Simulator::display() {
   ogstream gout;
   ground.draw(gout);
   howitzer.draw(gout, tInterval);
   projectile.draw(gout);

   // Display status
   Position start = Position();
   start.setPixelsX(550);
   start.setPixelsY(480);
   gout.setPosition(start);
   if (!projectile.isAlive())
       gout << "Angle: " << howitzer.getAngleDegrees();
   else {
       gout << "Altitude: " << projectile.getAltitude() << endl;
       gout << "Speed: " << projectile.getSpeed() << endl;
       gout << "Distance: " << projectile.getDistance() << endl;
       gout << "Hangtime: " << projectile.getHangTime() << endl;
   }
}

/**********************************
 * SIMULATOR :: RESET
 * INPUTS  :: NONE
 * OUTPUTS :: NONE
 * Resets the simulator for another
 * round.
 *********************************/
void Simulator::reset() {
   double topBorder = ptUpperRight.getMetersY();
   double rightBorder = ptUpperRight.getMetersX();
   
   double x = ((double)rand() / (double)RAND_MAX * (rightBorder));
   double y = ((double)rand() / (double)RAND_MAX * (topBorder));
   
   Position p(x, y);
   
   howitzer.reset(p);
   ground.reset(*howitzer.getPosition());
   projectile.reset();
}
