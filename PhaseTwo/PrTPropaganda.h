/* 
 * File:   PrTPropaganda.h
 * Author: JNM
 *
 * Placa de test per al PIC18F4321 controla el backlight del LCD, té un menú
 * interactiu pel SIO, i refresca l'estat dels perifèrics (2 pulsadors, 2 switchos,
 * 1 entrada analògica)
 * 
 */





#ifndef PRTPROPAGANDA_H
#define	PRTPROPAGANDA_H
#include <xc.h>
#include "SiTSio.h"
#include "PbTPushbutton.h"
#include "AuTAudio.h"
#include "LcTLCD.h"
#include "AdTADC.h"
#include "TITTIMER.h"
#include "TPWM.h"


#define PROPAGANDA_1 "\n\rPlaca LS69. Sistemes Digitals i uProcessadors\r\n\0"
#define PROPAGANDA_2 "vCli v1.0. Programa de test\r\n\0"


void myItoa(int num);
//Pre: 0<= num <= 9999
//Post: deixa a temp[3..0] el num en ASCII
void Menu(void);
//Pre: La SIO està inicialitzada
//Post: Pinta el menu pel canal sèrie

void initPropaganda(void);
//Pre: La SIO està inicialitzada
//Post: Inicialitza el timestamp i pinta la propaganda per la SIO

void MotorPropaganda(void);
void initMotorLCD(void);
//Pre: el LCD està inicialitzat
//Post: inicialitza el LCD per posar la marquesina a 0
void MotorLCD(void);

char getIDPos(char pos);
#endif	/* PRTPROPAGANDA_H */

