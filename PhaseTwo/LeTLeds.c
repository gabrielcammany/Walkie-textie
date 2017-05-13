#include "LeTLeds.h"



static unsigned char timerLed;
static unsigned char estatLed;
static unsigned char tempsOn;
static unsigned int heartBeat;

void LeInit(){
    SET_LED_DIR();
    LED_OFF();
    timerLed = TiGetTimer();
    estatLed = 0;
    tempsOn = 0;
    heartBeat = 0;
}




void MotorLed(){
    switch(estatLed){
        case 0:
            if (TiGetTics(timerLed)>=tempsOn){
                TiResetTics(timerLed);
                estatLed = 1;
                LED_OFF();
            }
            break;
        case 1:
            if (TiGetTics(timerLed)>=PERIOD - tempsOn){
                TiResetTics(timerLed);
                estatLed = 0;
                LED_ON();
                heartBeat++;
                if (heartBeat == STEP){
                    tempsOn++;
                    heartBeat = 0;
                    if (tempsOn == PERIOD){
                        estatLed = 2;
                    }
                }
            }
            break;

        case 2:
            if (TiGetTics(timerLed) >= tempsOn){
                TiResetTics(timerLed);
                estatLed = 3;
                LED_OFF();

            }
            break;
        case 3:
            if (TiGetTics(timerLed)>=PERIOD - tempsOn){
                TiResetTics(timerLed);
                estatLed = 2;
                LED_ON();
                heartBeat++;
                if (heartBeat == STEP){
                    tempsOn--;
                    heartBeat = 0;
                    if (tempsOn == 0){
                        estatLed = 0;
                        tempsOn = 0;
                    }
                }
            }
            break;
    }
}
