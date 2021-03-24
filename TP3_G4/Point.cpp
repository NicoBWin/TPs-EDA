/***************************************************************************//**
  @file     +Point.cpp+
  @brief    ++
  @author   +Grupo 4+
 ******************************************************************************/
#include "Point.h"
#include <cmath>


void Point::randPoint(int width, int height) {
	this->x = (float)(rand() % width);
	this->y = (float)(rand() % height);
}

void Point::movep(Point& offset) {
	this->x += offset.x;
	this->y += offset.y;
}

float Point::distBtwPoint(Point& p) {
	float var = (float) sqrt((pow((this->x - p.x), 2)) + (pow((this->y - p.y), 2)));
	return var;
}

float Point::angB2Point(Point& p2) { //Lo debe devolver en radianes!
	Point p1 = { this->x, this->y };
	float div = ((p2.y - this->y) / p1.distBtwPoint(p2));
	div = (float)(asin(div));
	return  div;//EL RESULTADO ES EN RADIANES, LE SUMO PI/2 PARA QUE EMPIECE DE ARRIBA, y luego lo opongo para que sea sentido horario
}