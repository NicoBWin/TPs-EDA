/***************************************************************************//**
@file     +NicoLCD+
@brief    +Header de la clase del display de Nicolás | Hereda de basicLCD+
@author   +Nicolás Bustelo+
******************************************************************************/

#ifndef _NICOLCD_H_
#define _NICOLCD_H_

/*******************************************************************************
* INCLUDE HEADER FILES
******************************************************************************/
#include "../basicLCD.h"

#include <iostream> 
#include <string>

/*******************************************************************************
* CONSTANT AND MACRO DEFINITIONS USING #DEFINE
******************************************************************************/



/*******************************************************************************
* ENUMERATIONS AND STRUCTURES AND TYPEDEFS
******************************************************************************/



/*******************************************************************************
* CLASS
******************************************************************************/
class DispN : public basicLCD {
public:
	DispN();

	~DispN();

	virtual bool lcdInitOk();

	virtual lcdError& lcdGetError();

	virtual bool lcdClear();

	virtual bool lcdClearToEOL();

	virtual basicLCD& operator<<(const char c);

	virtual basicLCD& operator<<(const char* c);

	virtual bool lcdMoveCursorUp();

	virtual bool lcdMoveCursorDown();

	virtual bool lcdMoveCursorRight();

	virtual bool lcdMoveCursorLeft();

	virtual bool lcdSetCursorPosition(const cursorPosition pos);

	virtual cursorPosition lcdGetCursorPosition();

private:
	cursorPosition coord;
	
	lcdError NicoError;
	//Buffer del mensaje a imprimir
	char msg[MAX_ROW + 1][MAX_COL + 1] = {' '};
	//Funcion para actualizar el display
	void updateDisplay();
	//Borra todo el mensaje
	void clearMsg();
	//Borra un caracter del mensaje en especifico
	void clearEOL();
};

/*******************************************************************************
******************************************************************************/

#endif // _NICOLCD_H_