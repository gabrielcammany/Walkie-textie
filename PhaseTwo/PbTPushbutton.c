
#include "PbTPushbutton.h"
#include "AuTAudio.h"
#include "BNumero.h"
#include "api.h"



static char estat,timer, temp;

void PbInit(void){
    TRISDbits.TRISD7=1;
    TRISDbits.TRISD6=1;
    CNPU2bits.CN16PUE = 1; //posso el pull UP
    CNPU1bits.CN15PUE=1;
    estat = 0;
    timer = TiGetTimer();
}


void MotorPulsador(){
    switch(estat){
        case 0:
            if (getPB1() == 0){
                TiResetTics(timer);
                estat = 1;
            }
            else if (getPB2() == 0){
                TiResetTics(timer);
                estat = 5;
            }
            break;
        case 1:
            //Espero rebots....
            if (TiGetTics(timer) >= 100){
                if (getPB1() == 0) estat = 2;
                else estat = 0;
            }
            break;
        case 2:
            //Han apretat el pulsador de - toca fer feina:
            //Toca reduir la frequència
                temp=getAudioPeriode();
                if (temp == 10) setAudioPeriode(1);
                else setAudioPeriode(++temp);
            
             estat = 3;
            break;
        case 3:
            //Espero a que desapretin...
            //Si apreten dos segons apago l'altaveu si estava encés...
            if (TiGetTics(timer) >= 2000){
                if (getAudioStatus()){
                    changeAudioStatus();
                }
                TiResetTics(timer);
            }
            if (getPB1() == 1){
                estat = 4;
                TiResetTics(timer);
            }
            break;
        case 4: //Rebots de baixada...
            if (TiGetTics(timer)>=100){
                estat = 0;
            }
            break;
        case 5:
            //Espero rebots....
            if (TiGetTics(timer) >= 100){
                if (getPB2() == 0) estat = 6;
                else estat = 0;
            }

            break;
        case 6:
            //Han apretat el pulsador de + toca fer feina
            //Toca reduir la frequencia
                temp=getAudioPeriode();
                if (temp == 1) setAudioPeriode(10);
                else setAudioPeriode(--temp);
           
            
            estat = 7;
            break;
        case 7:
            //Espero a que desapretin...
            //Si apreten més de 2 segons encenc l'altaveu si estava apagat
            if (TiGetTics(timer) >= 2000){
                if (getAudioStatus()==0){
                    changeAudioStatus();
                }
                TiResetTics(timer);
            }
            if (getPB2() == 1){
                estat = 4;
                TiResetTics(timer);
            }
            break;
    }
}
char getPB1(void){
    return  (PORTDbits.RD6 == 1);
}

char getPB2(void){
    return (PORTDbits.RD7 == 1);
}