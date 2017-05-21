#include "PrTPropaganda.h"

static char timerPropaganda, estatPropaganda;
static char opcio;
static char id[MAX_ID + EXTRA_ID_STRING],tempID[MAX_ID] ={'0','0','0'},
        tempReturn[MAX_ID] ={'0','\r','\0'};

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

void initPropaganda(void){
    //Pre: La sio està inicialitzada
    SiPutsCooperatiu(PROPAGANDA_1);
    SiPutsCooperatiu(PROPAGANDA_2);
    Menu();
    timerPropaganda= TiGetTimer();
    estatPropaganda = 0;
    id[0] = ':';
    id[1] = ':';
    id[2] = ':';
    id[3] = '0';
    id[4] = '\n';
    id[5] = '\0';
}

char getIDPos(unsigned char pos){
    return id[pos];
}

void setIDPos(int pos, unsigned char value){
    id[pos] = value;
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
            break;
        case 1:
            //Introduir un nou identificador
            if (SiCharAvail() != 0){
                if(id[3] == '0'){
                    if(id[0] == ':'){
                        setIDPos(0,SiGetChar());
                        tempReturn[0] = getIDPos(0);
                        SiPutsCooperatiu(tempReturn);
                    }else if(id[1] == ':'){
                        setIDPos(1,SiGetChar());
                        tempReturn[0] = getIDPos(1);
                        SiPutsCooperatiu(tempReturn);
                    }else if(id[2] == ':'){
                        setIDPos(2,SiGetChar());
                        tempReturn[0] = getIDPos(2);
                        SiPutsCooperatiu(tempReturn);
                    }else{
                        if (SiGetChar() == 27){
                            id[3] = '\r';
                            SiPutsCooperatiu("Has introduit el ID: ");
                            SiPutsCooperatiu(id);
                            Menu();
                            estatPropaganda=0;
                            setCadena(1);
                        }else{
                            SiPutsCooperatiu("Ja has introduit el ID!");
                            SiPutsCooperatiu(" Prem ESC per tornar al Menu!");
                            SiPutsCooperatiu("\r\n");
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
            SiPutsCooperatiu("Trames identificades: ");
            SiPutsCooperatiu(getTramesTotals());
            SiPutsCooperatiu("\r\n\0");
            Menu();
            estatPropaganda=0;    
            break;
        case 4:
            SiPutsCooperatiu("Trames rebudes: ");
            SiPutsCooperatiu(getTramesPropies());
            SiPutsCooperatiu("\r\n\0");
            Menu();
            estatPropaganda=0;    
            break;
        case 5:
            SiPutsCooperatiu("L'ultim missatge rebut es: ");
            SiPutsCooperatiu((char*)getMessage());
            SiPutsCooperatiu("\r\n\0");
            Menu();
            estatPropaganda=0;    
            break;
        default:
            Menu();
            estatPropaganda=0;
            break;
    }
}

#define     MAXCOLUMNES 16
static char estatLCD = 0;
const char cadena[MAXCADENES][MAXCOLUMNES] = 
{{"Waiting for ID  "},{"ID set          "},{"New Message     "}};
static unsigned char timerLCD, caracterIniciTrama, i,j,h,quina,timerCincSegons;
static unsigned char segonaLinia[MAXCOLUMNES];

void initMotorLCD(void){
    //Pre: El LCD està inicialitzat
    timerLCD = TiGetTimer();
    timerCincSegons= TiGetTimer();
    caracterIniciTrama = 0;
    LcClear();
    j = 0;
    h = 0;
    i = 0;
    quina = 0;
    segonaLinia[0]=' ';
    segonaLinia[1]=' ';
    segonaLinia[2]='/';
    segonaLinia[5]=' ';
    segonaLinia[6]=' ';
    segonaLinia[7]=' ';
    segonaLinia[8]=' ';
    segonaLinia[9] = ' ';
    segonaLinia[10] = ' ';
    segonaLinia[11]=' ';
    segonaLinia[12]=' ';
}

void startToCount(){
    TiResetTics(timerCincSegons);
}

unsigned char mostrarMissatge(){
    //Pre: Timer dels cinc segons demanat
    //Post: retorna 1 si es pot mostrar el missatge 0 altrament
    if(getLength() > 0 && TiGetTics(timerCincSegons) >= 5000){
        turnOffAudio();
        return 1;
    }else{
        j = 0;
        caracterIniciTrama = 0;
        return 0;
    }
}

int getVelocitat(int mostra){
    //Pre: Ad inicialitzat
    //Post: Retorna el temps de refresh del LCD a partir de l'entrada analogica
    return (((1.75)*(mostra))+200);
    /*
     * Haurem d'agafar la mostra i utilizar una 
     * equacio de la recta per obtenir el temps, 1.75*x+200
     */
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
                    LcPutChar(cadena[quina][h]);
                    h++;
                    if (h==MAXCOLUMNES){
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
            estatLCD= 2;
            break;
        case 2:
            segonaLinia[0]=getTramesPropies()[1];
            segonaLinia[1]=getTramesPropies()[0];
            segonaLinia[3]=getTramesTotals()[1];
            segonaLinia[4]=getTramesTotals()[0];
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
void setCadena(unsigned char seleccio){
    quina = seleccio;
}

/*
 *
 *          Fi  PROPAGANDAAAAAAAAAA!!!!
 *
 */

