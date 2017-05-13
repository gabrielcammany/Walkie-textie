//
// Mòdul de gestiò de Timers
// Barsalona, Novembre de 1995, Juan Perez & JM Ribes.
// Desembre de 2001. FEC. Ajustat per al Fujitsu 90583
// Març de 2010. FEC. Ajustat per al PIC24 (com passen els anys...)
// 

#include "time.h"
#include <xc.h>


//
//--------------------------------CONSTANTS---AREA-----------

#define         TI_FALS                         0
#define         TI_CERT                         1

// Tipus imbècils
typedef unsigned char        BYTE;
typedef unsigned short       WORD;

//
//---------------------------End--CONSTANTS---AREA-----------
//

//
//--------------------------------VARIABLES---AREA-----------
//
struct Timer {
	unsigned int h_TicsInicials;
	unsigned char b_busy;
} s_Timers[TI_NUMTIMERS]; 

static unsigned int  h_Tics=0;
static int counter;

//
//---------------------------End--VARIABLES---AREA-----------
//
//
//--------------------------------PRIVADES----AREA-----------
//
void TimerInit(void) {

}

void __attribute__((interrupt, no_auto_psv)) _T1Interrupt(void) { // Cada 1ms, amb un error del 1,74% (segons simulador).
    IFS0bits.T1IF = 0;    //Resetejo el flag de peticio d'interrupcio
	h_Tics++;
	if (h_Tics>=TI_MAXTICS) {
		// Abans que dongui la volta, tots avall (cada 30s)
		for (counter=0;counter<TI_NUMTIMERS;counter++)
			if (s_Timers[counter].b_busy==TI_CERT)
				s_Timers[counter].h_TicsInicials -= h_Tics;
		h_Tics=0;
	}
#ifdef	_TOUCH
	TcRSI();	
#endif

}
//
//---------------------------End--PRIVADES----AREA-----------
//

//
//--------------------------------PUBLIQUES---AREA-----------
//

void TiInit () {
	unsigned char counter;
	for (counter=0;counter<TI_NUMTIMERS;counter++) {
		s_Timers[counter].b_busy=TI_FALS;
	}
	h_Tics=0;
	// Suposo que anem a 8MHz
	T1CONbits.TCKPS1 = 0;
	T1CONbits.TCKPS0 = 1; // Preescaler a 1/8, pols de 2us
	// La resta de valors de T1CON per defecte
//	PR1 = 500;	// 500 * 2us = 1ms 
	PR1 = 4*500;	// 500 * 2us = 1ms , no anem a 8 si nó a 32
	T1CONbits.TON = 1;		// Activo el timer
	// Activo la interrupció del timer 1
	IFS0bits.T1IF = 0;
	IEC0bits.T1IE = 1;
}

int TiGetTimer() {
	unsigned char counter=0;
	while (s_Timers[counter].b_busy==TI_CERT) {
		counter++;
		if (counter == TI_NUMTIMERS) return -1;
	}
	s_Timers[counter].b_busy=TI_CERT;
	return (counter);
}

void TiResetTics (unsigned char Handle) {
	//__DI(); h_Tics integer, per tant, indivisible
	s_Timers[Handle].h_TicsInicials=h_Tics;
	//__EI();
}


unsigned int TiGetTics (unsigned char Handle) {
volatile unsigned int actual;
	actual=h_Tics; // indivisible si és un int, clar.
	return (actual-(s_Timers[Handle].h_TicsInicials));
}

void TiCloseTimer (unsigned char Handle) {
	s_Timers[Handle].b_busy=TI_FALS;
}


void TiEnd () {
}

//
//---------------------------End--PUBLIQUES---AREA-----------
//



