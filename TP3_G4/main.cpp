/***************************************************************************//**
  @file     +main.cpp+
  @brief    +Archivo principal+
  @author   +Grupo 4+
 ******************************************************************************/
//Common input un terminal: -modo 1 -blobs 200 -food 20 -Vel 100
//Remember to debug in Visual with x86
/*******************************************************************************
* INCLUDE HEADER FILES
******************************************************************************/
#include <iostream>
#include <stdbool.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>

#include "parse_lib/parse.h"
#include "all_tools.h"
#include "logic.h"
#include "Simu.h"


using namespace std;
/*******************************************************************************
* CONSTANT AND MACRO DEFINITIONS USING #DEFINE
******************************************************************************/
#define MODO1	1
#define MODO2	2

/*******************************************************************************
* ENUMERATIONS AND STRUCTURES AND TYPEDEFS
******************************************************************************/
struct myData {
    int modo;
    int initBlobs;
    int food;
    int maxVel;
};  //Esturctura para poder guardar todos los datos ingresados

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/
 //Callback de parseCmdLine, analiza lo ingresado y guarda en userData lo válido. Resive 2 punteros a char (clave y valor) y un puntero generico (userData). Devuelve un int.
int parseCallback(char* key, char* value, void* userData);		

//Funcion que convierte el string en un valor numerico. Recibe un puntero a char (lo que quiere convertir). Devuelve un int (lo convertido).
int strgtoint(char* stg);										


/******************************************************************************
*******************************************************************************
*						            MAIN
*******************************************************************************
******************************************************************************/
int main(int argc, char* argv[]) {
    myData userData;		//creo estructura userData
    int cant = parseCmdLine(argc, argv, &parseCallback, &userData);		//guardo lo que me da el paseCmdLine en cant
    if (cant < 0) {						//si es negativo hay error
        printf("PARSER ERROR%d\n\n", -cant);
    }
    else {								//si es positivo sigo
        if (init_allegro(H_PIXELS, W_PIXELS) == 0) {
            srand(time(NULL));

            Simu s(userData.modo, userData.initBlobs, userData.food, userData.maxVel, W_PIXELS, H_PIXELS, al_get_bitmap_width(babyBlobBM), al_get_bitmap_height(babyBlobBM), al_get_bitmap_width(grownBlobBM), al_get_bitmap_height(grownBlobBM), al_get_bitmap_width(goodOldBlobBM), al_get_bitmap_height(goodOldBlobBM));   //iniciar todos los tipitos
            if (s.food != NULL) { //Analizo si hay error en la asignacion de memoria de food
                drawWildlife(s);    //Dibujar una pantalla de instrucciones y el mundo con los tipitos
                bool do_exit = false;
                bool redraw = true;
                bool blink = true;
                al_start_timer(timer); // Comienza a correr el timer
                while (!do_exit)
                {
                    ALLEGRO_EVENT ev;
                    if (al_get_next_event(event_queue, &ev)) //Toma un evento de la cola, VER RETURN EN DOCUMENT.
                    {
                        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {   // Presiona cerrar
                            do_exit = true;                             //salgo del while
                        }
                        else if (ev.type == ALLEGRO_EVENT_KEY_DOWN)     //apreto tecla?
                        {
                            switch (ev.keyboard.keycode)            //que tecla apreto?
                            {
                            case ALLEGRO_KEY_UP: //aumentar cominda
                                s.foodCount++;
                                if (ammFood(s) == NULL) {
                                    cout << "Error while ralloc memory for food" << endl;
                                    do_exit = true;
                                }
                                
                                break;
                            case ALLEGRO_KEY_DOWN: //disminuir comida
                                s.foodCount--;
                                if (ammFood(s) == NULL) {
                                    cout << "Error while ralloc memory for food" << endl;
                                    do_exit = true;
                                }
                                break;

                            case ALLEGRO_KEY_LEFT:  //Aumenta la velocidad porcentual en 5%
                                s.relVel -= 5;
                                break;
                            case ALLEGRO_KEY_RIGHT: //Disminuye la velocidad porcentual en 5%
                                s.relVel += 5; 
                                break;

                            case ALLEGRO_KEY_R:  //aumentar radio de detección
                                s.smellRadius += 10;
                                break;
                            case ALLEGRO_KEY_E:  //Disminuir radio de deteccion
                                s.smellRadius -= 10;
                                break;

                            case ALLEGRO_KEY_S:  //Aumentar la probabilidad de muerte del BabyBlob
                                if (s.deathRateBB < 1.0) {
                                    s.deathRateBB += 0.05;
                                }
                                break;
                            case ALLEGRO_KEY_A:  //Disminuir la probabilidad de muerte
                                 if (s.deathRateBB >= 0.05) {
                                    s.deathRateBB -= 0.05;
                                 }
                                break;

                            case ALLEGRO_KEY_F:  //Aumentar la probabilidad de muerte del GrownBlob
                                if (s.deathRateGW < 1.0) {
                                    s.deathRateGW += 0.05;
                                }
                                break;
                            case ALLEGRO_KEY_D:  //Disminuir la probabilidad de muerte
                                if (s.deathRateGW >= 0.05) {
                                    s.deathRateGW -= 0.05;
                                }
                                 break;

                            case ALLEGRO_KEY_H:  //Aumentar la probabilidad de muerte del GoodOldBlob
                                 if (s.deathRateGO < 1.0) {
                                    s.deathRateGO += 0.05;
                                 }
                                  break;
                            case ALLEGRO_KEY_G:  //Disminuir la probabilidad de muerte
                                if (s.deathRateGO >= 0.05) {
                                    s.deathRateGO -= 0.05;
                                }
                                 break;

                            case ALLEGRO_KEY_V: //Aumenta el angulo máximo para el calculo de dirección en 10
                                s.randomJiggleLimit += 10;
                                break;
                            case ALLEGRO_KEY_C: //Disminuye el angulo máximo para el calculo de dirección en 10
                                s.randomJiggleLimit -= 10;
                                break;

                            case ALLEGRO_KEY_SPACE: //Se puede pausar la simulacion apretando space o enter
                            case ALLEGRO_KEY_ENTER:
                                if (blink == false) { // Blink apagado? lo enciendo
                                    blink = true;
                                    al_resume_timer(timer); // Comienza a correr el timer
                                }
                                else {
                                    blink = false;
                                    al_stop_timer(timer);       // Frena el timer
                                }
                                redraw = true;
                                break;
                            }
                        }
                        else if (ev.type == ALLEGRO_EVENT_TIMER) {  // Timer de blink
                            redraw = true;
                        }
                    }
                    if (redraw && al_is_event_queue_empty(event_queue)) {
                        //Funciones para simular y dibujar todo el mundo
                        redraw = false;
                        simular(s);
                        drawWildlife(s);    //Dibujar una pantalla de instrucciones y el mundo con los tipitos
                        draw_interface(s);
                        if (s.blobCount == 0) {       //Si no hay ningun blob vivo entonces salimos del while poniendo do_exit true
                            do_exit = true;
                        }
                    }
                }
                al_stop_timer(timer);       // Frena el timer
                s.deleteSimu();
            }
        destroy_allegro();      //Destruye recursos y finaliza la simulación
        }
    }
    cout << "See you next time..." << endl;
}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
 //Callback de parseCmdLine, analiza lo ingresado y guarda en userData lo válido. Recibe 2 punteros a char (clave y valor) y un puntero generico (userData). Devuelve un int.
int parseCallback(char* key, char* value, void* userData) {
    myData* Data = (myData*)userData;  //Referenciamos un puntero (Data) a userData 
    int temp;

    if (key == NULL) {			//si clave es NULL entonces ERROR 3
        cout<<"We don't expect parameters"<<endl;
        return ERROR3;
    }
    else if (strcmp(key, "modo") == 0 || strcmp(key, "Modo") == 0 || strcmp(key, "m") == 0 || strcmp(key, "M") == 0) {	//si clave es modo
        if (value[0] == '1' && value[1] == '\0') {	//si el valor es 1
            Data->modo = MODO1;						//entonces MODO 1, guardo
        }
        else if (value[0] == '2' && value[1] == '\0') {	//si el valor es 2
            Data->modo = MODO2;							//entonces MODO 2, guardo
        }
        else {											//sino es ni 1 ni 2 entonces hay ERROR 3
            cout<<"We only have MODE 1 or MODE 2"<<endl;
            return ERROR3;
        }
    }
    else if (strcmp(key, "blobs") == 0 || strcmp(key, "Blobs") == 0 || strcmp(key, "b") == 0 || strcmp(key, "B") == 0) {
        temp = strgtoint(value);	//convierto a valor numerico
        if (temp != ERROR1 && temp <= MAXBLOBS && temp != 0) {	//si no hay error y es valido entonces lo guardo
            Data->initBlobs = temp;
        }
        else {
            cout << "The maximum number of Blobs allowed is " << MAXBLOBS << endl; //se supero la cant max permitida de Blobs
            return ERROR3;
        }
    }
    else if (strcmp(key, "food") == 0 || strcmp(key, "Food") == 0 || strcmp(key, "f") == 0 || strcmp(key, "F") == 0){
        temp = strgtoint(value);
        if (temp != ERROR1 && temp <= MAXFOOD && temp != 0)
        {
            Data->food = temp;
        }
        else {
            cout<<"The maximum number of food allowed is " << MAXFOOD << endl;
            return ERROR3;
        }
    }
    else if (strcmp(key, "Vel") == 0 || strcmp(key, "vel") == 0 || strcmp(key, "v") == 0 || strcmp(key, "V") == 0){
        temp = strgtoint(value);
        if (temp != ERROR1 && temp <= MAXVEL && temp != 0){
            Data->maxVel = temp;
        }
        else {
            cout<<"The maximum speed allowed is "<< MAXVEL << endl;
            return ERROR3;
        }

    }
    return OKY;					//si no hubo errores devuelvo OKY que es 1
}


//funcion que convierte el string en un valor numerico. recibe un puntero a char (lo que quiere convertir). devuelve un int (lo convertido).
int strgtoint(char* stg) {
    int temp = 0;
    int value = 0;
    for (int i = 0; stg[i] != '\0'; i++) {			//for hasta que sea el terminador
        if (stg[i] >= '0' && stg[i] <= '9') {		//si esta entre el 0 y el 9
            value = temp * 10 + stg[i] - '0';		//guardamelo y restame ascci
        }
        else {										//si no es numero entonces error
            return ERROR1;
        }
        temp = value;								//lo guardo en temp
    }
    return value;									//si salio bien devuelve el numero
}