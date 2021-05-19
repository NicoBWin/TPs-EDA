#include "milagrosLCD.h"

//ALLEGRO LIB'S
#include <allegro5/allegro.h>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>


#define NEGRO		al_map_rgb(0, 0, 0)
#define ROSA		al_map_rgb(227, 13, 107)
#define BLANCO		al_map_rgb(255, 255, 255)
#define AMARILLO	al_map_rgb(248, 241, 8)
#define AZUL		al_map_rgb(8, 59, 248)
#define VERDE		al_map_rgb(8, 248, 12)
#define NARANJA		al_map_rgb(248, 153, 8)
#define VIOLETA		al_map_rgb(169, 35, 219)
#define CELESTE		al_map_rgb(35, 172, 219)


ALLEGRO_DISPLAY* displayM;
ALLEGRO_FONT* font;

using namespace std;

milagrosLCD::milagrosLCD() {
	//Inicializo los datos miembro.
	data = "                                ";
	initOK = true;
	displayM = nullptr;
	font = nullptr;

	if (!al_init() && initOK) { //Primera funcion a llamar antes de empezar a usar allegro.
		cout << "failed to initialize allegro!" << endl;
		initOK = false;
	}
	
	//Inicializo las fuentes y letras.
	init_fonts();

	//Inicializo el display
	init_display();

	lcdClear();
}

milagrosLCD::~milagrosLCD() {
	al_destroy_display(displayM);
	al_destroy_font(font);
}

void milagrosLCD::init_display(){
	displayM = al_create_display(C_SIZE*(COLUMNS+1), C_SIZE*(ROWS+1)); // Intenta crear display dependiendo del tamaño de la letra que elija
	al_set_window_title(displayM, "Mili's LCD");
	if (!displayM && initOK) {
		cout << "failed to create display!" << endl;
		initOK = false;
	}
}

void milagrosLCD::init_fonts() {
	if (!al_init_font_addon() && initOK) {
		cout << "failed to initialize font addon!" << endl;
		initOK = false;
	}
	if (!al_init_ttf_addon() && initOK) {
		cout << "failed to initialize ttf addon!" << endl;
	}
	font = al_load_font("LCDs/MiliLCD/resources/04B_30__.TTF", C_SIZE, 0);
	if(!font) {
		cout << "failed to initialize the font!\n" << endl;
		initOK = false;
	}
}

void milagrosLCD::clear_display() {
	al_set_target_backbuffer(displayM);
	al_clear_to_color(AMARILLO); //Hace clear del backbuffer del diplay al color amarillo
	data = "                                ";
	position.column = HOME_C;
	position.row = HOME_R;

	al_flip_display(); //Flip del backbuffer, pasa a verse a la pantalla
}

void milagrosLCD::write_display() {
	al_set_target_backbuffer(displayM);
	al_clear_to_color(AMARILLO);
	char print[2] = { 1,'\0'};
	//Voy imrpimiendo de a 1 letra.
	for (int i = 0; i < (int)data.size(); ++i) {
		print[0] = data[i];
		if (i < 16) {
			al_draw_text(font, NEGRO, getCoords(i), getCoords(0), 0, print);
		}
		if (i >= 16 && i < 32) {
			al_draw_text(font, NEGRO, getCoords(i-16), getCoords(1), 0, print);
		}
	}
	//Hago el flip para mostrarlo en el display todo de una.
	al_flip_display();
}

/*=====================================================
* Name: lcdInitOk
* Entra: -
* Resulta: No genera ningún cambio en el display.
* Devuelve en su nombre “true” si el display se inicializó
* correctamente (el constructor no tuvo errores) o “false
* en caso contrario.
*=====================================================*/
bool milagrosLCD::lcdInitOk() {
	return initOK;
}

/*=====================================================
* Name: lcdGetError
* Entra: -
* Resulta: No genera ningún cambio en el display.
* Devuelve en su nombre un lcdError&
*=====================================================*/
lcdError& milagrosLCD::lcdGetError() {
	return LCDError;
}

/*=====================================================
* Name: lcdClear
* Entra: -
* Resulta: Borra el display y el cursor va a HOME
*
* Devuelve en su nombre “true” si fue satisfactoria “false”
* en caso contrario.
*=====================================================*/
bool milagrosLCD::lcdClear() {
	bool clearOK = true; //Vale true si todo bien y false en caso contrario.
	//Lo establezco en la primer posicion
	position.column = HOME_C;
	position.row = HOME_R;

	data = "                                ";
	
	al_clear_to_color(AMARILLO);
	al_flip_display();

	return clearOK;
}

/*=====================================================
* Name: lcdClearToEOL
* Entra: -
* Resulta: Borra el display desde la posición actual
* del cursor hasta el final de la línea.
*
* Devuelve en su nombre “true” si fue satisfactoria “false”
* en caso contrario.
*=====================================================*/
bool milagrosLCD::lcdClearToEOL() {
	bool clearOK = true; //Vale true si todo bien y false en caso contrario.
	if (position.row == 0) {

		data.replace(position.column, 16 - position.column, 16 - position.column, ' ');
	}
	else if (position.row == 1) {

		data.replace(position.column + 16, 16 - position.column, 16 - position.column, ' ');
	}
	write_display();
	return clearOK;
}

/*=====================================================
* Name: operator<<()
* Entra: Un carácter
* Resulta: Pone el carácter en la posición actual
* del cursor del display y avanza el cursor a la próxima
* posición respetando el gap (si el carácter no es imprimible
* lo ignora)
*
* Devuelve en su nombre una referencia a un basicLCD que permite
* encascar la función:
* basicLCD lcd;
* lcd << ‘a’ << ‘b’ << ‘c’;
*=====================================================*/
basicLCD& milagrosLCD::operator<<(const char c) {
	//Lo imprime siempre que sea imprimible.
	if (isprint(c)) {
		if (position.row == 0)
			data.replace(position.column, 1, 1, c);
		else if (position.row == 1)
			data.replace(position.column + 16, 1, 1, c);

		write_display();

		lcdMoveCursorRight();
	}

	return *this;
}

/*=====================================================
* Name: operator<<()
* Entra: Una cadena de caracteres NULL terminated
* Resulta: imprime la cadena de caracteres en la posición actual
* del cursor y avanza el cursor al final de la cadena respetando
* el gap (si algún carácter no es imprimible lo ignora). Si recibe una
* cadena de más de 32 caracteres, muestra los últimos 32 en el display.
*
* Devuelve en su nombre una referencia a un basicLCD que permite
* encascar la función:
* basicLCD lcd;
* lcd << “Hola” << “ “ << “Mundo”;
*=====================================================*/
basicLCD& milagrosLCD::operator<<(const char* c) {
	string aux = string(c);
	int cont = 0;
	while((int) aux.size() > 32) {
		aux.erase(0, 1);
	}
	//Agrego lo enviado a la data del display. Además updateo el cursor.
	while (cont < (int) aux.size()) {
		if (position.row == 0) {
			data.replace(position.column, 1, 1, aux[cont]);
			++position.column;
			if (position.column >= 16) {
				position.column = 0;
				position.row = 1;
			}
		}
		else if (position.row == 1) {
			data.replace(position.column+16, 1, 1, aux[cont]);
			++position.column;
			if (position.column >= 16) {
				position.column = 0;
				position.row = 0;
			}
		}
		++cont;
	}
	write_display();

	return *this;
}

/*=====================================================
* Name: lcdMoveCursorUp
*
* Entra: -
* Resulta: Pasa el cursor a la primera línea del display sin
* alterar la columna en la que estaba.
*
* Devuelve en su nombre “true” si fue satisfactoria “false”
* en caso contrario.
*=====================================================*/
bool milagrosLCD::lcdMoveCursorUp() {
	bool moveOK = true; //Vale true si todo bien, false en lo contrario.
	switch (position.row) {
	case 0:
		break;
	case 1:
		position.row = 0;
		break;
	}
	return moveOK;
}

/*=====================================================
* Name: lcdMoveCursorDown
*
* Entra: -
* Resulta: Pasa el cursor a la segunda línea del display sin
* alterar la columna en la que estaba.
*
* Devuelve en su nombre “true” si fue satisfactoria “false”
* en caso contrario.
*=====================================================*/
bool milagrosLCD::lcdMoveCursorDown() {
	bool moveOK = true; //Vale true si todo bien y false en caso contrario.
	switch (position.row) {
	case 0:
		position.row = 1;
		break;
	case 1:
		break;
	}
	return moveOK;
}

/*=====================================================
* Name: lcdMoveCursorRight
*
* Entra: -
* Resulta: Avanza el cursor una posición
*
* Devuelve en su nombre “true” si fue satisfactoria “false”
* en caso contrario.
*=====================================================*/
bool milagrosLCD::lcdMoveCursorRight() {
	bool moveOK = true; //Vale true si todo bien y false en caso contrario.
	switch (position.column) {
	case 15:
		break;
	default:
		++position.column;
		break;
	}
	return moveOK;
}

/*=====================================================
* Name: lcdMoveCursorLeft
*
* Entra: -
* Resulta: Retrocede el cursor una posición
*
* Devuelve en su nombre “true” si fue satisfactoria “false”
* en caso contrario.
*=====================================================*/
bool milagrosLCD::lcdMoveCursorLeft() {
	bool moveOK = true; //Vale true si todo bien y false en caso contrario.
	switch (position.column) {
	case 0:
		break;
	default:
		--position.column;
		break;
	}
	return moveOK;
}

/*=====================================================
* Name: lcdSetCursorPosition
* Entra: Recibe una estructura tipo cursorPosition
* Resulta: Posiciona el cursor en la posición dada
* por row y column. row[0-1] col[0-19]. Ante un valor inválido
* de row y/o column ignora la instrucción (no hace nada).
*
* Devuelve en su nombre “true” si fue satisfactoria “false”
* en caso contrario.
*=====================================================*/
bool milagrosLCD::lcdSetCursorPosition(const cursorPosition pos) {
	bool setOK = true; //Vale true si todo bien y false en caso contrario.
	//Me fijo que los valores que me piden sean correctos.
	if (pos.row >= 2 || pos.row < 0 || pos.column >= 16 || pos.column < 0) {
		setOK = false;
		cout << "Invalid cursor position!" << endl;
	}
	this->position.column = pos.column;
	this->position.row = pos.row;
	return setOK;
}

/*=====================================================
* Name: lcdGetCursorPosition
* Entra: -
* Resulta: Devuelve la posición actual del cursor.
*
*
* Devuelve una estructura tipo cursorPosition
*=====================================================*/
cursorPosition milagrosLCD::lcdGetCursorPosition() {
	return position;
}

int milagrosLCD::getCoords(int num) {
	return C_SIZE * num + C_SIZE/2;
}