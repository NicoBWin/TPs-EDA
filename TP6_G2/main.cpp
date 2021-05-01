/***************************************************************************//**
@file     +main.cpp+
@brief    +Archivo principal+
@author   +Grupo 2+
******************************************************************************/

/******************************************************************************
* INCLUDE HEADER FILES
******************************************************************************/
#include <iostream> 
#include <string>

//DISPLAY CLASSES
#include "basicLCD.h"
#include "Class_NICOLAS.B/DispN.h"
#include "Class_JUAN.C/DispJ.h"
#include "Class_LUCIANO.C/DispL.h"

//ALLEGRO LIBS
#include <allegro5/allegro.h>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>

using namespace std;

/******************************************************************************
* VARIABLES WITH GLOBAL SCOPE
******************************************************************************/
ALLEGRO_EVENT_QUEUE* event_queue = NULL;

/******************************************************************************
* FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
******************************************************************************/
//Inicializa las funciones básicas de allegro
bool init_allegro();
//Funcion que destruye todo allegro
void destroy_allegro(void);

/******************************************************************************
*******************************************************************************
*						            MAIN
*******************************************************************************
******************************************************************************/
int main(void) {
    if (init_allegro()) {           // inicio allegro y verifico que no haya errores
        lcdError errors;
        basicLCD* LCD;

        DispN Nico_LCD;
        LCD = &Nico_LCD;
        if (!LCD->lcdInitOk()) {
            errors = LCD->lcdGetError(); //Leer el error de LCD
            cout << "Error: " << errors.getErrorName() << endl;
            cout << "Description: " << errors.getErrorDescription() << endl;
            cout << "Code: " << errors.getErrorCode() << endl;
            return 0;
        }
        DispJ Juan_LCD;
        LCD = &Juan_LCD;
        if (!LCD->lcdInitOk()) {
            errors = LCD->lcdGetError(); //Leer el error de LCD
            cout << "Error: " << errors.getErrorName() << endl;
            cout << "Description: " << errors.getErrorDescription() << endl;
            cout << "Code: " << errors.getErrorCode() << endl;
            return 0;
        }
        DispL Luciano_LCD;
        LCD = &Luciano_LCD;
        if (!LCD->lcdInitOk()) {
            errors = LCD->lcdGetError(); //Leer el error de LCD
            cout << "Error: " << errors.getErrorName() << endl;
            cout << "Description: " << errors.getErrorDescription() << endl;
            cout << "Code: " << errors.getErrorCode() << endl;
            return 0;
        }

        const unsigned char* test = (const unsigned char*)"Test message";
        const unsigned char* test2 = (const unsigned char*)"Long message to see what happen";
        bool do_exit = false;
        while (!do_exit)
        {
            ALLEGRO_EVENT ev;
            if (al_get_next_event(event_queue, &ev)) //Toma un evento de la cola. 
            {
                if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {    //apreto tecla?
                    switch (ev.keyboard.keycode) {           //que tecla apreto?

                    ////////////////// Asignación del display segun alumnos ////////////////////////
                    case ALLEGRO_KEY_1:
                    case ALLEGRO_KEY_N:
                        LCD = &Nico_LCD;
                        break;
                    case ALLEGRO_KEY_2:
                    case ALLEGRO_KEY_J:
                        LCD = &Juan_LCD;
                        break;
                    case ALLEGRO_KEY_3:
                    case ALLEGRO_KEY_L:
                        LCD = &Luciano_LCD;
                        break;

                    //////////////////////// Prueba de operators //////////////////////////////
                    case ALLEGRO_KEY_A: //Prueba del operador asignacion con chars
                        *LCD << 'H' << 'o' << 'l' << 'a';
                        break;
                    case ALLEGRO_KEY_S: //Prueba del operador asignacion con *chars
                        *LCD << test;
                        break;
                    case ALLEGRO_KEY_D: //Prueba del operador asignacion con *chars pero con un mensaje largo
                        *LCD << test2;
                        break;

                    //////////////////////// Movimiento del cursor  //////////////////////////
                    case ALLEGRO_KEY_LEFT:
                        if (!LCD->lcdMoveCursorLeft()) {
                            cout << "You got to the LEFT border" << endl;
                        }
                        break;
                    case ALLEGRO_KEY_RIGHT:
                        if (!LCD->lcdMoveCursorRight()) {
                            cout << "You got to the RIGHT border" << endl;
                        }
                        break;
                    case ALLEGRO_KEY_UP:
                        if (!LCD->lcdMoveCursorUp()) {
                            cout << "You got to the UP border" << endl;
                        }
                        break;
                    case ALLEGRO_KEY_DOWN:
                        if (!LCD->lcdMoveCursorDown()) {
                            cout << "You got to the DOWN border" << endl;
                        }
                        break;

                    //////////////////// Borrado de pantalla (completo o EOL) //////////////////////////
                    case ALLEGRO_KEY_C:     //Clear all screen
                        if (!LCD->lcdClear()) {
                            cout << "Error in lcd clear funtion" << endl;
                        }
                        break;
                    case ALLEGRO_KEY_X:     //Clear to EOL
                        if (!LCD->lcdClearToEOL()) {
                            cout << "Error in lcd clearEOL funtion" << endl;
                        }
                        break;

                    //////////////// Teclas para settear o saber la position dle cursor ////////////////////
                    case ALLEGRO_KEY_P:     //Get current position
                        cursorPosition temp = LCD->lcdGetCursorPosition();
                        cout << "Position: " << '(' << temp.row << ';' << temp.column << ')' << endl;
                        break;
                    case ALLEGRO_KEY_O:     //Set position
                        cursorPosition temp2;
                        cout << "Enter row position: 1" << endl;
                        temp.row = 1;
                        cout << "Enter column position: 5" << endl;
                        temp.column = 5;
                        if (!LCD->lcdSetCursorPosition(temp)) {
                            cout << "Invalid position" << endl;
                            cout << "Remember row[0-1] col[0-15]" << endl;
                            cout << "Try again, press O first" << endl;
                        }
                        break;

                    //////////////////////////// Salir del programa ////////////////////////////
                    case ALLEGRO_KEY_ESCAPE:    //Exits the program
                        do_exit = true;
                        break;
                    }
                }
            }
        }
    }
    else {                      // error de iinicializacion de allegro
        cout << endl;
        cout << "ALLEGRO ERROR" << endl;
        cout << endl;
    }
    destroy_allegro();      //Destruye recursos y finaliza la simulación
    cout << "See you next time..." << endl;
}


/*******************************************************************************
*******************************************************************************
			LOCAL FUNCTION DEFINITIONS
*******************************************************************************
******************************************************************************/
//Inicialización de los recursos basicos de allegro
bool init_allegro() {
    if (!al_init()) {
        cout << "failed to initialize allegro!!" << endl;
        return false;
    }
    else if (!al_init_font_addon()) { // initialize the font addon
        cout << "failed to initialize font addon!!" << endl;
        return false;
    }
    else if (!al_init_ttf_addon()) { // Initialize the ttf (True Type Font) addon
        cout << "failed to initialize ttf addon!!" << endl;
        al_shutdown_font_addon();
        return false;
    }
    else if (!al_init_image_addon()) {
        cout << "failed to init image addon!" << endl;
        al_shutdown_font_addon();
        al_shutdown_ttf_addon();
        return false;
    }
    else if (!al_install_keyboard()) {
        cout << "failed to init image addon!" << endl;
        al_shutdown_font_addon();
        al_shutdown_ttf_addon();
        al_shutdown_image_addon();
        return false;
    }
    else if (!al_init_primitives_addon()) {
        cout << "failed to init primitives addon!" << endl;
        al_shutdown_font_addon();
        al_shutdown_ttf_addon();
        al_shutdown_image_addon();
        al_uninstall_keyboard();
        return false;
    }
    event_queue = al_create_event_queue();
    if (!event_queue) {
        cout << "failed to load event queue!" << endl;
            al_shutdown_font_addon();
            al_shutdown_ttf_addon();
            al_shutdown_image_addon();
            return false;
        }
    al_register_event_source(event_queue, al_get_keyboard_event_source());              //Evento teclado
    return true;
}

//Funcion que destruye todos los recursos de allegro
void destroy_allegro(void) {
    al_shutdown_primitives_addon();
    al_shutdown_font_addon();
    al_shutdown_ttf_addon();
    al_shutdown_image_addon();
   
    al_destroy_event_queue(event_queue);

    al_uninstall_keyboard();
} 