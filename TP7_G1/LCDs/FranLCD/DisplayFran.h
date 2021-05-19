#ifndef FDISP
#define FDISP
#include "../basicLCD.h"

#include <string>

class displayFran: public basicLCD
{
public:
	displayFran();
	~displayFran();

	virtual bool lcdInitOk();
	virtual lcdError& lcdGetError();
	virtual bool lcdClear();
	virtual bool lcdClearToEOL();
	virtual basicLCD& operator<< (const char c);
	virtual basicLCD& operator<<(const char* c);
	virtual bool lcdMoveCursorUp();
	virtual bool lcdMoveCursorDown();
	virtual bool lcdMoveCursorRight();
	virtual bool lcdMoveCursorLeft();
	virtual bool lcdSetCursorPosition(const cursorPosition pos);
	virtual cursorPosition lcdGetCursorPosition();

	lcdError error;
private:
	cursorPosition cursor;
	std::string text;
	bool initOk;

	bool updateDisplay();

	bool clearDisplay(cursorPosition cursorini, cursorPosition cursorf);

};
#endif