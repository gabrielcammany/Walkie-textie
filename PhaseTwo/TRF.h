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

char valuePos(char);

char getTramesTotals(unsigned char pos);

char getTramesPropies(unsigned char pos);

char* getMessage();

int getLength();

char getSincro();

#endif	/* LETLEDS_H */

