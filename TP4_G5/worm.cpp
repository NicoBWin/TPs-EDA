/***************************************************************************//**
  @file     +Worm.cpp+
  @brief    +Funcionies miembro para la clase WORMS+
  @author   +Grupo 5+
 ******************************************************************************/
#include "Worm.h"

#include <cstdlib>

#include "allPrint.h"


//Constructor
Worm::Worm(void) {
    p.x = ((double) (rand() % (EXTREMDER-EXTREMIZQ) )) + EXTREMIZQ;//la pos x la seteo en un lugar random dentro de la consigna
    p.y = PISO;//empieza en el piso
    angle = 0;
    estado = IDLE;
    direction=RIGHT;
}

//
Point Worm::getPosition() {
    return p;
}
//
double Worm::getPosx() {
    return p.x;
}
//
double Worm::getPosy() {
    return p.y;
}
//
double Worm::getAngle() {
    return angle;
}
//
int Worm::getEstado() {
    return estado;
}
//
int Worm::getDirection() {
    return direction;
}

//
void Worm::setPos(Point& aux) {
    p = aux;
}
//
void Worm::setAngle(int angles) {
    this->angle = RAD(angles);
}
//
void Worm::setEstado(int newestado) {
    if (this->estado == IDLE) {
        this->estado = newestado;
    }
    else if (newestado == IDLE) {
        if (this->estado == MOVE_LEFT) {
            this->estado = FINISH_WALKING_L;
        }
        else if (this->estado == MOVE_RIGHT) {
            this->estado = FINISH_WALKING_R;
        }
        else if (this->estado == JUMP) {
            this->estado = FINISH_JUMP;
        }
        else {
            this->estado = IDLE;
        }
    }
}

//
void Worm::moveSetWorm(void) {
    Point auxp = { 0.0,0.0 };
    auxp.x = this->p.x;
    if (direction == RIGHT) {                                   //muevo a la derecha
            auxp.x += VELMOV;
    }
    else if (direction == LEFT) {                               //muevo a la izq
        auxp.x -= VELMOV;
    }
    //Me encargo de acomodar las posiciones para el caso de que se hayan ido de la pantalla
    if ((auxp.x > EXTREMIZQ) && (auxp.x < EXTREMDER)) {             //si no puedo ir mas a la izq o derecha no me muevo
        p.x = auxp.x;
    }
}

//
void Worm::jumpWorm(void) {
    Point auxp;
    static int conticks=0;
    auxp.x = p.x;
    auxp.y = p.y;
    if (conticks==0) {//si no estoy en medio de un salto
        if (this->direction == RIGHT) {//si muevo para la derecha
            angle = RAD(60);
        }
        else {  //muevo para la izquierda
            angle = RAD(120);
        }
        auxp.x += (VELJUMP*cos(angle));//vel en x es cte ecu en x=x0+vxT
        auxp.y -= (VELJUMP*sin(angle)-0.5*G*conticks*conticks);//y=y0+vyT-0.5GT^2
        if (auxp.x > EXTREMIZQ && auxp.x < EXTREMDER) {//si no puedo ir mas a la izq o derecha no me muevo
            p.x = auxp.x;
        }
        p.y = auxp.y;
        conticks++;
    }
    else {//estoy en medio de un salto
        auxp.x += (VELJUMP*cos(angle));//vel en x es cte ecu en x=x0+vxT
        auxp.y -= (VELJUMP*sin(angle)-0.5*G*conticks*conticks);//y=y0+vyT-0.5GT^2

        if (auxp.x > EXTREMIZQ && auxp.x < EXTREMDER) {//si no puedo ir mas a la izq o derecha no me muevo
            p.x = auxp.x;
        }
        if (auxp.y < PISO) {//si estoy por arriba del piso
            p.y = auxp.y;
            conticks++;
        }else{//si me paso del piso seteo en piso
            p.y = PISO;
            conticks = 0;
        }       
    }
}

//Actualiza el worm, ya sea posición, llama a funcion para actualizar imagen y actualiza su estado
void Worm::update(int worm) {
    static int frameCount[MAX_WORMS] = { 0 } ;  //Inicializo todos los elementos con 0
    switch (this->estado) {
    case IDLE:      //Estado inactivo o quieto
        printIdle(this->p, this->direction);
        frameCount[worm] = 0;
        break;

    case START_MOVING_LEFT: //Analiza si se preciono la tecla para moverse en esa direccion o solo para cambiar el lado al que mira
        if (frameCount[worm] < 5) {
            frameCount[worm]++;
            this->direction = LEFT;
            printIdle(this->p, this->direction);
        }
        else {
            estado = MOVE_LEFT;
            printIdle(this->p, this->direction);
        }
        break;
    case MOVE_LEFT:     //Se esta moviendo hacia la izquierda
    case FINISH_WALKING_L:  //Se esta moviendo a la izquierda pero cuando termine debe configurar en estado en IDLE porque el usuario dejo de precionar la tecla
        if (frameCount[worm] < 8) {
            printWarmup(this->p, frameCount[worm] - 4, this->direction);
        }
        else if (frameCount[worm] < 22) {
            if (frameCount[worm] == 21) {
                moveSetWorm();
            }
            printWalk(this->p, frameCount[worm] - 7, this->direction);
        }
        else if (frameCount[worm] < 36) {
            if (frameCount[worm] == 35) {
                moveSetWorm();
            }
            printWalk(this->p, frameCount[worm] - 21, this->direction);
        }
        else if (frameCount[worm] < 50) {
            if (frameCount[worm] == 49) {
                moveSetWorm();
            }
            printWalk(this->p, frameCount[worm] - 35, this->direction);
        }
        else {
            if (estado == FINISH_WALKING_L) {
                estado = IDLE;
            }
            else {
                estado = START_MOVING_LEFT;
            }
            printIdle(this->p, this->direction);
            frameCount[worm] = -1;          //Lo reinicio en -1 poque antes de salir del case hay un frameCount++
        }
        frameCount[worm]++;
        break;

    case START_MOVING_RIGHT:    //Analiza si se preciono la tecla para moverse en esa direccion o solo para cambiar el lado al que mira
        if (frameCount[worm] < 5) {
            frameCount[worm]++;
            this->direction = RIGHT;
            printIdle(this->p, this->direction);
        }
        else {
            estado = MOVE_RIGHT;
            printIdle(this->p, this->direction);
        }
        break;
    case MOVE_RIGHT:    //Se esta moviendo hacia la derecha
    case FINISH_WALKING_R:  //Se esta moviendo a la derechaa pero cuando termine debe configurar en estado en IDLE porque el usuario dejo de precionar la tecla
        if (frameCount[worm] < 8) {
            printWarmup(this->p, frameCount[worm] - 4, this->direction);
        }
        else if (frameCount[worm] < 22) {
            if (frameCount[worm] == 21) {
                moveSetWorm();
            }
            printWalk(this->p, frameCount[worm] - 7, this->direction);
        }
        else if (frameCount[worm] < 36) {
            if (frameCount[worm] == 35) {
                moveSetWorm();
            }
            printWalk(this->p, frameCount[worm] - 21, this->direction);
        }
        else if (frameCount[worm] < 50) {
            if (frameCount[worm] == 49) {
                moveSetWorm();
            }
            printWalk(this->p, frameCount[worm] - 35, this->direction);
        }
        else {
            if (estado == FINISH_WALKING_R) {
                estado = IDLE;
            }
            else {
                estado = START_MOVING_RIGHT;
            }
            printIdle(this->p, this->direction);
            frameCount[worm] = -1;              //Lo reinicio en -1 poque antes de salir del case hay un frameCount++
        }
        frameCount[worm]++;
        break;

    case START_JUMP:    //Analiza si se preciono la tecla para moverse en esa direccion
        if (frameCount[worm] < 5) {        
            frameCount[worm]++;
            printIdle(this->p, this->direction);
        }
        else {
            estado = JUMP;
            printIdle(this->p, this->direction);
        }
        break;
    case JUMP:  //Esta saltando
    case FINISH_JUMP: //Está saltando pero cuando termine debe configurar en estado en IDLE porque el usuario dejo de precionar la tecla
        if (frameCount[worm] < 9) {
            printJump(this->p, frameCount[worm], this->direction);
        }
        else if (frameCount[worm] < 45) {
            jumpWorm();
            printJump(this->p, frameCount[worm], this->direction);
            if (this->p.y == PISO) {
                frameCount[worm] = 45;
            }
        }
        else if ((frameCount[worm] >=45) && (frameCount[worm] < 50)) {
            printJump(this->p, frameCount[worm], this->direction);
        }
        else {
            if (estado == FINISH_JUMP) {
                estado = IDLE;
            }
            else {
                estado = START_JUMP;
            }
            printIdle(this->p, this->direction);
            frameCount[worm] = -1;              //Lo reinicio en -1 poque antes de salir del case hay un frameCount++
        }
        frameCount[worm]++;
        break;
    }
}