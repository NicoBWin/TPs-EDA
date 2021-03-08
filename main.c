/***************************************************************************//**
  @file     +main.c+
  @brief    +Archivo principal para probar las funciones y tener la callback+
  @author   +Grupo 10+
 ******************************************************************************/
/*
Compile with: gcc main.c parse.c parse.h -o main -Wall
Test with: ./main -op1 15 -op2 30 divide
           ./main - 15 -op2 30 divide       (ERROR1)
           ./main -op1 15 -op2 30 -divide   (ERROR2)
*/


//FALTA CREAR UN BANCO DEPRUEBAS!
/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include <stdio.h>
#include "parse.h"

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/
/**
 * @brief parseCmdLine
 * @param key Si es una opcion recibe la clave si es un parametro recibe NULL
 * @param value Si es una opcion recibe la valor si es un parametro recibe el mismo
 * @param userData Estructura donde se guardan los datos
 * @return Devuelve 0 si no tiene errores y -1 o -2 dependiendo del error que encuentre
*/
int parseCallback(char *key, char *value, void *userData); 

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
int main (int argc, char *argv[]) {
    myData_t userData;
    int cant=0;
    if ((cant = parseCmdLine(argc,argv, &parseCallback, &userData)) >= 0) {
      printf("Cantidad ingresada total: %d\n",cant);
      printf("(Cant. de parametros + cant. de opciones)\n\n");


    //Pequeña funcion para imprimir los datos ingresados y verificar que funciona todo bien
    for(int y=0;y<cant-1;y++) {
        printf("Key: %s\n",userData.option[y].key);
        printf("value: %s\n",userData.option[y].value);
    }
    printf("Parameter: %s\n", userData.parameter);

    }
    else {
      printf("ERROR%d\n\n",-cant);
    }

    return 0;
}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
int parseCallback(char *key, char *value, void *userData) {
    myData_t *Data = userData;  //Referenciamos un puntero (Data) a userData
    int state;
    static int i=0;
    if (key==NULL){
        Data->parameter=value;
    }
    else{
        Data->option[i].key=key;
        Data->option[i].value=value;
        i++;
    }
    state=OKY;
    return state;
}