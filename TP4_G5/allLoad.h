/***************************************************************************//**
  @file     +all_tools.h+
  @brief    +Header para los recursos de allegro+
  @author   +Grupo 5+
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

 /*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define WW1  "imgs/wwalking/wwalk-F1.png"		// defino defines de imagenes
#define WW2  "imgs/wwalking/wwalk-F2.png"
#define WW3  "imgs/wwalking/wwalk-F3.png"
#define WW4  "imgs/wwalking/wwalk-F4.png"
#define WW5  "imgs/wwalking/wwalk-F5.png"
#define WW6  "imgs/wwalking/wwalk-F6.png"
#define WW7  "imgs/wwalking/wwalk-F7.png"
#define WW8  "imgs/wwalking/wwalk-F8.png"
#define WW9  "imgs/wwalking/wwalk-F9.png"
#define WW10 "imgs/wwalking/wwalk-F10.png"
#define WW11 "imgs/wwalking/wwalk-F11.png"
#define WW12 "imgs/wwalking/wwalk-F12.png"
#define WW13 "imgs/wwalking/wwalk-F13.png"
#define WW14 "imgs/wwalking/wwalk-F14.png"
#define WW15 "imgs/wwalking/wwalk-F15.png"

#define WJ1  "imgs/wjump/wjump-F1.png"
#define WJ2  "imgs/wjump/wjump-F2.png"
#define WJ3  "imgs/wjump/wjump-F3.png"
#define WJ4  "imgs/wjump/wjump-F4.png"
#define WJ5  "imgs/wjump/wjump-F5.png"
#define WJ6  "imgs/wjump/wjump-F6.png"
#define WJ7  "imgs/wjump/wjump-F7.png"
#define WJ8  "imgs/wjump/wjump-F8.png"
#define WJ9  "imgs/wjump/wjump-F9.png"
#define WJ10 "imgs/wjump/wjump-F10.png"

#define BKG_FILE "imgs/Scenario.png"

#define W_PIXELS	1920				// tamaño del display
#define H_PIXELS	696

/*******************************************************************************
* ENUMERATIONS AND STRUCTURES AND TYPEDEFS
******************************************************************************/
enum ERRORES		// Distintos tipos de errores
{
	NO_ERROR = -2,
	ERROR = -1
};

/*******************************************************************************
* VARIABLE PROTOTYPES WITH GLOBAL SCOPE
******************************************************************************/
/*PUNTEROS ALLEGRO*/
extern ALLEGRO_DISPLAY* display;		// otros archivos puueden ver las variabes de allegro

extern ALLEGRO_TIMER* timer;
extern ALLEGRO_EVENT_QUEUE* event_queue;

extern ALLEGRO_BITMAP* wj1Image;
extern ALLEGRO_BITMAP* wj2Image;
extern ALLEGRO_BITMAP* wj3Image;
extern ALLEGRO_BITMAP* wj4Image;
extern ALLEGRO_BITMAP* wj5Image;
extern ALLEGRO_BITMAP* wj6Image;
extern ALLEGRO_BITMAP* wj7Image;
extern ALLEGRO_BITMAP* wj8Image;
extern ALLEGRO_BITMAP* wj9Image;
extern ALLEGRO_BITMAP* wj10Image;

extern ALLEGRO_BITMAP* bkgImage;

extern ALLEGRO_BITMAP* ww1Image;
extern ALLEGRO_BITMAP* ww2Image;
extern ALLEGRO_BITMAP* ww3Image;
extern ALLEGRO_BITMAP* ww4Image;
extern ALLEGRO_BITMAP* ww5Image;
extern ALLEGRO_BITMAP* ww6Image;
extern ALLEGRO_BITMAP* ww7Image;
extern ALLEGRO_BITMAP* ww8Image;
extern ALLEGRO_BITMAP* ww9Image;
extern ALLEGRO_BITMAP* ww10Image;
extern ALLEGRO_BITMAP* ww11Image;
extern ALLEGRO_BITMAP* ww12Image;
extern ALLEGRO_BITMAP* ww13Image;
extern ALLEGRO_BITMAP* ww14Image;
extern ALLEGRO_BITMAP* ww15Image;

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/
 //Funcion que le da la bienvenida al usuario por allegro
int init_allegro();

//Funcion que destruye todo allegro, no recibe nada y no devuelve nada
void destroy_allegro(void);

#endif	// _ALLTOOLS_H_