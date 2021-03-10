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
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define ARR 6

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
typedef struct {
    char* key;
    char* value;
} OPTION_t ;

typedef struct {
    OPTION_t option[SZ];    //Se puede cargar SZ opciones
    char* parameter;        //Solo se puede cargar un parametro,si cargas mas sobre escribe el ultimo ingresado
} myData_t ;


/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/
/**
 * @brief parseCallback Callback de parseCmdLine
 * @param key Si es una opcion recibe la clave si es un parametro recibe NULL
 * @param value Si es una opcion recibe la valor si es un parametro recibe el mismo
 * @param userData Estructura donde se guardan los datos
 * @return Devuelve 0 si no tiene errores y -3 si hay un error de la callback
*/
int parseCallback(char *key, char *value, void *userData); 


/**
 * @brief f_test Función para saber si parseCallback funciono o no
 * @param cant El resultado de parseCallback
 * @param userData Puntero a la estructura
*/
void f_test (int cant, myData_t *userData);
/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
int main () {
    myData_t userData;
    int cant=0;

    //TEST
    char* ar_test[ARR]={"./main","op1","10","op2","cuatro","divide"};
    cant = parseCmdLine(ARR,ar_test, &parseCallback, &userData);
    f_test(cant,&userData); 


    char* ar_test1[ARR]={"./main","-","10","-op2","5","divide"};
    cant = parseCmdLine(ARR,ar_test1, &parseCallback, &userData);
    f_test(cant,&userData); 


    char* ar_test2[ARR]={"./main","-op1","10","-op2","5","-divide"};
    cant = parseCmdLine(ARR,ar_test2, &parseCallback, &userData);
    f_test(cant,&userData); 


    char* ar_test3[ARR]={"./main","-op1","10","-op2","5","divide"};
    cant = parseCmdLine(ARR,ar_test3, &parseCallback, &userData);
    f_test(cant,&userData); 

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


void f_test (int cant, myData_t *userData) {
    if (cant >= 0) {
    /*  printf("Cantidad ingresada total: %d\n",cant);
      printf("(Cant. de parametros + cant. de opciones)\n");

    //Pequeña funcion para imprimir los datos ingresados y verificar que funciona todo bien
    
    for(int y=0;y<cant-1;y++) {
        printf("Key: %s\n",userData->option[y].key);
        printf("value: %s\n",userData->option[y].value);
    }
    printf("Parameter: %s\n", userData->parameter);*/
    printf("TODO OK!\n");
    printf("Cantidad ingresada total: %d\n",cant);
    printf("(Cant. de parametros + cant. de opciones)\n\n");
    }

    else {
      printf("ERROR%d\n\n",-cant);
    }

    printf("\n");
}