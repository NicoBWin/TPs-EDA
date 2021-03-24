/***************************************************************************//**
  @file     +logic.cpp+
  @brief    +Descripcion del archivo+
  @author   +Grupo 7+
 ******************************************************************************/

#ifndef _LOGIC_H_
#define _LOGIC_H_

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define CLEAN   false
#define DIRTY   true

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
typedef struct {
    double x;
    double y;
    double angle;
} ROBOT_t;

typedef struct {
    int width;
    int height;
    bool* tiles;
} FLOOR_t;

typedef struct {
    ROBOT_t* rob;
    FLOOR_t* flo;
    unsigned int tickCount;
} SIM_t;

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/
SIM_t* createSim(int width, int height, int robCount);          //Funcion que crea simulacion con toda la informacion y la guarda. Recibe 3 enteros: alto y largo del piso y cantidad de robots. Devuelve un puntero a la estructura si salio todo bien, sino NULL.

void freeAllmem(SIM_t* simu, FLOOR_t* f, ROBOT_t* r);           //Funcion que libera la memoria asignada para todas las esturcturas de la simulación. Recibe 3 punteros: a estructura de simulacion, a estructura de piso y a estructura de robots. No devuelve nada.

bool simulate(SIM_t* s, int robots);                            //Funcion que se encarga de la logica de la programacion, recibe un puntero a la simulacion y la cantidad de robots. Devuelve el estado total del piso (DIRTY o CLEAN).

/*******************************************************************************
 ******************************************************************************/

#endif // _LOGIC_H_
