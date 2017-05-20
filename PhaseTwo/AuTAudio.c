#include "AuTAudio.h"
#include <xc.h>
static char timerAudio, estat,periode[4]={1,1,1,1},frequencia;

void AuInit(){
    SET_AUDIO_DIR();
    AUDIO_OFF();
    timerAudio = TiGetTimer();
    estat = 2;
    frequencia = 0;
}

char getAudioPeriode(void){
    return periode;
}

void setAudioPeriode(char nouPeriode){
    periode[0] = nouPeriode;
    periode[1] = nouPeriode+1;
    periode[2] = nouPeriode+2;
    periode[3] = nouPeriode+3;

}

void turnOffAudio(){
    estat = 2;
}

char getAudioStatus(void){
    return estat != 2;
}

void MotorAudio(){
    switch(estat){
        case 0:
            if (TiGetTics(timerAudio)>=periode[frequencia]){
                TiResetTics(timerAudio);
                AUDIO_ON();
                estat = 1;
            }
            break;
        case 1:
            if (TiGetTics(timerAudio)>=periode[frequencia]){
                TiResetTics(timerAudio);
                AUDIO_OFF();
                estat = 0;
            }
            break;
        case 2:
            //Callo
            break;
    }
}


char changeAudioStatus(){
    //Post Canvia l'estat d'audio
    if (estat == 2){
        //estic callat, toca parlar!
        estat = 0;
    }else{
        //Estic parlant, callo!
        estat = 2;
        AUDIO_OFF();
    }
    return estat == 0;
}

void seguentFrequencia(){
    if(frequencia == 3){
        frequencia = 0;
    }else{
        frequencia++;
    }
}