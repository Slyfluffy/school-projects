/*********************************************************************
 * File: game.cpp
 * Description: Contains the implementation of the game class
 *  methods.
**********************************************************************/

#include "game.h"

// These are needed for the getClosestDistance function...
#include <limits>
#include <algorithm>
#include <cmath>
#include <string>
using namespace std;


// You may find this function helpful...

/**********************************************************
 * Function: getClosestDistance
 * Description: Determine how close these two objects will
 *   get in between the frames.
 **********************************************************/
/*
float Game :: getClosestDistance(const FlyingObject &obj1, const FlyingObject &obj2) const
{
   // find the maximum distance traveled
   float dMax = max(abs(obj1.getVelocity().getDx()), abs(obj1.getVelocity().getDy()));
   dMax = max(dMax, abs(obj2.getVelocity().getDx()));
   dMax = max(dMax, abs(obj2.getVelocity().getDy()));
   dMax = max(dMax, 0.1f); // when dx and dy are 0.0. Go through the loop once.
   
   float distMin = std::numeric_limits<float>::max();
   for (float i = 0.0; i <= dMax; i++)
   {
      Point point1(obj1.getPoint().getX() + (obj1.getVelocity().getDx() * i / dMax),
                     obj1.getPoint().getY() + (obj1.getVelocity().getDy() * i / dMax));
      Point point2(obj2.getPoint().getX() + (obj2.getVelocity().getDx() * i / dMax),
                     obj2.getPoint().getY() + (obj2.getVelocity().getDy() * i / dMax));
      
      float xDiff = point1.getX() - point2.getX();
      float yDiff = point1.getY() - point2.getY();
      
      float distSquared = (xDiff * xDiff) +(yDiff * yDiff);
      
      distMin = min(distMin, distSquared);
   }
   
   return sqrt(distMin);
}
*/

/***************************************************************
 * Non-Default Constructor. This makes setup of the game easier.
***************************************************************/
Game::Game(Point topLeft, Point bottomRight)
{
   this->topLeft = topLeft;
   this->bottomRight = bottomRight;
   rocks.push_back(NULL);
   setFrameCount(30);
   setRespawnCounter(5);
   setLivesRemaining(2);
}

/************
 * Destructor
************/
Game::~Game()
{
   vector<Rock*> :: iterator it = rocks.begin();
   while (it != rocks.end())
   {
      if ((*it)->isAlive())
         it = rocks.erase(it);
      else
         it++;
   }
   
}

/*****************************************************
 * General advance function. It will call the specific
 * advance functions needed to run the game.
*****************************************************/
void Game::advance()
{
   //main advance functions
   advanceRocks();
   advanceShip();
   advanceBullets();
   
   //check functions while advancing
   handleCollisions();
   cleanUpZombies();
}

/****************************************
 * Draws the different parts of the game!
****************************************/
void Game::draw(const Interface & ui)
{
   for (int i = 0; i < rocks.size(); i++)
      if ((rocks[i])->isAlive())
         (rocks[i])->draw();
   
   if ((ship).isAlive())
      (ship).draw();
   
   for (int i = 0; i < bullets.size(); i++)
      if ((bullets[i]).isAlive())
         (bullets[i]).draw();
}

/************************
 * Advances the asteroids
************************/
void Game::advanceRocks()
{
   if (rocks[0] == NULL && rocks.size() == 1)
      createLargeAsteroids();
   
   for (int i = 0; i < rocks.size(); i++)
      if (rocks[i]->isAlive())
         rocks[i]->advance(topLeft, bottomRight);
}

/*******************
 * Advances the ship
 ******************/
void Game::advanceShip()
{
   if (ship.isAlive())
      (ship).advance(topLeft, bottomRight);
   else
   {
      if (respawnCounter != -1)
         drawText(Point(-20, 0), "You Crashed!");
      handleRespawn();
   }
}

/**********************
 * Advances the bullets
 *********************/
void Game::advanceBullets()
{
   for (int i = 0; i < bullets.size(); i++)
      if (bullets[i].isAlive())
         bullets[i].advance(topLeft, bottomRight);
}

/************************************
 * Handles collisions within the game
 ***********************************/
void Game::handleCollisions()
{
   handleBulletCollisions();
   handleShipCollisions();
}

/***********************************
 * Handles all the input of the game
***********************************/
void Game::handleInput(const Interface & ui)
{
   if (ship.isAlive())
   {
      if (ui.isLeft())
         ship.turnLeft();
   
      if (ui.isRight())
         ship.turnRight();
   
      if (ui.isUp())
         ship.applyThrust();
   
      if (ui.isSpace())
      {
         Bullet newBullet;
         newBullet.fire(ship.getPoint(), ship.getAngle(), ship.getVelocity());
         
         bullets.push_back(newBullet);
      }
   }
}

/***************************
* Handles Bullet Collision!
**************************/
void Game::handleBulletCollisions()
{
   for (int i = 0; i < bullets.size(); i++)
   {
      if (bullets[i].isAlive())
      {
         for (int j = 0; j < rocks.size(); j++)
            if (rocks[j] != NULL && rocks[j]->isAlive())
               if (fabs(bullets[i].getPoint().getX() -
                   rocks[j]->getPoint().getX()) < rocks[j]->getRadius() &&
                   fabs(bullets[i].getPoint().getY() -
                   rocks[j]->getPoint().getY()) < rocks[j]->getRadius())
               {
                  rocks[j]->hit();
                  bullets[i].kill();
                  crashAsteroids(rocks[j]);
               }
      }
   }
}

/*************************
* Handles ship Collision!
************************/
void Game::handleShipCollisions()
{
   for (int i = 0; i < rocks.size(); i++)
   {
      if (rocks[i]->isAlive())
         if (fabs(ship.getPoint().getX() - rocks[i]->getPoint().getX())
             < rocks[i]->getRadius() &&
             fabs(ship.getPoint().getY() - rocks[i]->getPoint().getY())
             < rocks[i]->getRadius())
         {
            rocks[i]->hit();
            ship.crash();
            crashAsteroids(rocks[i]);
         }
   }
}

/***********************
* Handles ship respawn!
**********************/
void Game::handleRespawn()
{
   Point mP = Point(-25, -10);
   Point mP2 = Point(-30, -20);
   Point rP = Point(0, 0);
   Velocity rV = Velocity(0, 0);
   
   if (respawnCounter != -1)
      setFrameCount(getFrameCount() - 1);
   
   if (getFrameCount() == 0 && respawnCounter != -1)
   {
      setRespawnCounter(getRespawnCounter() - 1);
      setFrameCount(30);
   }
   
   switch (respawnCounter) {
      case 5:
         drawText(mP, "respawning in 5");
         break;
      case 4:
         drawText(mP, "respawning in 4");
         break;
      case 3:
         drawText(mP, "respawning in 3");
         break;
      case 2:
         drawText(mP, "respawning in 2");
         break;
      case 1:
         drawText(mP, "respawning in 1");
         break;
      case -1:
         drawText(Point(-10, 0), "Game Over!");
         break;
      default:
         setRespawnCounter(5);
         ship.setAlive(true);
         ship.setPoint(rP);
         ship.setVelocity(rV);
         ship.setShipOrientation(0);
         ship.setAngle(90);
         setLivesRemaining(getLivesRemaining() - 1);
         break;
   }
   
   switch (livesRemaining)
   {
      case 2:
         drawText(mP2, "Lives remaining: 2");
         break;
      case 1:
         drawText(mP2, "Lives remaining: 1");
         break;
      default:
         setRespawnCounter(-1);
         break;
   }
}

/**************************************
 * Cleans up any wandering dead objects
**************************************/
void Game::cleanUpZombies()
{
   vector<Rock*>::iterator rockIt = rocks.begin();
   while (rockIt != rocks.end())
   {
      Rock* pRock = *rockIt;
      // Asteroids Hint:
      // If we had a list of pointers, we would need this line instead:
      //Bullet* pBullet = *bulletIt;
      
      if (!pRock->isAlive())
      {
         // If we had a list of pointers, we would need to delete the memory here...
         
         
         // remove from list and advance
         rockIt = rocks.erase(rockIt);
      }
      else
         rockIt++; // advance
   }
   
   // Look for dead bullets
   vector<Bullet>::iterator bulletIt = bullets.begin();
   while (bulletIt != bullets.end())
   {
      Bullet bullet = *bulletIt;
      // Asteroids Hint:
      // If we had a list of pointers, we would need this line instead:
      //Bullet* pBullet = *bulletIt;
      
      if (!bullet.isAlive())
      {
         // If we had a list of pointers, we would need to delete the memory here...
         
         // remove from list and advance
         bulletIt = bullets.erase(bulletIt);
      }
      else
         bulletIt++; // advance
   }
}

/******************************************
 * Creates the Large asteroids for our game
******************************************/
void Game::createLargeAsteroids()
{
   rocks.clear();
   
   for (int i = 0; i < 5; i++)
   {
      float randomY = random(-199, 199);
      if (randomY < 30 && randomY > 0)
         randomY = 175.00;
      else if (randomY > -30 && randomY < 0)
         randomY = -175.00;
      
      float randomX = random(-199, 199);
      if (randomX < 30 && randomX > 0)
         randomX = 175.00;
      else if (randomX > -30 && randomX < 0)
         randomX = -175.00;
      
      
      Point tempPoint;
      tempPoint.setX(randomX);
      tempPoint.setY(randomY);
      
      int speed = 1;
      int rotationSpeed = 0;
      float dx = 0;
      float dy = 0;
      
      
      float randomSpeed = random(1, 5);
      if (randomSpeed == 1)
      {
         dx = speed * (-cos(M_PI / 180.0));
         dy = speed * (sin(M_PI / 180.0));
         rotationSpeed = 2;
      }
      else if (randomSpeed == 2)
      {
         dx = speed * (-cos(M_PI / 180.0));
         dy = speed * (-sin(M_PI / 180.0));
         rotationSpeed = 2;
      }
      else if (randomSpeed == 3)
      {
         dx = speed * (cos(M_PI / 180.0));
         dy = speed * (sin(M_PI / 180.0));
         rotationSpeed = -2;
      }
      else if (randomSpeed == 4)
      {
         dx = speed * (cos(M_PI / 180.0));
         dy = speed * (-sin(M_PI / 180.0));
         rotationSpeed = -2;
      }


      Velocity tempV;
      tempV.setDx(dx);
      tempV.setDy(dy);
      
      LargeRock * rock = new LargeRock(tempPoint, tempV, rotationSpeed);
      rocks.push_back(rock);
   }
}

/*******************************
* Creates our medium asteroids!
******************************/
void Game::createMediumAsteroids(Point point, Velocity velocity)
{
   //the different Velocities!
   Velocity v1 = velocity;
   Velocity v2 = velocity;
   
   //for the different direction rocks
   int rotationSpeed1 = 0;
   int rotationSpeed2 = 0;
   
   //setup for first rock
   v1.setDy(v1.getDy() + 1.0);
   if (v1.getDx() >= 0)
      rotationSpeed1 = -5;
   else
      rotationSpeed1 = 5;
   
   MediumRock * rock1 = new MediumRock(point, v1, rotationSpeed1);
   rocks.push_back(rock1);
   
   //now for number two!
   v2.setDy(v2.getDy() - 1.0);
   if (v2.getDx() >= 0)
      rotationSpeed2 = -5;
   else
      rotationSpeed2 = 5;
   
   MediumRock * rock2 = new MediumRock(point, v2, rotationSpeed2);
   rocks.push_back(rock2);
}

/**************************
* Creates small asteroids!
*************************/void Game::createSmallAsteroids(Point point, Velocity velocity, int selector)
{
   Velocity v1 = velocity;
   Velocity v2 = velocity;
   
   int rotationSpeed1 = 0;
   int rotationSpeed2 = 0;
   
   if (selector == 1)
   {
      v1.setDx(v1.getDx() + 2.0);
      if (v1.getDx() >= 0)
         rotationSpeed1 = -10;
      else
         rotationSpeed1 = 10;
      
      SmallRock * rock = new SmallRock(point, v1, rotationSpeed1);
      rocks.push_back(rock);
   }
   else if (selector == 2)
   {
      v1.setDx(v1.getDx() + 3.0);
      if (v1.getDx() >= 0)
         rotationSpeed1 = -10;
      else
         rotationSpeed1 = 10;
      
      SmallRock * rock1 = new SmallRock(point, v1, rotationSpeed1);
      rocks.push_back(rock1);
      
      v2.setDx(-3.0);
      rotationSpeed2 = -10;
      
      SmallRock * rock2 = new SmallRock(point, v2, rotationSpeed2);
      rocks.push_back(rock2);
   }
   
}

/*************************************************************
* This handles the creation of rocks when hit or crashed into
************************************************************/
void Game::crashAsteroids(Rock * rock)
{
   if (rock->getRadius() == 16)
   {
      createMediumAsteroids(rock->getPoint(),
                            rock->getVelocity());
      createSmallAsteroids(rock->getPoint(),
                           rock->getVelocity(), 1);
   }
   else if (rock->getRadius() == 8)
      createSmallAsteroids(rock->getPoint(),
                           rock->getVelocity(), 2);
}
