/* 
 * File:   main.c
 * Author: 
 *
 * Created on 24 de noviembre de 2013, 19:08
 */

#include "api.h"


void init()
{
    TiInit();
    i2Init();
    AuInit();
    LcInit(4, 16); // L'inicialització no fa servir el motor I2c
    SiInit();
    LeInit();
    BNinit();
    initPropaganda();
    PbInit();
    AdInit();
    initMotorLCD();

}

void motors(){

    MotorAudio();
    motorBNumero(); // gestiona el polsador del mode
    MotorPropaganda();
    MotorLed();
    MotorSIO();
    MotorPulsador();
    LcMotor(0); // Alerta!, no barrejar mode coperatiu i no coperatiu, ja que
                // Les ordres del I2c sens desordenarien i seria un pitate.
    MotorI2C();
    MotorLCD();

}

            
    

int main(){

    init();

    while (1)
    {
         motors();
        ClrWdt();
        
    }
    return 0;
}

