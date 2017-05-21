/*
 * File:   TRF.c
 * Author: Gabriel Cammany
 *
 * Created on 3 de mayo de 2017, 12:48
 */

#include "TPWM.h"

#define MAX_BUFFER 8
#define MAX_MESSAGE 316
#define END_BYTE 0x40
#define START_BYTE 0x40
#define IN PORTBbits.RB13
static unsigned estatRF,input,tramaRebudaPropia[2] = {'0','0'}, 
        tramaRebudaTotals[2] = {'0','0'};
static char timerRF, sincronized;
static int caracter, totalBytesMessage;
static unsigned char pos;
static unsigned char missatge[MAX_MESSAGE],inValue,id_trama[3];

void RFInit(){
    timerRF = TiGetTimer();
    estatRF = 0;
    TRISBbits.TRISB13 = 0;
    sincronized = 0;
    pos = 0;
    totalBytesMessage = 0;
    id_trama[0] = ':';
    id_trama[1] = ':';
    id_trama[2] = ':';
    for(caracter = 0;caracter<16;caracter++){
        missatge[caracter] = ' ';
    }
}



void MotorRF () {
	switch(estatRF) {
	case 0:
		if(IN == 1){
            TiResetTics(timerRF);
            estatRF = 1;
        }
		break;
	case 1:
        if(IN == 0){
            if(TiGetTics(timerRF) > 4){
                estatRF = 2;
                TiResetTics(timerRF);
             }else{
                estatRF = 0;
             }
        }
		break;
	case 2:
		if(IN == 1){
            if(TiGetTics(timerRF) < 5){
                estatRF = 0;
            }else{
                if(TiGetTics(timerRF) < 14){
                    estatRF = 0;
                }else{
                    inValue = 0;
                    pos = 0;
                    TiResetTics(timerRF);
                    estatRF = 3;
                    caracter = 0;
                }
            }
        }
		break;
	case 3:
        if(TiGetTics(timerRF) == 1){
            input = IN;
        }
		if(IN == !input && TiGetTics(timerRF) > 1){
            TiResetTics(timerRF);
            estatRF = 4;
        }
		break;
	case 4:
        if(TiGetTics(timerRF) > 4){ 
            if(pos<7){
                estatRF = 3;
                pos++;
                addBit();
				TiResetTics(timerRF);
            }else{
                if (sincronized == 1){
                    estatRF = 5;
                }else{
                    estatRF = 6;
                }
            }
        }
		break;
    case 5:
        if(inValue == END_BYTE){
            sincronized = 0;
            estatRF = 7;
        }else{
            missatge[caracter] = inValue;
            caracter++;
            estatRF = 3;
        }
        inValue = 0;
        pos = 0;
        addBit();
        TiResetTics(timerRF);
		break;
    case 6:
        if(inValue == START_BYTE){
            sincronized = 2;
            estatRF = 3;
            caracter = 0;
        }else{
            if (caracter < 3 ){
                id_trama[caracter] = inValue;
                caracter++;
                estatRF = 3;
            }else{
                if(getIDPos(0) == id_trama[0] && getIDPos(1) == id_trama[1] && getIDPos(2) == id_trama[2]){
                    sincronized = 1;
                    estatRF = 3;
                    caracter = 16;
                    missatge[caracter] = inValue;
                    caracter++;
                }else{
                    estatRF = 0;
                    incrementa(tramaRebudaTotals);
                }
            }
        }
        inValue = 0;
        pos = 0;
        addBit();
        TiResetTics(timerRF);
		break;
    case 7:
        incrementa(tramaRebudaPropia);
        incrementa(tramaRebudaTotals);
        estatRF = 0;
        startToCount();
        setCadena(2);
        totalBytesMessage = caracter;
        setAudioPeriode((caracter)/30);
        changeAudioStatus();
        break;
    }
}

void incrementa(char *comptador){
    if(comptador[0] == '9'){
       comptador[0] = '0'; 
       comptador[1]++;
    }else{
        comptador[0]++;
    }
}

void addBit(){
    if(input == 1){
        inValue = (inValue >> 1) & 0x7F ;;
    }else{
        inValue = ((inValue | 0x80) >> 1) & 0x7F ;
    }
}

char getTramesTotals(unsigned char pos){
    return tramaRebudaTotals[pos];
}

char getTramesPropies(unsigned char pos){
    return tramaRebudaPropia[pos];
}

char* getMessage(){
    return missatge;
}

int getLength(){
    return totalBytesMessage;
}

char getSincro(){
    return sincronized;
}
