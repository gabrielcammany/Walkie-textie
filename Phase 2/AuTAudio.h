/* 
 * File:   AuTAudio.h
 * Author: JNM
 *
 * Created on 26 de enero de 2014, 23:20
 */

#ifndef AUTAUDIO_H
#define	AUTAUDIO_H
//#include "time.h"
#include "api.h"



#define SET_AUDIO_DIR()     TRISBbits.TRISB14 = 0;
#define AUDIO_ON()          LATBbits.LATB14=1;
#define AUDIO_OFF()         LATBbits.LATB14=0;

void AuInit(void);
//Posa el port RA1 de sortida i demana un timer

void MotorAudio(void);

char changeAudioStatus();
    //Post Canvia l'estat d'audio

char getAudioStatus(void);
    //Post: retorna 1 si audio ences o 0 si apagat


char getAudioPeriode(void);
// Post: Retorna el valor del periode

void setAudioPeriode(char nouPeriode);
// Pre: nouPeriode >= 1



#endif	/* AUTAUDIO_H */
