#include "PrTPropaganda.h"

#define MAX_ID 3

static int timestamp;
static char timerPropaganda, estatPropaganda,timerCincSegons;
static char temp[50], opcio;
static char id[MAX_ID_STRING],tempID[MAX_ID] ={'0','0','0'};

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
    SiPutsCooperatiu("1-Introduir un nou identificador\r\n\0");
    SiPutsCooperatiu("2-Consultar ID actual\r\n\0");
    SiPutsCooperatiu("3-Consultar trames identificades\r\n\0");
    SiPutsCooperatiu("4-Consultar trames rebudes\r\n\0");
    SiPutsCooperatiu("5-Visualizar l'ultim missatge rebut\r\n\0");
    SiPutsCooperatiu("\r\n");
    SiPutsCooperatiu("Per sortir de qualsevol opcio, prem ESC\r\n");
}

char getIDPos(unsigned char pos){
    return id[pos];
}

char* getID(){
    return id;
}

void setIDPos(int pos, unsigned char value){
    id[pos] = value;
}

char getTemps(int pos){
    return tempID[pos];
}

char* getTemp(){
    return temp;
}

void initPropaganda(void){
    //Pre: La sio està inicialitzada
    SiPutsCooperatiu(PROPAGANDA_1);
    SiPutsCooperatiu(PROPAGANDA_2);
    Menu();
    timestamp = 0;
    timerPropaganda= TiGetTimer();
    estatPropaganda = 0;
    id[0] = ':';
    id[1] = ':';
    id[2] = ':';
    id[3] = '0';
    id[4] = '\n';
    id[5] = '\0';
     TRISBbits.TRISB13 = 1;
}

void startToCount(){
    TiResetTics(timerCincSegons);
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
                if(id[3] == '0'){
                    if(id[0] == ':'){
                        id[0] = SiGetChar();
                    }else if(id[1] == ':'){
                        id[1] = SiGetChar();
                    }else if(id[2] == ':'){
                        id[2] = SiGetChar();
                    }else{
                        if (SiGetChar() == 27){
                            id[3] = '\r';
                            SiPutsCooperatiu("Has introduit el ID: ");
                            SiPutsCooperatiu(id);
                            Menu();
                            estatPropaganda=0;
                            setCadena(3);
                        }
                    }
                }else{
                    id[3] = '0';
                    id[0] = ':';
                    id[1] = ':';
                    id[2] = ':';
                }
            }
            break;

        case 2:
            SiPutsCooperatiu("ID: ");
            SiPutsCooperatiu(id);
            Menu();
            estatPropaganda=0;            
            break;
        case 3:
            //Trames identificades
            //Visualització pulsadors
            if (TiGetTics(timerPropaganda) > 500){
                TiResetTics(timerPropaganda);
                SiSendChar('\r');
                SiPutsCooperatiu("PB1: \0");
                SiSendChar(PORTBbits.RB13+'0');
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
        default:
            Menu();
            estatPropaganda=0;
            break;
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
const unsigned char cadena[]=   {"Waiting for ID  "};
const unsigned char cadenaID[]= {"ID set          "};
const unsigned char cadenaNew[]={"New Message     "}; //Més val que tingui 16 caràcters...
static unsigned char timerLCD, caracterInici,caracterIniciTrama, i,j,h,quina,melodia;
static unsigned char segonaLinia[MAXCOLUMNES];

void initMotorLCD(void){
    //Pre: El LCD està inicialitzat
    timerLCD = TiGetTimer();
    timerCincSegons= TiGetTimer();
    caracterInici = 0;
    caracterIniciTrama = 0;
    LcClear();
    j=0;
    quina = 1;
    melodia = 0;
    //Hi ha caselles de la segona línia que sempre valdran el mateix, les preparo!
    segonaLinia[0]=' ';
    segonaLinia[1]=' ';
    segonaLinia[2]='/';
    segonaLinia[5]=' ';
    segonaLinia[6]=' ';
    segonaLinia[7]=' ';
    segonaLinia[8]=' ';
    segonaLinia[11]=' ';
}

char mostrarMissatge(){
    if(getLength() > 0 && TiGetTics(timerCincSegons) >= 5000){
        turnOffAudio();
        return 1;
    }else{
        j = 0;
        caracterIniciTrama = 0;
        return 0;
    }
}

void setCadena(unsigned char cadena){
    quina = cadena;
}

void MotorLCD(void){
    switch (estatLCD){
        case 0:
            if(TiGetTics(timerLCD) > getVelocitat(AdGetMostra())){
                if(mostrarMissatge()){
                    LcPutChar(getMessage()[(j)]);
                    j++;
                    if (j >= getLength()){
                        j = 0;
                    }
                }else{
                    if(quina == 1){
                        LcPutChar(cadena[h]);
                    }else if(quina == 2){
                        seguentFrequencia();
                        LcPutChar(cadenaNew[h]);
                    }else if(quina == 3){
                        LcPutChar(cadenaID[h]);
                    }
                    h++;
                    if (h==16){
                        h = 0;
                    }
                }
                if (++i > MAXCOLUMNES) {
                    estatLCD = 1;
                    TiResetTics(timerLCD);
                    LcGotoXY(0,1);
                }
            }
            break;
        case 1: //Preparo el string
            if(id[0] ==  ':'){
                tempID[0] = (tempID[0]<'9' ? (tempID[0] + 1) : '0');
                tempID[1] = (tempID[1]<'9' ? (tempID[1] + 1) : '0');
                tempID[2] = (tempID[2]<'9' ? (tempID[2] + 1) : '0');
            }else{
                if(tempID[0] != id[0]){
                    tempID[0] = (tempID[0]<'9' ? (tempID[0] + 1) : '0');
                }
                if(tempID[1] != id[1]){
                    tempID[1] = (tempID[1]<'9' ? (tempID[1] + 1) : '0');
                }
                 if(tempID[2] != id[2]){
                    tempID[2] = (tempID[2]<'9' ? (tempID[2] + 1) : '0');
                }
            }
            segonaLinia[0]=getTramesPropies(1);
            segonaLinia[1]=getTramesPropies(0);
            segonaLinia[3]=getTramesTotals(1);
            segonaLinia[4]=getTramesTotals(0);
            segonaLinia[9] = ' ';
            segonaLinia[10] = ' ';
            estatLCD= 2;
            break;
        case 2: //Aquí faig l'itoa, que deu trigar una bona estona el pobre...
            segonaLinia[12]=' ';
            segonaLinia[13] = tempID[0];
            segonaLinia[14] = tempID[1];
            segonaLinia[15] = tempID[2];
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
                if(getLength() > 0){
                    caracterIniciTrama++;
                    if (caracterIniciTrama>=getLength()){
                        caracterIniciTrama=0;
                    }
                    j = caracterIniciTrama;
                }
                estatLCD = 0;
                LcGotoXY(0,0);
                i=0;
                h = 0;
            }
            break;
    }
}

/*
 *
 *          Fi  PROPAGANDAAAAAAAAAA!!!!
 *
 */

