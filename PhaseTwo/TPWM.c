/*
 * File:   TPWM.c
 * Author: Gabriel Cammany
 *
 * Created on 3 de mayo de 2017, 12:48
 */

#include "TPWM.h"

#define MAX_ID 3
#define MAX_PWM 3
#define PWM0 LATBbits.LATB10 
#define PWM1 LATBbits.LATB11 
#define PWM2 LATBbits.LATB12

static char estatPWM;
static char timerPWM, temps;
static char PWM[MAX_ID];

void PwInit(){
    timerPWM = TiGetTimer();
    estatPWM = 0;
    PWM[0] = '0';
    PWM[1] = '0'; 
    PWM[2] = '0';
    TRISBbits.TRISB10 = 0;
    TRISBbits.TRISB11 = 0;
    TRISBbits.TRISB12 = 0;
}

void incrementaPWM(unsigned char quin){
    //Pre: 0<= quin < MAX_PWM
    //Post: Incrementa el PWM si es diferent al ID, en cas contrari 
    //no el modifica
    PWM[quin] = ((PWM[quin] != getIDPos(quin)) ? (PWM[quin] + 1) : PWM[quin]);
}


void changePWM(unsigned char quin){
    //Pre: 0<= quin < MAX_PWM
    //Post: Posa a 1 o 0 el PWM especificat, depenent del temps que estigui
    switch (quin){
    case 0:
        PWM0 = (PWM[quin] >= temps ? 1 : 0);
        break;
    case 1:
        PWM1 = (PWM[quin] >= temps ? 1 : 0);
        break;
    case 2:
        PWM2 = (PWM[quin] >= temps ? 1 : 0);
        break;
    }
}


void MotorPWM (unsigned char quin) {
	switch(estatPWM) {
	case 0:
		PWM[quin] = (PWM[quin]<':' ? PWM[quin] : '0');
		if(quin == ((MAX_PWM)-1)){
            estatPWM = 1;
            TiResetTics(timerPWM);
        }
		break;
	case 1:
		if(TiGetTics(timerPWM)>=20){
            incrementaPWM(quin);
            if(quin == ((MAX_PWM)-1))estatPWM = 0;
		}else{
            temps = '9' - (TiGetTics(timerPWM) >> 1);
            changePWM(quin);
		}
		break;
	}
}
