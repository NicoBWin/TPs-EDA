/***************************************************************************//**
  @file     +Simu.h+
  @brief    +Descripcion del archivo+
  @author   +Grupo 4+
 ******************************************************************************/

#ifndef _SIMU_H_
#define _SIMU_H_

/******************************************************************************
* INCLUDE HEADER FILES
******************************************************************************/
#include "Blob.h"


#define MAXBLOBS	200
#define SMELL_INIT	100
#define ANG_INIT	360
#define VEL_INIT	100
#define AL_PROB		1.0

struct Food {
	Point p;
};

class Simu {
public:
	Blob blob[MAXBLOBS];
	Food* food;

	int smellRadius;	//Inidica el radio de smell de los blobs
	int randomJiggleLimit;	//Angulo random para los merge
	int blobCount; //Indica la cantidad de blobs vivos
	int relVel;	//Velocidad maxima definida por el usuario
	int foodCount;	//Cantidad de comida de 0 a foodCount-1
	
	float deathRateBB; //Probabilidad de muerte para cada grupo etario definida por usuario
	float deathRateGW;
	float deathRateGO;

	//Dimensiones del area de simulación
	int width;
	int height;
	//Dimensiones de babyblob
	int widthBBB;		
	int heightBBB;
	//Dimensiones de grownblob
	int widthGWB;
	int heightGWB;
	//Dimensiones de goodoldblob
	int widthGOB;
	int heightGOB;

	Simu(int modo, int initBlobs, int food, int relVel, int width, int height, int width1, int height1, int width2, int height2, int width3, int height3);

	//Libera la memoria asignada para food
	void deleteSimu();
};

#endif // _SIMU_H_
