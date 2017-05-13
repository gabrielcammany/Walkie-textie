/* 
 * File:   SwTSwitch.h
 * Author: JNM
 *
 * Created on 26 de enero de 2014, 23:57
 */

#ifndef SWTSWITCH_H
#define	SWTSWITCH_H

#include <xc.h>
void SwInit();
//Post: Posa els pins RB13 i RB12 d'entrada i activa els pull-ups interns

char getSwitch1();
//Post: retorna el valor del Switch 1 (1 o 0)

char getSwitch2();
//Post: retorna el valor del Switch 2 (1 o 0)
#endif	/* SWTSWITCH_H */

