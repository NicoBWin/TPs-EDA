/***************************************************************************//**
  @file     +blob.h+
  @brief    +Descripcion del archivo+
  @author   +Grupo 4+
 ******************************************************************************/

#ifndef _BLOBTYPE_H_
#define _BLOBTYPE_H_

/******************************************************************************
* INCLUDE HEADER FILES
******************************************************************************/
#include <cstdbool>
#include "Point.h"

#define EAT true
#define NEAT false

#define ALIVE true
#define DEATH false

enum AGES { babyBlob = 0, grownBlob, goodOldBlob };

/*******************************************************************************
* CLASS
******************************************************************************/
class Blob {
public:
	Point p;
	float angle;
	int age;
	int foodCount;	//Contador de comida para cada blob
	float maxVel;	//Velocidad relativa a la maxima de simulación
	float aliveProb;	//Probabilidad de blob de muerte, se randomiza con cada tick
	bool isAlive;	//Variable bool para definir si estan vivos o no

	//Mueve el blob designado a la posicion indicada por newPos
	void moveb(Point& offset);

	//Incrementa el alimento del blob
	void feed();

	//Setea un angulo random, recibe el maximo cono grados
	void blobRAngle(int angleG);

	//Genera un nuevo blob a partir de la cantidad de comida ingerida
	bool blobBirth();

	//Combina dos o mas blob del mismo grupo etario 
	int blobDeath(float prob, int counter);
}; 

#endif // _BLOBTYPE_H_