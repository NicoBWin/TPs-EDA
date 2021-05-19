/***************************************************************************//**
@file     +Client.h+
@brief    +Header con las funciones para descargar los JSON+
@author   +Grupo 1+
******************************************************************************/

#ifndef _CLIENT_H_
#define _CLIENT_H_

/*******************************************************************************
* INCLUDE HEADER FILES
******************************************************************************/
#include <iostream>
#include <string>
#include <exception>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

/*******************************************************************************
* FUNCTION PROTOTYPES WITH GLOBAL SCOPE
******************************************************************************/
//Función que descarga los json
//Recibe el query y el json por referencia y guarda lo descargado ahí
//Devuelve en su nombre “true” si descargó correctamente o “false" en caso contrario.
bool client(nlohmann::json& j, int counter, std::string user);

/*******************************************************************************
******************************************************************************/

#endif // _CLIENT_H_