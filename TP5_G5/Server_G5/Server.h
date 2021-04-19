/***************************************************************************//**
@file     +Server.h+
@brief    +Header clase SERVER+
@author   +Grupo 5+
******************************************************************************/

#ifndef _SERVER_H_
#define _SERVER_H_

/******************************************************************************
* INCLUDE HEADER FILES
******************************************************************************/
#include <boost\asio.hpp>
#include <string>

#include "Connection.h"
#include "HTTP.h"
/******************************************************************************
* CONST & DEFINES
******************************************************************************/
#define PORT 80			//Puerto para HTTP = 80
#define IP (tcp::v4())	//Local IP 127.0.0.1

/******************************************************************************
* CLASS
******************************************************************************/
class Server {
public:
	Server(boost::asio::io_context& context);	//Constructor
	~Server();									//Destructor

private:

	//Funcion que crea una conexion y espera a que se conecte un cliente
	void awaitConnection();

	//Callback de la conexion establecida que recibe una conexion y un error de boost. La funcion comienza correr el ciclo de la conexion
	void establishedConnection_cb(Connection::pointer currentConnection, const boost::system::error_code& error);
	
	boost::asio::io_context& context_;			//Contexto de la conexion
	boost::asio::ip::tcp::acceptor acceptor_;	//Aceptor de la conexion

};

#endif // _SERVER_H_