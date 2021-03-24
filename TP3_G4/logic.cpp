/***************************************************************************//**
  @file     +logic.cpp+
  @brief    ++
  @author   +Grupo 4+
 ******************************************************************************/
#include <math.h>
#include <cstdlib>
#include <time.h>

#include "logic.h"

#define VEL(x)  ((x)/100)

/******************************************************************************
* FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
******************************************************************************/
static void eatFood(Simu& s);

//Funcion para ver si hay comida en el radio
static void smeelFood(Simu& s);


static void newBlob(Simu& s);

/**
 * @brief Ve si dos cajas estan chocando o no
 * @param collBox_t*  la direccion de la caja 1
 * @param collBox_t* la direccion de la caja 2
 * @return 1 si chocan 0 si no
**/
static int isCollision(collBox* box1, collBox* box2);


static collBox blobSizeCallbox(Simu& s, int n);


static void colition(Simu& s);


static void tanslateBlobs(Simu& s);


int orden(const void* p1, const void* p2);


static float RAngle(int angleLimit);
/*******************************************************************************
*******************************************************************************
                    GLOBAL FUNCTION DEFINITIONS
*******************************************************************************
******************************************************************************/
void simular(Simu& s) {
    //Analiza si algun blob debe morir
    for (int i = 0; i < s.blobCount; i++) {
        switch (s.blob[i].age) {
        case babyBlob:
            s.blobCount = s.blob[i].blobDeath(s.deathRateBB, s.blobCount);
            break;
        case grownBlob:
            s.blobCount = s.blob[i].blobDeath(s.deathRateGW, s.blobCount);
            break;
        case goodOldBlob:
            s.blobCount = s.blob[i].blobDeath(s.deathRateGO, s.blobCount);
            break;
        }
    }
    //ORDENAR ARREGLO Y MODIFICAR BLOBCOUNT
    qsort(s.blob, MAXBLOBS, sizeof(Blob), &orden);

    //ANALIZAR SI HAY COLISION DE BLOBS Y HACE MERGE
    colition(s);
    
    //ORDENAR EL ARREGLO DE BLOBS SI MERGEA ALGUNO
    qsort(&s.blob, MAXBLOBS, sizeof(Blob), &orden);

    //Buscan la comida mas cercana
    smeelFood(s);

    //MOVER A LOS BLOBS
    tanslateBlobs(s);

    //COMIERON?
    eatFood(s);

    //Analiza si cmoieron mucho y crea un nuevo blob
    newBlob(s);
    qsort(s.blob, MAXBLOBS, sizeof(Blob), &orden);
}


//Modifica la cantida de comida en la simulación
Food* ammFood(Simu& s) {
    s.food = (Food*) realloc(s.food, sizeof(Food) * s.foodCount);
    if (s.food != NULL) {
        s.food[s.foodCount-1].p.randPoint(s.width, s.height);
    }
    return s.food;
}

/******************************************************************************
*******************************************************************************
                    LOCAL FUNCTION DEFINITIONS
*******************************************************************************
******************************************************************************/
//Funcion que analiza el food count en cada blob para originar uno nuevo
static void newBlob (Simu& s){
    int cant = s.blobCount;
    for (int i = 0; i < cant; i++) {
        if ( s.blob[i].blobBirth() == EAT ) {
            ++s.blobCount;
            s.blob[s.blobCount].p.randPoint(s.width, s.height);
            s.blob[s.blobCount].blobRAngle(360);
            s.blob[s.blobCount].age = babyBlob;
            s.blob[s.blobCount].foodCount = 0;
            s.blob[s.blobCount].aliveProb = 1.0;
            s.blob[s.blobCount].isAlive = ALIVE;
        }
    }
}


//Si llego a la comida incermenta foodCount
void eatFood(Simu& s) { //REVISAR
    for (int i = 0; i < s.blobCount; i++) {
        for (int j = 0; j < s.foodCount; j++) {
            if ((int) s.blob[i].p.distBtwPoint(s.food[j].p) == 0) { //CHECKEAR QUE SE PUEDA HACER == 0 POR SER DOUBLE
                s.blob[i].feed();
                s.food[j].p.randPoint(s.width, s.height);
            }
        }
    }
}


//Funcion para ver si hay comida en el radio
static void smeelFood(Simu& s) {
    int i = 0, j = 0;
    int found = -1;
    float var1 = 0, var2 = 0;
    for (int j = 0; j < s.blobCount; j++) {
        for (i = 0; i < s.foodCount; i++) {
            if (s.blob[j].p.distBtwPoint(s.food[i].p) < (float) s.smellRadius) { //saca si la distancia entre el blob y la comida es menor al radio maximo
                if (var1 == 0) {
                    var1 = s.blob[j].p.distBtwPoint(s.food[i].p);
                    found = i;
                }
                else {
                    var2 = s.blob[j].p.distBtwPoint(s.food[i].p);
                }

                if (var1 < var2) {
                    var2 = 0;
                }
                else if ((var2 < var1) && var2 != 0) {
                    var1 = var2;
                    var2 = 0;
                    found = i;
                }
            }
        }
        if (found != -1){
            s.blob[j].angle = s.blob[j].p.angB2Point(s.food[found].p);
        }
    }
}


//Analiza todos los blobs y se fija si colisionan, si colicionan hace merge
static void colition(Simu& s) {
    int cant = s.blobCount;
    for (int i = 0; i < cant-1; i++) {
        if (s.blob[i].isAlive == ALIVE) {
            if (s.blob[i].age != goodOldBlob) {
                collBox Blob1 = blobSizeCallbox(s, i);
                for (int j = i+1; j < cant; j++) {
                    if (s.blob[j].isAlive == ALIVE) {
                        if (s.blob[i].age == s.blob[j].age) {
                            collBox Blob2 = blobSizeCallbox(s, j);
                            if (isCollision(&Blob1, &Blob2)) {  // Choque con Cannon
                                if (s.blob[i].age == babyBlob) {
                                    s.blob[i].age = grownBlob;
                                }
                                else {
                                    s.blob[i].age = goodOldBlob;
                                }
                                s.blob[i].p.x = (s.blob[i].p.x + s.blob[j].p.x) / 2;
                                s.blob[i].p.y = (s.blob[i].p.y + s.blob[j].p.y) / 2;
                                s.blob[i].angle = ((s.blob[i].angle + s.blob[j].angle) / 2) + RAngle(s.randomJiggleLimit);
                                s.blob[i].foodCount = 0;
                                s.blob[i].maxVel = (s.blob[i].maxVel + s.blob[j].maxVel) / 2;
                                s.blob[j].isAlive = DEATH;
                                s.blobCount--;
                            }
                        }
                    }
                }
            }
        }
    }
}


static collBox blobSizeCallbox(Simu& s, int n) {
    collBox CBlob = { 0.0 };
    switch (s.blob[n].age) {
    case babyBlob:
        CBlob = { s.blob[n].p.x , s.blob[n].p.y, s.widthBBB, s.heightBBB };
        break;
    case grownBlob:
        CBlob = { s.blob[n].p.x , s.blob[n].p.y, s.widthGWB, s.heightGWB };
        break;
    }
    return CBlob;
}


/**
* @brief Ve si dos cajas estan chocando o no
*/
static int isCollision(collBox* box1, collBox* box2) {
    if (box1->x <= box2->x + box2->width &&                          // Para ver si dos cajas estan chocando
        box2->x <= box1->x + box1->width &&
        box1->y <= box2->y + box2->height &&
        box2->y <= box1->y + box1->height)
        return 1;

    return 0;
}


//Mueve el blob
static void tanslateBlobs(Simu& s) {  //AGREGAR DEPENDENCIA DE VELOCIDAD
    for (int r = 0; r < s.blobCount; r++) {
        float x = s.blob[r].p.x;
        float y = s.blob[r].p.y;
        x += (float) (cos(s.blob[r].angle) * VEL((float) s.relVel) * VEL(s.blob[r].maxVel));
        y += (float) (sin(s.blob[r].angle) * VEL((float) s.relVel) * VEL(s.blob[r].maxVel));
        if (x > 0 && x < s.width && y > 0 && y < s.height) {     //si no choca con la pared entonces lo muevo guardando data de posicion
            s.blob[r].p.x = x;        //me muevo en x lo que vale el coseno del angulo del robot
            s.blob[r].p.y = y;        //me muevo en y lo que vale el seno del angulo del robot
        }
        else if(x > s.width) {
            s.blob[r].p.x = 0;
        }
        else if (y > s.height) {
            s.blob[r].p.y = 0;
        }
        else if (x < 0) {
            s.blob[r].p.x = (float) s.width;
        }
        else if (y < 0) {
            s.blob[r].p.y = (float) s.height;
        }
    }
}


static int orden(const void* p1, const void* p2) {
    int result=-1; //-1 equivale a que b1 tiene que estar antes que b2
    Blob* b1 = (Blob*) p1;
    Blob* b2 = (Blob*) p2;
    if (b1->isAlive == DEATH) {
        if (b2->isAlive == DEATH) {
            result = 0; //No hay que hacer cambios al arreglo
        }
        else {
            result = 1; //En b2 tiene que ir antes que b1
        }
    }
    return result;
}

//Funcion que genera un angulo random en radianes. No recibe nada. Devuelve un float (el angulo generado).
static float RAngle(int angleLimit) {
    float angle = 0;
    angle = (float) (rand() % angleLimit);       //opciones de 360 numeros
    angle = (angle * PI) / 180; //Convierto el angulo a radianes
    return angle;               //devuelvo angulo en radianes
}