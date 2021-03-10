/***************************************************************************//**
  @file     +main.c+
  @brief    +Archivo principal para probar las funciones y tener la callback+
  @author   +Grupo 10+

  @Compile  with: gcc main.c parse.c parse.h -o main -Wall
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include <stdio.h>
#include <string.h>

#include "parse.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define SZ      99   //Cantidad máxima de opciones que podemos cargar
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
} myData_t ;    //Esturctura para poder guardar todos los datos ingresados

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
*/
void f_test (int cant);
/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
int main (void) {
    myData_t userData;
    int cant=0;

    //TESTS

    //Test0 - Todo OK + Impresion de datos ingresados
    char* ar_test[ARR]={"./main","-op1","10","-op2","cuatro","ejemplo"};
    cant = parseCmdLine(ARR,ar_test, &parseCallback, &userData);

    for(int y=0;y<cant-1;y++) {     //Por única vez imprimimos para analizar como guardo los datos
        printf("Key: %s\n",userData.option[y].key);
        printf("value: %s\n",userData.option[y].value);
    }
    printf("Parameter: %s\n", userData.parameter);
    f_test(cant); 

    //Test1 - ERROR tipo 1
    char* ar_test1[ARR]={"./main","-","10","-op2","5","ejemplo"};   
    cant = parseCmdLine(ARR,ar_test1, &parseCallback, &userData);
    f_test(cant); 

    //Test2 - ERROR tipo 2
    char* ar_test2[ARR]={"./main","-op1","10","-op2","5","-divide"};    
    cant = parseCmdLine(ARR,ar_test2, &parseCallback, &userData);
    f_test(cant);

    //Test3 - ERROR tipo 3
    char* ar_test3[ARR]={"./main","-operador","25","NOejemplo","-operador","10"};   
    cant = parseCmdLine(ARR,ar_test3, &parseCallback, &userData);
    f_test(cant);

    //Test4 - 5 Parametros ingresados
    char* ar_test4[ARR]={"./main","ejemplo","ejemplo","ejemplo","ejemplo","ejemplo"};   
    cant = parseCmdLine(ARR,ar_test4, &parseCallback, &userData);
    f_test(cant); 

    //Test5 - 4 Datos ingresados
    char* ar_test5[ARR]={"./main","-key","value","ejemplo","ejemplo","ejemplo"};   
    cant = parseCmdLine(ARR,ar_test5, &parseCallback, &userData);
    f_test(cant); 

    //Test6 - Igual que test1 pero con los datos cambiados de lugar
    char* ar_test6[ARR]={"./main","ejemplo","-op","25","-op","10"};     
    cant = parseCmdLine(ARR,ar_test6, &parseCallback, &userData);
    f_test(cant);

    //Test7 - No se ingreso nada solo se ejecuto el programa: Devuelve 0 el parse
    char* ar_test7[1]={"./main"};     
    cant = parseCmdLine(1,ar_test7, &parseCallback, &userData);
    f_test(cant);

    return 0;
}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
//Callback de parseCmdLine
int parseCallback(char *key, char *value, void *userData) {
    myData_t *Data = userData;  //Referenciamos un puntero (Data) a userData
    int state;
    static int i=0;
    if (key==NULL){
        if(strcmp(value,"ejemplo") == 0){   //Como ejemplo analizo el parametro ingresado  y si no coincide devuelve error la callback
            Data->parameter=value;
            state=OKY;
        }
        else
            state=ERROR3;
    }
    else{
        Data->option[i].key=key;
        Data->option[i].value=value;
        i++;
        state=OKY;
    }
    return state;
}


//Pequeña funcion para imprimir la cant. de datos ingresados y verificar si parse no delvolvió error
void f_test (int cant) {
    if (cant >= 0) {
    printf("TODO OK!\n");
    printf("Cantidad ingresada total: %d\n",cant);
    printf("(Cant. de parametros + cant. de opciones)\n\n");
    }

    else {
      printf("ERROR%d\n\n",-cant);
    }

    printf("\n");
}