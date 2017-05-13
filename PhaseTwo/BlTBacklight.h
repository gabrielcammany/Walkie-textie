/* 
 * File:   BlTBacklight.h
 * Author: navarrito
 *
 * Created on 3 de febrero de 2014, 12:14
 */

#ifndef BLTBACKLIGHT_H
#define	BLTBACKLIGHT_H

#include "time.h"
#include <xc.h>

#define SET_BL_DIR()  TRISBbits.TRISB14= 0;
#define BL_OFF() LATBbits.LATB14=0;
#define BL_ON() LATBbits.LATB14=1;

#define PERIODBL   20  //Periode del led de backlight
void BlInit(void);
//Post: demana una timer i inicialitza el backlight al 50%

void BlMotor(void);

char getBlDuty(void);
//Post: retorna el valor de duty del backlight (entre 0 i 20)

void setBlDuty(char);
//Pre: duty val entre 0 i 20
//Post: evident




#endif	/* BLTBACKLIGHT_H */

