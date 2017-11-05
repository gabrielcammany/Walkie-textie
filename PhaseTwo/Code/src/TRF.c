/*
 * File:   TRF.c
 * Author: Gabriel Cammany
 *
 * Created on 3 de mayo de 2017, 12:48
 */

#include "TPWM.h"

#define END_BYTE                0x00
#define START_BYTE              0x40
#define IN                      PORTBbits.RB13

static char estatRF,input,tramaRebudaPropia[MAX_DIGITS_RECIEVED] = {'0','0','\r','\0'}, 
        tramaRebudaTotals[MAX_DIGITS_RECIEVED] = {'0','0','\r','\0'};
static char timerRF, sincronized;
static int caracter, totalBytesMessage;
static unsigned char pos;
static unsigned char missatge[MAX_MESSAGE],inValue,id_trama[MAX_ID];

void afegeixEspais(){
    //Post: Afageix EXTRA_SPACES al principi del missatge
    for(caracter = 0;caracter<EXTRA_SPACES;caracter++){
        missatge[caracter] = ' ';
    }
}

void inicializaID(){
    //Post: Afageix EXTRA_SPACES al principi del missatge
    for(caracter = 0;caracter<MAX_ID;caracter++){
        id_trama[caracter] = ':';
    }
}

void RFInit(){
    TRISBbits.TRISB13 = 1;
    AD1PCFGbits.PCFG11 = 1; //No vull entrada analògica (AN11)!!
    CNPU1bits.CN13PUE = 1;
    timerRF = TiGetTimer();
    
    estatRF = 0;
    sincronized = 0;
    pos = 0;
    totalBytesMessage = 0;
    inicializaID();
    afegeixEspais();
}

void incrementa(char *comptador){
    //Pre: comptador ha de tenir valors entre '0' <= comptador[MAX_DIGITS_RECIEVED] <= '9' 
    //Post: Incrementa el contingut
    if(comptador[1] == '9'){
       comptador[1] = '0'; 
       comptador[0]++;
    }else{
        comptador[1]++;
    }
}

void addBit(char input){
    //Post: Afegeix un 1 i rota cap a la dreta, en el MSB, en cas de 
    //que input valgui 1 o rota cap a la dreta altrament
    if(input == 1){
        inValue = (inValue >> 1) & 0x7F ;;
    }else{
        inValue = ((inValue | 0x80) >> 1) & 0x7F ;
    }
}


void fiTramaPropia(){
    //Post: Incrementa les variables de trames, Comenca a comptar
    //els 5 segons, canvia el missatge del LCD i fa el calcul del periode del altaveu
    //finalment encent el altaveu
    incrementa(tramaRebudaPropia);
    incrementa(tramaRebudaTotals);
    startToCount();
    setCadena(2);
    totalBytesMessage = caracter;
    setAudioPeriode((caracter)/30);
    changeAudioStatus();
}

void exitStateInstructions(){
    //Post: Reseteja les variables inValue i pos. Afegeix el
    //bit i fa el resettics
    inValue = 0;
    pos = 0;
    addBit(input);
    TiResetTics(timerRF);
}

char comprovaID(){
    //Post: Retorna 1 en cas de que el id de la trama sigui el mateix que 
    //el ID que ha introduit l'usuari, 0 altrament.
    return (getIDPos(0) == id_trama[0] && getIDPos(1) == id_trama[1] && getIDPos(2) == id_trama[2]);
}

void MotorRF () {
	switch(estatRF) {
	case 0:
		if(IN == 1){
            TiResetTics(timerRF);
            estatRF = 1;
        }
		break;
	case 1:
        if(IN == 0){
            if(TiGetTics(timerRF) > 4){
                estatRF = 2;
                TiResetTics(timerRF);
             }else{
                estatRF = 0;
             }
        }
		break;
	case 2:
		if(IN == 1){
            if(TiGetTics(timerRF) < 5){
                   estatRF = 0;
            }else{
                if(TiGetTics(timerRF) < 14){
                    estatRF = 0;
                }else{
                    inValue = 0;
                    pos = 0;
                    TiResetTics(timerRF);
                    estatRF = 3;
                    caracter = 0;
                }
            }
        }
		break;
	case 3:
        if(TiGetTics(timerRF) == 1){
            input = IN;
        }
		if(IN == !input && TiGetTics(timerRF) > 1){
            TiResetTics(timerRF);
            estatRF = 4;
        }
		break;
	case 4:
        if(TiGetTics(timerRF) > 4){ 
            if(pos<7){
                estatRF = 3;
                pos++;
                addBit(input);
				TiResetTics(timerRF);
            }else{
                if (sincronized == 1){
                    estatRF = 5;
                }else{
                    estatRF = 6;
                }
            }
        }
		break;
    case 5:
        if(inValue == END_BYTE){
            sincronized = 0;
            estatRF = 8;
        }else{
            missatge[caracter] = inValue;
            caracter++;
            estatRF = 3;
        }
        exitStateInstructions();
		break;
    case 6:
        if(inValue == START_BYTE){
            sincronized = 2;
            estatRF = 3;
            caracter = 0;
            exitStateInstructions();
        }else{
            if(sincronized == 2){
                if (caracter < 3 ){
                    id_trama[caracter] = inValue;
                    caracter++;
                    estatRF = 3;
                    exitStateInstructions();
                }else{
                    if(sincronized != 4)estatRF = 7;
                    else estatRF = 0;sincronized = 0;
                }
            }else{
                estatRF = 0;sincronized = 0;
            }
            
        }
		break;
    case 7:
        if(comprovaID()){
            sincronized = 1;
            estatRF = 3;
            caracter = 16;
            missatge[caracter] = inValue;
            caracter++;
        }else{
            estatRF = 0;
            sincronized = 4;
            incrementa(tramaRebudaTotals);
        }
        exitStateInstructions();
        break;
    case 8:
        fiTramaPropia();
        estatRF = 0;
        break;
    }
}

char* getTramesTotals(){
    return tramaRebudaTotals;
}

char* getTramesPropies(){
    return tramaRebudaPropia;
}

unsigned char* getMessage(unsigned char offset){
    return (missatge+offset);
}

int getLength(){
    return totalBytesMessage;
}
