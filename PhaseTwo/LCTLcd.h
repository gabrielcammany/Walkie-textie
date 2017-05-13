#ifndef LCTLCD_H
#define	LCTLCD_H
//
// TAD per a manipular un display alfanumÀric basat en el
// controlador HD44780 usant nom»s 4 bits de dades.
// Aquest »s el controlador que porten la  immensa majoria de displays.
// Tamany m?xim, 4 files per 40 columnes
//
// F. Escudero vCli v1.0 Piera, gener de 2004
//
// He provat aquest TAD amb un LCD de 2x16. Qualsevol error que observeu,
// us agra‘ria que m'ho f»ssiu saber a sisco@salleurl.edu.
//
// Vcli V1.1, Sisco, a 26 de novembre de 2004. He vist que en alguns LCDs cal esperar 2ms
// despr»s de fer un Clear, amb independÀncia del que digui el Busy.
// Sembla extrany, quan tingui temps haig de mirar el bit de Busy amb l'oscil.loscop
// De moment, retardo amb timer.
//
// VCli V1.3, jnavarro, a 2013. He ampliat els temps d'inicialitzaciÛ (ara trigo uns
// 150 ms. perÚ inicialitzo a la primera (sobretot en cold starts). MÈs info aquÃ:
// http://web.alfredstate.edu/weimandn/lcd/lcd_initialization/lcd_initialization_index.html
// Segueixo observant
// la mateixa anomalia del Busy, menys mal del timeout perquË sempre que li ho pregunto
// el fastigÚs busy est‡ 1...

//
// Durant el proc»s d'inicialitzaciÛ, es demanar? un timer al tad timer
//
// ------------------------------------HARDWARE---AREA--------------------
// La connexiÛ ser‡ de 4 bits de dades (D4 a D7), i els senyals
// RS, R/W i E.
// En aquest exemple, tenim connectat
//
// 	RS				a RB3
//	R/!W                            a RB15
//      E				a RB5
//	D4				a RB6
//	D5				a RB7
//	D6				a RB8
//	D7				a RB9
//  ?s important que els senyals D0..D3 del LCD no es deixin a l'aire i
//  es connectin a GND mitjan¡ant resistÀncies de, per exemple, 4K7
//
// 	Es pot usar qualsevol altre configuraci€. Nom»s
//  cal ajustar els defines que venen a continuaci€.
//

#include <xc.h>


#define SetD4_D7Sortida()		(TRISBbits.TRISB6 = TRISBbits.TRISB7 = TRISBbits.TRISB8 = TRISBbits.TRISB9 = 0)
#define SetD4_D7Entrada()		(TRISBbits.TRISB6 = TRISBbits.TRISB7 = TRISBbits.TRISB8 = TRISBbits.TRISB9 = 1)
#define SetControlsSortida()            (TRISBbits.TRISB3 = TRISBbits.TRISB15 = TRISBbits.TRISB5 = 0)
#define SetD4(On)				(LATBbits.LATB6 = (On))
#define SetD5(On)				(LATBbits.LATB7 = (On))
#define SetD6(On)				(LATBbits.LATB8 = (On))
#define SetD7(On)				(LATBbits.LATB9 = (On))
#define GetBusyFlag()                           (PORTBbits.RB9)
#define RSUp()					(LATBbits.LATB3 = 1)
#define RSDown()				(LATBbits.LATB3 = 0)
#define RWUp()					(LATBbits.LATB15 = 1)
#define RWDown()				(LATBbits.LATB15 = 0)
#define EnableUp()				(LATBbits.LATB5 = 1)
#define EnableDown()                            (LATBbits.LATB5 = 0)
// -------------------------------END--HARDWARE---AREA--------------------


void LcInit(char Files, char Columnes);
// Pre: Files = {1, 2, 4}  Columnes = {8, 16, 20, 24, 32, 40 }
// Pre: Hi ha un timer lliure
// Post: L'Hitachi merd€s necessita 40ms de tranquilitat desde
// la pujada de Vcc fins cridar aquest constructor
// Post: Aquesta rutina pot trigar fins a 100ms
// Post: El display queda esborrat, el cursor apagat i a la
// posici€ 0, 0.

void LcEnd(void);
// El Destructor

void LcClear(void);
// Post: Esborra el display i posa el cursor a la posici€ zero en
// l'estat en el que estava.
// Post: La propera ordre pot trigar fins a 1.6ms

void LcCursorOn(void);
// Post: Enc»n el cursor
// Post: La propera ordre pot trigar fins a 40us

void LcCursorOff(void);
// Post: Apaga 0el cursor
// Post: La propera ordre pot trigar fins a 40us

void LcGotoXY(char Columna, char Fila);
// Pre : Columna entre 0 i 39, Fila entre 0 i 3
// Post: Posiciona el cursor en aquestes coordenades
// Post: La propera ordre pot trigar fins a 40us

void LcPutChar(char c);
// Post: Pinta C en l'actual poscici€ del cursor i incrementa
// la seva posici€. Si la columna arriba a 39, salta a 0 tot
// incrementant la fila si el LCD »s de dues files.
// Si es de 4 files, incrementa de fila en arribar a la columna 20
// AixÃ mateix, la fila 4 passa a la zero.
// En els LCDs d'una fila, quan la columna arriba a 39, torna
// a zero. No s'incrementa mai la fila

void LcPutString(char *s);
// Post: Pinta l'string a apartir de la posici€ actual del cursor.
// El criteri de coordenades »s el mateix que a LcPutChar
// Post: Pot trigar fins a 40us pel nombre de chars de s a sortir de
// la rutina




#endif	/* LCTLCD_H */

