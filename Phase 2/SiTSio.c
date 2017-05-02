#include "SiTSio.h"



static char BufferRX[BUFFER_RX_SIZE];
static char* BufferTX[BUFFER_TX_SIZE];
static unsigned char IniciTx, IniciRx, FiTx, FiRx;
static unsigned char  QuantsRX, QuantsTX;


static char estat;

static void PosaRX(char);
static char TreuRX(void);
static void PosaTX(char*);
static char* TreuTX(void);



void SiInit(void){
//Post: Inicialitza la SIO als pins RP2 (Tx) i RP4(Rx) i demana un timer (de moment passo d'interrupcions)

    //Configurem la SIO

    U2MODE = 0x8000;
    U2BRG = 25;
    U2STAbits.UTXEN = 1;
    estat = 0;
    // Activo les interrupcions
    IEC1bits.U2RXIE = 1;
    QuantsTX = QuantsRX = IniciRx = IniciTx = FiRx = FiTx = 0;
}



static char *temporal;

void MotorSIO(void){
    switch(estat){
        case 0:
            if (QuantsTX > 0){
                estat = 1;
            }
            break;
        case 1:
            temporal=TreuTX();
            estat = 2;
            break;
        case 2:
            if (*temporal)
                estat = 3;
            else
                estat = 0;
            break;
        case 3:
            if (U2STAbits.TRMT != 0){
                U2TXREG = *temporal++;
                U2STAbits.UTXEN = 1;
                estat=2;
            }
            break;
    }
}



void SiPuts(char *s) {
    //Versi€ vintage
	while(*s) SiSendChar(*s++);
}

//Provo de fer un invent cooperatiu ja que si char *s »s molt llarg
//Amorro tots els motors cooperatius

void SiPutsCooperatiu(char *s) {
    //Pre: La referÀncia de char *s »s o b» un const char o b» puc garantir que
    //     no es sobreescriur? fins que no l'hagi enviat...
    //Post: Encua *s a la cua de cadenes per enviar...
    PosaTX(s);
}



int SiCharAvail(void) {
// Pre: retorna el nombre de car?cters rebuts que no s'han recollit
// amb la funci? GetChar encara
	return QuantsRX;
}

char SiGetChar(void) {
// Pre: SiCharAvail() ªs major que zero
// Post: Treu i retorna el primer car?cter de la cua de recepci?.
	return TreuRX();
}

void SiSendChar(char c) {
// Post: espera que el car?cter anterior s'hagi enviat i envia aquest
	while(U2STAbits.TRMT == 0) ClrWdt();
	U2TXREG = c;
	U2STAbits.UTXEN = 1;
}

//
//---------------------------End--PUBLIQUES---AREA-----------
//

//
//--------------------------------PRIVADES----AREA-----------
//


static void PosaTX(char *c) {
	BufferTX[IniciTx++] = c;
	if (IniciTx == BUFFER_TX_SIZE) IniciTx = 0;
	QuantsTX++;
}


static char* TreuTX(void) {
	char *tmp;
	tmp = BufferTX[FiTx++];
	if (FiTx == BUFFER_TX_SIZE) FiTx = 0;
	QuantsTX --;
	return tmp;
}



static void PosaRX(char c) {
	BufferRX[IniciRx++] = c;
	if (IniciRx == BUFFER_RX_SIZE) IniciRx = 0;
	QuantsRX++;
}
static char TreuRX(void) {
	char tmp;
	tmp = BufferRX[FiRx++];
	if (FiRx == BUFFER_RX_SIZE) FiRx = 0;
	QuantsRX --;
	return tmp;
}
void __attribute__((interrupt, no_auto_psv)) _U2RXInterrupt(void) {
        IFS1bits.U2RXIF = 0;
	if (U2STAbits.OERR == 1) U2STAbits.OERR = 0;
	PosaRX(U2RXREG);
}
//
//---------------------------End--PRIVADES----AREA-----------
//



