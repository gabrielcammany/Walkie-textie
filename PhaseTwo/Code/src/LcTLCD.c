//
// TAD per a manipular un display alfanumÀric basat en el
// controlador HD44780. Aquest »s el controlador que porten la
// immensa majoria de displays. Tamany m?xim, 4 files per 40 columnes
//
// Durant el proc»s d'inicialitzaci€, es demanar? un timer al tad timer
// que no s'usar? per a res m»s. El constructor alliberar? aquest timer.

// Vcli V1.1, Sisco, a 26 de novembre de 2004. He vist que en alguns LCDs cal esperar
// despr»s de fer un Clear, amb independÀncia del que digui el Busy.
// Sembla extrany, quan tingui temps haig de mirar el bit de Busy amb l'oscil.loscop
// De moment, rertardo amb timer.

// VCli V1.3, jnavarro, a 2013. He ampliat els temps d'inicialitzaci€ (ara trigo uns
// 150 ms. per⁄ inicialitzo a la primera (sobretot en cold starts). M»s info aquÃ:
// http://web.alfredstate.edu/weimandn/lcd/lcd_initialization/lcd_initialization_index.html
// Segueixo observant
// la mateixa anomalia del Busy, menys mal del timeout perquÀ sempre que li ho pregunto
// el fastig€s busy est? a 1...

#include <xc.h>
#include "time.h"
#include "LcTLCD.h"


//
//--------------------------------CONSTANTS---AREA-----------
//
#define FUNCTION_SET	0x20
#define BITS_8			0x10
#define DISPLAY_CONTROL	0x08
#define DISPLAY_ON		0x04
#define CURSOR_ON		0x02
#define DISPLAY_CLEAR	0x01
#define ENTRY_MODE		0x04
#define SET_DDRAM		0x80
//
//---------------------------End--CONSTANTS---AREA-----------
//


//
//--------------------------------VARIABLES---AREA-----------
//
static unsigned char Files, Columnes;
static unsigned char FilaAct, ColumnaAct;
static int Timer;
//
//---------------------------End--VARIABLES---AREA-----------
//

//
//--------------------------------PROTOTIPUS--AREA-----------
//
void Espera(int Timer, int ms);
void CantaIR(char IR);
void CantaData(char Data);
void WaitForBusy(void);
void EscriuPrimeraOrdre(char);

//
//---------------------------End--PROTOTIPUS--AREA-----------
//


//
//--------------------------------PUBLIQUES---AREA-----------
//
void LcInit(char files, char columnes) {
// Pre: Files = {1, 2, 4}  Columnes = {8, 16, 20, 24, 32, 40 }
// Pre: L'Hitachi merd€s necessita 40ms de tranquilitat desde
// la pujada de Vcc fins cridar aquest constructor
// Pre: Hi ha un timer lliure
// Post: Aquesta rutina pot trigar fins a 100ms
// Post: El display queda esborrat, el cursor apagat i a la
// posici€ 0, 0.
	int i;
	Timer = TiGetTimer(); // Val m»s que n'hi hagi
	Files = files; Columnes = columnes;
	FilaAct = ColumnaAct = 0;
	SetControlsSortida();
	for (i = 0; i < 2; i++) {
		Espera(Timer, 100);
		// El manual de l'hitachi diu que s'ha de fer aquesta
		// seq∏Ància...

		EscriuPrimeraOrdre(CURSOR_ON | DISPLAY_CLEAR);
		Espera(Timer, 5);
		EscriuPrimeraOrdre(CURSOR_ON | DISPLAY_CLEAR);
		Espera(Timer, 1);
		EscriuPrimeraOrdre(CURSOR_ON | DISPLAY_CLEAR);
		Espera(Timer, 1);
		// .. tres vegades. Potser »s una penitÀncia?
		// Ara una a 4 bits
		EscriuPrimeraOrdre(CURSOR_ON);
		Espera(Timer, 1);
		CantaIR(FUNCTION_SET | DISPLAY_CONTROL); 	// 4bits, 1 files, font 5x7
		// Observo que just aquÃ s'esborra la primera linia
		// Ara ja podem mirar el busy
		WaitForBusy(); 	CantaIR(DISPLAY_CONTROL);  	// Display Off
		WaitForBusy(); 	CantaIR(DISPLAY_CLEAR);	   	// Tot espais
		Espera(Timer,3); // 1.64ms V1.1
		WaitForBusy(); 	CantaIR(DISPLAY_ON | CURSOR_ON); // Auto Increment i shift
		WaitForBusy(); 	CantaIR(DISPLAY_CONTROL | DISPLAY_ON | CURSOR_ON | DISPLAY_CLEAR); 		// Display On
	}
	// El manual dir? el que vulgui per⁄ jo he vist que despr»s
	// d'una arrencada freda, no sempre s'inicialitza correctament
	// tot i esperar els 40ms. Per aix⁄, he posat un bucle de dues
	// inicialitzacions i a partir d'aquÃ, la inicialitzaci€ m'ha funcionat
	// correctament tant si faig un reset com si apago i engego l'alimentaci€.
}

void LcEnd(void) {
// El Destructor
	TiCloseTimer (Timer); // Ja no ens far? cap falta
}

void LcClear(void) {
// Post: Esborra el display i posa el cursor a la posici€ zero en
// l'estat en el que estava.
// Post: La propera ordre pot trigar fins a 1.6ms
	WaitForBusy(); 	CantaIR(DISPLAY_CLEAR);	   // Tot espais
	Espera(Timer, 3); // V1.1
}

void LcCursorOn(void) {
// Post: Enc»n el cursor
// Post: La propera ordre pot trigar fins a 40us
	WaitForBusy();
	CantaIR(DISPLAY_CONTROL | DISPLAY_ON | CURSOR_ON);
}

void LcCursorOff(void) {
// Post: Apaga 0el cursor
// Post: La propera ordre pot trigar fins a 40us
	WaitForBusy();
	CantaIR(DISPLAY_CONTROL | DISPLAY_ON);
}

void LcGotoXY(char Columna, char Fila) {
// Pre : Columna entre 0 i 39, Fila entre 0 i 3
// Post: Posiciona el cursor en aquestes coordenades
// Post: La propera ordre pot trigar fins a 40us
	int Fisica;
	// d'entrada calculo la direcci€ efectiva de la ram del LCD
	switch (Files) {
		case 2:
			Fisica = Columna + (!Fila ? 0 : 0x40); break;
		case 4:
			Fisica = Columna;
			if (Fila == 1) Fisica += 0x40; else
			if (Fila == 2) Fisica += Columnes;      /* 0x14; */ else
			if (Fila == 3) Fisica += 0x40+Columnes; /* 0x54; */
			break;
		case 1:
		default:
			Fisica = Columna; break;
	}
	// i l'aplico amb la comanda
	WaitForBusy();
	CantaIR(SET_DDRAM | Fisica);
	// Finalment, actualitzo les imatges locals
	FilaAct    = Fila;
	ColumnaAct = Columna;
}

void LcPutChar(char c) {
// Post: Pinta C en l'actual poscici€ del cursor i incrementa
// la seva posici€. Si la columna arriba a 39, salta a 0 tot
// incrementant la fila si el LCD »s de dues files.
// Si es de 4 files, incrementa de fila en arribar a la columna 20
// AixÃ mateix, la fila 4 passa a la zero.
// En els LCDs d'una fila, quan la columna arriba a 39, torna
// a zero. No s'incrementa mai la fila
	// D'entrada escric el char
	WaitForBusy(); CantaData(c);
	// i ara recalculo la posici€ del cursor
	++ColumnaAct;
	if (Files == 3) {
		if (ColumnaAct >= 20) {
			ColumnaAct = 0;
			if (++FilaAct >= 4) FilaAct = 0;
			LcGotoXY(ColumnaAct, FilaAct);
		}
	} else
	if (Files == 2) {
		if (ColumnaAct >= 40) {
			ColumnaAct = 0;
			if (++FilaAct >= 2) FilaAct = 0;
			LcGotoXY(ColumnaAct, FilaAct);
		}
	} else
	if (FilaAct == 1) {
		if (ColumnaAct >= 40) ColumnaAct = 0;
		LcGotoXY(ColumnaAct, FilaAct);
	}
}


void LcPutString(char *s) {
// Post: Pinta l'string a apartir de la posici€ actual del cursor.
// El criteri de coordenades »s el mateix que a LcPutChar
// Post: Pot trigar fins a 40us pel nombre de chars de s a sortir de
// la rutina
	while(*s) LcPutChar(*s++);
}

//
//---------------------------End--PUBLIQUES---AREA-----------
//

//
//--------------------------------PRIVADES----AREA-----------
//

void Espera(int Timer, int ms) {
	TiResetTics(Timer);
	while(TiGetTics(Timer) < ms);
}

void CantaPartAlta(char c) {
	 SetD7(c & 0x80 ? 1 : 0);
	 SetD6(c & 0x40 ? 1 : 0);
	 SetD5(c & 0x20 ? 1 : 0);
	 SetD4(c & 0x10 ? 1 : 0);
}

void CantaPartBaixa(char c) {
	 SetD7(c & 0x08 ? 1 : 0);
	 SetD6(c & 0x04 ? 1 : 0);
	 SetD5(c & 0x02 ? 1 : 0);
	 SetD4(c & 0x01 ? 1 : 0);
}

void CantaIR(char IR) {
	SetD4_D7Sortida();
	RSDown();
	RWDown();
	EnableUp();
	CantaPartAlta(IR); 		// Segons hitachi Data Setup = 80ns (cap problema)
	EnableUp();				// Asseguro els 500ns de durada de pols
	EnableDown();   		// i l'amplada del pols "enable" major que 230n
	EnableDown();
	EnableUp();
	CantaPartBaixa(IR); 	// Segons hitachi Data Setup = 80ns (cap problema)
	EnableUp();				// Asseguro els 500ns de durada de pols
	EnableDown();   		// i l'amplada del pols "enable" major que 230n
	SetD4_D7Entrada();
}

void CantaData(char Data) {
	SetD4_D7Sortida();
	RSUp();
	RWDown();
	EnableUp();
	CantaPartAlta(Data); 	// Segons hitachi Data Setup = 80ns (cap problema)
	EnableUp();				// Asseguro els 500ns de durada de pols
	EnableDown();   		// i l'amplada del pols "enable" major que 230n
	EnableDown();
	EnableUp();
	CantaPartBaixa(Data); 	// Segons hitachi Data Setup = 80ns (cap problema)
	EnableUp();				// Asseguro els 500ns de durada de pols
	EnableDown();   		// i l'amplada del pols "enable" major que 230n
	SetD4_D7Entrada();
}

void WaitForBusy(void) { char Busy;
	SetD4_D7Entrada();
	RSDown();
	RWUp();
	TiResetTics(Timer);
	do {
		EnableUp();EnableUp(); // Asseguro els 500ns de durada de pols
		Busy = GetBusyFlag();
		EnableDown();
		EnableDown();
		EnableUp();EnableUp();
		// AquÃ ve la part baixa del comptador d'adreces, que no ens interessa
		EnableDown();
		EnableDown();
		if (TiGetTics(Timer)) break; // M»s d'un ms vol dir quel LCD est? boig
	} while(Busy);
}

void EscriuPrimeraOrdre(char ordre) {
	// Escric el primer com si fossin 8 bits
	SetD4_D7Sortida();  RSDown(); RWDown();
	EnableUp(); EnableUp();
	 SetD7(ordre & 0x08 ? 1 : 0);
	 SetD6(ordre & 0x04 ? 1 : 0);
	 SetD5(ordre & 0x02 ? 1 : 0);
	 SetD4(ordre & 0x01 ? 1 : 0);
	EnableDown();
}

//
//---------------------------End--PRIVADES----AREA-----------
//



