/*********************************************************************
 * File: game.h
 * Description: Defines the game class for Asteroids.
 *********************************************************************/

#ifndef GAME_H
#define GAME_H

#include <vector>

#include "rocks.h"
#include "ship.h"
#include "bullet.h"
#include "point.h"
#include "uiDraw.h"
#include "uiInteract.h"

/***************************************************************
 * Game is the game itself. It will handle everything to get the
 * game up and running!
***************************************************************/
class Game
{
private:
   Point topLeft;
   Point bottomRight;
   
   std::vector<Rock*> rocks;
   std::vector<Bullet> bullets;
   
   Ship ship;
   
   int respawnCounter;
   int frameCount;
   int livesRemaining;
   
   //private methods. These will be specific to our classes
   //create functions
   void createLargeAsteroids();
   void createMediumAsteroids(Point point, Velocity velocity);
   void createSmallAsteroids(Point point, Velocity velocity, int selector);
   void crashAsteroids(Rock * rock);
   
   //advance functions
   void advanceRocks();
   void advanceShip();
   void advanceBullets();
   
   //private methods that will help run the game!
   void handleCollisions();
   void handleBulletCollisions();
   void handleShipCollisions();
   void handleRespawn();
   
   void cleanUpZombies();
   
public:
   Game(Point tl, Point br);
   ~Game();

   int getRespawnCounter() { return respawnCounter; }
   void setRespawnCounter(int respawnCounter)
   { this->respawnCounter = respawnCounter; };
   
   int getFrameCount() { return frameCount; }
   void setFrameCount(int frameCount) { this->frameCount = frameCount; }
   
   int getLivesRemaining() { return livesRemaining; }
   void setLivesRemaining(int lives) { livesRemaining = lives; }
   
   //These are public methods that help run the game!
   void handleInput(const Interface & ui);
   void advance();
   void draw(const Interface & ui);
   
};

#endif /* GAME_H */
