/***************************************************************************//**
  @file     +all_tools.cpp+
  @brief    +Funciones para los recursos de allegro+
  @author   +Grupo 4+
 ******************************************************************************/

 /******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include <iostream>
#include <cstdbool>
#include "allLoad.h"

using namespace std;
/******************************************************************************
* CONSTANT AND MACRO DEFINITIONS USING #DEFINE
******************************************************************************/
#define FPS 50.0       //Constante para timer

/*******************************************************************************
* FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
******************************************************************************/
/**
 * @brief Carga la imagenes.
*/
static int loadim_walk();

/**
 * @brief Carga la imagenes del jump.
*/
static int loadim_jump();

/*******************************************************************************
* VARIABLES WITH GLOBAL SCOPE
******************************************************************************/
/*PUNTEROS ALLEGRO*/
ALLEGRO_DISPLAY* display = NULL;

ALLEGRO_TIMER* timer = NULL;
ALLEGRO_EVENT_QUEUE* event_queue = NULL;

ALLEGRO_BITMAP* wj1Image = NULL;            // creo variables de imagenes del salto
ALLEGRO_BITMAP* wj2Image = NULL;
ALLEGRO_BITMAP* wj3Image = NULL;
ALLEGRO_BITMAP* wj4Image = NULL;
ALLEGRO_BITMAP* wj5Image = NULL;
ALLEGRO_BITMAP* wj6Image = NULL;
ALLEGRO_BITMAP* wj7Image = NULL;
ALLEGRO_BITMAP* wj8Image = NULL;
ALLEGRO_BITMAP* wj9Image = NULL;
ALLEGRO_BITMAP* wj10Image = NULL;

ALLEGRO_BITMAP* bkgImage = NULL;            // creo varibles de backround

ALLEGRO_BITMAP* ww1Image = NULL;            // creo variables de imagenes del walk
ALLEGRO_BITMAP* ww2Image = NULL;
ALLEGRO_BITMAP* ww3Image = NULL;
ALLEGRO_BITMAP* ww4Image = NULL;
ALLEGRO_BITMAP* ww5Image = NULL;
ALLEGRO_BITMAP* ww6Image = NULL;
ALLEGRO_BITMAP* ww7Image = NULL;
ALLEGRO_BITMAP* ww8Image = NULL;
ALLEGRO_BITMAP* ww9Image = NULL;
ALLEGRO_BITMAP* ww10Image = NULL;
ALLEGRO_BITMAP* ww11Image = NULL;
ALLEGRO_BITMAP* ww12Image = NULL;
ALLEGRO_BITMAP* ww13Image = NULL;
ALLEGRO_BITMAP* ww14Image = NULL;
ALLEGRO_BITMAP* ww15Image = NULL;

/*******************************************************************************
*******************************************************************************
                       GLOBAL FUNCTION DEFINITIONS
*******************************************************************************
******************************************************************************/
//Funcion que inicializa todo lo que es allegro
int init_allegro() {            // incio allegro
    if (!al_init()) {
        cout << "failed to initialize allegro!!" << endl;
        return -1;
    }
    else if (!al_init_image_addon()) {
        cout << "failed to init image addon!" << endl;
        return -1;
    }
    else if (!(display = al_create_display(W_PIXELS, H_PIXELS))) {
        cout << "failed to create display!" << endl;
        return -1;
    }
    else if (!(timer = al_create_timer(1/FPS))) {
        cout << "failed to create timer!" << endl;
        al_destroy_display(display);
        return -1;
    }
    event_queue = al_create_event_queue();
    if (!event_queue) {
        cout << "failed to load event queue!" << endl;
        al_destroy_display(display);
        al_destroy_timer(timer);
        return -1;
    }
    else if (!al_install_keyboard()) {
        cout << "failed to install keyboard!" << endl;
        al_destroy_display(display);
        al_destroy_timer(timer);
        al_destroy_event_queue(event_queue);
        return -1;
    }
    else if (!(bkgImage = al_load_bitmap(BKG_FILE))) {
        cout << "failed to load background image" << endl;
        al_destroy_display(display);
        al_destroy_timer(timer);
        al_destroy_event_queue(event_queue);
        al_uninstall_keyboard();
        return -1;
    }
    if ((loadim_walk()) == NO_ERROR) {
        al_register_event_source(event_queue, al_get_keyboard_event_source());              //Evento teclado      
        al_register_event_source(event_queue, al_get_display_event_source(display));        //Cruz roja de salir
        al_register_event_source(event_queue, al_get_timer_event_source(timer));
    }
    else {
        cout << "failed to initialize imgs!!" << endl;
        al_destroy_display(display);
        al_destroy_timer(timer);
        al_destroy_event_queue(event_queue);
        al_uninstall_keyboard();
        al_destroy_bitmap(bkgImage);
        return -1;
    }
    return NO_ERROR;
}

//Funcion que destruye todo allegro
void destroy_allegro(void) {
    al_destroy_display(display);

    al_destroy_bitmap(bkgImage);

    al_destroy_bitmap(ww1Image);
    al_destroy_bitmap(ww2Image);
    al_destroy_bitmap(ww3Image);
    al_destroy_bitmap(ww4Image);
    al_destroy_bitmap(ww5Image);
    al_destroy_bitmap(ww6Image);
    al_destroy_bitmap(ww7Image);
    al_destroy_bitmap(ww8Image);
    al_destroy_bitmap(ww9Image);
    al_destroy_bitmap(ww10Image);
    al_destroy_bitmap(ww11Image);
    al_destroy_bitmap(ww12Image);
    al_destroy_bitmap(ww13Image);
    al_destroy_bitmap(ww14Image);
    al_destroy_bitmap(ww15Image);

    al_destroy_bitmap(wj1Image);
    al_destroy_bitmap(wj2Image);
    al_destroy_bitmap(wj3Image);
    al_destroy_bitmap(wj4Image);
    al_destroy_bitmap(wj5Image);
    al_destroy_bitmap(wj6Image);
    al_destroy_bitmap(wj7Image);
    al_destroy_bitmap(wj8Image);
    al_destroy_bitmap(wj9Image);
    al_destroy_bitmap(wj10Image);

    al_destroy_timer(timer);
    al_destroy_event_queue(event_queue);

    al_shutdown_image_addon();

    al_uninstall_keyboard();
}


/**
 * @brief Carga la imagenes.
*/
static int loadim_walk()           // Cargo todas las imagenes de walk y vereifico que no haya error
{
    ww1Image = al_load_bitmap(WW1);
    if (ww1Image) {
        ww2Image = al_load_bitmap(WW2);
        if (ww2Image) {
            ww3Image = al_load_bitmap(WW3);
            if (ww3Image) {
                ww4Image = al_load_bitmap(WW4);
                if (ww4Image) {
                    ww5Image = al_load_bitmap(WW5);
                    if (ww5Image) {
                        ww6Image = al_load_bitmap(WW6);
                        if (ww6Image) {
                            ww7Image = al_load_bitmap(WW7);
                            if (ww7Image) {
                                ww8Image = al_load_bitmap(WW8);
                                if (ww8Image) {
                                    ww9Image = al_load_bitmap(WW9);
                                    if (ww9Image) {
                                        ww10Image = al_load_bitmap(WW10);
                                        if (ww10Image) {
                                            ww11Image = al_load_bitmap(WW11);
                                            if (ww11Image) {
                                                ww12Image = al_load_bitmap(WW12);
                                                if (ww12Image) {
                                                    ww13Image = al_load_bitmap(WW13);
                                                    if (ww13Image) {
                                                        ww14Image = al_load_bitmap(WW14);
                                                        if (ww14Image) {
                                                            ww15Image = al_load_bitmap(WW15);
                                                            if (ww15Image) {
                                                                if (loadim_jump() == NO_ERROR) {
                                                                    return NO_ERROR;
                                                                }
                                                                else
                                                                    cout << "ERROR: failed to load jump imgs!" << endl;
                                                                al_destroy_bitmap(ww15Image);
                                                            } else
                                                                cout << "ERROR: failed to load ww15Image!" << endl;
                                                            al_destroy_bitmap(ww14Image);
                                                        } else
                                                            cout << "ERROR: failed to load ww14Image!" << endl;
                                                        al_destroy_bitmap(ww13Image);
                                                    } else
                                                        cout << "ERROR: failed to load ww13Image!" << endl;
                                                    al_destroy_bitmap(ww12Image);
                                                } else
                                                    cout << "ERROR: failed to load ww12Image!" << endl;
                                                al_destroy_bitmap(ww11Image);
                                            } else
                                                cout << "ERROR: failed to load ww11Image!" << endl;
                                            al_destroy_bitmap(ww10Image);
                                        } else
                                            cout << "ERROR: failed to load ww10Image!" << endl;
                                        al_destroy_bitmap(ww9Image);
                                    } else
                                        cout << "ERROR: failed to load ww9Image!" << endl;
                                    al_destroy_bitmap(ww8Image);
                                } else
                                    cout << "ERROR: failed to load ww8Image!" << endl;
                                al_destroy_bitmap(ww7Image);
                            } else
                                cout << "ERROR: failed to load ww7Image!" << endl;
                            al_destroy_bitmap(ww6Image);
                        } else
                            cout << "ERROR: failed to load ww6Image!" << endl;
                        al_destroy_bitmap(ww5Image);
                    } else
                        cout << "ERROR: failed to load ww5Image!" << endl;
                    al_destroy_bitmap(ww4Image);
                } else
                    cout << "ERROR: failed to load ww4Image!" << endl;
                al_destroy_bitmap(ww3Image);
            } else
                cout << "ERROR: failed to load ww3Image!" << endl;
            al_destroy_bitmap(ww2Image);
        } else
            cout << "ERROR: failed to load ww2Image!" << endl;
        al_destroy_bitmap(ww1Image);
    } else
        cout << "ERROR: failed to load ww1Image!" << endl;
    return ERROR;
}

/**
 * @brief Carga la imagenes del jump.
*/
static int loadim_jump() {        // Cargo todas las imagenes de jump y vereifico que no haya error
    wj1Image = al_load_bitmap(WJ1);
    if (wj1Image) {
        wj2Image = al_load_bitmap(WJ2);
        if (wj2Image) {
            wj3Image = al_load_bitmap(WJ3);
            if (wj3Image) {
                wj4Image = al_load_bitmap(WJ4);
                if (wj4Image) {
                    wj5Image = al_load_bitmap(WJ5);
                    if (wj5Image) {
                        wj6Image = al_load_bitmap(WJ6);
                        if (wj6Image) {
                            wj7Image = al_load_bitmap(WJ7);
                            if (wj7Image) {
                                wj8Image = al_load_bitmap(WJ8);
                                if (wj8Image) {
                                    wj9Image = al_load_bitmap(WJ9);
                                    if (wj9Image) {
                                        wj10Image = al_load_bitmap(WJ10);
                                        if (ww10Image)
                                        {
                                            return NO_ERROR;
                                        }
                                        else
                                            cout << "ERROR: failed to load wj10imag!" << endl;
                                        al_destroy_bitmap(wj9Image);
                                    } else
                                        cout << "ERROR: failed to load wj9image!" << endl;
                                    al_destroy_bitmap(wj8Image);
                                } else
                                    cout << "ERROR: failed to load wj8image png!" << endl;
                                al_destroy_bitmap(wj7Image);
                            } else
                                cout << "ERROR: failed to load wj7imag" << endl;
                            al_destroy_bitmap(wj6Image);
                        } else
                            cout << "ERROR: failed to load wj6image " << endl;
                        al_destroy_bitmap(wj5Image);
                    } else
                        cout << "ERROR: failed to load wj5image " << endl;
                    al_destroy_bitmap(wj4Image);
                } else
                     cout << "ERROR: failed to load wj4image" << endl;
                al_destroy_bitmap(wj3Image);
            } else
                cout << "ERROR: failed to load wj3image" << endl;
            al_destroy_bitmap(wj2Image);
        } else
            cout << "ERROR: failed to load wj2image " << endl;
        al_destroy_bitmap(wj1Image);
    } else
        cout<< "ERROR: failed to load wj1image"<< endl;
    return ERROR;
 } 