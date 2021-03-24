/***************************************************************************//**
  @file     +all_tools.h+
  @brief    +Header para los recursos de allegro+
  @author   +Grupo 4+
 ******************************************************************************/

#ifndef _ALLTOOLS_H_
#define _ALLTOOLS_H_

/*******************************************************************************
* INCLUDE HEADER FILES
******************************************************************************/
#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>

#include "Simu.h"
/*******************************************************************************
* CONSTANT AND MACRO DEFINITIONS USING #DEFINE
******************************************************************************/
#define BACKG_FILE  "imgs/background.jpg"
#define BBYBLOB_FILE  "imgs/babyblob.png"
#define GRWBLOB_FILE  "imgs/grownblob.png"
#define OLDBLOB_FILE   "imgs/goodoldblob.png"
#define FOOD_FILE	"imgs/food.png"

#define FONT1_FILE "fonts/font.ttf"

#define W_PIXELS	900
#define H_PIXELS	470

/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/
/*PUNTEROS ALLEGRO*/
extern ALLEGRO_DISPLAY* display;
extern ALLEGRO_DISPLAY* display2;

extern ALLEGRO_TIMER* timer;
extern ALLEGRO_EVENT_QUEUE* event_queue;
extern ALLEGRO_FONT* font1;

extern ALLEGRO_BITMAP* babyBlobBM;
extern ALLEGRO_BITMAP* grownBlobBM;
extern ALLEGRO_BITMAP* goodOldBlobBM;
extern ALLEGRO_BITMAP* backgroundBM;
extern ALLEGRO_BITMAP* foodBM;

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/
 //Funcion que le da la bienvenida al usuario por allegro
int init_allegro(int height, int width);		

//Funcion que destruye todo allegro, no recibe nada y no devuelve nada
void destroy_allegro(void);

//Dibuja la interface
void draw_interface(Simu& s);

//Muestra en la pantalla de allegro los blobs y las comida
void drawWildlife(Simu& sim);

#endif	// _ALLTOOLS_H_