#include <iostream>
#include "DisplayFran.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_primitives.h>


#define ROWMAX	1
#define COLMAX	15
#define CHARS	((ROWMAX+1)*(COLMAX+1))
#define POSACTUAL (cursor.column + cursor.row * (COLMAX+1))


#define DISPLAYWIDTH	330
#define DISPLAYHEIGHT	147
#define BLACK		al_map_rgb(0, 0 ,0)
#define XOFFSET		40
#define YOFFSET		55
#define CHARDISTX	16
#define CHARDISTY	25
#define POSX(col)	((col)*CHARDISTX + XOFFSET)
#define POSY(row)	((row)*CHARDISTY + YOFFSET)
#define RECT_HEIGH	20
#define RECT_WIDTH	5

using namespace std;

static ALLEGRO_DISPLAY* displayF;
static ALLEGRO_FONT* fontF;
static ALLEGRO_BITMAP* bitmapBackground;

bool initAllegro(lcdError& error);
bool destroyAllegro();

displayFran:: displayFran(){
		
	initOk = true;

	//Limpiamos el display
	initOk &= clearDisplay({ 0,0 }, {ROWMAX,COLMAX});

	initOk &= initAllegro(this->error);

	if (initOk == false) {
		destroyAllegro();
	}
	cursor.column = 0;
	cursor.row = 0;

	this->updateDisplay();
}

displayFran:: ~displayFran(){
	clearDisplay({0,0}, {ROWMAX,COLMAX});
	destroyAllegro();
}

bool displayFran :: lcdInitOk() {
	return	initOk;
}

lcdError& displayFran::lcdGetError() {
	return this->error;
}

bool displayFran::lcdClear() {
	if (!clearDisplay({0,0}, {ROWMAX,COLMAX})) {
		//error= lcdError("Cleaning error.", "The display can't be cleaned", FAIL_CLEAR);
		return false;
	}
	cursor.row = 0;
	cursor.column = 0;

	updateDisplay();
	return true;
}

bool displayFran::lcdClearToEOL() {
	if (!clearDisplay(cursor, {cursor.row, COLMAX})) {
		//error.setError("Cleaning error.", "The display can't be cleaned to EOL", FAIL_CLEAR);
		return false;
	}
		
	updateDisplay();
	return true;
}

basicLCD& displayFran :: operator<< (const char c) {
	text[POSACTUAL] = c;
		
	if (cursor.column == COLMAX && cursor.row < ROWMAX) {
		cursor.row++;
		cursor.column = 0;
	}
	else if (cursor.column < COLMAX) {
		cursor.column++;
	}
	updateDisplay();
	return *this;
}

basicLCD& displayFran:: operator<<(const char* c) {
	string temp((const char*)c);
	int tempSize = (int) temp.size();
	int posActual = cursor.column + cursor.row * (COLMAX + 1);
	
	if (tempSize >= CHARS)
	{
		text.assign(temp, tempSize - CHARS, tempSize);
	}

	else 
	{
		string string1=temp.substr(0, CHARS - posActual);   
		text.replace(posActual, string1.size(), string1);

		if ((posActual + tempSize) > CHARS)
		{
			string string2= temp.substr(string1.size());
			text.replace(0, string2.size(), string2);

			cursor.row = (int)(string2.size() / (COLMAX + 1));
			cursor.column = (int)(string2.size() % (COLMAX + 1));
		}
		else 
		{
			// Posicion final del cursor para el caso de que no se haya usado string2
			int posFinal = string1.size() + posActual;
			if (!(posFinal % CHARS))
			{
				cursor.row = 0;
				cursor.column = 0;
			}
			else
			{
				cursor.row = posFinal / (COLMAX + 1);
				cursor.column = posFinal % (COLMAX + 1);
			}
		}
	}


	updateDisplay();
	return *this;
}

bool displayFran::lcdMoveCursorUp()
{
	if (cursor.row > 0)
	{
		cursor.row-=1;
		this->updateDisplay();
		return true;
	}
	else
	{

		//error.setError("Up Boundry error.", "The cursor is already in the high boundary", UP_BOUNDARY);
		return false;
	}
}

bool displayFran::lcdMoveCursorDown()
{
	if (cursor.row < ROWMAX)
	{
		cursor.row+=1;
		this->updateDisplay();
		return true;
	}
	else
	{
		//error.setError("Down Boundry error.", "The cursor is already in the low boundary", DOWN_BOUNDARY);
		return false;
	}
}

bool displayFran::lcdMoveCursorRight() {
	if (cursor.column < COLMAX)
	{
		cursor.column++;
		this->updateDisplay();
		return true;
	}
	else
	{
		if (cursor.row == COLMAX)
		{

			//error.setError("Right Boundry error.", "The cursor is already in the right boundary", RIGHT_BOUNDARY);
			return false;
		}
		else
		{
			cursor.column = 0;
			cursor.row++;
			updateDisplay();
			return true;
		}
	}
}

bool displayFran::lcdMoveCursorLeft() {
	
	if (cursor.column >= 1)
	{
		cursor.column--;
		updateDisplay();
		return true;
	}
	else
	{
		if (cursor.row <= 0)
		{
			//error.setError("Left Boundry error.", "The cursor is already in the left boundary", LEFT_BOUNDARY);
			//error = lcdError("Bitmap", "ERROR: failed to load BKG Bitmap!", ImgError);
			return false;
		}
		else
		{
			cursor.column = COLMAX;
			cursor.row--;
			updateDisplay();
			return true;
		}
	}
}

bool displayFran::lcdSetCursorPosition(const cursorPosition pos) {
	if (pos.row <= COLMAX && pos.row >= 0 && pos.column >= 0 && pos.column <= ROWMAX)
	{
		cursor.row = pos.row;
		cursor.column = pos.column;
		return true;
	}
	else 
	{
		//error.setError("Cursor Out of bounds", "The cursor position is bigger than the display.", OUT_OF_BOUNDS);
		return false;
	}
}

cursorPosition displayFran::lcdGetCursorPosition(){
	return cursor;
}

bool displayFran::updateDisplay() {
	
	al_set_target_backbuffer(displayF);

	al_draw_bitmap(bitmapBackground, 0.0, 0.0, 0);

	for (int row = 0; row <= ROWMAX; row++) {
		for (int col = 0; col <= COLMAX; col++) {
			char c = text[row * (COLMAX+1) + col];
			al_draw_text(fontF, BLACK, POSX(col), POSY(row), ALLEGRO_ALIGN_LEFT, &c);
			al_flip_display();
		}
	}

	int posx = (cursor.column)*CHARDISTX + XOFFSET;
	int posy = (cursor.row)*CHARDISTY + YOFFSET;

	al_draw_filled_rectangle(posx, posy - 6, posx + RECT_WIDTH, posy + RECT_HEIGH + 2 - 5, BLACK);

	al_flip_display();

	return	true;
}

bool displayFran::clearDisplay(cursorPosition cursorini, cursorPosition cursorf)
{
	try {
		int posi = cursorini.column + cursorini.row * (COLMAX+1);
		int posf = cursorf.row * (COLMAX+1) + cursorf.column;
		text = text.replace(posi, posf - posi+1, posf - posi+1, ' ');
	}
	catch (const std::out_of_range& oor)
	{
		cout << oor.what() << endl;
		return false;
	}

	catch (const std::length_error& l_)
	{
		cout << l_.what() << endl;
		return false;
	}

	catch (const std::bad_alloc& ba)
	{
		cout << ba.what() << endl;
		return false;
	}

	return true;
}

bool initAllegro(lcdError& error) {
	/*
	if (!al_init()){
		error.setError("Initializing allegro error", "An error ocurred while al_init()", ALINIT);
		return false;
	}

	al_init_image_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	al_init_primitives_addon();
	*/

	displayF = al_create_display(DISPLAYWIDTH, DISPLAYHEIGHT);
	al_set_window_title(displayF, "Fran's LCD");
	bitmapBackground = al_load_bitmap("LCDs/FranLCD/resources/displayFran.png");
	fontF = al_load_ttf_font("LCDs/FranLCD/resources/FontFran.ttf", 16, 0);

	if (displayF == NULL){
		//error.setError("Display Error", "Error creating display", DISPLAY);
		error = lcdError("Cleaning error.", "The display can't be cleaned", DispError);
		return false;
	}
	else if (bitmapBackground == NULL) {
		//error.setError("Background error.", "Error loading the background", BACKGROUND);
		cout << "Pincho bitmap" << endl;
			return false;

	} else if (fontF == NULL) {
		//error.setError("Font error", "Error loading the font", FONT);
		cout << "Pincho font" << endl;
			return false;
	}

	return true;
}

bool destroyAllegro() {
	al_destroy_font(fontF);
	al_destroy_display(displayF);
	al_destroy_bitmap(bitmapBackground);
	/*
	al_shutdown_image_addon();
	al_shutdown_font_addon();
	al_shutdown_ttf_addon();
	*/

	return true;
}