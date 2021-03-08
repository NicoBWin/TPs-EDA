/***************************************************************************//**
  @file     +Parse.h+
  @brief    +Header de la libreria  +
  @author   +Grupo 10+
 ******************************************************************************/

#ifndef _PARSE_H_
#define _PARSE_H_

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define OKY 0        //La funcion recursiva no encontro errores
#define ERROR1  -1   //Dentro de lo que sería una opcion no hay clave
#define ERROR2  -2   //Termina con una opcion sin valor
#define ERROR3  -3   //Error de la callback
#define SZ    10     //Cantidad maxima de opciones que podemos cargar

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
typedef int (*pCallback) (char *, char*, void *); 

typedef struct {
    char* key;
    char* value;
} OPTION_t ;

typedef struct {
    OPTION_t option[SZ];    //Se puede cargar SZ opciones
    char* parameter;        //Solo se puede cargar un parametro,si cargas mas sobre escribe el ultimo ingresado
} myData_t ;

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/
/**
 * @brief parseCmdLine Recibe el ingreso de terminal y analiza si es una opcion o un parametro
 * @param argc
 * @param argv[]
 * @param p Es un puntero a la función callback
 * @param userData Es un puntero a la estructura donde estara toda la info guardada
 * @return Si no hubo errores devuelve la suma de opciones y parametros
*/
int parseCmdLine(int argc, char *argv[], pCallback p, void *userData);

/*******************************************************************************
 ******************************************************************************/

#endif // _PARSE_H_