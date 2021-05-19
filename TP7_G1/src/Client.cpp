/***************************************************************************//**
@file     +Client.cpp+
@brief    +Funciones para descargar los Tweets+
@author   +Grupo 1+
******************************************************************************/

/*******************************************************************************
* INCLUDE HEADER FILES
******************************************************************************/
#include "Client.h"
#include <iostream>
#include <string>
#include <fstream>

//Vamos a usar la librería NLOHMANN JSON para el TOKEN
using json = nlohmann::json;

#define PATH	"Tweets_JSON/downlTweet.json"

/*******************************************************************************
* FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
******************************************************************************/
static size_t myCallback(void* contents, size_t size, size_t nmemb, void* userp);

/*******************************************************************************
*******************************************************************************
						GLOBAL FUNCTION DEFINITIONS
*******************************************************************************
******************************************************************************/
//Obtiene twits de una cuenta pública (en este caso, de "La Nación")
bool client(json& j, int counter, std::string user) {
	// Vamos a utilizar la librería CURL ya que debemos conectarons a un servidor HTTPS
	// (HTTP seguro) el cual requeire un protocolo especial de encriptación
	CURL* curl;					//Variable donde vamos a guardar las configuraciones de una transferencia
	CURLM* multiHandle;			//Variable donde vamos a atachear los easy handles
	CURLcode res;
	std::string readString, token;

	// Query es la direcci�n de Twitter que vamos a consultar. vamos a bajar los &count twits de screen_name en formato JSON.
	std::string query = "https://api.twitter.com/1.1/statuses/user_timeline.json?screen_name=";
	query.append(user);
	query.append("&count=");
	query.append(std::to_string(counter));

	//Las dos constantes de abajo son el API_Key y API_SecretKey que necesita Twitter para realizar la autenticación de nuestra App
	// y por lo tanto permitirnos el acceso a sus servidores. Son parte de la estrategia de autenticación Oauth2.
	//Ambas se obtienen de la app que creen en la pagina de Twitter
	std::string API_key = "HCB39Q15wIoH61KIkY5faRDf6";
	std::string API_SecretKey = "7s8uvgQnJqjJDqA6JsLIFp90FcOaoR5Ic41LWyHOic0Ht3SRJ6";

	/************************************************************************************
	*                      Get Bearer Token from the Twitter API						*
	************************************************************************************/
	// Primero realizamos la inicialización de curl para obtener el token de Twitter el cual vamos
	// a utilizar luego para realizar la autenticación por Oauth2.
	curl = curl_easy_init();
	if (curl) {
		//Seteamos primero la pagina donde nos vamos a conectar. Para buscar el token es siempre la misma
		curl_easy_setopt(curl, CURLOPT_URL, "https://api.twitter.com/oauth2/token");

		// Si la p�gina nos redirije a alg�n lado, le decimos a curl que siga dicha redirecci�n.
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

		// Le decimos a CURL que trabaje tanto con HTTP como HTTPS, Autenticaci�n por HTTP en modo b�sico.
		curl_easy_setopt(curl, CURLOPT_PROTOCOLS, CURLPROTO_HTTP | CURLPROTO_HTTPS);
		curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);

		//Preparamos el password para la autenticaci�n
		std::string userPwd = API_key + ":" + API_SecretKey;
		//Se lo seteamos a CURL.
		curl_easy_setopt(curl, CURLOPT_USERPWD, userPwd.c_str());

		struct curl_slist* list = NULL;
		//Pueden setear el Header con la linea de abajo, pero necesitan codificar las claves en Base64
		//list = curl_slist_append(list, "Authorization: Basic YlB5alk1bWRMR2V4TlhPTHhSUjd3MUVjUzpkR2liU3FIcURrektQMUtPbzFJTjRBd21tZGI1Tnl5ZjBFQTZkTDBLWlpmZDE0ZnhQQw==");

		//Le decimos a CURL que vamos a mandar URLs codificadas y adem�s en formato.
		list = curl_slist_append(list, "Content-Type: application/x-www-form-urlencoded;charset=UTF-8");
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);

		//Le decimos a CURL que trabaje con credentials.
		std::string data = "grant_type=client_credentials";
		curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, data.size());
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());

		//Le decimos a curl que cuando haya que escribir llame a myCallback
		//y que use al string readString como user data.
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, myCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readString);

		// Perform the request, res will get the return code
		// Con lo de abajo le decimos a curl que intente conectarse a la p�gina (recordemos que la idea es
		// obtener la autenticaci�n as� luego Twitter nos deja conectarnos a cualquier usuario p�blico)
		//Recordar que easy_perform es bloqueante, en este caso es correcto asi al ser inicializacion y no tardar mucho tiempo.
		res = curl_easy_perform(curl);

		// Nos fijamos si hubo algún error
		if (res != CURLE_OK) {
			std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
			//Hacemos un clean up de curl antes de salir.
			curl_easy_cleanup(curl);
			return false;
		}

		// Si no hubo errores hago un clean up antes de realizar un nuevo query.
		curl_easy_cleanup(curl);

		//Si el request de CURL fue exitoso entonces twitter devuelve un JSON
		//Abajo busco el token en el JSON para luego acceder a los twits.
		j = json::parse(readString);

		//Se encierra el parseo en un bloque try-catch porque la libreria maneja errores por excepciones.
		//Pueden tambien parsear usando iteradores o la forma que quieras, buscar en la documentacion las diferentes formas.
		try {
			//Tratamos de acceder al campo acces_token del JSON
			std::string aux = j["access_token"];
			token = aux;
			//std::cout << "Bearer Token get from Twitter API: \n" << token << std::endl;
		}
		catch (std::exception& e) {
			//Si hubo algun error, se muestra el error que devuelve la libreria
			std::cerr << e.what() << std::endl;
			return false;
		}
	}
	else {
		std::cout << "Cannot download tweets. Unable to start cURL for Oauth2" << std::endl;
		return false;
	}

	//Una vez obtenido el Token ahora voy a buscar los Tweets
	/************************************************************************************
	*                        Get Tweets from the Twitter API							*
	************************************************************************************/
	curl = curl_easy_init();	// Nuevamente reinicializo todo curl.
	multiHandle = curl_multi_init();
	readString = "";
	int stillRunning = 0;
	if ((curl != NULL) & (multiHandle != NULL)) {
		//Attacheo el easy handle para manejar una coneccion no bloqueante.
		curl_multi_add_handle(multiHandle, curl);

		//Seteamos URL FOLLOWLOCATION y los protocolos a utilizar igual que antes.
		curl_easy_setopt(curl, CURLOPT_URL, query.c_str());
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curl, CURLOPT_PROTOCOLS, CURLPROTO_HTTP | CURLPROTO_HTTPS);

		//Construimos el Header de autenticacion como lo especifica la API
		//usando el token que obtuvimos antes
		struct curl_slist* list = NULL;
		std::string aux = "Authorization: Bearer ";
		aux = aux + token;
		list = curl_slist_append(list, aux.c_str());
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);

		//Seteamos los callback igual que antes
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, myCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readString);

		//Realizamos ahora un perform no bloqueante
		curl_multi_perform(multiHandle, &stillRunning);
		while (stillRunning) {
			//Debemos hacer polling de la transferencia hasta que haya terminado
			curl_multi_perform(multiHandle, &stillRunning);

			//Mientras tanto podemos hacer otras cosas
		}

		//Checkeamos errores
		if (res != CURLE_OK) {
			std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
			//Hacemos un clean up de curl antes de salir.
			curl_easy_cleanup(curl);
			return false;
		}

		//Siempre realizamos el cleanup al final
		curl_easy_cleanup(curl);

		//Si el request de CURL fue exitoso entonces twitter devuelve un JSON
		//con toda la informacion de los tweets que le pedimos la parceamos y la guardamos en la referencia j
		j = json::parse(readString);

		// ------------------------------------------------------------------------------------- //
		// Guardamos el JSON descargado
		std::ofstream outputFile(PATH, std::ios::out | std::ios::binary);
		outputFile << j.dump() << std::endl;    //Lo guardamos como string
		outputFile.close();

	}
	else
		std::cout << "Cannot download tweets. Unable to start cURL" << std::endl;
	return true;
}


/*******************************************************************************
 *******************************************************************************
								Callback
 *******************************************************************************
 ******************************************************************************/
//Concatena lo recibido en content a s
static size_t myCallback(void* contents, size_t size, size_t nmemb, void* userp) {
	size_t realsize = size * nmemb;
	char* data = (char*)contents;
	//fprintf(stdout, "%s",data);
	std::string* s = (std::string*)userp;
	s->append(data, realsize);
	return realsize;						//recordar siempre devolver realsize
}