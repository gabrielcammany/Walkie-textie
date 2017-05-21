/* 
 * File:   AuTAudio.h
 * Author: JNM
 *
 * Created on 26 de enero de 2014, 23:20
 */

#ifndef AUTAUDIO_H
#define	AUTAUDIO_H
#include "time.h"



#define SET_AUDIO_DIR()     TRISAbits.TRISA1 = 0;
#define AUDIO_ON()          LATAbits.LATA1=1;
#define AUDIO_OFF()         LATAbits.LATA1=0;
#define MAX_PERIODES        4

void AuInit(void);
//Posa el port RA1 de sortida i demana un timer

void MotorAudio(void);

char changeAudioStatus();
    //Post Canvia l'estat d'audio

char getAudioStatus(void);
    //Post: retorna 1 si audio ences o 0 si apagat


unsigned char* getAudioPeriode(void);
// Post: Retorna l'array de periodes

void setAudioPeriode(char nouPeriode);
// Pre: nouPeriode >= 1

void turnOffAudio();
//Post: Apaga el Altaveu

void turnOnAudio();
//Post: Encent l'altaveu

void seguentFrequencia();
//Post: Canvia el periode del altaveu amb una desviació maxima del 
//original de MAX_PERIODES


#endif	/* AUTAUDIO_H */
