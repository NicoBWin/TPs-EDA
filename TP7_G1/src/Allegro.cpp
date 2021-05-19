/***************************************************************************//**
@file     +Allegro.cpp+
@brief    +Funciones de inicializacion y destrucción de allegro+
@author   +Grupo 1+
******************************************************************************/

/******************************************************************************
* INCLUDE HEADER FILES
******************************************************************************/
#include <iostream> 
#include <string>
#include <vector>

#include "Allegro.h"

using namespace std;
/******************************************************************************
* VARIABLES WITH GLOBAL SCOPE
******************************************************************************/
ALLEGRO_EVENT_QUEUE* event_queue = NULL;
ALLEGRO_TIMER* timer = NULL;
ALLEGRO_DISPLAY* displayAll = NULL;

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
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
        cout << "failed to install keyboard!" << endl;
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
    else if (!al_install_mouse()) {
        cout << "failed to install  mouse!" << endl;
        al_shutdown_font_addon();
        al_shutdown_ttf_addon();
        al_shutdown_image_addon();
        al_uninstall_keyboard();
        al_shutdown_primitives_addon();
        return false;
    }
    else if (!(timer = al_create_timer(0.5))) {
        cout << "failed to create timer!" << endl;
        al_shutdown_font_addon();
        al_shutdown_ttf_addon();
        al_shutdown_image_addon();
        al_uninstall_keyboard();
        al_shutdown_primitives_addon();
        al_uninstall_mouse();
        return false;
    }
    displayAll = al_create_display(W_PIXELS, H_PIXELS);
    if (!displayAll) {
        cout << "failed to create display!" << endl;
        al_shutdown_font_addon();
        al_shutdown_ttf_addon();
        al_shutdown_image_addon();
        al_uninstall_keyboard();
        al_shutdown_primitives_addon();
        al_uninstall_mouse();
        al_destroy_timer(timer);
        return false;
    }
    al_set_new_display_flags(ALLEGRO_RESIZABLE);
    event_queue = al_create_event_queue();
    if (!event_queue) {
        cout << "failed to load event queue!" << endl;
        al_shutdown_font_addon();
        al_shutdown_ttf_addon();
        al_shutdown_image_addon();
        al_uninstall_keyboard();
        al_shutdown_primitives_addon();
        al_uninstall_mouse();
        al_destroy_timer(timer);
        al_destroy_display(displayAll);
        return false;
    }

    al_set_window_title(displayAll, "TP7 - G1");
    al_register_event_source(event_queue, al_get_display_event_source(displayAll));  //Agrego los eventos a la cola
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_mouse_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    return true;
}

//Funcion que destruye todos los recursos de allegro
void destroy_allegro(void) {
    al_shutdown_font_addon();
    al_shutdown_image_addon();
    al_shutdown_primitives_addon();

    al_destroy_timer(timer);
    al_destroy_display(displayAll);
    al_destroy_event_queue(event_queue);

    al_uninstall_keyboard();
    al_uninstall_mouse();
}