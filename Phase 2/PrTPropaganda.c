#include "PrTPropaganda.h"


static int timestamp;
static char timerPropaganda, estatPropaganda;
static char temp[50], opcio;

void myItoa(int num){
    //Post: escriu el valor ascii de num a tmp;
    temp[0] = (char)((int)num/(int)1000);
    num = num - (temp[0]*1000);
    temp[1] = (char)(num /100);
    num = num - (temp[1]*100);
    temp[2] = (char) (num /10);
    num = num - (temp[2]*10);
    temp[3] = num+'0';
    temp[4] = '\0';
    temp[2]+='0';
    temp[1]+='0';
    temp[0]+='0';

}
void Menu(void){
    SiPutsCooperatiu("\r\nLes opcions de test son:\r\n\0");
    SiPutsCooperatiu("\r\n\0");
    SiPutsCooperatiu("1-Introduir un nou identificador\r\n\0");
    SiPutsCooperatiu("2-Consultar ID actual\r\n\0");
    SiPutsCooperatiu("3-Consultar trames identificades\r\n\0");
    SiPutsCooperatiu("4-Consultar trames rebudes totals\r\n\0");
    SiPutsCooperatiu("5-Visualitzar	l?últim	missatge rebut\r\n\n\0");
    SiPutsCooperatiu("\r\n");
    SiPutsCooperatiu("Per sortir de qualsevol opcio, prem ESC\r\n");
}

void initPropaganda(void){
    //Pre: La sio està inicialitzada
    SiPutsCooperatiu(PROPAGANDA_1);
    SiPutsCooperatiu(PROPAGANDA_2);
    Menu();
    timerPropaganda= TiGetTimer();
    estatPropaganda = 0;
}

void MotorPropaganda(void){
       // SiPutsCooperatiu("\n\ESTAT Nº \n\r");
        //SiSendChar(estatPropaganda);
    switch(estatPropaganda){
        case 0:
            //Aquí estem esperant ordre del menú principal
            if (SiCharAvail() != 0){
                opcio= SiGetChar();
                SiPutsCooperatiu("\n\rNew Char \n\r");
                SiSendChar(opcio);
                if ((opcio <= '7') && (opcio >= '1')){
                    estatPropaganda = opcio-'0';
                }else{
                    SiSendChar('\r');
                    SiSendChar(opcio);
                }
            }
            if (TiGetTics(timerPropaganda) > 1000){
                TiResetTics(timerPropaganda);
                if (++timestamp == 10000) timestamp=0;
            }
            break;
        case 1:
            //Mostrar timestamp
            if (TiGetTics(timerPropaganda) > 1000){
                SiSendChar('\r');
                SiPutsCooperatiu(temp);
                TiResetTics(timerPropaganda);
            }
            if (SiCharAvail() != 0){
                if (SiGetChar() == 27){
                    Menu();
                    estatPropaganda=0;
                }
            }
            break;
        case 2:
            //Visualització pulsadors
            if (TiGetTics(timerPropaganda) > 1000){
                TiResetTics(timerPropaganda);
                SiSendChar('\r');
                SiPutsCooperatiu("PB1: \0");
                    SiSendChar(getPB1()+'0');
                estatPropaganda = 21;
            }
            if (SiCharAvail() != 0){
                if (SiGetChar() == 27){
                    Menu();
                    estatPropaganda=0;
                }
            }
            break;
       
        case 3:
            SiPutsCooperatiu("\n\rPrem J per encendre o apagar l'altaveu \n\r");
            if (getAudioStatus()) SiPutsCooperatiu("\rAltaveu ences! \0");
                        else SiPutsCooperatiu("\rAltaveu apagat!\0");
            estatPropaganda = 31;
            break;

        case 4:
                if (TiGetTics(timerPropaganda) > 1000){
                    TiResetTics(timerPropaganda);
                    SiSendChar('\r');
                    SiPutsCooperatiu("Numero: \0");
                        SiSendChar(getNumero()+'0');
                    estatPropaganda = 4;
                }
            if (SiCharAvail() != 0){
                if (SiGetChar() == 27){
                    Menu();
                    estatPropaganda=0;
                }
            }
            break;
        
        case 5:
            __asm__ volatile ("reset");
            break;
       case 21:
            SiPutsCooperatiu("\t\tPB2: \0");
            SiSendChar(getPB2()+'0');
            estatPropaganda= 2;
            break;
        case 31:
            if (TiGetTics(timerPropaganda) > 1000){
                TiResetTics(timerPropaganda);
            }
            if (SiCharAvail() != 0){
                switch (SiGetChar()){
                    case 27:
                        Menu();
                        estatPropaganda=0;
                        break;
                    case 'J':
                        if (changeAudioStatus()) SiPutsCooperatiu("\rAltaveu ences! \0");
                        else SiPutsCooperatiu("\rAltaveu apagat!\0");
                        break;
                    case 'j':
                        if (changeAudioStatus()) SiPutsCooperatiu("\rAltaveu ences! \0");
                        else SiPutsCooperatiu("\rAltaveu apagat!\0");
                        break;
                }
            }
            break;
    }

}

#define     MAXCOLUMNES 16
static char estatLCD = 0;
const unsigned char cadena[]={"S6M 2013-14     "}; //Més val que tingui 16 caràcters...
static unsigned char timerLCD, caracterInici, i,j;
static unsigned int mostra;
static unsigned char segonaLinia[MAXCOLUMNES];


void initMotorLCD(void){
    //Pre: El LCD està inicialitzat
    timerLCD = TiGetTimer();
    caracterInici = 0;
    LcClear(0); 
    //Hi ha caselles de la segona línia que sempre valdran el mateix, les preparo!
    segonaLinia[0]='P';
    segonaLinia[1]='B';
    segonaLinia[2]=':';
    segonaLinia[5]=' ';
	segonaLinia[6]=' ';
}


void MotorLCD(void){
    switch (estatLCD){
        case 0:
            LcPutChar(cadena[j++]);
            if (j==16) j= 0;
            if (i == MAXCOLUMNES-1) {
                estatLCD = 1;
                TiResetTics(timerLCD);
                LcGotoXY(0,1);
            }
            i++;
            break;

        case 1: //Preparo el string
            segonaLinia[3] = getPB1() + '0';
            segonaLinia[4] = getPB2() + '0';
            estatLCD= 2;
            break;
        case 2: //Aquí faig l'itoa, que deu trigar una bona estona el pobre...
            mostra = AdGetMostra();
            myItoa(mostra);
            segonaLinia[7]=temp[0];
            segonaLinia[8]=temp[1];
            segonaLinia[9]=temp[2];
            segonaLinia[10]=temp[3];
            segonaLinia[11]=segonaLinia[12]=segonaLinia[13]=segonaLinia[14]=segonaLinia[15]=' ';
            estatLCD = 3;
            break;
        case 3:
            if (TiGetTics(timerLCD)>50){
                //Observo que si estresso molt al LCD arriba un punt que alguna
                //vegada pinta malament un caràcter i l'altaveu no funciona correctament
                //Cap problema, donem 50 ms. de calma entre ràfega i ràfega
                TiResetTics(timerLCD);
                i=0;
                estatLCD = 4; 
            }
            break;
        case 4:
            LcPutChar(segonaLinia[i++]);
            if (i == MAXCOLUMNES) {
                estatLCD = 5;
                TiResetTics(timerLCD);
            }
            break;
                
        case 5:
            if (TiGetTics(timerLCD)>=1500){
                //Alerta, li donc un temps perque tingui temps de desencuar totes les ordres
                // i no saturar-lo, ja que si l'atavalo molt observo que ens costa complir la
                //frequencia de l'altaveu.
                caracterInici++;
                if (caracterInici==16)
                    caracterInici=0;
                estatLCD = 0;
                LcGotoXY(0,0);
                j = caracterInici;
                i=0;
            }
            break;
    }
}

/*
 *
 *          Fi  PROPAGANDAAAAAAAAAA!!!!
 *
 */

