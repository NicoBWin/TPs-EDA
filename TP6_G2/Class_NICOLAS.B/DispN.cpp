/***************************************************************************//**
@file     +DispN+
@brief    +Funciones de la clase de Nicolás+
@author   +Nicolás Bustelo+
******************************************************************************/

/*******************************************************************************
* INCLUDE HEADER FILES
******************************************************************************/
#include "DispN.h"

//ALLEGRO LIB'S
#include <allegro5/allegro.h>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>

using namespace std;

/*******************************************************************************
* CONSTANT AND MACRO DEFINITIONS USING #DEFINE
******************************************************************************/
#define BACKG_FILE	"Class_NICOLAS.B/resources/background.jpg"
#define FONT_FILE "Class_NICOLAS.B/resources/font.TTF"

#define D_WIDTH     500
#define D_HEIGHT    250

#define SIZE_FONT    35

ALLEGRO_DISPLAY* displayN = NULL;
ALLEGRO_FONT* font = NULL;
ALLEGRO_BITMAP* backgroundBM = NULL;

/*******************************************************************************
* ENUMERATIONS AND STRUCTURES AND TYPEDEFS
******************************************************************************/


/******************************************************************************
* MEMBER FUNTIONS
******************************************************************************/
DispN::DispN() {
    this->coord.row = 0;
    this->coord.column= 0;
}

DispN::~DispN() {
    al_destroy_font(font);
    al_destroy_bitmap(backgroundBM);
    al_destroy_display(displayN);
}

//Inicializa el display
bool DispN::lcdInitOk() {
    displayN = al_create_display(D_WIDTH, D_HEIGHT);
    if (displayN) {
        backgroundBM = al_load_bitmap(BACKG_FILE);
        if (backgroundBM) {
            font = al_load_ttf_font(FONT_FILE, SIZE_FONT, 0);
            if (font) {
                updateDisplay();
                return true;
            }
            else
                NicoError = lcdError("Font", "ERROR: failed to load FONT!", FontError);
            al_destroy_bitmap(backgroundBM);
        }
        else
            NicoError = lcdError("Bitmap", "ERROR: failed to load BKG Bitmap!", ImgError);
            cout << "ERROR: failed to load BKG Bitmap!" << endl;
        al_destroy_display(displayN);
    }
    NicoError = lcdError("Display", "ERROR: failed to load display!", DispError);
    return false;
}

lcdError& DispN::lcdGetError() {
    return NicoError;
}

bool DispN::lcdClear() {
    clearMsg();
    this->coord.row = 0;
    this->coord.column = 0;
    updateDisplay();
    return true;
}

bool DispN::lcdClearToEOL() {
    clearEOL();
    updateDisplay();
    return true;
}

basicLCD& DispN::operator<<(const unsigned char c) {
    msg[coord.row][coord.column] = c;
    if (this->coord.column < MAX_COL) {
        this->coord.column++;
    }
    else if (this->coord.column == MAX_COL && this->coord.row == MAX_ROW) {
        this->coord.row = 0;
        this->coord.column = 0;
    }
    else {
        this->coord.row++;
        this->coord.column=0;
    }
    updateDisplay();
    return *this;
}

basicLCD& DispN::operator<<(const unsigned char* c) {
    for (int i = 0; c[i] != '\0'; i++) {
        msg[coord.row][coord.column] = c[i];
        if (this->coord.column < MAX_COL) {
            this->coord.column++;
        }
        else if (this->coord.column == MAX_COL && this->coord.row == MAX_ROW) {
            this->coord.row = 0;
            this->coord.column = 0;
        }
        else {
            this->coord.row++;
            this->coord.column = 0;
        }
    }
    updateDisplay();
    return *this;
}

bool DispN::lcdMoveCursorUp() {
    if (this->coord.row > MIN_ROW) {
        this->coord.row--;
        updateDisplay();
        return true;
    }
    else {
        return false;
    }
}

bool DispN::lcdMoveCursorDown() {
    if (this->coord.row < MAX_ROW) {
        this->coord.row++;
        updateDisplay();
        return true;
    }
    else {
        return false;
    }
}

bool DispN::lcdMoveCursorRight() {
    if (this->coord.column < MAX_COL) {
        this->coord.column++;
        updateDisplay();
        return true;
    }
    else {
        return false;
    }
}

bool DispN::lcdMoveCursorLeft() {
    if (this->coord.column > MIN_COL) {
        this->coord.column--;
        updateDisplay();
        return true;
    }
    else {
        return false;
    }
}

bool DispN::lcdSetCursorPosition(const cursorPosition pos) {
    if (pos.row >= MIN_ROW || pos.row <= MAX_ROW) {
        if (pos.column >= MIN_COL || pos.column <= MAX_COL) {
            this->coord = pos;
            updateDisplay();
            return true;
        }
    }
    return false;
}

cursorPosition DispN::lcdGetCursorPosition() {
    return this->coord;
}

void DispN::updateDisplay() {
    al_set_target_backbuffer(displayN);
    al_draw_scaled_bitmap(backgroundBM, 0, 0,
        al_get_bitmap_width(backgroundBM), al_get_bitmap_height(backgroundBM),
        0, 0, D_WIDTH, D_HEIGHT,      // Con que tamaño queres que se dibuje la imagen
        0);
    
    for (int j = 0; j <= MAX_ROW; j++) {
        for (int i = 0; i <= MAX_COL; i++) { //Imprimo todos los caracteres en el display
            char temp[2];
            temp[0] = msg[j][i];
            temp[1] = '\0';
            al_draw_text(font, al_map_rgb(0, 0, 0), 71 + (23.8 * i), 90 + (40 * j), ALLEGRO_ALIGN_CENTER, temp);
        }
    }

    char higligt[2];
    if (msg[coord.row][coord.column] == ' ' || msg[coord.row][coord.column] == 0) {
        higligt[0] = '_';
    }
    else {
        higligt[0] = msg[coord.row][coord.column];
    }
    higligt[1] = '\0';
    al_draw_text(font, al_map_rgb(255, 0, 0), 71 + (23.8 * coord.column), 90 + (40 * coord.row), ALLEGRO_ALIGN_CENTER, higligt); //Imprime la legra a higligtear

    al_flip_display();
}

void DispN::clearMsg() {
    for (int j = 0; j <= MAX_ROW; j++) {
        for (int i = 0; i <= MAX_COL; i++) {
            msg[j][i] = ' ';
        }
    }
}

void DispN::clearEOL() {
    for (int i = coord.column; i <= MAX_COL; i++) {
        msg[coord.row][i] = ' ';
    }
}