/***************************************************************************//**
  @file     +Point.h+
  @brief    +Descripcion del archivo+
  @author   +Grupo 4+
 ******************************************************************************/

#ifndef _POINT_H_
#define _POINT_H_

#define PI 3.1415           //numero PI

class Point {
public:
    float x;
    float y;

    //Configura un nuevo punto segun los valores maximos ingresados por W y H
    void randPoint(int width, int height);

    //Mueve el punto según el offset que reciba
    void movep(Point& offset);

    //Calcula la distancia entre dos puntos
    float distBtwPoint(Point& p);

    //Calcula el ángulo entre dos puntos
    float angB2Point(Point& p2);
};

#endif // _POINT_H_