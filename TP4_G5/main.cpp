/***************************************************************************//**
  @file     +main.cpp+
  @brief    +Archivo principal+
  @author   +Grupo 5+
 ******************************************************************************/

/*******************************************************************************
* INCLUDE HEADER FILES
******************************************************************************/
#include  <iostream>
#include <cstdlib>
#include <ctime>

#include "Worm.h"
#include "allLoad.h"
#include "const.h"
#include "allPrint.h"

using namespace std;

/******************************************************************************
*******************************************************************************
*						            MAIN
*******************************************************************************
******************************************************************************/
int main() {
    if (init_allegro() == NO_ERROR) {           // inicio allegro y verifico que no haya errores
        srand(time(NULL));
        Worm w[MAX_WORMS];                      // creo arreglo de worms
        bool do_exit = false;                   
        al_start_timer(timer); // Comienza a correr el timer
        while (!do_exit)
        {
            ALLEGRO_EVENT ev;
            if (al_get_next_event(event_queue, &ev)) //Toma un evento de la cola. 
            {
                if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {   // Presiona cerrar
                    do_exit = true;                             //salgo del while
                }
                else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {    //apreto tecla?
                    switch (ev.keyboard.keycode) {           //que tecla apreto?
                    case ALLEGRO_KEY_A:
                        w[0].setEstado(START_MOVING_LEFT);   // si apreto A se mueve a la izq el worm 0
                        break;
                    case ALLEGRO_KEY_D:  
                        w[0].setEstado(START_MOVING_RIGHT);  // si apreto D se mueve a la der el worm 0
                        break;
                    case ALLEGRO_KEY_W: 
                        w[0].setEstado(START_JUMP);          // si apreto W comieza a saltar el worm 0
                        break;

                    case ALLEGRO_KEY_LEFT:                   // si apreto flecha iziquierda se mueve a la izq el worm 1
                        w[1].setEstado(START_MOVING_LEFT);
                        break;
                    case ALLEGRO_KEY_RIGHT:                 //  si flecha derechase mueve a la der el worm 1
                        w[1].setEstado(START_MOVING_RIGHT);
                        break;
                    case ALLEGRO_KEY_UP:                    // si apreto flecha hacia arriba comieza a saltar el worm 1
                        w[1].setEstado(START_JUMP);
                        break;
                    }
                }
                else if (ev.type == ALLEGRO_EVENT_KEY_UP) { //Solto tecla?
                    switch (ev.keyboard.keycode) {           //que tecla solto?
                    case ALLEGRO_KEY_A:
                    case ALLEGRO_KEY_D:
                    case ALLEGRO_KEY_W:  // si solto tecla A/D/W se pone en estado pasivo al worm 0
                        w[0].setEstado(IDLE);
                        break;

                    case ALLEGRO_KEY_UP:    // si solto tecla  poner en estado pasivo al worm 1
                    case ALLEGRO_KEY_LEFT:
                    case ALLEGRO_KEY_RIGHT: 
                        w[1].setEstado(IDLE);
                        break;
                    }
                }
                else if (ev.type == ALLEGRO_EVENT_TIMER) {  // si paso 1 frame ( 1/50 sec )
                    al_clear_to_color(al_map_rgb(0, 0, 200));   //Pongo la pantalla en azul para contrastar
                    al_draw_bitmap(bkgImage, 0, 0, 0);      // dibujo fondo
                    for (int i = 0; i < MAX_WORMS; i++) {
                       w[i].update(i);                      // actualizo dependiendo de los eventos ocurridos
                    }
                    al_flip_display();                      // muestro display
                }
            }
        }
        destroy_allegro();      //Destruye recursos y finaliza la simulación
        al_stop_timer(timer);       // Frena el timer
    }
    else {                      // error de iinicializacion de allegro
        cout << endl;
        cout << "ALLEGRO ERROR" << endl;
        cout << endl;
    }
    cout << "See you next time..." << endl;
}