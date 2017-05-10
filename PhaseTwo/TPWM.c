/*
 * File:   TPWM.c
 * Author: gabri
 *
 * Created on 3 de mayo de 2017, 12:48
 */

#include "TPWM.h"

#define MAX_ID    3
static unsigned char estatPWM;
static char timerPWM,temps;
static unsigned char PWM[MAX_ID];

void PwInit(){
    timerPWM = TiGetTimer();
    estatPWM = 0;
    PWM[0] = PWM[1] = PWM[2]
    TRISDbits.TRISD5 = 0;
    TRISDbits.TRISD6 = 0;
    TRISDbits.TRISD7 = 0;
    temps = 0;
}

void MotorPWM () {
	switch(estatPWM) {
	case 0:
		PWM[0] = (PWM[0]<':' ? PWM[0] : '0');
		PWM[1] = (PWM[0]<':' ? PWM[1] : '0');
		PWM[2] = (PWM[0]<':' ? PWM[2] : '0');
		TiResetTics(timerPWM);
		estatPWM = 1;
		break;
	case 1:
		temps = '9' - (TiGetTics(timerPWM)/2);
		if(TiGetTics(timerPWM)>20){
			PWM[0] = (PWM[0]!=getIDPos(0) ? PWM[0]++ : PWM[0]);
			PWM[1] = (PWM[1]!=getIDPos(1) ? PWM[1]++ : PWM[1]);
			PWM[2] = (PWM[2]!=getIDPos(2) ? PWM[2]++ : PWM[2]);
			estatPWM = 0;
		}else{
			LATDbits.LATD5 = (PWM[0]>=temps ? 1 : 0);
			LATDbits.LATD6 = (PWM[1]>=temps ? 1 : 0);
			LATDbits.LATD7 = (PWM[2]>=temps ? 1 : 0);
		}
		break;
	}
}

char getTemps(){
    return temps;
}
