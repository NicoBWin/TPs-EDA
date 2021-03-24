/***************************************************************************//**
  @file     +Simu.cpp+
  @brief    +Descripcion del archivo+
  @author   +Grupo 4+
 ******************************************************************************/
#include "Simu.h"

#include <cstdlib>
#include <time.h>

Simu::Simu(int modo, int initBlobs, int food, int maxVel, int width, int height, int width1, int height1, int width2, int height2, int width3, int height3) {
    for (int i = 0; i < MAXBLOBS; i++) {  //Iniciamos cada blob segun sus parámetros
        blob[i].p.randPoint(width, height);
        blob[i].blobRAngle(360);
        blob[i].age = babyBlob;
        blob[i].foodCount = 0;
        if (modo == 1) {
            blob[i].maxVel = maxVel;
        }
        else {
            blob[i].maxVel = (float) (rand() % maxVel);
        }
        blob[i].aliveProb = AL_PROB;
        if (i < initBlobs) {
            blob[i].isAlive = ALIVE;
        }
        else {
            blob[i].isAlive = DEATH;
        }
    }
    Food* f = (Food*)malloc(sizeof(Food) * food);
    if (f != NULL) {
        for (int j = 0; j < food; j++) {        //Le aisgnamos una posicion random a cada comida
            f[j].p.randPoint(width, height);
        }
    }
    this->food = f;
    this->foodCount = food;
    this->blobCount = initBlobs;
    this->relVel = VEL_INIT;
    this->smellRadius = SMELL_INIT;
    this->randomJiggleLimit = ANG_INIT; //Lo inicializamos con 360 grados


    this->deathRateBB = 0; // (float)((rand() % 10) * 0.001);
    this->deathRateGW = 0; // (float)((rand() % 10) * 0.001);
    this->deathRateGO = 0; // (float)((rand() % 10) * 0.001);

    //Parametros
    this->width = width;
    this->height = height;

    this->widthBBB = width1;
    this->heightBBB = height1;

    this->widthGWB = width2;
    this->heightGWB = height2;

    this->widthGOB = width3;
    this->heightGOB = height3;
}

//Funcion que libera la memoria asignada para todas las esturcturas de la simulación.
void Simu::deleteSimu() {
    free(food);
}