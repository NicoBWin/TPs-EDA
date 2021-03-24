/***************************************************************************//**
  @file     +Blob.cpp+
  @brief    ++
  @author   +Grupo 4+
 ******************************************************************************/
#include "Blob.h"
#include <cstdlib>

void Blob::moveb(Point& nexPos) {
	this->p.movep(nexPos);
}

void Blob::feed(void) {
	this->foodCount++;
}

//Setea un angulo random y recibe un angulo en grados (numero entero)
void Blob::blobRAngle(int angleG) { 
	float temp = rand() % angleG;       //opciones de 360 numeros
	this->angle = (temp * PI) / 180; //Convierto el angulo a radianes
}


int Blob::blobDeath(float prob, int counter) { //Recibe la prob del grupo etario y segun eso mata al Blob. Devuelve la cantidad de Blobs vivos
	if ( (this->aliveProb) < prob) {
		this->isAlive = DEATH;
		counter--;
	}
	this->aliveProb = (float) (((rand() % 10) + 1) * 0.1);
	return counter;
}


bool Blob::blobBirth(void){
	bool var = NEAT;
	switch (this->age){
		case babyBlob:
			if (this->foodCount == 5){
				this->foodCount = 0;
				var = EAT;
			}
			break;
		case grownBlob:
			if (this->foodCount == 4){
				this->foodCount = 0;
				var = EAT;
			}
			break;
		case goodOldBlob:
			if (this->foodCount == 3){
				this->foodCount = 0;
				var = EAT;
			}
			break;
	}
	return var;
}

