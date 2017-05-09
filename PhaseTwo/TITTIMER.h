//
// MÚdul de gestiÚ de Timers
// Barsalona, Novembre de 1995, Juan Perez & JM Ribes.
// Desembre de 2001. FEC. Ajustat per al Fujitsu 90583
// MarÁ de 2010. FEC. Ajustat per al PIC24 (com passen els anys...)
//


#ifndef _TITTIMER_H_
#define _TITTIMER_H_


// Si anem justos de memoria, aquest define indica el
// nombre de timers d'aquest TAD i es pot modificar sense
// problemes.
#define         TI_NUMTIMERS                    32


// El seg¸ent define marca la dist‡ncia temporal m‡xima
// entre un gettics i un resettics
//#define         TI_MAXTICS                      8640000000L //Suficient per aprox. 100 dies
#define         TI_MAXTICS                      30000 //Suficient per aprox. 30 o 40 segons

void TiInit (void);
	/*********************************************************************
	//Postcondicions: Inicialitza el T.A.D. ês precondici¢ global haver cridat
	//                aquesta funci¢ quan es cridi qualsevol altra funci¢.
	\\*********************************************************************/

int TiGetTimer (void);
	/*********************************************************************
	//Precondicions: Hi ha algun timer lliure. Maxim TI_NUMTIMERS
	//Postcondicions: Retorna un handle per usar les funcions TiGetTics i
	//TiResetTics. Retorna -1 si no hi ha cap timer disponible.
	\\*********************************************************************/

void TiResetTics (unsigned char Handle);
	/*********************************************************************
	//Precondicions: Handle ha estat retornat per Ti_OpenTimer.
	//Postcondicions: Engega la temporitzaci¢ associada a 'Handle'.
	//              i agafa la referencia temporal del sistema
	\\*********************************************************************/

unsigned int  TiGetTics (unsigned char Handle);
	/*********************************************************************
	//Precondicions: Handle ha estat retornat per TiGetTimer.
	// La dist‡ncia temmporal entre GetTics i ResetTics ha de ser menor
	// de TI_MAXTICS ms (actualment, 30 segons)
	//Postcondicions: Retorna els milisegons transcorreguts des de la crida
	//                a l'StartTimer del 'Handle'.
	\\*********************************************************************/

void TiCloseTimer (unsigned char Handle);
	/*********************************************************************
	//Precondicions: Handle ha estat retornat per Ti_OpenTimer.
	//Postcondicions: allibera el timer que porta associat aquest 'Handle'
	\\*********************************************************************/

void TiEnd (void);
	/*********************************************************************
	//Postcondicions: Tanca el T.A.D.
	\\*********************************************************************/

#endif
