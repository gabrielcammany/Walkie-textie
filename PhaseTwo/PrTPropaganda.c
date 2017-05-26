#include "PrTPropaganda.h"

static char timerPropaganda, estatPropaganda;
static char opcio;
static char id[MAX_ID + EXTRA_ID_STRING],tempID[MAX_ID] ={'0','0','0'};

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
                    SiSendChar('\r');
                    SiSendChar(opcio);
                    SiSendChar('\n');
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
                    setIDPos(0,SiGetChar());
                    SiSendChar(getIDPos(0));
                    estatPropaganda=18; 
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
            estatPropaganda=50;            
            break;
        case 3:
            SiPutsCooperatiu("Trames identificades: ");
            SiPutsCooperatiu(getTramesTotals());
            estatPropaganda=50;    
            break;
        case 4:
            SiPutsCooperatiu("Trames rebudes: ");
            SiPutsCooperatiu(getTramesPropies());
            estatPropaganda=50;    
            break;
        case 5:
            SiPutsCooperatiu("L'ultim missatge rebut es: ");
            SiPutsCooperatiu((char*)getMessage(15));
            estatPropaganda=50;    
            break;
        case 18:
            if (SiCharAvail() != 0){
                setIDPos(1,SiGetChar());
                SiSendChar(getIDPos(1));
                estatPropaganda=19;  
            }
            break;
        case 19:
            if (SiCharAvail() != 0){
                setIDPos(2,SiGetChar());
                SiSendChar(getIDPos(2));
                estatPropaganda=20;
            }
            break;
        case 20:
            if(SiCharAvail() != 0){
                if (SiGetChar() == 27){
                    id[3] = '\r';
                    SiPutsCooperatiu("\r\n");
                    SiPutsCooperatiu("Has introduit el ID: ");
                    SiPutsCooperatiu(id);
                    setCadena(1);
                    estatPropaganda=50;
                }else{
                    SiPutsCooperatiu("\r\n");
                    SiPutsCooperatiu("Ja has introduit el ID!");
                    SiPutsCooperatiu(" Prem ESC per tornar al Menu!");
                    SiPutsCooperatiu("\r\n");
                }
            }
            break;
        case 50:
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
    segonaLinia[10] = 'I';
    segonaLinia[11]='D';
    segonaLinia[12]=':';
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
        if(getLength() > 0)seguentFrequencia();
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

void incrementaIDs(){
    //Post: Incrementa el ID que esta per pantalla si no hi ha cap afegit, en cas
    //contrari, incrementa fins arribar al numero
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
}

void actualitzaTramesLCD(){
    //Post: Actualiza les columnes 0,1,3,4 del LCD amb les dades de 
    //trames propies i totals
    segonaLinia[0]=getTramesPropies()[0];
    segonaLinia[1]=getTramesPropies()[1];
    segonaLinia[3]=getTramesTotals()[0];
    segonaLinia[4]=getTramesTotals()[1];
}

void actualitzaIDLCD(){
    //Post: Actualiza les columnes 13,14,15 del LCD amb el ID actual desat en
    //tempID
    segonaLinia[13] = tempID[0];
    segonaLinia[14] = tempID[1];
    segonaLinia[15] = tempID[2];
}

void rotaMissatge(){
    //Post: Rota el missatge que surt per pantalla ficant un offset a la j
    caracterIniciTrama++;
    if (caracterIniciTrama>=getLength()){
        caracterIniciTrama=0;
    }
    j = caracterIniciTrama;
}

void MotorLCD(void){
    switch (estatLCD){
        case 0:
            if(TiGetTics(timerLCD) > getVelocitat(AdGetMostra())){
                if(mostrarMissatge()){
                    estatLCD = 1;
                }else{
                    estatLCD = 2;
                }
            }
            break;
        case 1:
            LcPutChar(getMessage(0)[(j)]);
            j++;
            if (j >= getLength()) j = 0;
            estatLCD = 3;
            break;
        case 2:
            LcPutChar(cadena[quina][h]);
            h++;
            if (h==MAXCOLUMNES) h = 0;
            estatLCD = 3;
        case 3:
            if (++i > MAXCOLUMNES) {
                estatLCD = 4;
                TiResetTics(timerLCD);
                LcGotoXY(0,1);
            }else{
                estatLCD = 0;
            }
            break;
        case 4: //Preparo el string
            incrementaIDs();
            estatLCD= 5;
            break;
        case 5:
            actualitzaTramesLCD();
            actualitzaIDLCD();
            estatLCD = 6;
            break;
        case 6:
            if (TiGetTics(timerLCD)>50){
                //Observo que si estresso molt al LCD arriba un punt que alguna
                //vegada pinta malament un caràcter. Deu tenir una cua interna?
                //si la té, aposto a que és de 24 posicions (mal número)...
                //Cap problema, donem 50 ms. de calma entre ràfega i ràfega i gas
                TiResetTics(timerLCD);
                i=0;
                estatLCD = 7;
            }
            break;
        case 7:
            LcPutChar(segonaLinia[i++]);
            if (i > MAXCOLUMNES) {
                estatLCD = 8;
                TiResetTics(timerLCD);
            }
            break;
        case 8:
            if (TiGetTics(timerLCD)>= 250){
                //Alerta, ja porto 50 ms. des de l'últim refresc
                if(getLength() > 0)rotaMissatge();
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

