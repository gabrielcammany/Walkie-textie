/* 
 * File:   AdTADC.h
 * Author: JNM
 *
 * Created on 27 de enero de 2014, 19:53
 * No em despentino massa... Aquest TAD està unicament dissenyat per a que funcioni
 * amb el canal AN0. Passo de tot, converteixo a tota llet i no genero interrupcions,
 * qui vulgui una mostra que l'agafi.
 */

#ifndef ADTADC_H
#define	ADTADC_H

#include <xc.h>

void AdInit(void);
//Inicialitza el conversor A/D únicament per al canal RA0
int AdGetMostra(void);
//Post: Retorna la mostra convertida en 10 bits
#endif	/* ADTADC_H */