/***************************************************************************//**
  @file     +Worm.h+
  @brief    +Header para la clase WORMS+
  @author   +Grupo 5+
 ******************************************************************************/

#ifndef _WORM_H_
#define _WORM_H_

#include "const.h"
#include <stdio.h>
#include <stdbool.h>
#include <string>
#include <ctime>
#include <stdlib.h>
#include <stddef.h>
#include <math.h>
#include <iomanip>
#include <random>
#include <vector>

struct Point {
	double x;
	double y;
};

class Worm {		// creo clase de worms
public:

	Worm(void);
	Point getPosition(void);
	double getPosx(void);//te dice donde esta el worm en x
	double getPosy(void);//te dice donde esta el worm en y
	double getAngle(void);//da el angulo del worm
	int getEstado(void);
	int getDirection(void);

	void setPos(Point& aux);//cambia la pos del worm
	void setAngle(int angle);
	void setEstado(int newEstado); //Configura el estado del worm según el estado que recibe
	
	void moveSetWorm(void);//mueve el worm
	void jumpWorm(void);//salta el worm

	void update(int worm);	//Actualiza el worm, ya sea posición, llama a funcion para actualizar imagen y actualiza su estado


private:
	Point p;
	double angle;
	int estado;
	int direction;
};

#endif // _WORM_H_