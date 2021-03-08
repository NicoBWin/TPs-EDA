/***************************************************************************//**
  @file     +Parse.c+
  @brief    +Archivo principal de la libreria+
  @author   +Grupo 10+
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
int parseCmdLine(int argc, char *argv[], pCallback p, void *userData) {
  int ammount=0;
  int result;
  int flag=0;

  for(int i=1; i<argc; i++){
    
    if(argv[i][0] == '-'){ //Si empieza con - es porque es una opción
      if (i != (argc - 1)) { //Si no es el ultimo string
        if (argv[i][1] == '\0'){ //Si es una opcion sin clave
          return ERROR1;
        }
        else{
          result=p(argv[i]+1, argv[i+1], userData);
          i++;  //No considero la clave
        }
      }
      else {  //Si es el ultimo string y empieza con "-" entonces hay error tipo 2
        return ERROR2;
      }
    }
    else {  //Si no es una opcion entonces es un parametro
      result=p(NULL, argv[i], userData);
      flag++;
    }
    
    if( result == OKY || flag==1){ //Solo cuenta un parametro y todas las opciones ingresadas
      ammount++;
    } 
    else{             //Si encontro error la callback devuelde ese error (ERROR3)
      return result;
    }
    
  }
  return ammount;
}