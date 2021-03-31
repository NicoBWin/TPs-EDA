/***************************************************************************//**
@file     +allPrint.cpp+
@brief    +Funciones para imprimir los warms segun el  frameCount que corresponda+
@author   +Grupo 5+
******************************************************************************/
#include "allPrint.h"
#include "const.h"

/******************************************************************************
* FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
******************************************************************************/
///Busca imagen de caminata correspondiente al frame recibido
ALLEGRO_BITMAP* getCorrectimgW(int frameCount);

//Busca imagen de salto correspondiente al frame recibido
ALLEGRO_BITMAP* getCorrectimgJ(int frameCount);

/******************************************************************************
*******************************************************************************
					GLOBAL FUNCTION DEFINITIONS
*******************************************************************************
******************************************************************************/
//Printea worm en estado pasivo
void printIdle(Point& p, int direction) {
	if (direction == LEFT) {
		al_draw_bitmap(ww1Image, p.x, p.y, 0);
	}
	else {
		al_draw_bitmap(ww1Image, p.x, p.y, ALLEGRO_FLIP_HORIZONTAL);
	}
}

//Printea a worm caminando
void printWalk(Point& p, int frameCount, int direction) {
	ALLEGRO_BITMAP* image = getCorrectimgW(frameCount);	// busca imagen corrrespondiente al frame
	if (direction == LEFT) {
		al_draw_bitmap(image, p.x, p.y, 0);				// si se esta moviendo a la derecha
	}
	else                                               // si se esta moviendo a la izquierda

	{
		al_draw_bitmap(image, p.x, p.y, ALLEGRO_FLIP_HORIZONTAL);
	}
}

//Printea imagen de warmup
void printWarmup(Point& p, int frameCount, int direction) {	
	ALLEGRO_BITMAP* image = NULL;
	if (frameCount == 1) {
		image = ww1Image;
	}
	else if (frameCount == 2) {
		image = ww2Image;
	}
	else if (frameCount == 3) {
		image = ww3Image;
	}
	if (direction == LEFT) {
		al_draw_bitmap(image, p.x, p.y, 0);
	}
	else {
		al_draw_bitmap(image, p.x, p.y, ALLEGRO_FLIP_HORIZONTAL);
	}
}

//Printea imagen correspondiete al frame de salto enviado
void printJump(Point& p, int frameCount, int direction) {
	ALLEGRO_BITMAP* image = getCorrectimgJ(frameCount);
	if (direction == LEFT) {		// si esta saltando hacia la izquierda
		
		al_draw_bitmap(image, p.x, p.y, 0);
	}
	else {							// Si esta saltando hacia la derecha
		al_draw_bitmap(image, p.x, p.y, ALLEGRO_FLIP_HORIZONTAL);
	}
}


//Busca imagen de caminata correspondiente al frame recibido
ALLEGRO_BITMAP* getCorrectimgW(int frameCount) {
	if (frameCount == 1) {
		return ww4Image;
	}
	else if (frameCount == 2) {
		return ww5Image;
	}
	else if (frameCount == 3) {
	    return ww6Image;
	}
	else if (frameCount == 4) {
		return ww7Image;
	}
	else if (frameCount == 5) {
		return ww8Image;
	}
	else if (frameCount == 6) {
		return ww9Image;
	}
	else if (frameCount == 7) {
		return ww10Image;
	}
	else if (frameCount == 8) {
		return ww11Image;
	}
	else if (frameCount == 9) {
		return ww11Image;
	}
	else if (frameCount == 10) {
		return ww12Image;
	}
	else if (frameCount == 11) {
		return ww13Image;
	}	
	else if (frameCount == 12) {
		return ww14Image;
	}
	else if (frameCount == 13) {
		return ww15Image;
	}
	else if(frameCount == 14) {
		return ww4Image;
	}
	else {
		return ww1Image;
	}
}

//Busca imagen de salto correspondiente al frame recibido
ALLEGRO_BITMAP* getCorrectimgJ(int frameCount) {
	if ( frameCount == 5) {
		return wj1Image;
	}
	if ( frameCount == 6) {
		return wj2Image;
	}
	if ( frameCount == 7) {
		return wj3Image;
	}
	if ( frameCount == 8) { 
		return wj4Image;
	}
	if ((frameCount > 8) && (frameCount < 45)) {
		return wj5Image;
	}
	if ( frameCount == 45) {
		return wj6Image;
	}
	if ( frameCount == 46) {
		return wj7Image;
	}
	if ( frameCount == 47) {
		return wj8Image;
	}
	if (frameCount == 48) {
		return wj9Image;
	}
	if ( frameCount == 49) {
		return wj10Image;
	}
	else {
		return wj1Image;
	}
}