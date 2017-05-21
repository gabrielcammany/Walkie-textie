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
#include "BlTBacklight.h"
#include "SiTSio.h"
#include "PbTPushbutton.h"
#include "AuTAudio.h"
#include "LcTLCD.h"
#include "AdTADC.h"
#include "SwTSwitch.h"
#include "TPWM.h"
#include "TRF.h"


#define PROPAGANDA_1 "\n\rPlaca grup 779. Sistemes Digitals i uProcessadors\r\n\0"
#define PROPAGANDA_2 "Menu de seleccio\r\n\0"
#define MAX_ID 3
#define EXTRA_ID_STRING 3
#define MAXCADENES 3

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

char getIDPos(unsigned char pos);
//Pre: 0 <= pos < MAX_ID_STRING
//Post: Retorna el digit del ID que es demani

void setCadena(unsigned char seleccio);
//Pre: 0 <= seleccio < MAXCADENES
//Post: Retorna el digit del ID que es demani

void startToCount();
//Pre: Timer dels cinc segons demanat
//Post: ResetTics del timer cinc segons

#endif	/* PRTPROPAGANDA_H */

