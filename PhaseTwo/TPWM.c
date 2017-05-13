/*
 * File:   TPWM.c
 * Author: gabri
 *
 * Created on 3 de mayo de 2017, 12:48
 */

#include "TPWM.h"

#define MAX_ID 3
static char estatPWM;
static char timerPWM,temps;
static unsigned char PWM[MAX_ID];

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

void MotorPWM () {
	switch(estatPWM) {
	case 0:
		PWM[0] = (PWM[0]<':' ? PWM[0] : '0');
		PWM[1] = (PWM[1]<':' ? PWM[1] : '0');
		PWM[2] = (PWM[2]<':' ? PWM[2] : '0');
		TiResetTics(timerPWM);
		estatPWM = 1;
		break;
	case 1:
		temps = '9' - (TiGetTics(timerPWM) >> 1);
		if(TiGetTics(timerPWM)>=20){
			PWM[0] = ((PWM[0] != getIDPos(0)) ? (PWM[0] = PWM[0] + 1) : PWM[0]);
			PWM[1] = ((PWM[1] != getIDPos(1)) ? (PWM[1] = PWM[1] + 1) : PWM[1]);
			PWM[2] = ((PWM[2] != getIDPos(2)) ? (PWM[2] = PWM[2] + 1) : PWM[2]);
			estatPWM = 0;
		}else{
			LATBbits.LATB10 = (PWM[0] >= temps ? 1 : 0);
			LATBbits.LATB11 = (PWM[1] >= temps ? 1 : 0);
			LATBbits.LATB12 = (PWM[2] >= temps ? 1 : 0);
		}
		break;
	}
}

char getTemps(){
    return temps;
}
