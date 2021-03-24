/***************************************************************************//**
  @file     +Parse.cpp+
  @brief    +Archivo principal de la libreria parser+
  @author   +Grupo 7+
******************************************************************************/

/*******************************************************************************
* INCLUDE HEADER FILES
******************************************************************************/
#include <stdlib.h>       //Solo para NULL
#include "parse.h" 

/*******************************************************************************
*******************************************************************************
                       GLOBAL FUNCTION DEFINITIONS
*******************************************************************************
******************************************************************************/
//Recibe el ingreso de terminal y analiza si es una opcion o un parametro y los envía a la callback p
int parseCmdLine(int argc, char* argv[], pCallback p, void* userData) {
    int ammount = 0;
    int result;

    for (int i = 1; i < argc; i++) {

        if (argv[i][0] == '-') { //Si empieza con - es porque es una opción
            if (i != (argc - 1)) { //Si no es el ultimo string
                if (argv[i][1] == '\0') { //Si es una opcion sin clave
                    return ERROR1;
                }
                else {
                    result = p(argv[i] + 1, argv[i + 1], userData); //argv[i]+1 es para no enviar el - inicial de una opción
                    i++;  //Salteo la clave para analizar el siguiente dato ingresado 
                }
            }
            else {  //Si es el ultimo string y empieza con "-" entonces hay error tipo 2
                return ERROR2;
            }
        }
        else {  //Si no es una opcion entonces es un parametro
            result = p(NULL, argv[i], userData);
        }

        if (result == OKY) { //Solo cuenta un parametro y todas las opciones ingresadas
            ammount++;
        }
        else {             //Si encontro error la callback devuelde ese error (ERROR3)
            return result;
        }

    }
    return ammount;
}