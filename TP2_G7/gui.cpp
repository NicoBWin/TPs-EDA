/***************************************************************************//**
  @file     +gui.cpp+
  @brief    +Archivo principal para usar los recursos de allegro+
  @author   +Grupo 7+
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>

#include "gui.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define BLINK_TIME 0.1

#define S_PIXELES 100
#define CHART_PIXELS 9

#define CFLOOR_FILE  "clean_floor.jpg"
#define DFLOOR_FILE  "dirty_floor.jpg"
#define ROBOT_FILE  "robot.png"
#define FONT_FILE   "font.ttf"

/*******************************************************************************
 * VARIABLES WITH GLOBAL SCOPE
 ******************************************************************************/
/*PUNTEROS ALLEGRO*/
ALLEGRO_DISPLAY *display=NULL;                       
ALLEGRO_BITMAP *clean_floor=NULL;
ALLEGRO_BITMAP *dirty_floor=NULL;
ALLEGRO_BITMAP *cleaner_robot=NULL;
ALLEGRO_TIMER *timer = NULL;
ALLEGRO_EVENT_QUEUE *event_queue = NULL;
ALLEGRO_FONT *font = NULL;

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/
void bienvenida_allegro(int width, int height);		//Funcion que le da la bienvenida al usuario por allegro

void end_allegro(SIM_t* s, int r);			        //Funcion que le da la despedida al usuario diciendole la informacion del programa por allegro

void drawSim(SIM_t* s, int r);				        //Funcion que dibuja y muestra con allegro la simulacion del piso y los robots

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
 //Funcion que dibuja y muestra el grafico del modo 2 con allegro
void drawChart(double tickcounter[],int r, int h, int w) {
    int i;
    bool do_exit = false;
    int screen = CHART_PIXELS * S_PIXELES - (S_PIXELES / 2);
    float jump1 = screen / (r + 1);
    float jump2 = screen / (1.2 * tickcounter[0]);
    init_allegro(CHART_PIXELS, CHART_PIXELS);
    for (i = 0; i < r; i++) {
        al_draw_line(jump1 * (i + 1) + (S_PIXELES / 2), screen, jump1 * (i + 1) + (S_PIXELES / 2), screen - jump2 * tickcounter[i], al_map_rgb(255, 0, 0), 2);
        al_draw_filled_circle(jump1 * (i + 1) + (S_PIXELES / 2), screen - jump2 * tickcounter[i], 5, al_map_rgb(0, 255, 0));
    }
    
    al_draw_text(font, al_map_rgb(135, 26, 186), (S_PIXELES * CHART_PIXELS) / 2, (S_PIXELES * h) / 2 - 100, ALLEGRO_ALIGN_CENTER, "SIMULATION");
    al_draw_textf(font, al_map_rgb(174, 0, 255), (S_PIXELES * CHART_PIXELS) / 2, (S_PIXELES * h) / 2 - 50, ALLEGRO_ALIGN_CENTER, "IN A %dx%d FLOOR", h, w);
    al_draw_textf(font, al_map_rgb(0, 0, 220), (S_PIXELES * CHART_PIXELS) / 2, (S_PIXELES * h) / 2 + 50 , ALLEGRO_ALIGN_CENTER, "MAX. ROBOTS %d", r);
    al_draw_textf(font, al_map_rgb(0, 0, 255), (S_PIXELES * CHART_PIXELS) / 2, (S_PIXELES * h) / 2 + 100, ALLEGRO_ALIGN_CENTER, "TICKS: %.1f MAX. %.1f MIN.  AVG.", tickcounter[0], tickcounter[r - 1]);
                            //imprime la informacion del programa en una punta del cuadro y lo muestra
    al_flip_display();

    printf("To exit press the red cross on chart screen\n");

    while (!do_exit)
    {
        ALLEGRO_EVENT ev;
        if (al_get_next_event(event_queue, &ev)) //Toma un evento de la cola, VER RETURN EN DOCUMENT.
        {
            if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {   // Presiona cerrar
                do_exit = true;                             //si apreta la cruz roja entonces sale del while cambiando el valor de do_exit
            }
        }
    }
    destroy_allegro();          //destruimos allegro antes de salir              
}


//Funcion que se encarga de la logica con allegro y timer
void drawBack(SIM_t* s, int r) {
    bienvenida_allegro(s->flo->width, s->flo->height);      //doy bienvenida al usuario por allegro
    drawSim(s, r);          //dibuja simulacion
    bool do_exit = false;
    bool redraw = true;
    bool blink = true;
    bool sim;
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
                case ALLEGRO_KEY_SPACE: //Se puede pausar la simulacion apretanto space o enter
                case ALLEGRO_KEY_ENTER:
                    if (blink == false)  // Blink apagado? lo enciendo
                    {
                        blink = true;
                        al_resume_timer(timer); // Comienza a correr el timer
                    }
                    else
                    {
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
        if (redraw && al_is_event_queue_empty(event_queue))
        {
            redraw = false;
            sim = simulate(s, r);
            drawSim(s, r);         //dibuja simulacion
            if(sim == CLEAN) {       //si esta todo limpio entonces salimos del while poniendo do_exit true
                do_exit = true;
                end_allegro(s,r);           //imprime pantalla de cierre
            }
        }
    }
    al_stop_timer(timer);       // Frena el timer
}


//Funcion que inicializa todo lo que es arreglo
int init_allegro(int height, int width) {

    /*INICIALIZACION DISPLAY Y FOTOS*/
    if (!al_init()){
        fprintf(stderr, "Unable to start allegro\n");                                   //realizo la inicializacion de allegro
        return -1;
    } 
    else if (!al_init_primitives_addon()) {
        fprintf(stderr, "Unable to start primitives addon\n");                                   //realizo la inicializacion de allegro
        al_destroy_display(display);
        return -1;
    }
    else if (!al_init_image_addon()) {
        fprintf(stderr, "Unable to start image addon \n");                              //si hubo un error en la inicializacion imprimo el srderr
        al_shutdown_primitives_addon();
        al_uninstall_system();
        return -1;
    }
    else if (!(display = al_create_display(S_PIXELES*height ,S_PIXELES*width))) {            //se controlan si hubo problemas en las
        fprintf(stderr, "Unable to create display\n");                                  //distintas inicializaciones 
        al_uninstall_system();
        al_shutdown_image_addon();                              
        return -1;
    } 
    else if (!(clean_floor = al_load_bitmap(CFLOOR_FILE))) {           //se carga imagen de clean floor
        fprintf(stderr, "Unable to load clean_floor\n");
        al_uninstall_system();
        al_shutdown_image_addon();
        al_destroy_display(display);
        return -1;
    }
    else if (!(dirty_floor = al_load_bitmap(DFLOOR_FILE))) {           //se carga imagen de dirty floor
        fprintf(stderr, "Unable to load dirty_floor\n");
        al_uninstall_system();
        al_shutdown_image_addon();
        al_destroy_display(display);
        return -1;
    }
    else if (!(cleaner_robot = al_load_bitmap(ROBOT_FILE))) {           //se carga imagen de cleaner_robot
        fprintf(stderr, "Unable to load cleaner_robot\n");
        al_uninstall_system();
        al_shutdown_image_addon();
        al_destroy_display(display);
        return -1;
    }

    /*INICIALIZACION FUENTE*/
    al_init_font_addon(); // initialize the font addon
    al_init_ttf_addon(); // initialize the ttf (True Type Font) addon

    font = al_load_ttf_font(FONT_FILE, 40, 0); 

    if (!font) {
        fprintf(stderr, "Could not load 'font.ttf'.\n");             //se carga la fuente font
        return -1;
    } 

    /*INICIALIZACION TIMER*/
    timer = al_create_timer(BLINK_TIME);
    if (!timer) {
        fprintf(stderr, "failed to create timer!\n");
        return -1;
    }

    /*INICIALIZACION EVENTOS Y TECLADO*/
    event_queue = al_create_event_queue();                                                             
    if (!event_queue) {                                                                                
        fprintf(stderr, "failed to create event_queue!\n");                                            
        destroy_allegro();                                                                             
        return -1;                                                                                     
    }     
    if (!al_install_keyboard()) {                                                                      
        fprintf(stderr, "failed to initialize the keyboard!\n");                                       
        destroy_allegro();                                                                             
        return -1;                                                                                     
    }                                                                                                  
    al_register_event_source(event_queue, al_get_keyboard_event_source());              //Evento teclado      
    al_register_event_source(event_queue, al_get_display_event_source(display));        //Cruz roja de salir
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    return 0;
}


//Funcion que destruye todo allegro, no recibe nada y no devuelve nada
void destroy_allegro(void) {                    
    al_destroy_display(display);                //se libera la memoria dinamica , destruyendo los elemntos usados

    al_destroy_bitmap(clean_floor);
    al_destroy_bitmap(dirty_floor);
    al_destroy_bitmap(cleaner_robot);
    al_uninstall_audio();   

    al_destroy_timer(timer);
    al_destroy_event_queue(event_queue); 

    al_shutdown_primitives_addon();

    al_destroy_font(font);
    al_shutdown_font_addon();
    al_shutdown_ttf_addon();
    al_shutdown_image_addon();

    al_uninstall_keyboard();
}


/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/


 //Funcion que dibuja y muestra con allegro la simulacion del piso y los robots
void drawSim(SIM_t* s, int r) {
    al_rest(0.5);
    float largo_elemento = 0;            //variables que usamos para movernos por elementos, ya que la imagen se divide en cuadrados de S_PIXELES x S_PIXELES pixeles
    float alto_elemento = 0;             //cada imagen es de S_PIXELES x S_PIXELES pixeles
       
    for (int i = 0; i < (s->flo->height); i++) {    //Recorro el todo el piso moviendome en for
        for (int j = 0; j < (s->flo->width); j++) {
            if ((s->flo->tiles[(i*(s->flo->width)+j) * sizeof(bool)]) == CLEAN) {       //si el la baldosa esta limpia, dibujame la baldosa limpia
                al_draw_bitmap(clean_floor, largo_elemento, alto_elemento, 0);          //en las coordenadas (largo_elemento, alto_elemento) de la pantalla
            }
            else {                                                                      //si la baldosa esta sucia, dibujame la baldosa sucia
                al_draw_bitmap(dirty_floor, largo_elemento, alto_elemento, 0);          //en las coordenadas (largo_elemento, alto_elemento) de la pantalla
            }
            largo_elemento += S_PIXELES;                                                //a largo_elemento S_PIXELES que es el largo en pixeles de la foto de cada baldosa, asi me muevo por columna en pantalla
        }
        largo_elemento = 0;                         //pongo largo_elemento en 0 para volver a arrancar desde la columna 0
        alto_elemento += S_PIXELES;                 //sumo a alto_elemento S_PIXELES que es el alto en pixeles de la foto de cada baldosa, asi paso a la siguiente fila en pantalla
    }

    for (int k = 0; k < r ; k++) {          //con este for recorro la posicion de los robots
        float x = (int)s->rob[k].x; 
        float y = (int)s->rob[k].y;                 //guardo en estas variables donde esta el robots
        x = x * S_PIXELES + S_PIXELES / 2;          //en esta guardo donde estaria centrado
        y = y * S_PIXELES + S_PIXELES / 2;
        al_draw_rotated_bitmap(cleaner_robot, al_get_bitmap_height(cleaner_robot) / 2, al_get_bitmap_width(cleaner_robot) / 2, x, y, s->rob[k].angle - (ALLEGRO_PI/2), 0);
    }   //dibujo el robot centrado en la bandosa y rotado segun el angulo de direccion en el que apunta

    al_flip_display();    //lo imprimo todo
}


//Funcion que le da la bienvenida al usuario por allegro
void bienvenida_allegro(int width, int height){
    al_clear_to_color(al_map_rgb(255, 255, 255));   //pantalla en blanco
    al_draw_text(font, al_map_rgb(0, 0, 255), (S_PIXELES * width) / 2, (S_PIXELES * height) / 2 - 50, ALLEGRO_ALIGN_CENTER, "WELCOME TO THE TP2");
    al_draw_text(font, al_map_rgb(0, 0, 255), (S_PIXELES * width) / 2, (S_PIXELES * width) / 2 + 50, ALLEGRO_ALIGN_CENTER, "BY GROUP 7");   //mensaje centrado en pantalla
    al_flip_display();      //imprimo
    al_rest(2.0);           //espero
}

//Funcion que le da la despedida al usuario diciendole cuantos ticks fueron necesarios por allegro
void end_allegro(SIM_t *s, int r) {
    int width = s->flo->width;
    int height = s->flo->height;
    al_clear_to_color(al_map_rgb(255, 255, 255));       //pantalla en blanco
    al_draw_text(font, al_map_rgb(0, 0, 255), (S_PIXELES * width) / 2, (S_PIXELES * height) / 2 - 100, ALLEGRO_ALIGN_CENTER, "THE FLOOR IS CLEAN");     //mensaje centrado en pantalla
    al_draw_textf(font, al_map_rgb(0, 0, 255), (S_PIXELES * width) / 2, (S_PIXELES * height) / 2 - 50, ALLEGRO_ALIGN_CENTER, "IT TAKES %u TICKS", s->tickCount);
    al_draw_textf(font, al_map_rgb(0, 0, 255), (S_PIXELES * width) / 2, (S_PIXELES * height) / 2 + 50, ALLEGRO_ALIGN_CENTER, "WITH %u ROBOTS", r);
    al_draw_textf(font, al_map_rgb(0, 0, 255), (S_PIXELES * width) / 2, (S_PIXELES * height) / 2 + 100, ALLEGRO_ALIGN_CENTER, "IN A %dx%d FLOOR", s->flo->width,s->flo->height);
    al_flip_display();      //lo imprimo
    al_rest(5.0);           //espero
}

