/*
 * File:   TPWM.h
 *
 * TAD que fa encendre un led com si fos un heartbeat. Em serveix per veure si
 * el micro s'ha quedat penjat
 *
 * Created on 25 de enero de 2014, 23:02
 */

#ifndef TRF_H
#define	TRF_H
#include <xc.h>
#include "time.h"
#include "PrTPropaganda.h"
#include "AuTAudio.h"

void RFInit();
//Pre: Requereix del TITITMER.c
//Post: Demana un timer i posa la sortida del LED a 0
void MotorRF();

char* getTramesTotals();
//Pre: -
//Post: Retorna el numero de trames totals

char* getTramesPropies();
//Pre: -
//Post: Retorna el numero de trames propies

unsigned char* getMessage();
//Pre: -
//Post: Retorna el missatge que hi ha actualment

int getLength();
//Pre: -
//Post: Retorna la mida del missatge tenim desat


#endif	/* LETLEDS_H */

