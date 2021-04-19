/***************************************************************************//**
@file     +HTTP.h+
@brief    +Header clase HTTP+
@author   +Grupo 5+
******************************************************************************/

#ifndef _HTTP_H_
#define _HTTP_H_

/******************************************************************************
* INCLUDE HEADER FILES
******************************************************************************/
#include <string>
#include <iostream>

#define HTTPENDLINE	("\r\n")		//End-line for HTTP line
#define HTMLENDMSG	("\r\n\r\n")	//End-line for HTTP message
#define VERB ("GET")
#define VERSION ("HTTP/1.1")

using namespace std;
/******************************************************************************
* CLASS
******************************************************************************/
class HTTP {
public:

	/*
	 * dataDirectory: Directorio al que se accederá para buscar los archivos solicitados por clientes.
	 *				Puede ser un path relativo o absoluto.
	 * defaultHTML: Archivo que se utilizará cuando se recibe un request con '/' como dirección
	 */
	HTTP(string dataDirectory, string defaultHTML = "index.html");	//Constructor
	~HTTP();		//Destructor
	
	////Recibe un string con lo recibido y llama a función que valida el mensaje y llama a función que envia el mensaje
	void dataReceived(string& data);

	//Getter del mensaje a enviar
	string getMsg();
	//Getter del path
	string getPath();

private:
	int state;				// Estado del mensaje recibido
	string serverPath;		// Ubicación de los archivos públicos del servidor. En este directorio se buscarán todos los archivos
	string filePath;			// Donde esta el archivo solicitado, relativo a serverPath
	string defaultIndex;	// Nombre del archivo index. Es aquel que se envía al solicitarse "/"

	string msg;			// Mensaje recibido que luego modificaremos y lo enviaremos

	//Valida los datos ingresados y devuelve un estado seg?n corresponda
	void validateData(string& data);
	//Arma el mensaje HTTP segun la validacion que recibe 
	void dataToSend();
};


/******************************************************************************
* CONST & DEFINES & ENUMS
******************************************************************************/
enum httpState { 
	OK,		//No hay problemas en el mensaje y llego una ruta que existe 
	ERROR_PATH, //No hay problemas en el mensaje pero la ruta o el archivo indicado no existe
	ERROR_MSG	//Hay problemas en el mensaje
};


//String de los mensajes de respuesta comunes de HTTP
const std::string OK_S =
"HTTP/1.1 200 OK\r\n";
const std::string CREATED_S =
"HTTP/1.1 201 Created\r\n";
const std::string ACCEPTED_S =
"HTTP/1.1 202 Accepted\r\n";
const std::string NO_CONTENT_S =
"HTTP/1.1 204 No Content\r\n";
const std::string MULTIPLE_CHOICES_S =
"HTTP/1.1 300 Multiple Choices\r\n";
const std::string MOVED_PERMANENTLY_S =
"HTTP/1.1 301 Moved Permanently\r\n";
const std::string MOVED_TERMPORARILY_S =
"HTTP/1.1 302 Moved Temporarily\r\n";
const std::string NOT_MODIFIED_S =
"HTTP/1.1 304 Not Modified\r\n";
const std::string BAD_REQUEST_S =
"HTTP/1.1 400 Bad Request\r\n";
const std::string UNAUTHORIZED_S =
"HTTP/1.1 401 Unauthorized\r\n";
const std::string FORBIDDEN_S =
"HTTP/1.1 403 Forbidden\r\n";
const std::string NOT_FOUND_S =
"HTTP/1.1 404 Not Found\r\n";
const std::string INTERNAL_SERVER_ERROR_S =
"HTTP/1.1 500 Internal Server Error\r\n";
const std::string NOT_IMPLEMENTED_S =
"HTTP/1.1 501 Not Implemented\r\n";
const std::string BAD_GATEWAY_S =
"HTTP/1.1 502 Bad Gateway\r\n";
const std::string SERVICE_UNAVAILABLE_S =
"HTTP/1.1 503 Service Unavailable\r\n";


#endif	// _HTTP_H_