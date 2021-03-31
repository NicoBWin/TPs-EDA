/***************************************************************************//**
  @file     +const.h+
  @brief    +Header con constantes importantes y enumeracion de eventos+
  @author   +Grupo 5+
 ******************************************************************************/

#ifndef _CONST_H_
#define _CONST_H_

 /*******************************************************************************
  * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
  ******************************************************************************/
#define EXTREMIZQ 674	// limites mundo
#define EXTREMDER 1176

#define DISTANCE 1.0
#define PI 3.141592
#define MOD(x) ( ( (x)>=0 ) ? (x) : (-(x)) )
#define RAD(x) ((x)*PI/180)

#define MAX_WORMS 2	// numero de worms
#define G 0.24		// valor de gravedad
#define VELMOV 9	// velocidad de walk
#define VELJUMP 4.5	// velocidad de salto
#define PISO 600	// coordenada en x del piso

#define ANGLE 60	// angulo de salto
/******************************************************************************
* ENUMERATIONS AND STRUCTURES AND TYPEDEFS
******************************************************************************/
enum EVENTS {		// cte de eventos
	IDLE = 0,

	START_MOVING_LEFT,
	START_MOVING_RIGHT,
	START_JUMP,

	MOVE_LEFT,
	MOVE_RIGHT,
	JUMP,

	FINISH_WALKING_L,
	FINISH_WALKING_R,
	FINISH_JUMP,
	
	RIGHT,
	LEFT
};

#endif // _CONST_H_