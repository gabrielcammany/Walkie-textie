/* 
 * File:   LeTLeds.h
 * Author: JNM
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


#define PERIOD  20  //Periode del hearbeat
#define STEP    4   //Nombre de períodes abans d'incrementar el tempsOn
//Compte, PERIOD ha de ser multiple de STEP, si no, no se què passarà, ja ho miraré...

#define SET_LED_DIR()  TRISAbits.TRISA4=0;
#define LED_OFF() LATAbits.LATA4=0;
#define LED_ON() LATAbits.LATA4=1;

void LeInit();
//Post: Demana un timer i posa la sortida del LED a 0
void MotorLed();


#endif	/* LETLEDS_H */

