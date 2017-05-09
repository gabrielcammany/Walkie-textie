/*
 * File:   TPWM.h
 *
 * TAD que fa encendre un led com si fos un heartbeat. Em serveix per veure si
 * el micro s'ha quedat penjat
 *
 * Created on 25 de enero de 2014, 23:02
 */

#ifndef TPWM_H
#define	TPWM_H
#include <xc.h>
#include "time.h"
#include "api.h"

void PwInit();
//Pre: Requereix del TITITMER.c
//Post: Demana un timer i posa la sortida del LED a 0
void MotorPWM();


#endif	/* LETLEDS_H */

