#include "PbTPushbutton.h"
#include "AuTAudio.h"
#include "SwTSwitch.h"



static char estat,timer, temp;

void PbInit(void){
    TRISAbits.TRISA3 = 1;
    TRISAbits.TRISA2 = 1;
    CNPU2bits.CN30PUE = 1;  //RA2
    CNPU2bits.CN29PUE = 1;  //RA3
    estat = 0;
    timer = TiGetTimer();
}


void MotorPulsador(){
    switch(estat){
        case 0:
            if (PORTAbits.RA3 == 0){
                TiResetTics(timer);
                estat = 1;
            }
            else if (PORTAbits.RA2 == 0){
                TiResetTics(timer);
                estat = 5;
            }
            break;
        case 1:
            //Espero rebots....
            if (TiGetTics(timer) >= 100){
                if (PORTAbits.RA3 == 0) estat = 2;
                else estat = 0;
            }
            break;
        case 2:
            //Han apretat el pulsador de - toca fer feina:
            if (getSwitch1()== 0) {
            //Toca reduir la frequencia
                temp=getAudioPeriode();
                if (temp == 10) setAudioPeriode(1);
                else setAudioPeriode(++temp);
            }
            if (getSwitch2() == 0){
                //toca reduïr el BL
                temp = getBlDuty();
                if (temp == 0) setBlDuty(PERIODBL);
                else setBlDuty(temp -STEPBL);
            }
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
            if (PORTAbits.RA3 == 1){
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
                if (PORTAbits.RA2 == 0) estat = 6;
                else estat = 0;
            }
            break;
        case 6:
            //Han apretat el pulsador de + toca fer feina
            if (getSwitch1()== 0) {
            //Toca reduir la frequencia
                temp=getAudioPeriode();
                if (temp == 1) setAudioPeriode(10);
                else setAudioPeriode(--temp);
            }
            if (getSwitch2() == 0){
                //reduïr el BL
                temp = getBlDuty();
                if (temp == PERIODBL) setBlDuty(0);
                else setBlDuty(temp +STEPBL);
            }
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
            if (PORTAbits.RA2 == 1){
                estat = 4;
                TiResetTics(timer);
            }
            break;
    }
}
char getPB1(void){
    return  (PORTAbits.RA3 == 1);
}

char getPB2(void){
    return (PORTAbits.RA2 == 1);
}