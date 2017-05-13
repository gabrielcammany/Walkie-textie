#include "PrTPropaganda.h"

#define MAX_ID 3

static int timestamp;
static char timerPropaganda, estatPropaganda;
static char temp[50], opcio;
static char id[MAX_ID];

void myItoa(int num){
    //Post: escriu el valor ascii de num a tmp;
    temp[0] = (char)(num/1000);
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
    SiPutsCooperatiu("1-Visualitzacio del timestamp\r\n\0");
    SiPutsCooperatiu("2-Visualitzacio dels micro-interruptors\r\n\0");
    SiPutsCooperatiu("3-Visualitzacio dels pulsadors\r\n\0");
    SiPutsCooperatiu("4-Mostrar intensitat del backlight\r\n\0");
    SiPutsCooperatiu("5-Modificar intensitat del backlight\r\n\0");
    SiPutsCooperatiu("6-Encendre/apagar altaveu\r\n\0");
    SiPutsCooperatiu("7-Reset\r\n\n\0");
    SiPutsCooperatiu("\r\n");
    SiPutsCooperatiu("Per sortir de qualsevol opcio, prem ESC\r\n");
    
    SiPutsCooperatiu("\r\nLes opcions de test son:\r\n\0");
    SiPutsCooperatiu("\r\n\0");
    SiPutsCooperatiu("1-Introduir un nou identificador\r\n\0");
    SiPutsCooperatiu("2-Consultar ID actual\r\n\0");
    SiPutsCooperatiu("3-Consultar trames identificades\r\n\0");
    SiPutsCooperatiu("4-Consultar trames rebudes\r\n\0");
    SiPutsCooperatiu("5-Visualizar l'ultim missatge rebut\r\n\0");
    SiPutsCooperatiu("\r\n");
    SiPutsCooperatiu("Per sortir de qualsevol opcio, prem ESC\r\n");
}

char getIDPos(char pos){
    return id[0];
}

void initPropaganda(void){
    //Pre: La sio està inicialitzada
    SiPutsCooperatiu(PROPAGANDA_1);
    SiPutsCooperatiu(PROPAGANDA_2);
    Menu();
    timestamp = 0;
    timerPropaganda= TiGetTimer();
    estatPropaganda = 0;
}

void MotorPropaganda(void){

    switch(estatPropaganda){
        case 0:
            //Aquí estem esperant ordre del menú principal
            if (SiCharAvail() != 0){
                opcio= SiGetChar();
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
            //Introduir un nou identificador
            if (SiCharAvail() != 0){
                switch(SiCharAvail()){
                    case 1:
                        id[0] = SiGetChar();
                        id[1] = '0';
                        id[2] = '0';
                        break;
                    case 2:
                        id[0] = SiGetChar();
                        id[1] = SiGetChar();
                        id[2] = '0';
                        break;
                    case 3:
                        id[0] = SiGetChar();
                        id[1] = SiGetChar();
                        id[2] = SiGetChar();
                        break;
                    default:    
                        SiSendChar('\r');
                        SiSendChar(SiCharAvail());
                        break;
                }
            }
            //Mostrar timestamp
            /*
            if (TiGetTics(timerPropaganda) > 1000){
                if (++timestamp == 10000) timestamp=0;
                myItoa(timestamp);
                SiSendChar('\r');
                SiPutsCooperatiu(temp);
                TiResetTics(timerPropaganda);
            }
            if (SiCharAvail() != 0){
                if (SiGetChar() == 27){
                    Menu();
                    estatPropaganda=0;
                }
            }*/
            if (SiCharAvail() != 0){
                if (SiGetChar() == 27){
                    Menu();
                    estatPropaganda=0;
                }
            }
            break;

        case 2:
            SiSendChar('\r');
            SiPutsCooperatiu("ID: \0");
            SiPutsCooperatiu(id);
            //Visualització uInterruptors
            /*
            if (TiGetTics(timerPropaganda) > 1000){
                TiResetTics(timerPropaganda);
                if (++timestamp == 10000) timestamp=0;
                SiSendChar('\r');
                SiPutsCooperatiu("SW1: \0");
                SiSendChar(getSwitch1()+'0');
                estatPropaganda = 21;
            }*/
            if (SiCharAvail() != 0){
                if (SiGetChar() == 27){
                    Menu();
                    estatPropaganda=0;
                }
            }
            break;
        case 3:
            //Trames identificades
            //Visualització pulsadors
            if (TiGetTics(timerPropaganda) > 1000){
                TiResetTics(timerPropaganda);
                if (++timestamp == 10000) timestamp=0;
                SiSendChar('\r');
                SiPutsCooperatiu("PB1: \0");
                SiSendChar(getPB1()+'0');
                estatPropaganda = 31;
            }
            if (SiCharAvail() != 0){
                if (SiGetChar() == 27){
                    Menu();
                    estatPropaganda=0;
                }
            }
            break;
        case 4:
            if (TiGetTics(timerPropaganda) > 1000){
                TiResetTics(timerPropaganda);
                if (++timestamp == 10000) timestamp=0;
                SiSendChar('\r');
                myItoa(getBlDuty()*5);
                SiSendChar(temp[1]);
                SiSendChar(temp[2]);
                SiSendChar(temp[3]);
            }
            if (SiCharAvail() != 0){
                if (SiGetChar() == 27){
                    Menu();
                    estatPropaganda=0;
                }
            }
            break;
        case 5:
            SiPutsCooperatiu("\n\rIntrodueix valor de la nova intensitat (0-4):\n\r");
            estatPropaganda = 51;
            break;
        /*case 6:
            SiPutsCooperatiu("\n\rPrem J per encendre o apagar l'altaveu \n\r");
            if (getAudioStatus()) SiPutsCooperatiu("\rAltaveu ences! \0");
                        else SiPutsCooperatiu("\rAltaveu apagat!\0");
            estatPropaganda = 61;
            break;

        case 7:
            __asm__ volatile ("reset");
            break;*/
        /*case 21:
            SiPutsCooperatiu("\t\tSW2: \0");
            SiSendChar(getSwitch2()+'0');
            estatPropaganda= 2;
            break;
        case 31:
            SiPutsCooperatiu("\t\tPB2: \0");
            SiSendChar(getPB2()+'0');
            estatPropaganda= 3;
            break;
        case 51:
            if (TiGetTics(timerPropaganda) > 1000){
                TiResetTics(timerPropaganda);
                if (++timestamp == 10000) timestamp=0;
            }
            if (SiCharAvail() != 0){
                switch (SiGetChar()){
                    case 27:
                        Menu();
                        estatPropaganda=0;
                        break;
                    case '0':
                        SiPutsCooperatiu("\r000\0");
                        setBlDuty(0);
                        break;
                    case '1':
                        SiPutsCooperatiu("\r025\0");
                        setBlDuty(PERIODBL/4);
                        break;
                    case '2':
                        SiPutsCooperatiu("\r050\0");
                        setBlDuty(PERIODBL/2);
                        break;
                    case '3':
                        SiPutsCooperatiu("\r075\0");
                        setBlDuty(PERIODBL*3/4);
                        break;
                    case '4':
                        SiPutsCooperatiu("\r100\0");
                        setBlDuty(PERIODBL);
                        break;
                }
            }
            break;
        case 61:

            if (TiGetTics(timerPropaganda) > 1000){
                TiResetTics(timerPropaganda);
                if (++timestamp == 10000) timestamp=0;
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
            break;*/
    }

}

int getVelocitat(int mostra){
    return (((1.75)*(mostra))+200);
    /*
     * Haurem d'agafar la mostra i utilizar una ecuacio de la recta per obtenir el temps, 1.75*x+200
     */
}


#define     MAXCOLUMNES 16
static char estatLCD = 0;
const unsigned char cadena[]={"SDM 2013-14     "}; //Més val que tingui 16 caràcters...
static unsigned char timerLCD, caracterInici, i,j;
static unsigned int mostra;
static unsigned char segonaLinia[MAXCOLUMNES];


void initMotorLCD(void){
    //Pre: El LCD està inicialitzat
    timerLCD = TiGetTimer();
    caracterInici = 0;
    LcClear();
    //Hi ha caselles de la segona línia que sempre valdran el mateix, les preparo!
    segonaLinia[0]='X';
    segonaLinia[1]='/';
    segonaLinia[2]='Y';
    segonaLinia[5]=' ';
    segonaLinia[6]=' ';
    segonaLinia[7]=' ';
    segonaLinia[8]=' ';
    segonaLinia[11]=' ';
}


void MotorLCD(void){
    switch (estatLCD){
        case 0:
            LcPutChar(cadena[j++]);
            if (j==16) j= 0;
            if (i++ > MAXCOLUMNES) {
                estatLCD = 1;
                TiResetTics(timerLCD);
                LcGotoXY(0,1);
            }
            break;

        case 1: //Preparo el string
            segonaLinia[3] = getSwitch1()+'0';
            segonaLinia[4] = getSwitch2()+'0';
            segonaLinia[9] = getPB1()+'0';
            segonaLinia[10] = getPB2() + '0';
            estatLCD= 2;
            break;
        case 2: //Aquí faig l'itoa, que deu trigar una bona estona el pobre...
            mostra = AdGetMostra();
            myItoa(mostra);
            segonaLinia[12]=' ';
            segonaLinia[13] = getTemps();
            segonaLinia[14] = getTemps();
            segonaLinia[15] = getTemps();
            estatLCD = 3;
            break;
        case 3:
            if (TiGetTics(timerLCD)>50){
                //Observo que si estresso molt al LCD arriba un punt que alguna
                //vegada pinta malament un caràcter. Deu tenir una cua interna?
                //si la té, aposto a que és de 24 posicions (mal número)...
                //Cap problema, donem 50 ms. de calma entre ràfega i ràfega i gas
                TiResetTics(timerLCD);
                i=0;
                estatLCD = 4;
            }
            break;
        case 4:
            LcPutChar(segonaLinia[i++]);
            if (i > MAXCOLUMNES) {
                estatLCD = 5;
                TiResetTics(timerLCD);
            }
            break;
                
        case 5:
            if (TiGetTics(timerLCD)>= 250){
                //Alerta, ja porto 50 ms. des de l'últim refresc
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

