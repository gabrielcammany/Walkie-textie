 //---------------------------------------------------------------------------------------
// TAD per a manipular un display alfanumèric basat en el
// controlador HD44780. Aquest és el controlador que porten la
// immensa majoria de displays. Tamany màxim, 4 files per 40 columnes
//
// Durant el procés d'inicialització, es demanarà un timer al tad timer
// que no s'usarà per a res més. El constructor alliberarà aquest timer.

// Vcli V1.1, Sisco, a 26 de novembre de 2004. He vist que en alguns LCDs cal esperar
// després de fer un Clear, amb independència del que digui el Busy.
// Sembla extrany, quan tingui temps haig de mirar el bit de Busy amb l'oscil.loscop
// De moment, rertardo amb timer.

// Vcli V2.0, Sisco, 3 de març de 2010. L'adapto per a funcionar sobre un PIC24F16KA102
// Vcli V2.1, Sisco, JULIOL de 2010. L'adapto per a funcionar sobre un PIc24fj64ga006
//---------------------------------------------------------------------------------------


#include "LCTLcd.h"
#include <p24fxxxx.h>
#include "TITTIMER.h"
#include "I2TI2C.h"

//
//--------------------------------CONSTANTS---AREA-----------
//
#define VERSIO_CUA
#define FUNCTION_SET            0x20
#define BITS_8			0x10
#define DISPLAY_CONTROL         0x08
#define DISPLAY_ON		0x04
#define CURSOR_ON		0x02
#define DISPLAY_CLEAR           0x01
#define ENTRY_MODE		0x04
#define SET_DDRAM		0x80
//
//---------------------------End--CONSTANTS---AREA-----------
//


//
//--------------------------------VARIABLES---AREA-----------
//
static int Files, Columnes;
static int FilaAct, ColumnaAct;
static int Timer;
#define MAX_CUA_LCD		640
#define ES_IR			0x8000
static unsigned int Buffer[MAX_CUA_LCD];
static int Inici = 0;
static int Fi    = 0;
static int Quants = 0;
//
//---------------------------End--VARIABLES---AREA-----------
//

//
//--------------------------------PROTOTIPUS--AREA-----------
//
void PosaOrdre(unsigned int);
unsigned int  TreuOrdre(void);
void TramitaOrdre(char);
void Espera(int Timer, int ms);
void CantaIR(unsigned char IR);
void CantaData(unsigned char Data);
void WaitForBusy();
void EscriuPrimeraOrdre(char ordre,char priority);
void InicialitzaMCP2317(char priority);
static void SetD4_D7Sortida(char priority);
//
//---------------------------End--PROTOTIPUS--AREA-----------
//


//
//--------------------------------PUBLIQUES---AREA-----------
//


static char timerLCD=0;
static char estat=0;
void LcMotor(char priority) {
#ifdef VERSIO_CUA
    //Pre : ha de rebre prioritat, que pot valer 0 o 1
    // 0-> no te prioritat, l'ordre es desencuara quan el motor I2c la pugui trametre.
    // 1-> Te prioritat, tramitara l'ordre sense ser cooperatiu.
    // Lo normal és fer-ho sense prioritat
    //Post : Si el motor I2C no té feina desencuara es tramitara una nova ordre
    
     if (priority){
       TramitaOrdre(priority);
     }else{
    switch (estat){
        case 0:
             if(i2cFree()){
              TramitaOrdre(priority);
              estat=0;
              TiResetTics(timerLCD);
             }
            break;
        case 1:
            if(TiGetTics(timerLCD)>10){
                estat=0;
            }
            break;
    }
  }

#endif
}


void IoWait(unsigned int Micros) { int micros;
// Pre : Micros és positiu
// Post: Espera el nombre de microsegons especificat. NO ES COOPERATIVA
// i per tant s'ha d'usar amb molta precaució. Esta afinada per a un clock de 8M
	micros=Micros>>1;
	while(micros!=0) {
		ClrWdt();
		--micros;
		Nop(); Nop(); Nop();
	}
}


void LcInit(int files, int columnes) {
//Pre_ L'inici no és coperatiu, el TAD I2C no es comporta de forma Cooperativa
// Pre: Files = {1, 2, 4}  Columnes = {8, 16, 20, 24, 32, 40 }
// Pre: L'Hitachi merdós necessita 40ms de tranquilitat desde
// la pujada de Vcc fins cridar aquest constructor
// Pre: Hi ha un timer lliure
// Post: Aquesta rutina pot trigar fins a 100ms
// Post: El display queda esborrat, el cursor apagat i a la
// posició 0, 0.
	int i;
        timerLCD=TiGetTimer();
	Timer = TiGetTimer(); // Val més que n'hi hagi
	Files = files; Columnes = columnes;
	FilaAct = ColumnaAct = 0;
	InicialitzaMCP2317(1);
	SetD4_D7Sortida(1);
	for (i = 0; i < 2; i++) {
		Espera(Timer, 40);
		// El manual de l'hitachi diu que s'ha de fer aquesta
		// seqüència...
		EscriuPrimeraOrdre(FUNCTION_SET | BITS_8,1);
		Espera(Timer, 4);
		EscriuPrimeraOrdre(FUNCTION_SET | BITS_8,1);
		Espera(Timer, 4);
		EscriuPrimeraOrdre(FUNCTION_SET | BITS_8,1);
		// .. tres vegades. Potser és una penitència?
		// Ara una a 4 bits
		EscriuPrimeraOrdre(FUNCTION_SET,1);
		Espera(Timer, 1);
		if (files == 1)
			CantaIR(FUNCTION_SET); 			// 4bits, 1 files, font 5x7
		else
			CantaIR(FUNCTION_SET | 0x08); 	// 4bits, 1 files, font 5x7
		LcMotor(1); LcMotor(1);
		// Ara ja podem mirar el busy
		WaitForBusy(); 	CantaIR(DISPLAY_CONTROL);  	// Display Off
		WaitForBusy(); 	CantaIR(DISPLAY_CLEAR);	   	// Tot espais
		LcMotor(1); LcMotor(1);
		Espera(Timer,2); // 1.64ms V1.1
		WaitForBusy(); 	CantaIR(ENTRY_MODE | 0x02); // Auto Increment i shift
		WaitForBusy(); 	CantaIR(DISPLAY_CONTROL | DISPLAY_ON); 		// Display On
		LcMotor(1); LcMotor(1);
	}
	// El manual dirà el que vulgui però jo he vist que després
	// d'una arrencada freda, no sempre s'inicialitza correctament
	// tot i esperar els 40ms. Per això, he posat un bucle de dues
	// inicialitzacions i a partir d'aquí, la inicialització m'ha funcionat
	// correctament tant si faig un reset com si apago i engego l'alimentació.
}

void LcEnd(void) {
// El Destructor
	TiCloseTimer (Timer); // Ja no ens farà cap falta
}

void LcClear(char priority) {
//Pre : priority indica si l'ordre es tramita de forma cooperativa o no
// 0-> cooperatiu (us normal)
// 1-> no coperatiu (iniciar)
// Post: Esborra el display i posa el cursor a la posició zero en
// l'estat en el que estava.
// Post: La propera ordre pot trigar fins a 1.6ms
	WaitForBusy();
	CantaIR(DISPLAY_CLEAR);	   // Tot espais
	MotorI2C();
	if (priority) Espera(Timer, 2); // V1.1
}

void LcCursorOn() {
// Post: Encén el cursor
// Post: La propera ordre pot trigar fins a 40us
	WaitForBusy();
	CantaIR(DISPLAY_CONTROL | DISPLAY_ON | CURSOR_ON);
}

void LcCursorOff(void) {
// Post: Apaga 0el cursor
// Post: La propera ordre pot trigar fins a 40us
	WaitForBusy();
	CantaIR(DISPLAY_CONTROL | DISPLAY_ON);
}

void LcGotoXY(int Columna, int Fila) {
// Pre : Columna entre 0 i 39, Fila entre 0 i 3
// Post: Posiciona el cursor en aquestes coordenades
// Post: La propera ordre pot trigar fins a 40us
	int Fisica;
	// d'entrada calculo la direcció efectiva de la ram del LCD
	switch (Files) {
		case 2:
			Fisica = Columna + (!Fila ? 0 : 0x40); break;
		case 4:
			Fisica = Columna;
			if (Fila == 1) Fisica += 0x40; else
			if (Fila == 2) Fisica += Columnes;      /* 0x14; */ else
			if (Fila == 3) Fisica += 0x40+Columnes; /* 0x54; */
			break;
		case 1:
		default:
			Fisica = Columna; break;
	}
	// i l'aplico amb la comanda
	WaitForBusy();
	CantaIR(SET_DDRAM | Fisica);
	// Finalment, actualitzo les imatges locals
	FilaAct    = Fila;
	ColumnaAct = Columna;
}

void LcPutChar(char c) {
// Post: Pinta C en l'actual posició del cursor i incrementa
// la seva posició. Si la columna arriba a 39, salta a 0 tot
// incrementant la fila si el LCD és de dues files.
// Si es de 4 files, incrementa de fila en arribar a la columna 20
// Així mateix, la fila 4 passa a la zero.
// En els LCDs d'una fila, quan la columna arriba a 39, torna
// a zero. No s'incrementa mai la fila
	// D'entrada escric el char
	WaitForBusy(); CantaData(c);
	// i ara recalculo la posició del cursor
	++ColumnaAct;
	if (Files == 3) {
		if (ColumnaAct >= 20) {
			ColumnaAct = 0;
			if (++FilaAct >= 4) FilaAct = 0;
			LcGotoXY(ColumnaAct, FilaAct);
		}
	} else
	if (Files == 2) {
		if (ColumnaAct >= 40) {
			ColumnaAct = 0;
			if (++FilaAct >= 2) FilaAct = 0;
			LcGotoXY(ColumnaAct, FilaAct);
		}
	} else
	if (FilaAct == 1) {
		if (ColumnaAct >= 40) ColumnaAct = 0;
		LcGotoXY(ColumnaAct, FilaAct);
	}
}

void LcPutString(char *s) {
// Post: Pinta l'string a apartir de la posició actual del cursor.
// El criteri de coordenades és el mateix que a LcPutChar
// Post: Pot trigar fins a 40us pel nombre de chars de s a sortir de
// la rutina
	while(*s) LcPutChar(*s++);
}

//
//---------------------------End--PUBLIQUES---AREA-----------
//

//
//--------------------------------PRIVADES----AREA-----------
//


// Registres del MCP2317
#define I2CADDR_23	0x40>>1
#define IODIRA_23	0
#define IODIRB_23	1
#define GPIOA_23	0x12
#define GPIOB_23	0x13
#define OLAT_A		0x14
#define OLAT_B		0x15

// Variables de mirall
static unsigned char PORTA_23 = 0;
static unsigned char PORTB_23 = 0;
#define RSBit		1
#define RWBit		2
#define EBit 		4


static void Escriu2317(unsigned char registre, unsigned char valor,char priority) {
    //Pre: si priority val 1, escriura de forma no coperativa en i2c.
    
    switch (priority){
        case 0:
            i2WriteData(I2CADDR_23, registre, valor);
            break;
        case 1:
            i2WriteDataFast(I2CADDR_23, registre, valor);
            break;
        default:
            i2WriteData(I2CADDR_23, registre, valor);
            break;


    }
    if (!priority) i2WriteData(I2CADDR_23, registre, valor);
    else i2WriteDataFast(I2CADDR_23, registre, valor);
	
}

static unsigned char Llegeix2317(unsigned char registre) {
	unsigned char dades[1];
	i2ReadData(I2CADDR_23, registre, 1, dades);
	return dades[1];
}

static void CANTA_A(char priority) {
	Escriu2317(GPIOA_23, PORTA_23,priority);
}

static void CANTA_B(char priority) { Escriu2317(OLAT_B, PORTB_23,priority); }

static void SetD4_D7Sortida(char priority){
	Escriu2317(IODIRA_23, 0x00,priority);
}
/*
static void SetD4_D7Entrada(void) {
	Escriu2317(IODIRA_23, 0x07);
}
*/
static void SetControlsSortida(char priority){
	Escriu2317(IODIRB_23, 0x00,priority);
}

static void SetD4D7(unsigned char c,char priority) {
	if (c & 1) PORTA_23 |= 0x8; else PORTA_23 &= ~(0x8);	// D0
	if (c & 2) PORTA_23 |= 0x4; else PORTA_23 &= ~(0x4);	// D1
	if (c & 4) PORTA_23 |= 0x2; else PORTA_23 &= ~(0x2);	// D2
	if (c & 8) PORTA_23 |= 0x1; else PORTA_23 &= ~(0x1);	// D3
	CANTA_A(priority);
}

unsigned char GetBusyFlag(void){
	unsigned char v;
	v = Llegeix2317(GPIOA_23);
	if (v & 1) return 1; else return 0;
}

static void RSUp(char priority)   	 { PORTB_23 |=  RSBit; CANTA_B(priority); }
static void RSDown(char priority)	 { PORTB_23 &= ~RSBit; CANTA_B(priority); }
//static void RWUp(void)		 { PORTB_23 |=  RWBit; CANTA_B(); }
static void RWDown(char priority)	 { PORTB_23 &= ~RWBit; CANTA_B(priority); }
static void EnableUp(char priority)	 { PORTB_23 |=  EBit; CANTA_B(priority); }
static void EnableDown(char priority) { PORTB_23 &= ~EBit;  CANTA_B(priority); }

void InicialitzaMCP2317(char priority) {
	// No sembla que calgui fer res més
	SetControlsSortida(priority);
	CANTA_A(priority); CANTA_B(priority);
}

void Espera(int Timer, int ms) {
	TiResetTics(Timer);
	while(TiGetTics(Timer) < ms){
            ClrWdt();
            //MotorI2C();
        }
}

void CantaPartAlta(char c,char priority) {
	 SetD4D7((c>>4) & 0x0f,priority);
}

void CantaPartBaixa(char c,char priority) {
	 SetD4D7(c & 0x0f, priority);
}

void CantaIR(unsigned char IR) {
#ifdef VERSIO_CUA
	PosaOrdre(IR|ES_IR);


#else
	SetD4_D7Sortida();
	RSDown();
	EnableUp();
	CantaPartAlta(IR); 		// Segons hitachi Data Setup = 80ns (cap problema)
	EnableDown();   		// i l'amplada del pols "enable" major que 230n
	EnableUp();
	CantaPartBaixa(IR); 	// Segons hitachi Data Setup = 80ns (cap problema)
	EnableUp();				// Asseguro els 500ns de durada de pols
	EnableDown();   		// i l'amplada del pols "enable" major que 230n
	RSUp();
#endif
}

void CantaData(unsigned char Data) {	// Aproximandament, 3ms per cada byte que escrivim
#ifdef VERSIO_CUA
	PosaOrdre(Data);
#else
	IOWait(100);
	EnableUp();
	CantaPartAlta(Data); 	// Segons hitachi Data Setup = 80ns (cap problema)
	EnableDown();   		// i l'amplada del pols "enable" major que 230n
	EnableUp();
	CantaPartBaixa(Data); 	// Segons hitachi Data Setup = 80ns (cap problema)
	EnableUp();				// Asseguro els 500ns de durada de pols
	EnableDown();   		// i l'amplada del pols "enable" major que 230n
#endif
}

void TramitaOrdre(char priority) {
	unsigned int t;
	if (Quants) {
		t = TreuOrdre();
		if (t & ES_IR) {
			t &= 0x00ff;
			SetD4_D7Sortida(priority); RSDown(priority);
			EnableUp(priority);
			CantaPartAlta(t,priority); 		// Segons hitachi Data Setup = 80ns (cap problema)
			EnableDown(priority);   		// i l'amplada del pols "enable" major que 230n
			EnableUp(priority);
			CantaPartBaixa(t,priority); 	// Segons hitachi Data Setup = 80ns (cap problema)
			EnableUp(priority);				// Asseguro els 500ns de durada de pols
			EnableDown(priority);   		// i l'amplada del pols "enable" major que 230n
			RSUp(priority);
		} else {
			if (priority) IoWait(100);
			EnableUp(priority);
			CantaPartAlta(t,priority); 	// Segons hitachi Data Setup = 80ns (cap problema)
			EnableDown(priority);   		// i l'amplada del pols "enable" major que 230n
			EnableUp(priority);
			CantaPartBaixa(t,priority); 	// Segons hitachi Data Setup = 80ns (cap problema)
			EnableUp(priority);				// Asseguro els 500ns de durada de pols
			EnableDown(priority);   		// i l'amplada del pols "enable" major que 230n
		}
	}
}

void WaitForBusy(void) {

}

void EscriuPrimeraOrdre(char ordre, char priority) {
	// Escric el primer com si fossin 8 bits
	SetD4_D7Sortida(priority);
	RSDown(priority); RWDown(priority);
	EnableUp(priority); EnableUp(priority);
	SetD4D7( (ordre >> 4) & 0x0f,priority);
	EnableDown(priority);
}

void PosaOrdre(unsigned int Ordre) {
	Buffer[Inici++] = Ordre;
	if (Inici == MAX_CUA_LCD) Inici = 0;
	++Quants;
}

unsigned int  TreuOrdre(void) {
	unsigned int t;
	t = Buffer[Fi++];
	if (Fi == MAX_CUA_LCD) Fi = 0;
	Quants--;
	return t;
}

//
//---------------------------End--PRIVADES----AREA-----------
//

