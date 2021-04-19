/***************************************************************************//**
@file     +HTTP.cpp+
@brief    +Formato de los mensajes HTTP+
@author   +Grupo 5+
******************************************************************************/


/******************************************************************************
* INCLUDE HEADER FILES
******************************************************************************/
#include <fstream>

#include "HTTP.h"
#include "mime_types.h"

#pragma warning(disable : 4996)

/******************************************************************************
* MEMBER FUNTIONS
******************************************************************************/
//Constructor
HTTP::HTTP(string dataDirectory, string defaultHTML) {
	if (dataDirectory.length() == 0) {
		cout << "Empty path for server files." << endl;
		return;
	}
	this->serverPath = dataDirectory;
	this->defaultIndex = defaultHTML;
	state = OK;
}

//Destructor
HTTP::~HTTP()
{
}

//Getter del mensaje a enviar
string HTTP::getMsg() {
	return msg;
}

//Getter del path
string HTTP::getPath() {
	return filePath;
}

//Recibe un string con lo recibido y llama a función que valida el mensaje y llama a función que envia el mensaje
void HTTP::dataReceived(string& data) {
	validateData(data);	//Validamos el mensaje que nos llego
	dataToSend();					//Creamos un mensaje html en funcion de la data validada
}

//Valida los datos ingresados y devuelve un estado seg?n corresponda
void HTTP::validateData(string& data) {

	string varData = data;
	size_t pos = 0;
	ifstream file;

	if (data.compare(0, 3, "GET") != 0) {
		state = ERROR_MSG;
		return;
	}
	else
	{

		pos = varData.find(" ") + 2;	//Buscamos el path
		varData.erase(0, pos);	//Borramos el get y el espacio de data

		//Guardamos el filePath
		string possibleFileName = varData.substr(0, varData.find(" "));	

		filePath.assign(serverPath);

		if (possibleFileName.length() == 0 || possibleFileName == "/") {
			filePath.append("/" + defaultIndex);
		}
		else {
			filePath.append("/" + possibleFileName);
		}

		file.open(filePath); //Tratamos de abrir el path solicitado
		if (file.fail()) {			//Si no se encontro el archivo o fallo la apertura
			file.close();
			state = ERROR_PATH;
			return;
		}
		file.close();

		varData.erase(0, varData.find(" ") + 1); //Borramos el path y el espacio en blanco

		if (varData.compare(0, 8, VERSION) != 0) {
			state = ERROR_MSG;
			return;
		}
		else {

			varData.erase(0, std::string(VERSION).size());	//Borramos hasta el terminador HTTP

			if (varData.compare(0, 2, HTTPENDLINE) != 0) {
				state = ERROR_MSG;
				return;
			}
			else {
				varData.erase(0, std::string(HTTPENDLINE).size());	//Borramos el terminador de linea

				if (varData.compare(0, 15, "Host: 127.0.0.1") != 0) {
					state = ERROR_MSG;
					return;
				}
				else {
					pos = varData.find(HTTPENDLINE);
					varData.erase(0, pos);	//Borramos el host y el espacio

					if (varData.compare(0, 2, HTTPENDLINE) != 0) {
						state = ERROR_MSG;
						return;
					}
					else {
						varData.erase(0, std::string(HTTPENDLINE).size());	//Borramos el terminador de linea

						pos = varData.find(HTTPENDLINE);
						if (pos == string::npos) {		//Si no encontro el terminador
							state = ERROR_MSG;
							return;
						}
						else {
							state = OK;
						}
					}
				}
			}
		}
	}

	return;
}

//Arma el mensaje HTTP segun la validacion que recibe 
void HTTP::dataToSend() {
	ifstream file;

	time_t now = time(0);							
	tm* nowTm = gmtime(&now);
	char timeNow[256];
	strftime(timeNow, 256, "%a, %d %b %Y %T GMT", nowTm);	//Ej: Tue, 04 Sep 2018 18:21:49 GMT

	time_t exp = now + 30;
	tm* expTm = gmtime(&exp);
	char timeExp[256];
	strftime(timeExp, 256, "%a, %d %b %Y %T GMT", expTm);	//Ej: Tue, 04 Sep 2018 18:21:49 GMT

	// Largo del nombre del archivo siendo enviado.
	size_t file_len = 0;

	switch (state) {
	case OK:

		msg.clear();
		file.open(filePath, ios::binary | ios::in);

		if (file.is_open()) {
			// Como el archivo se abrió correctamente, creamos el header en _msg_
			// correspondiente y agregamos el contenido del archivo al final del _msg_.

			msg.assign((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>())); //Copia byte a byte le archivo al string msg
			file_len = msg.length();

			file.close(); //Cerramos el stream del file

			std::string extension = filePath;		//Analizamos el Content Type para enviar el correcto segun el tipo de archivo
			extension.erase(0, filePath.find("."));
			std::string contentType = http::server::mime_types::extension_to_type(extension);

			msg = OK_S +
				"Date: " + timeNow + HTTPENDLINE +
				"Location: 127.0.0.1" + filePath.substr(serverPath.length()) + HTTPENDLINE +
				"Cache-Control: public, max-age=30" + HTTPENDLINE +
				"Expires: " + timeExp + HTTPENDLINE +
				"Content-Length: " + to_string(file_len) + HTTPENDLINE + // Largo del nombre del archivo
				"Content-Type:" + contentType + HTTPENDLINE +
				HTTPENDLINE + msg; 
		}
		else {
			msg = INTERNAL_SERVER_ERROR_S +
				"Date: " + timeNow + HTTPENDLINE +
				"Cache-Control: public, max-age=30" + HTTPENDLINE +
				"Expires: " + timeExp + HTTPENDLINE +
				"Content-Length: 0" + HTTPENDLINE +
				HTTPENDLINE;
		}
		break;

	case ERROR_PATH:
		msg = NOT_FOUND_S +
			"Date: " + timeNow + HTTPENDLINE +
			"Cache-Control: public, max-age=30" + HTTPENDLINE +
			"Expires: " + timeExp + HTTPENDLINE +
			"Content-Length: 0" + HTTPENDLINE +
			HTTPENDLINE;
		break;

	case ERROR_MSG:
		msg = BAD_REQUEST_S +
			"Date: " + timeNow + HTTPENDLINE +
			"Cache-Control: public, max-age=30" + HTTPENDLINE +
			"Expires: " + timeExp + HTTPENDLINE +
			"Content-Length: 0" + HTTPENDLINE +
			HTTPENDLINE;
		break;

	default:
		break;
	}
}