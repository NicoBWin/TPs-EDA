/***************************************************************************//**
  @file     +main.c+
  @brief    +Archivo principal+
  @author   +Grupo 7: Nicolas Bustelo, Mariano Agustin Dolhare y Valentino Venier Anache+
 ******************************************************************************/
//Common input un terminal: -R 10 -M 2 -H 5 -W 5
//Remember to debug in Visual with x86
 /*******************************************************************************
  * INCLUDE HEADER FILES
  ******************************************************************************/

#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "gui.h"
#include "parse.h"
#include "logic.h"

/*******************************************************************************
* CONSTANT AND MACRO DEFINITIONS USING #DEFINE
******************************************************************************/


#define MODO1	1
#define MODO2	2

//Constantes máximas para la simulación
#define MAX_H	70
#define MAX_W	100
#define MAX_R	100

#define MAX_SIM	1000
#define MAX_ROB	500


/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
typedef struct {
	int modo;
	int robots;
	int heigh;
	int width;
} myData_t;    //Esturctura para poder guardar todos los datos ingresados


/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/
int parseCallback(char* key, char* value, void* userData);		//Callback de parseCmdLine, analiza lo ingresado y guarda en userData lo válido. Resive 2 punteros a char (clave y valor) y un puntero generico (userData). Devuelve un int.


int strgtoint(char* stg);										//Funcion que convierte el string en un valor numerico. Recibe un puntero a char (lo que quiere convertir). Devuelve un int (lo convertido).


bool conditionRobots(double tickCounter[], int r);				//Funcion que Cómpara si la anterior simulacion se pudo realizar con menos pasos. Recibe un arreglo de ticks counters y un entero (que robot). Devuelve un bool.

/******************************************************************************
*******************************************************************************
				  GLOBAL FUNCTION DEFINITIONS
*******************************************************************************
******************************************************************************/
int main(int argc, char* argv[]) {
	myData_t userData;		//creo estructura userData
	srand(time(NULL));
	int cant = parseCmdLine(argc, argv, &parseCallback, &userData);		//guardo lo que me da el paseCmdLine en cant
	if (cant < 0) {						//si es negativo hay error
		printf("ERROR%d\n\n", -cant);
	}
	else {								//si es positivo sigo
		if (userData.modo == MODO1) {	//si ingreso en MODO 1
			printf("Mode 1\n");
			SIM_t* s = createSim(userData.width, userData.heigh, userData.robots);		//creo simulacion
			init_allegro(userData.width, userData.heigh);		//inicializo allegro
			if (s != NULL) {						//si no hubo error en crear simulacion (NULL) 
				drawBack(s, userData.robots);		//le doy poder a drawBack para que siga y se encargue con allegro de las funciones logicas
			}
			else {									//hubo error al crear simulacion (NULL) entonces termina el programa
				printf("Memory allocation error!\n");
				return 0;
			}
			freeAllmem(s, s->flo, s->rob);		//Libero data
			destroy_allegro();					//destruyo allegro
		}
		else if (userData.modo == MODO2) {		//si ingreso en MODO 2
			printf("Mode 2\nProcessing, please wait...\n");
			double tickCounter[MAX_SIM] = { 0.0 };			//Inicializamos y creamos un array con todos 0
			static int r;									// Es r-1 porque le pasamos el resultado de la simulación anterior
			for (r = 0; conditionRobots(tickCounter, r - 1) == false && r < MAX_ROB; r++) {		//un for hasta que recorra todos los robots o la diferencia sea de 0.1
				for (int i = 0; i < MAX_SIM; i++) {			//for hasta la siulacion maxima (1000)
					SIM_t* s = createSim(userData.width, userData.heigh, r + 1);		//creo simulacion
					if (s != NULL) {								//si no hubo error en crear simulacion (NULL)
						while(simulate(s, r + 1) == DIRTY) {		//sigue simulando hasta que el piso quede todo limpio
							continue;
						}
						tickCounter[r] += s->tickCount;				//sumamos tick en el respectivo robot
						freeAllmem(s, s->flo, s->rob);				//libero data
					}
					else {											//hubo error al crear simulacion (NULL) entonces termina el programa
						printf("Memory allocation error!\n");
						return 0;
					}
				}
				tickCounter[r] /= MAX_SIM;							//cantidad de ticks dividio max de simulaciones, para el promedio
			}
			/*for (int i = 0; i < r; i++) {	//Pequeña funcion para ver los resultados de la simulación
				printf("Ticks en %d robots: %f\n\n", i + 1, tickCounter[i]);
			}*/
			drawChart(tickCounter, r, userData.width, userData.heigh);		//llamo a drawChart que se encarga del modo 2 con allegro
		}
		//printf("Modo: %d || R: %d ||H: %d || W: %d\n\n", userData.modo, userData.robots, userData.heigh, userData.width);
	}
	printf("See you next time..\n");
}


/*******************************************************************************
 *******************************************************************************
						LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
 //Callback de parseCmdLine, analiza lo ingresado y guarda en userData lo válido. Resive 2 punteros a char (clave y valor) y un puntero generico (userData). Devuelve un int.
int parseCallback(char* key, char* value, void* userData) {
	myData_t* Data = (myData_t*) userData;  //Referenciamos un puntero (Data) a userData 
	int temp;

	if (key == NULL) {			//si clave es NULL entonces ERROR 3
		printf("Whe don't expect parameters\n");
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
			printf("Whe only have MODE 1 or MODE 2\n");
			return ERROR3;
		}
	}
	else if (strcmp(key, "width") == 0 || strcmp(key, "Width") == 0 || strcmp(key, "w") == 0 || strcmp(key, "W") == 0) {	//si clave es width
		temp = strgtoint(value);	//convierto a valor numerico
		if (temp != ERROR1 && temp <= MAX_W && temp != 0) {	//si no hay error y es valido entonces lo guardo
			Data->width = temp;
		}
		else {												//si hay, ERROR 3
			printf("Width error...");
			return ERROR3;
		}
	}
	else if (strcmp(key, "heigh") == 0 || strcmp(key, "Heigh") == 0 || strcmp(key, "h") == 0 || strcmp(key, "H") == 0) {	//si clave es heigh
		temp = strgtoint(value);		//convierto a valor numerico
		if (temp != ERROR1 && temp <= MAX_H && temp != 0) {	//si no hay error y es valido entonces lo guardo
			Data->heigh = temp;
		}
		else {												//si hay, ERROR 3
			printf("Heigh error...");
			return ERROR3;
		}
	}
	else if (strcmp(key, "robots") == 0 || strcmp(key, "Robots") == 0 || strcmp(key, "r") == 0 || strcmp(key, "R") == 0) {	//si clave es robots
		temp = strgtoint(value);		//convierto a valor numerico
		if (temp != ERROR1 && temp <= MAX_R && temp != 0) {	//si no hay error y es valido entonces lo guardo
			Data->robots = temp;
		}
		else {												//si hay, ERROR 3
			printf("Robots amount error...");
			return ERROR3;
		}
	}
	return OKY;					//si no hubo errores devuelvo OKY que es 1
}


//Funcion que convierte el string en un valor numerico. Recibe un puntero a char (lo que quiere convertir). Devuelve un int (lo convertido).
int strgtoint(char* stg) {
	int temp = 0;
	int value = 0;
	for (int i = 0; stg[i] != '\0'; i++) {			//for hasta que sea el terminador
		if (stg[i] >= '0' && stg[i] <= '9') {		//si esta entre el 0 y el 9
			value = temp * 10 + stg[i] - '0';		//guardamelo y restame ASCCI
		}
		else {										//si no es numero entonces error
			return ERROR1;
		}
		temp = value;								//lo guardo en temp
	}
	return value;									//si salio bien devuelve el numero
}


//Funcion que C¡compara si la anterior simulacion se pudo realizar con menos pasos. Recibe un arreglo de ticks counters y un entero (que robot). Devuelve un bool.
bool conditionRobots(double tickCounter[], int r) {
	if (r > 1) {
		double diff = tickCounter[r - 1] - tickCounter[r];		//diferencia entre ticks de robots
		if (diff < 0.1) {		//si la diferencia fue del 0.1 entonces devuelvo true
			return true;
		}
		else {					//sino sigo en falso
			return false;
		}
	}
	else {						//si r no es mayor a 1 entonces sigo en false
		return false;
	}
}