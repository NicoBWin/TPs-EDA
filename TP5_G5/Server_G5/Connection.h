/***************************************************************************//**
@file     +Connection.h+
@brief    +Header clase CONNECTION+
@author   +Grupo 5+
******************************************************************************/

#ifndef _CONNECTION_H_
#define _CONNECTION_H_

/******************************************************************************
* INCLUDE HEADER FILES
******************************************************************************/
#include <string>
#include <iostream>

#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>

#include "HTTP.h"

using boost::asio::ip::tcp;


/******************************************************************************
* CLASS
******************************************************************************/
class Connection : public boost::enable_shared_from_this<Connection> {
public:
	Connection(boost::asio::io_context& io_context);	//Constructor
	~Connection();										//Destructor

	typedef boost::shared_ptr<Connection> pointer;		

	static pointer create(boost::asio::io_context& io_context);	//Funcion para reservar memoria para una nueva conexion

	tcp::socket& getSocket();									//Getter del socket

	void readData(Connection::pointer currentConnection);		//Funcion que recibe la informacion del cliente
	void sendData(std::string msg);								//Funcion que envia la respuesta al cliente
	HTTP http;													//Clase http que envia la conexion

private:
	//Buffer de lectura de datos 
	boost::asio::streambuf buffer_;		//Buffer de lectura para readData()

	std::string sendMsg;				//String del mensaje a enviar en sendData()

	//Una vez recibido todo el mensaje en este callback se encarga de llamar a funcion que analiza el mensaje, 
	// llama a función que envia el mensaje y una vez finalizado vuelve a esperar mensaje en readData.
	void messageReceived_cb(const boost::system::error_code& error, std::size_t size, Connection::pointer currentConnection);

	//Indica si se envio el mansaje correctamente, cuantos bytes se enviaron
	void responseSent_cb(const boost::system::error_code& error, size_t bytes_sent);

	tcp::socket socket_;				//Socket de la conexion
};

#endif // _CONNECTION_H_