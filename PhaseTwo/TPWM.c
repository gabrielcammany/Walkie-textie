/*
 * File:   TPWM.c
 * Author: Gabriel Cammany
 *
 * Created on 3 de mayo de 2017, 12:48
 */

#include "TPWM.h"

#define MAX_ID 3
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

void MotorPWM (int quin) {
	switch(estatPWM) {
	case 0:
		PWM[quin] = (PWM[quin]<':' ? PWM[quin] : '0');
		if(quin == ((MAX_ID)-1)){
            estatPWM = 1;
            TiResetTics(timerPWM);
        }
		break;
	case 1:
		if(TiGetTics(timerPWM)>=20){
            estatPWM = 2;
		}else{
            temps = '9' - (TiGetTics(timerPWM) >> 1);
            switch (quin){
            case 0:
                estatPWM = 3;
                break;
            case 1:
                estatPWM = 4;
                break;
            case 2:
                estatPWM = 5;
                break;
            }
		}
		break;
    case 2:
        PWM[quin] = ((PWM[quin] != getIDPos(quin)) ? (PWM[quin] + 1) : PWM[quin]);
        if(quin == ((MAX_ID)-1)){
            estatPWM = 0;
        }
		break; 
    case 3:
        LATBbits.LATB10 = (PWM[0] >= temps ? 1 : 0);
        estatPWM = 1;
        break;
    case 4:
        LATBbits.LATB11 = (PWM[1] >= temps ? 1 : 0);
        estatPWM = 1;
		break;
    case 5:
        LATBbits.LATB12 = (PWM[2] >= temps ? 1 : 0);
        estatPWM = 1;
		break;
	}
}

