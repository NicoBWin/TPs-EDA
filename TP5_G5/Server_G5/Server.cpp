/***************************************************************************//**
@file     +Server.cpp+
@brief    +Funciones de la clase Server+
@author   +Grupo 5+
******************************************************************************/


/******************************************************************************
* INCLUDE HEADER FILES
******************************************************************************/
#include <iostream>
#include "Server.h"

using boost::asio::ip::tcp;

/******************************************************************************
* MEMBER FUNTIONS
******************************************************************************/
//Aca construyo a connection con el io_context?, porque el io context del server no recibe un io_context, 
//es como si en la clase tenemos un objeto y en este constructor creamos otro con el io_context que nunca guardamos
Server::Server(boost::asio::io_context& io_context)
	: context_(io_context),
	acceptor_(io_context, tcp::endpoint(IP, PORT))
{
	std::cout << "Welcome to Group 5 server" << std::endl; //Mostramos el mensaje de bienvenida
	std::cout << "The best server ;)" << std::endl;
	std::cout << std::endl;
	awaitConnection(); //Comenzamos a esperar conexiones
}

////Destructor
Server::~Server()
{
}

//Funcion que crea una conexion y espera a que se conecte un cliente
void Server::awaitConnection() {
	Connection::pointer new_connection = Connection::create(context_);  //Creamos una nueva conexion
	acceptor_.async_accept(new_connection->getSocket(),
		boost::bind(&Server::establishedConnection_cb, this, new_connection, boost::asio::placeholders::error));  //Habilitamos la conexion nueva y esperamos a que se conecte el cliente
}

/******************************************************************************
* CALLBACKS
******************************************************************************/
//Callback de la conexion establecida que recibe una conexion y un error de boost. La funcion comienza correr el ciclo de la conexion
void Server::establishedConnection_cb(Connection::pointer currentConnection, const boost::system::error_code& error) {
	std::cout << "Connection Established" << std::endl;
	if (!error)				//Si no hubo errores
	{
		currentConnection->readData(currentConnection);  //Comenzamos a esperar un mensaje entrante
	}
	else {
		std::cout << error.message() << std::endl;		
	}
	awaitConnection();	//Volvemos a crear una nueva conexion
}
