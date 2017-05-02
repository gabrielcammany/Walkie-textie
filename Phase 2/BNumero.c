#include "api.h"
#include "BNumero.h"
#include "TITTIMER.h"

#define NUMMAXIM 9

static char num;
static char estat;
static char timer;

void BNinit(){
	num=0;
	estat=0;
        TRISDbits.TRISD5;
	timer = TiGetTimer(); // per controlar els rebots
	CNPU2bits.CN16PUE = 1; //posso el pull-up
        CNPU1bits.CN14PUE=1;
}

void motorBNumero(){
	 switch(estat){
        case 0:
            if (PORTDbits.RD5 == 0){
                TiResetTics(timer);
                estat = 1;
            }
            break;
        case 1:
            //Espero rebots....
            if (TiGetTics(timer) >= 100){
                if (PORTDbits.RD5 == 0) estat = 2;
                else estat = 0;
            }
            break;
        case 2:
            //Han apretat el pulsador
        	num++;
        	if (num>NUMMAXIM) num=0;
            estat = 3;
            break;
        case 3:

            if (PORTDbits.RD5 == 1){
                estat = 4;
                TiResetTics(timer);
            }
            break;
        case 4: //Rebots de baixada...
            if (TiGetTics(timer)>=100){
                estat = 0;
            }
            break;
    }
}

char getNumero(){

    return num;
}

char resetNumero(){

    num=0;
    return 0;
}
