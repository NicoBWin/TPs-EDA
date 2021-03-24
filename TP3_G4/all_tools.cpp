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
#include "all_tools.h"

#include "Blob.h"

using namespace std;
/******************************************************************************
* CONSTANT AND MACRO DEFINITIONS USING #DEFINE
******************************************************************************/
#define FPS 0.01       //Constante para timer

/*******************************************************************************
* VARIABLES WITH GLOBAL SCOPE
******************************************************************************/
/*PUNTEROS ALLEGRO*/
ALLEGRO_DISPLAY* display = NULL;
ALLEGRO_DISPLAY* display2 = NULL;

ALLEGRO_TIMER* timer = NULL;
ALLEGRO_EVENT_QUEUE* event_queue = NULL;
ALLEGRO_FONT* font1 = NULL;

ALLEGRO_BITMAP* babyBlobBM = NULL;
ALLEGRO_BITMAP* grownBlobBM = NULL;
ALLEGRO_BITMAP* goodOldBlobBM = NULL;
ALLEGRO_BITMAP* backgroundBM = NULL;
ALLEGRO_BITMAP* foodBM = NULL;

/*******************************************************************************
*******************************************************************************
                       GLOBAL FUNCTION DEFINITIONS
*******************************************************************************
******************************************************************************/
//Funcion que inicializa todo lo que es allegro
int init_allegro(int height, int width) {
    al_init_font_addon(); // initialize the font addon
    al_init_ttf_addon(); // initialize the ttf (True Type Font) addon
    if (!al_init()) {
        cout << "failed to initialize allegro!!" << endl;
        return -1;
    }
    else if (!al_init_image_addon()) {
        cout << "failed to init image addon!" << endl;
        return -1;
    }
    else if (!(display = al_create_display(width, height))) {
        cout << "failed to create display!" << endl;
        return -1;
    }
    else if (!(timer = al_create_timer(FPS))) {
        cout << "failed to create timer!" << endl;
        al_destroy_display(display);
        return -1;
    }
    else if (!(font1 = al_load_ttf_font(FONT1_FILE, 20, 0))) {
        cout << "failed to load font!" << endl;
        al_destroy_display(display);
        al_destroy_timer(timer);
        return -1;
    }    
    else if (!(babyBlobBM = al_load_bitmap(BBYBLOB_FILE))){
        cout << "failed to load images1!" << endl;
        al_destroy_display(display);
        al_destroy_timer(timer);
        al_destroy_font(font1);
        return -1;
    }
    else if (!(grownBlobBM = al_load_bitmap(GRWBLOB_FILE))){
        cout << "failed to load images2!" << endl;
        al_destroy_display(display);
        al_destroy_timer(timer);
        al_destroy_font(font1);
        al_destroy_bitmap(babyBlobBM);
        return -1;
    }
    else if (!(goodOldBlobBM = al_load_bitmap(OLDBLOB_FILE))){
        cout << "failed to load images3!" << endl;
        al_destroy_display(display);
        al_destroy_timer(timer);
        al_destroy_font(font1);
        al_destroy_bitmap(babyBlobBM);
        al_destroy_bitmap(grownBlobBM);
        return -1;
    }
    else if (!(backgroundBM = al_load_bitmap(BACKG_FILE))){
        cout << "failed to load images4!" << endl;
        al_destroy_display(display);
        al_destroy_timer(timer);
        al_destroy_font(font1);
        al_destroy_bitmap(babyBlobBM);
        al_destroy_bitmap(grownBlobBM);
        al_destroy_bitmap(goodOldBlobBM);
        return -1;
    }
    else if (!(foodBM = al_load_bitmap(FOOD_FILE))){
        cout << "failed to load images5!" << endl;
        al_destroy_display(display);
        al_destroy_timer(timer);
        al_destroy_font(font1);
        al_destroy_bitmap(babyBlobBM);
        al_destroy_bitmap(grownBlobBM);
        al_destroy_bitmap(goodOldBlobBM);
        al_destroy_bitmap(backgroundBM);
        return -1;
    }

    event_queue = al_create_event_queue();
    if (!event_queue) {
        cout << "failed to load event queue!" << endl;
        al_destroy_display(display);
        al_destroy_timer(timer);
        al_destroy_font(font1);
        al_destroy_bitmap(babyBlobBM);
        al_destroy_bitmap(grownBlobBM);
        al_destroy_bitmap(goodOldBlobBM);
        al_destroy_bitmap(backgroundBM);
        al_destroy_bitmap(foodBM);
        return -1;
    }
    if (!al_install_keyboard()) {
        cout << "failed to install keyboard!" << endl;
        al_destroy_display(display);
        al_destroy_timer(timer);
        al_destroy_font(font1);
        al_destroy_bitmap(babyBlobBM);
        al_destroy_bitmap(grownBlobBM);
        al_destroy_bitmap(goodOldBlobBM);
        al_destroy_bitmap(backgroundBM);
        al_destroy_bitmap(foodBM);
        al_destroy_event_queue(event_queue);
        return -1;
    }

    else if (!(display2 = al_create_display(width, height))) {
        cout << "failed to create display2!" << endl;
        al_destroy_display(display);
        al_destroy_timer(timer);
        al_destroy_font(font1);
        al_destroy_bitmap(babyBlobBM);
        al_destroy_bitmap(grownBlobBM);
        al_destroy_bitmap(goodOldBlobBM);
        al_destroy_bitmap(backgroundBM);
        al_destroy_bitmap(foodBM);
        al_destroy_event_queue(event_queue);
        al_uninstall_keyboard();
        return -1;
    }

    al_draw_bitmap(backgroundBM, 0, 0, 0);
    al_flip_display();
    al_register_event_source(event_queue, al_get_keyboard_event_source());              //Evento teclado      
    al_register_event_source(event_queue, al_get_display_event_source(display));        //Cruz roja de salir
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    return 0;
}

//Funcion que destruye todo allegro
void destroy_allegro(void) {

    al_destroy_display(display);
    al_destroy_display(display2);

    al_destroy_bitmap(foodBM);
    al_destroy_bitmap(grownBlobBM);
    al_destroy_bitmap(goodOldBlobBM);
    al_destroy_bitmap(babyBlobBM);
    al_destroy_bitmap(backgroundBM);

    al_destroy_timer(timer);
    al_destroy_event_queue(event_queue);

    al_shutdown_primitives_addon();

    al_destroy_font(font1);
    al_shutdown_font_addon();
    al_shutdown_ttf_addon();
    al_shutdown_image_addon();

    al_uninstall_keyboard();
}


//Dibuja la interface
void draw_interface(Simu& s) {
    al_set_target_backbuffer(display2);
    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_draw_textf(font1, al_map_rgb(255, 0, 0), 10, 10, 0, "Comida:    %d", s.foodCount);
    al_draw_textf(font1, al_map_rgb(255, 255, 255), 10, 40, 0, "KEY UP: aumentar comida    KEY DOWN: disminuir comida");
    al_draw_textf(font1, al_map_rgb(255, 0, 0), 10, 70, 0, "Velocidad porcentual:    %d", s.relVel);
    al_draw_textf(font1, al_map_rgb(255, 255, 255), 10, 100, 0, "KEY LEFT: aumentar velocidad   KEY RIGHT: disminuir velocidad");
    al_draw_textf(font1, al_map_rgb(255, 0, 0), 10, 130, 0, "Radio de deteccion:    %d", s.smellRadius);
    al_draw_textf(font1, al_map_rgb(255, 255, 255), 10, 160, 0, "R: aumentar radio de deteccion       E: disminuir radio de deteccion");
    al_draw_textf(font1, al_map_rgb(255, 0, 0), 10, 190, 0, "RandomJiggleLimit:    %d", s.randomJiggleLimit);
    al_draw_textf(font1, al_map_rgb(255, 255, 255), 10, 220, 0, "V: aumentar el randomJiggleLimit       C: disminuir el randomJiggleLimit");
    al_draw_textf(font1, al_map_rgb(255, 0, 0), 10, 250, 0, "Prob. de muerte bblob    %0.2f", s.deathRateBB);
    al_draw_textf(font1, al_map_rgb(255, 255, 255), 10, 280, 0, "S: aumentar la prob. de muerte bblob       A: disminuir la prob. de muerte bblob");
    al_draw_textf(font1, al_map_rgb(255, 0, 0), 10, 310, 0, "Prob. de muerte gwblob    %0.2f", s.deathRateGW);
    al_draw_textf(font1, al_map_rgb(255, 255, 255), 10, 340, 0, "F: aumentar la prob. de muerte gwblob        D: disminuir la prob. de muerte gwblob");
    al_draw_textf(font1, al_map_rgb(255, 0, 0), 10, 370, 0, "Prob. de muerte gblob    %0.2f", s.deathRateGO);
    al_draw_textf(font1, al_map_rgb(255, 255, 255), 10, 400, 0, "H: aumentar la prob. de muerte gblob         G: disminuir la prob. de muerte gblob");
    al_draw_textf(font1, al_map_rgb(220, 220, 220), 280, H_PIXELS - 30, 0, "SPACE / ENTER: pausar la simulacion");

    al_flip_display();
}


//Dibuja toda la simulacón
void drawWildlife(Simu& sim) {
    //Print all foods and all blobs in the screen 1
    al_set_target_backbuffer(display);
    al_draw_bitmap(backgroundBM, 0, 0, 0);
    for (int i = 0; i < MAXBLOBS; i++) {
        if ( sim.blob[i].isAlive == ALIVE ) {
            switch (sim.blob[i].age) {
                case babyBlob:
                    al_draw_rotated_bitmap(babyBlobBM, al_get_bitmap_width(babyBlobBM) / 2, al_get_bitmap_height(babyBlobBM) / 2, sim.blob[i].p.x, sim.blob[i].p.y, sim.blob[i].angle - (ALLEGRO_PI / 2), 0);
                    break;
                case grownBlob:
                    al_draw_rotated_bitmap(grownBlobBM, al_get_bitmap_width(grownBlobBM) / 2, al_get_bitmap_height(grownBlobBM) / 2, sim.blob[i].p.x, sim.blob[i].p.y, sim.blob[i].angle - (ALLEGRO_PI / 2), 0);
                    break;
                case goodOldBlob:
                    al_draw_rotated_bitmap(goodOldBlobBM, al_get_bitmap_width(goodOldBlobBM) / 2, al_get_bitmap_height(goodOldBlobBM) / 2, sim.blob[i].p.x, sim.blob[i].p.y, sim.blob[i].angle - (ALLEGRO_PI / 2), 0);
                    break;

            }
        }
    }
    for (int j = 0; j < sim.foodCount; j++) {
        al_draw_bitmap(foodBM, sim.food[j].p.x, sim.food[j].p.y, 0);
    }
    al_flip_display();
}