/***************************************************************************//**
  @file     +logic.cpp+
  @brief    +Descripcion del archivo+
  @author   +Grupo 4+
 ******************************************************************************/

#ifndef _LOGIC_H_
#define _LOGIC_H_

 /*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "Simu.h"

/*******************************************************************************
* CONSTANT AND MACRO DEFINITIONS USING #DEFINE
******************************************************************************/
#define MAXVEL		1000
#define MAXFOOD		50

#define PI 3.1415           //numero PI

/*******************************************************************************
* ENUMERATIONS AND STRUCTURES AND TYPEDEFS
******************************************************************************/
// Clase caja de disparo. A todos los objetos capaces de colisionar, para la dteccion de colision se le arma una caja de colision que sirve para el algoritmo de deteccion de colision
struct collBox {
    float x;         // Alcanza con la posicion del punto de arriba a la izquierda de la caja y el ancho y largo
    float y;
    int height;
    int width;
};



/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/
void simular(Simu& s);

Food* ammFood(Simu& s);
/*******************************************************************************
******************************************************************************/

#endif // _LOGIC_H_