/***************************************************************************//**
  @file     +gui.h+
  @brief    +header con los prototipos de gui+
  @author   +Grupo 7+
 ******************************************************************************/

#ifndef _GUI_h_
#define _GUI_h_

/*******************************************************************************
 * include header files
 ******************************************************************************/
#include "logic.h"

/*******************************************************************************
 * function prototypes with global scope
 ******************************************************************************/
int init_allegro(int width, int heigh);									//Funcion que inicializa todo lo que es arreglo

void destroy_allegro(void);												//Funcion que destruye todo allegro, no recibe nada y no devuelve nada

void drawBack(SIM_t *s, int r);											//Funcion que se encarga de la logica con allegro y timer

void drawChart(double tickCounter[], int r, int widht, int height);		//Funcion que dibuja y muestra el grafico del modo 2 con allegro

/*******************************************************************************
 ******************************************************************************/

#endif // _allfunc_h_