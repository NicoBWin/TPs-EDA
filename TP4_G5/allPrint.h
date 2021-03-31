/***************************************************************************//**
  @file     +allPrint.h+
  @brief    +Header para la clase allPrint+
  @author   +Grupo 5+
 ******************************************************************************/

#ifndef _ALL_PRINT_
#define _ALL_PRINT_

#include "allLoad.h"
#include "Worm.h"

//
void printIdle(Point& p, int direction);	// printeo worm pasivo

//
void printWalk(Point& p, int frameCount, int direction);	// printeo worm caminando


//
void printWarmup(Point& p, int frameCount, int direction);	// printeo worm iniciando la caminata


//
void printJump(Point& p, int frameCount, int direction);	// printeo worm saltando



#endif // _ALL_PRINT_
