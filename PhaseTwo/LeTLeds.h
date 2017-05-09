/*
 * File:   LeTLeds.h
 *
 * TAD que fa encendre un led com si fos un heartbeat. Em serveix per veure si
 * el micro s'ha quedat penjat
 *
 * Created on 25 de enero de 2014, 23:02
 */

#ifndef LETLEDS_H
#define	LETLEDS_H
#include <xc.h>
#include "time.h"
#include "api.h"

#define PERIOD  20  //Període del hearbeat
#define STEP    4   //Nombre de períodes abans d'incrementar el tempsOn
//Compte, PERIOD ha de ser multiple de STEP, si no, no se què passar?, ja ho miraré...

//PIN 52 de LSMaker->LEDS Placa Base
#define SET_LED_DIR()  TRISDbits.TRISD4=0;
#define LED_OFF() LATDbits.LATD4=0;
#define LED_ON() LATDbits.LATD4=1;

void LeInit();
//Pre: Requereix del TITITMER.c
//Post: Demana un timer i posa la sortida del LED a 0
void MotorLed();


#endif	/* LETLEDS_H */

