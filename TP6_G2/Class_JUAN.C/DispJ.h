/***************************************************************************//**
@file     +DispJ+
@brief    +Header. Métodos del display. Hereda de basicLCD.h+
@author   +Juan Ignacio Causse. Legajo 61.105+
******************************************************************************/

#ifndef _DISPJ_H_
#define _DISPJ_H_

/*******************************************************************************
* INCLUDE HEADER FILES
******************************************************************************/

//Custom header files
#include "../basicLCD.h"

//Standard header files
#include <iostream> 
#include <string>

//Allegro header files
#include <allegro5/allegro.h>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>

/*******************************************************************************
* CONSTANT AND MACRO DEFINITIONS USING #DEFINE
******************************************************************************/

//Constants for Allegro resources
#define FONT_SIZE 24
#define DISPLAY_WIDTH 500
#define DISPLAY_HEIGHT 376

//Constants for Allegro drawing
#define XOFFSET 71
#define YOFFSET 90
#define	XINCREMENT 23.8
#define YINCREMENT 40

//Initial position constants
#define INITIAL_ROW 0
#define INITIAL_COLUMN 0

/*******************************************************************************
* CLASS
******************************************************************************/
class DispJ : public basicLCD {								//Inheritance
public:
	//Constructors, destructor and initializers
	DispJ();												//Default constructor
	~DispJ();												//Destructor
	virtual bool lcdInitOk();								//LCD Object initializer

	//Error control methods
	virtual lcdError& lcdGetError();						//Gets LCD status and errors

	//Display content methods
	virtual bool lcdClear();								//Clear entire display
	virtual bool lcdClearToEOL();							//Clear line
	virtual basicLCD& operator<<(const unsigned char c);	//Operator << overloading for char
	virtual basicLCD& operator<<(const unsigned char* c);	//Operator << overloading for string

	//Cursor control methods
	virtual bool lcdMoveCursorUp();							//Cursor control: up
	virtual bool lcdMoveCursorDown();						//Cursor control: down
	virtual bool lcdMoveCursorRight();						//Cursor control: right
	virtual bool lcdMoveCursorLeft();						//Cursor control: left
	virtual bool lcdSetCursorPosition(const cursorPosition pos);	//Sets position values
	virtual cursorPosition lcdGetCursorPosition();			//Returns current position

private:
	//Data members - Internal display
	lcdError JError;										//Error object
	cursorPosition JCursorPosition;							//Cursor position structure
	char JDisplayContent[MAX_ROW + 1][MAX_COL + 1];			//Char array for LCD content storage
	
	//Data members - Allegro resources
	ALLEGRO_DISPLAY* JDisplay = NULL;
	ALLEGRO_BITMAP* JBackground = NULL;
	ALLEGRO_FONT* JFont = NULL;

	//Methods - Initializers and getters for Allegro Resources
	lcdError& JDisplayInit(int height, int width);				//Initialize display
	lcdError& JBackgroundInit(const char* path);					//Initialize background
	lcdError& JFontInit(const char* path, int size);				//Initialize font
	ALLEGRO_DISPLAY* JGetDisplay();							//Display getter
	ALLEGRO_BITMAP* JGetBackground();						//Background getter
	ALLEGRO_FONT* JGetFont();								//Font getter

	//Methods - Other private methods for Allegro usage
	void LCDRefresh();		//Draw background and text;
	void LCDReset();		//Draw background and delete written text
};

/*******************************************************************************
******************************************************************************/

#endif // _DISPJ_H_