/***********************************************************************
 * Header File:
 *    User Interface Draw : put pixels on the screen
 * Author:
 *    Br. Helfrich
 * Summary:
 *    This is the code necessary to draw on the screen. We have a collection
 *    of procedural functions here because each draw function does not
 *    retain state. In other words, they are verbs (functions), not nouns
 *    (variables) or a mixture (objects)
 ************************************************************************/

#pragma once

#include <string>     // To display text on the screen
#include <cmath>      // for M_PI, sin() and cos()
#include <algorithm>  // used for min() and max()
#include "position.h" // Where things are drawn
using std::string;
using std::min;
using std::max;

/*************************************************************************
 * DRAW TEXT
 * Draw text using a simple bitmap font
 ************************************************************************/


#include <sstream>

/*************************************************************************
 * GRAPHICS STREAM
 * A graphics stream that behaves much like COUT except on a drawn screen
 *************************************************************************/
class ogstream : public std::ostringstream
{
public:
   ogstream()                    : pos()    {          }
   ogstream(const Position& pos) : pos(pos) {          }
   ~ogstream()                              { flush(); }
   
   // Methods specific to drawing text on the screen
   void flush();
   void setPosition(const Position& pos) { flush(); this->pos = pos; }
   ogstream& operator = (const Position& pos)
   {
      setPosition(pos);
      return *this;
   }
   
   // Specific methods for the game
   void drawLine(const Position & begin, const Position & end,
                 double red = 0.0, double green = 0.0, double blue = 0.0);
   void drawRectangle(const Position & begin, const Position & end,
                      double red = 0.0, double green = 0.0, double blue = 0.0);
   void drawProjectile(const Position& pos, double age = 0.0);
   void drawHowitzer(const Position & pos, double angle, double age);
   void drawTarget(const Position& pos);
   void drawText(const Position & topLeft, const char * text);
private:
   
   Position rotate(const Position& origin, double x, double y, double rotation);
   Position pos;
};


/******************************************************************
 * RANDOM
 * This function generates a random number.  The user specifies
 * The parameters 
 *    INPUT:   min, max : The number of values (min <= num <= max)
 *    OUTPUT   <return> : Return the integer/double
 ****************************************************************/
int    random(int    min, int    max);
double random(double min, double max);


