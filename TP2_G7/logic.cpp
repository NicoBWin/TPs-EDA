/***************************************************************************//**
  @file     +logic.cpp+
  @brief    +Logica de la simulación y algunas funciones de ayuda para la simulación+
  @author   +Grupo 7+
 ******************************************************************************/
 
/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdint.h>
#include <stdbool.h>

#include "logic.h"
#include "gui.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define PI 3.1415           //numero PI

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/
FLOOR_t* createFloor(int width, int height);                        //Funcion que asigna memoria para colocar las esturcturas del piso. Recibe 2 enteros: alto y largo del piso. Devuelve puntero a piso si todo salio bien, sino NULL.


ROBOT_t* createRobots(int width, int height, int cant);             //Funcion que asigna memoria para colocar las esturcturas de los robots. Recibe 3 enteros: alto y largo del piso y cantidad de robots. Devuelve puntero a robot si todo salio bien, sino NULL.


bool allTilesclean(SIM_t* s);                                       //Funcion que analiza si todo el piso está limpio. Recibe un puntero a la sim. Devuelve un bool (DIRTY o CLEAN).


void cleanTile(SIM_t* s, int r);                                    //Funcion que se encarga de limpiar la baldosa en la que está el robot. Recibe un puntero a sim y un entero que indica que robot es. No devuelve nada.


void translateRobot(SIM_t* s, int r);                               //Funcion que mueve la posición del robot de un punto a otro. Recibe un puntero a sim y un entero que indica que robot es. No devuelve nada.


double GetRandAngle(void);                                          //Funcion que genera un angulo random en radianes. No recibe nada. Devuelve un double (el angulo generado).


/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/


//Funcion que crea simulacion con toda la informacion y la guarda. Recibe 3 enteros: alto y largo del piso y cantidad de robots. Devuelve un puntero a la estructura si salio todo bien, sino NULL.
SIM_t* createSim(int width, int height, int robCount) {
    SIM_t* simu = (SIM_t*) malloc(sizeof(SIM_t));           //creo espacio en la memoria dinamica
    if (simu != NULL) {                                     //si es diferente de NULL entonces creame el piso, los robots y guardame la data
        simu->flo = createFloor(width, height);
        simu->rob = createRobots(width, height, robCount);
        simu->tickCount = 0;
        if (simu->flo == NULL || simu->rob == NULL) {       //Si puntero piso o robots es NULL entonces liberame y que sim valga NULL
            freeAllmem(simu, simu->flo, simu->rob);
            simu = NULL;
        }
    }
    else {
        free(simu);                 //si es NULL liberame la memoria
        simu = NULL;                //que sim valga NULL
    }
    return simu;                //devuelvo el puntero a sim
}


//Funcion que se encarga de la logica de la programacion, recibe un puntero a la simulacion y la cantidad de robots. Devuelve el estado total del piso (DIRTY o CLEAN).
bool simulate(SIM_t* s, int robots) {
    for (int i = 0; i < robots; i++) {      //voy robot por robot
            cleanTile(s, i);        //limpia baldosa donde esta el robot
            translateRobot(s, i);   //lo muevo a la proxima posicion
    }
    s->tickCount++;                 //incremento el tinck counter 
    return allTilesclean(s);        //devuelvo estado total del piso
}


//Funcion que libera la memoria asignada para todas las esturcturas de la simulación. Recibe 3 punteros: a estructura de simulacion, a estructura de piso y a estructura de robots. No devuelve nada.
void freeAllmem(SIM_t* simu, FLOOR_t* f, ROBOT_t* r) {
        free(r);            //libero robots

        free(f->tiles);     //libero estado de pisos
        free(f);            //libero piso

        free(simu);         //libero sim
}


/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
//Funcion que asigna memoria para colocar las esturcturas del piso. Recibe 2 enteros: alto y largo del piso. Devuelve puntero a piso si todo salio bien, sino NULL.
FLOOR_t* createFloor(int width, int height) {
    FLOOR_t* piso = (FLOOR_t*) malloc(sizeof(FLOOR_t));         //me manejo con memoria dinamica para el piso
    if (piso != NULL) {                                         //si es diferente de NULL entonces guardame la data
        piso->width = width;                                    //y creame las tiles para el estado de los pisos
        piso->height = height;
        piso->tiles = (bool*) malloc(sizeof(bool) * width * height);
        if (piso->tiles) {
            for (int i = 0; i < (width * height); i++) {    //Ensucio el piso entero
                piso->tiles[i] = DIRTY;
            }
        }
        else {                                              //sino duvuelvo NULL y libero memoria
            free(piso->tiles);
            free(piso);
            piso = NULL;
        }
    }
    return piso;                    //devuelvo puntero a piso o NULL
}


//Funcion que asigna memoria para colocar las esturcturas de los robots. Recibe 3 enteros: alto y largo del piso y cantidad de robots. Devuelve puntero a robot si todo salio bien, sino NULL.
ROBOT_t* createRobots(int width, int height, int cant) {
    ROBOT_t* robs = (ROBOT_t*) malloc(sizeof(ROBOT_t)*cant);                //me manejo con memoria dinamica para los robots
    if (robs != NULL) {                                                     //si es diferente de NULL entonces guardame la data
        for (int i = 0; i < cant; i++) {
            robs[i].x = (double) ((rand() % width) + (rand() % 100) * 0.01);      //generame posicion random en x con dos decimales
            robs[i].y = (double) ((rand() % height) + (rand() % 100) * 0.01);     //generame posicion random en y con dos decimales
            robs[i].angle = GetRandAngle();                                     //generame angulo random
        }
    }
    else {                                      //si fue NULL liberame memoria y devolveme NULL
        free(robs);
        robs = NULL;
    }
    return robs;                //devuelvo puntero a robots o NULL
}


//Funcion que analiza si todo el piso está limpio. Recibe un puntero a la sim. Devuelve un bool (DIRTY o CLEAN).
bool allTilesclean(SIM_t* s) {
    for (int i = 0; i < (s->flo->width * s->flo->height); i++) {        //recorro el piso
        if (s->flo->tiles[i] == CLEAN) {            //si esta limpio segui analizando
            continue;
        }
        else {                                  //si encuentra sucio devolveme DIRTY
            return DIRTY;
        }
    }
    return CLEAN;               //si esta todo limpio devolveme CLEAN
}


//Funcion que se encarga de limpiar la baldosa en la que está el robot. Recibe un puntero a sim y un entero que indica que robot es. No devuelve nada.
void cleanTile(SIM_t* s, int r) {
    int x = (int) s->rob[r].x;          //donde esta en x
    int y = (int) s->rob[r].y;          //donde esta en y
    int w = s->flo->width;              //largo del piso
    s->flo->tiles[ x + y * w ] = CLEAN;     //guardame limpio
}


//Funcion que mueve la posición del robot de un punto a otro. Recibe un puntero a sim y un entero que indica que robot es. No devuelve nada.
void translateRobot(SIM_t* s, int r) {
    double x = s->rob[r].x;
    double y = s->rob[r].y;
    x += cos(s->rob[r].angle);
    y += sin(s->rob[r].angle);
    if(x > 0 && x < s->flo->width && y > 0 && y < s->flo->height) {     //si no choca con la pared entonces lo muevo guardando data de posicion
        s->rob[r].x = x;        //me muevo en x lo que vale el coseno del angulo del robot
        s->rob[r].y = y;        //me muevo en y lo que vale el seno del angulo del robot
    }
    else {
        s->rob[r].angle = GetRandAngle();           //si choca con la pared generame un nuevo angulo random y guardalo
    }
}


//Funcion que genera un angulo random en radianes. No recibe nada. Devuelve un double (el angulo generado).
double GetRandAngle(void) {
    double angle = 0;
    angle = rand() % 360;       //opciones de 360 numeros
    angle = (angle * PI) / 180; //Convierto el angulo a radianes
    return angle;               //devuelvo angulo en radianes
}