#ifndef MILAGROS_LCD_H__
#define MILAGROS_LCD_H__

#include "../basicLCD.h"

#include <string>

#define HOME_C	0	
#define HOME_R	0	

#define C_SIZE	45	//Tamaño de las letras

#define COLUMNS	16
#define ROWS	2

class milagrosLCD : public basicLCD {
public:
	milagrosLCD();
	~milagrosLCD();

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
	lcdError LCDError;
	cursorPosition	position;
	bool initOK;
	std::string data;


	void init_display();
	void init_fonts();

	void clear_display();
	void write_display();

	int getCoords(int num);
};

#endif	// MILAGROS_LCD_H__