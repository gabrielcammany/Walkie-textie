#include "BlTBacklight.h"


static unsigned char estat, timer, tempsOn;
void BlInit(void){
    SET_BL_DIR();
    BL_OFF();
    estat = 0;
    timer = TiGetTimer();
    tempsOn=10;
}
//Post: demana una timer i inicialitza el backlight al 50%

void BlMotor(void){
        switch(estat){
        case 0:
            if (TiGetTics(timer)>=tempsOn){
                TiResetTics(timer);
                estat = 1;
                BL_OFF();
            }
            break;
        case 1:
            if (TiGetTics(timer)>= PERIODBL - tempsOn){
                TiResetTics(timer);
                estat = 0;
                BL_ON();
            }
            break;
        }
}

char getBlDuty(void){
//Post: retorna el valor de duty del backlight (entre 0 i 20)
    return tempsOn;
}

void setBlDuty(char nouTemps){
    //Pre: duty val entre 0 i 20
    tempsOn=nouTemps;
}
//Post: evident
