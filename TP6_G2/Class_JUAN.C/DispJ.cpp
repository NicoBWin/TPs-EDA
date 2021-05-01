/***************************************************************************//**
@file     +DispJ.cpp+
@brief    +Source. Métodos del display. Hereda de basicLCD.h+
@author   +Juan Ignacio Causse. Legajo 61.105+
******************************************************************************/

/*******************************************************************************
* INCLUDE HEADER FILES
******************************************************************************/

//Custom header files
#include "DispJ.h"

/******************************************************************************
* CLASS PUBLIC MEMBER METHODS
******************************************************************************/

DispJ::DispJ() {    //Default constructor
    //Set starting position
    this->JCursorPosition.row = INITIAL_ROW;
    this->JCursorPosition.column = INITIAL_COLUMN;
    //Clear content  matrix
    unsigned short i, j;
    for (i = 0; i <= MAX_ROW; i++) {
        for (j = 0; j <= MAX_COL; j++) {
            JDisplayContent[i][j] = ' ';
        }
    }
}

DispJ::~DispJ() {   //Destructor
    al_destroy_display(JDisplay);
    al_destroy_font(JFont);
    al_destroy_bitmap(JBackground);
}

bool DispJ::lcdInitOk() {
    unsigned long error;
    error = (DispJ::JDisplayInit(DISPLAY_HEIGHT, DISPLAY_WIDTH)).getErrorCode();
    if (error == DispError) {
        return false;
    }
    error = ((DispJ::JBackgroundInit("Class_JUAN.C/res/Jbackground.png")).getErrorCode());
    if (error == ImgError) {
        al_destroy_display(this->JDisplay);
        return false;
    }
    error = (DispJ::JFontInit("Class_JUAN.C/res/verdana.ttf", FONT_SIZE).getErrorCode());
    if (error == FontError) {
        al_destroy_display(this->JDisplay);
        al_destroy_bitmap(this->JBackground);
        return false;
    }
    DispJ::LCDRefresh();
    return true;
}

lcdError& DispJ::lcdGetError() {
    return JError;
}

bool DispJ::lcdClear() {
    bool returnValue = true, contentIsEmpty = true;
    unsigned short i, j;
    for (i = 0; i <= MAX_ROW; i++) {
        for (j = 0; j <= MAX_COL; j++) {
            if (this->JDisplayContent[i][j] != ' ') {
                contentIsEmpty = false;
                break;
            }
        }
    }
    if (contentIsEmpty)
        returnValue = false;
    else {
        for (i = 0; i <= MAX_ROW; i++) {
            for (j = 0; j <= MAX_COL; j++) {
                this->
                    JDisplayContent[i][j] = ' ';
            }
        }
    }
    LCDReset();
    return returnValue;
}

bool DispJ::lcdClearToEOL() {
    bool returnValue = true;
    if (JCursorPosition.column == MAX_COL)
        returnValue = false;
    else {
        unsigned short i;
        for (i = 0; i <= MAX_COL; i++)
            JDisplayContent[JCursorPosition.row][JCursorPosition.column + i] = ' ';
    }
    LCDRefresh();
    return returnValue;
}

basicLCD& DispJ::operator<<(const unsigned char c) {
    this->JDisplayContent[JCursorPosition.row][JCursorPosition.column] = c;
    if (this->JCursorPosition.column < MAX_COL)
        this->JCursorPosition.column++;
    else if ((this->JCursorPosition.row == MAX_ROW) && (this->JCursorPosition.column == MAX_COL)) {
        this->JCursorPosition.row = 0;
        this->JCursorPosition.column = 0;
    }
    else {
        this->JCursorPosition.row++;
        this->JCursorPosition.column++;
    }
    LCDRefresh();
    return *this;
}

basicLCD& DispJ::operator<<(const unsigned char* c) {
    unsigned short i;
    for (i = 0; c[i] != '\0'; i++) {
        this->JDisplayContent[JCursorPosition.row][JCursorPosition.column] = c[i];
        if (this->JCursorPosition.column < MAX_COL)
            this->JCursorPosition.column++;
        else if ((this->JCursorPosition.row == MAX_ROW) && (this->JCursorPosition.column == MAX_COL)) {
            this->JCursorPosition.row = 0;
            this->JCursorPosition.column = 0;
        }
        else {
            this->JCursorPosition.row++;
            this->JCursorPosition.column++;
        }
    }
    LCDRefresh();
    return *this;
}

bool DispJ::lcdMoveCursorUp() {
    bool returnValue = true;
    if (JCursorPosition.row > MIN_ROW)
        JCursorPosition.row--;
    else
        returnValue = false;
    return returnValue;
}

bool DispJ::lcdMoveCursorDown() {
    bool returnValue = true;
    if (JCursorPosition.row < MAX_ROW)
        JCursorPosition.row++;
    else
        returnValue = false;
    return returnValue;
}

bool DispJ::lcdMoveCursorRight() {
    bool returnValue = true;
    if (JCursorPosition.column < MAX_COL)
        JCursorPosition.column++;
    else
        returnValue = false;
    return returnValue;
}

bool DispJ::lcdMoveCursorLeft() {
    bool returnValue = true;
    if (JCursorPosition.column > MIN_COL)
        JCursorPosition.column--;
    else
        returnValue = false;
    return returnValue;
}

bool DispJ::lcdSetCursorPosition(const cursorPosition pos) {
    bool returnValue = true;
    if (!((pos.row >= MIN_ROW) && (pos.row <=  MAX_ROW)))
        returnValue = false;
    else if (!((pos.column >= MIN_COL) && (pos.column <= MAX_COL)))
        returnValue = false;
    else {
        this->JCursorPosition.row = pos.row;
        this->JCursorPosition.column = pos.column;
    }
    return returnValue;
}

cursorPosition DispJ::lcdGetCursorPosition() {
    return this->JCursorPosition;
}

/******************************************************************************
* CLASS PRIVATE MEMBER METHODS
******************************************************************************/
lcdError& DispJ::JDisplayInit(int height, int width){
    this->JDisplay = al_create_display(width, height);
    if (this->JDisplay == NULL)
        this->JError = lcdError("Juan: Display Error.", "Failed to create display.", DispError);
    else
        this->JError = lcdError("Juan: Display: No error", "Succesfully created display.", NOERROR);
    return this->JError;
}

lcdError& DispJ::JBackgroundInit(const char* path){
    this->JBackground = al_load_bitmap(path);
    if (this->JBackground == NULL)
        this->JError = lcdError("Juan: Background Error.", "Failed to load background image.", ImgError);
    else
        this->JError = lcdError("Juan: Background: No error", "Succesfully loaded background.", NOERROR);
    return this->JError;
}

lcdError& DispJ::JFontInit(const char* path, int size){
    this->JFont = al_load_ttf_font(path, size, 0);
    if (this->JFont == NULL)
        this->JError = lcdError("Juan: Font Error.", "Failed to load TTF font.", FontError);
    else
        this->JError = lcdError("Juan: Font: No error", "Succesfully loaded TTF font.", NOERROR);
    return this->JError;
}

ALLEGRO_DISPLAY* DispJ::JGetDisplay(){
    return JDisplay;
}

ALLEGRO_BITMAP* DispJ::JGetBackground(){ 
    return JBackground; 
}

ALLEGRO_FONT* DispJ::JGetFont(){ 
    return JFont; 
}

void DispJ::LCDRefresh() {
    al_set_target_backbuffer(this->JDisplay);
    al_draw_bitmap(this->JBackground,0,0,0);
    unsigned short i, j;
    char temporalOutputArray[2];
    temporalOutputArray[1] = '\0';
    char cursorArray[] = { '_', '\0' };
    for (i = 0; i <= MAX_ROW; i++) {
        for (j = 0; j <= MAX_COL; j++) {
            temporalOutputArray[0] = *(*((this->JDisplayContent) + i) + j);
            al_draw_text(this->JFont, al_map_rgb(0, 0, 0),
                XOFFSET + j * XINCREMENT, YOFFSET + i * YINCREMENT,
                ALLEGRO_ALIGN_CENTER, temporalOutputArray);
        }
    }
    al_draw_text(this->JFont, al_map_rgb(0, 0, 0),
        XOFFSET + (this->JCursorPosition.column) * XINCREMENT, 
        YOFFSET + (this->JCursorPosition.row) * YINCREMENT,
        ALLEGRO_ALIGN_CENTER, temporalOutputArray);
    al_flip_display();
    return;
}

void DispJ::LCDReset() {
    al_set_target_backbuffer(this->JDisplay);
    al_draw_bitmap(this->JBackground,0,0,0);
    al_flip_display();
    return;
}