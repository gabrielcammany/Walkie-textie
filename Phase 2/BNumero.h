/*
 * File:   BNumero.h
 * Author: RBB
 *
 * Created on 26 de enero de 2014, 23:57
 */

#ifndef BNNUMERO_H
#define	BNNUMERO_H




void BNinit();
//post: Posa el pint CN16, com a entrada i activa el pull-up intern,
//també solicita un timer

void motorBNumero();
// motor que va actualitzant el valor del numero, té en compte els rebots tan de pujada com de baixada
char getNumero();
//post: retorna el número de vegades que s'ha premut, es ciclic a partir de 10

char resetNumero();
//post: sempre retorna 0

#endif	/* BNNUMERO_H */



