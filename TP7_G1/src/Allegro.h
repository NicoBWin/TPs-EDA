/***************************************************************************//**
@file     +Allegro.h+
@brief    +Header de las funciones de inicializacion de allegro+
@author   +Grupo 1+
******************************************************************************/

#ifndef _ALLEGRO_H_
#define _ALLEGRO_H_

/*******************************************************************************
* INCLUDE HEADER FILES
******************************************************************************/
//ALLEGRO LIBS
#include <allegro5/allegro.h>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>

/*******************************************************************************
* VARIABLE PROTOTYPES WITH GLOBAL SCOPE
******************************************************************************/
/*PUNTEROS ALLEGRO*/
extern ALLEGRO_EVENT_QUEUE* event_queue;
extern ALLEGRO_DISPLAY* displayAll;
extern ALLEGRO_TIMER* timer;
/*******************************************************************************
* CONSTANT AND MACRO DEFINITIONS USING #DEFINE
******************************************************************************/
#define W_PIXELS	800
#define H_PIXELS	400

/*******************************************************************************
* FUNCTION PROTOTYPES WITH GLOBAL SCOPE
******************************************************************************/
//Inicializa las funciones básicas de allegro
bool init_allegro();
//Funcion que destruye todo allegro
void destroy_allegro(void);

/*******************************************************************************
******************************************************************************/
#endif  // _ALLEGRO_H_
