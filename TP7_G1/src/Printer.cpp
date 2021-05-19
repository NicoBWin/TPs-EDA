/***************************************************************************//**
@file     +Printer.cpp+
@brief    +Funciones ...+
@author   +Grupo 1+
******************************************************************************/

/******************************************************************************
* INCLUDE HEADER FILES
******************************************************************************/
#include "Printer.h"

//Vamos a usar la librería NLOHMANN JSON 
using json = nlohmann::json;

using namespace std;
/******************************************************************************
* VARIABLES WITH GLOBAL SCOPE
******************************************************************************/

/******************************************************************************
* FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
******************************************************************************/
void formatFecha(string& date);

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
string getDate(json& j, int tweetNumber) {
    string date;
    if (j.is_object()) {
        date = "ERROR CODE: ";
        date.append(to_string(j["errors"][0]["code"]));
    }
    else {
        date = j[tweetNumber]["created_at"];
        formatFecha(date);
    }
    return date;
}

string getTweet(json& j, int tweetNumber) {
    string msg;
    if (j.is_object()) {
        msg = j["errors"][0]["message"];
    }
    else {
        msg = j[tweetNumber]["user"]["name"];
        msg.append(" - ");
        msg.append(j[tweetNumber]["text"]);
        int extended = msg.find("https");
        msg = msg.substr(0, extended);
    }
    return msg;
}

/******************************************************************************
*******************************************************************************
            LOCAL FUNCTION DEFINITIONS
*******************************************************************************
******************************************************************************/
void formatFecha(string& date) { //BUSCA EL AÑO? 
    string aux = date;
    date = "00/00/00 - 00:00";
    string months[] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };

    //Parseamos el MES
    int i = 0;
    for (; aux.find(months[i]) == string::npos; i++);
    i++;
    
    if (i < 10) {
        date.replace(4, 1, to_string(i));
    }
    else if (i > 10) {
        date.replace(3, 2, to_string(i));
    }

    date.replace(0, 2, aux.substr(8, 2));     //Copiamos el dia
    date.replace(6, 2, aux.substr(28, 2));    //Copiamos el año
 
    //Parseamos la hora al formato 24hs.
    int timeArg = stoi(aux.substr(11, 2));
    timeArg = ((timeArg-3) >= 0)? timeArg-3 : (24-(3-timeArg));
    string hour;
    if (timeArg < 10) {
        hour = "0";
        hour.append(to_string(timeArg));
    }
    else {
        hour = to_string(timeArg);
    }
    date.replace(11, 2, hour);   //Copiamos la hora
    date.replace(14, 2, aux.substr(14, 2));   //Copiamos el minuto
}