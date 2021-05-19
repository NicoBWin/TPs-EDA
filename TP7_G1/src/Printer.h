/***************************************************************************//**
@file     +Printer.h+
@brief    +Header de las funciones ...+
@author   +Grupo 1+
******************************************************************************/

#ifndef _PRINTER_H_
#define _PRINTER_H_

/******************************************************************************
* INCLUDE HEADER FILES
******************************************************************************/
#include <iostream>
#include <string>
#include <fstream>
#include <list>
#include <exception>
#include <nlohmann/json.hpp>

/******************************************************************************
* CONSTANT, ENUMS AND MACRO DEFINITIONS USING #DEFINE
******************************************************************************/
enum months { Jan = 1, Feb, Mar, Apr, May, Jun, Jul, Aug, Sep, Oct, Nov, Dec };

/******************************************************************************
* FUNCTION PROTOTYPES WITH GLOBAL SCOPE
******************************************************************************/
//
std::string getDate(nlohmann::json& j, int tweetNumber);
//
std::string getTweet(nlohmann::json& j, int tweetNumber);

/*******************************************************************************
******************************************************************************/
#endif  // _PRINTER_H_
