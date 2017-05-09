//
// TAD per a manipular un display alfanum�ric basat en el
// controlador HD44780 usant nom�s 4 bits de dades.
// Aquest �s el controlador que porten la  immensa majoria de displays.
// Tamany m�xim, 4 files per 40 columnes
//
// F. Escudero vCli v1.0 Piera, gener de 2004
//
// He provat aquest TAD amb un LCD de 2x16. Qualsevol error que observeu,
// us agra�ria que m'ho f�ssiu saber a sisco@salleurl.edu.
//
// Vcli V1.1, Sisco, a 26 de novembre de 2004. He vist que en alguns LCDs cal esperar 2ms
// despr�s de fer un Clear, amb independ�ncia del que digui el Busy.
// Sembla extrany, quan tingui temps haig de mirar el bit de Busy amb l'oscil.loscop
// De moment, retardo amb timer.
//
// Vcli V2.0, Sisco, 3 de mar� de 2010. L'adapto per a funcionar sobre un PIC24F16KA102
// Vcli V2.1, Sisco, 2 de juny de 2010. L'adapto per a funcionar a trav�s d'un expansor I2C

//
// Durant el proc�s d'inicialitzaci�, es demanar� un timer al tad timer
//
// ------------------------------------HARDWARE---AREA--------------------
// La connexi� ser� de 4 bits de dades (D4 a D7), i els senyals
// RS, R/W i E.
//
// 	Es pot usar qualsevol altre configuraci�. Nom�s
//  cal ajustar els defines que venen a continuaci�.
//
// -------------------------------END--HARDWARE---AREA--------------------

#include "I2TI2C.h"

void LcInit(int Files, int Columnes);
// Pre: Files = {1, 2, 4}  Columnes = {8, 16, 20, 24, 32, 40 }
// Pre: Hi ha un timer lliure
// Post: L'Hitachi merd�s necessita 40ms de tranquilitat desde
// la pujada de Vcc fins cridar aquest constructor
// Post: Aquesta rutina pot trigar fins a 100ms
// Post: El display queda esborrat, el cursor apagat i a la
// posici� 0, 0.
void LcMotor(char priority);
// Si prioritat val 1 tramitara les ordres si o si
// Si val 0 es tramitarant nom�s si el motor I2C est� lliure

void LcEnd(void);
// El Destructor

void LcClear(char priority);
// Post: Esborra el display i posa el cursor a la posici� zero en
// l'estat en el que estava.
// Post: La propera ordre pot trigar fins a 1.6ms

void LcCursorOn(void);
// Post: Enc�n el cursor
// Post: La propera ordre pot trigar fins a 40us

void LcCursorOff(void);
// Post: Apaga 0el cursor
// Post: La propera ordre pot trigar fins a 40us

void LcGotoXY(int Columna, int Fila);
// Pre : Columna entre 0 i 39, Fila entre 0 i 3
// Post: Posiciona el cursor en aquestes coordenades
// Post: La propera ordre pot trigar fins a 40us

void LcPutChar(char c);
// Post: Pinta C en l'actual poscici� del cursor i incrementa
// la seva posici�. Si la columna arriba a 39, salta a 0 tot
// incrementant la fila si el LCD �s de dues files.
// Si es de 4 files, incrementa de fila en arribar a la columna 20
// Aix� mateix, la fila 4 passa a la zero.
// En els LCDs d'una fila, quan la columna arriba a 39, torna
// a zero. No s'incrementa mai la fila

void LcPutString(char *s);
// Post: Pinta l'string a apartir de la posici� actual del cursor.
// El criteri de coordenades �s el mateix que a LcPutChar
// Post: Pot trigar fins a 40us pel nombre de chars de s a sortir de
// la rutina

void LcPrintf(int X, int Y, char *fmt, ... );
// Pre : Columna entre 0 i 39, Fila entre 0 i 3
// Post: rutina equivalent a la printf, per� amb la difer�ncia que el
// resultat el pinta en el display a la fila i columna especificada

void LcPutIntXY(int X, int Y, int v);
