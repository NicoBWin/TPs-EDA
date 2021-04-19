/***************************************************************************//**
@file     +Connection.cpp+
@brief    +Funciones de la conexion de HTTP+
@author   +Grupo 5+
******************************************************************************/


/******************************************************************************
* INCLUDE HEADER FILES
******************************************************************************/
#include "Connection.h"

using boost::asio::ip::tcp;

/******************************************************************************
* MEMBER FUNTIONS
******************************************************************************/
//Constructor
Connection::Connection(boost::asio::io_context& io_context) : socket_(io_context), http("serverData") {
}

//Destructor
Connection::~Connection() 
{
}

//Funcion para reservar memoria para una nueva conexion
Connection::pointer Connection::create(boost::asio::io_context& io_context) {
	return pointer(new Connection(io_context));
}

//Getter del socket
tcp::socket& Connection::getSocket() {
	return socket_;
}

//Funcion que recibe la informacion del cliente
void Connection::readData(Connection::pointer currentConnection) {

	boost::asio::async_read_until(socket_, buffer_, HTMLENDMSG,			// Leemos hasta el terminador HTTP = "\r\n\r\n"
		boost::bind(
			&Connection::messageReceived_cb,
			this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred,
			currentConnection)
	);
}


//Funcion que envia la respuesta al cliente
void Connection::sendData(std::string msg) {

	sendMsg = msg;

	/*
	boost::asio::async_write(		//Enviamos el mensaje
		socket_,
		boost::asio::buffer(sendMsg),
		boost::bind(
			&Connection::responseSent_cb,
			this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred
		)
	);
	*/

	boost::asio::async_write(socket_, boost::asio::buffer(sendMsg),
		boost::bind(
			&Connection::responseSent_cb,
			shared_from_this(),
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred
		)
	);
}


/******************************************************************************
* CALLBACKS
******************************************************************************/

//Funcion de callback que lee el buffer y en funcion del estado de la conexion, la cierra o genera y envia la respuesta a peticion del cliente
void Connection::messageReceived_cb(const boost::system::error_code& error, std::size_t size, Connection::pointer currentConnection) {
	
	std::string aux;				//Pasamos el buffer de asio a string
	std::istream is(&buffer_);
	std::getline(is, aux, {});

	if (error == boost::asio::error::eof) {			//Si llego el cliente se desconecto, cerramos la conexion
		std::cout << "Conexion finalizada" << std::endl;
	}
	else {
		std::cout << size << " bytes received" << std::endl;	//Mostramos el pedido del cliente
		std::cout << "Received: " << aux << std::endl;

		http.dataReceived(aux);			//Leemos el msg recibida, lo validamos y generamos el mensaje de respuesta
		sendData(http.getMsg());		//Enviamos el msg de respuesta
		readData(currentConnection);	//Volvemos a esperar otro mensaje con la conexion actual
	}
}


//Indica si se envio el mansaje correctamente, cuantos bytes se enviaron
void Connection::responseSent_cb(boost::system::error_code const& error, size_t bytes_sent) {
	if (!error) {
		//Mostramos en pantalla la cantidad de bytes enviados
		std::cout << "Response sent: " << bytes_sent << " bytes (header + file content)." << std::endl << std::endl;
	}
}

