//-----------------------------------------------------------------
// TAD de gestió del BUS I2C usant les rutines d'exemple que dona
// MIcrochip. NO està ben provat i no es presentable, ho deixo per a
// més endavant.
// Les rutines no son cooperatives. El timeout es resol per bucle salvatge
// i no hi ha control de Watch Dog. Una mica merdós, la veritat.
//-----------------------------------------------------------------
#include "I2TI2C.h"
#include "TITTIMER.h"
#include <p24fxxxx.h>
#include "api.h"

//-------------------------------CONSTANTS-------------------------
#define Fosc	(8000000) 	// Relloge intern
#define Fcy		(Fosc*2/2)	// w.PLL (Instructions per segon)
//#define Fsck	200000		// 200kHz I2C
#define Fsck	100000		// 100kHz I2C
//#define Fsck	25000		// 2.5kHz I2C
#define I2C_BRG	((Fcy/2/Fsck)-1)



//--------------------------END--CONSTANTS-------------------------

//-------------------------------VARIABLES-------------------------
static int Timer;
//--------------------------END--VARIABLES-------------------------

//-------------------------------PROTOTIPUS--LOCALS----------------
unsigned int StartI2C(void);
unsigned int RestartI2C(void);
unsigned int StopI2C(void);
unsigned int WriteI2C(unsigned char byte);
unsigned int IdleI2C(void);
unsigned int ACKStatus(void);
unsigned int NotAckI2C(void);
unsigned int AckI2C(void);
unsigned int getsI2C(unsigned char *rdptr, unsigned char Length);
unsigned int getI2C(void);


//----------------Llista d'ordres---------------
unsigned char estatI2C,quantes, primer, ultim;
#define MAXTRAMESI2C 50
typedef struct {
    char slaveAddress;
    unsigned char Address;
    unsigned char Dada;
}tramaI2C;

tramaI2C trames[MAXTRAMESI2C];
tramaI2C tramaDeTreball;
//------------------Fi llista d'ordres--------




//-------------------------------PROTOTIPUS--LOCALS----------------

//-------------------------------PUBLIQUES-------------------------
int i2Init(void) {
// Post: retorna 0 si tot ok, 2 si no queden timers
	Timer = TiGetTimer();
	if (Timer == -1) return 2;
	// Configuro l'I2C com a Master a 100KHz
         I2C1BRG = I2C_BRG;

	// Inicialitzo l'I2C, a Master mode, i no Slew Rate
	I2C1CON = 0x1200;	// primer el desactivo
	I2C1RCV = 0x0000;	// Netejo
	I2C1TRN = 0x0000;
	// ---------------------------ACCELEROMETRE-----------
	// Ara provoco una caiguda de tensió en l'acceleròmetre
	// Poso els ports SDA, SCL i INTACC de sortida i a zero
	_TRISG2 = 0;
	_TRISG3 = 0;
	_LATG2  = 0;
	_LATG3  = 0;
	_TRISC13 = 0;
	_LATC13 = 0;
	// El control del transistor Q5
	_TRISB8 = 0;
	_LATB8 = 1;		// Trec l'alimentació
//	IOWait(1500);
	_LATB8 = 0;		// Restauro l'alimentació de l'acceleròmetre
	_TRISC13 = 1;   // El pin d'interrupció ha de ser d'entrada
	_TRISG2  = 1;	// Així com els pins de l'I2C
	_TRISG3  = 1;
	// Encara no se si això serveix per a res.
	// --------------------FI-----ACCELEROMETRE-----------

	I2C1CON = 0x9200;	// I activo
        estatI2C = 0;
        quantes = primer = ultim = 0; // inicialitzo els parametres de la cua
    return 0;
}

void i2End(void) {
}



void MotorI2C(void){
    //Pre: Desencua ordres d'I2c
    //Post: Te 9 estats, es força lent, es complicat saturar-lo perque
    // a la que tingui una ordre no n'acceptara més
    char aux;

    switch (estatI2C){
        case 0:
            if (quantes != 0){
                estatI2C = 1;
                tramaDeTreball.Address = trames[primer].Address;
                tramaDeTreball.slaveAddress = trames[primer].slaveAddress;
                tramaDeTreball.Dada = trames[primer].Dada;
                 primer = (primer +1) % MAXTRAMESI2C;
            }
            break;
        case 1: //Primer Idle
            if (I2C1STATbits.TRSTAT == 0){
                I2C1CONbits.SEN = 1;		//Generate Start COndition
                estatI2C = 2;
            }
            break;
        case 2: //Start
            if (I2C1CONbits.SEN == 0){	//Wait for Start COndition
                aux = I2C1STATbits.S;
                estatI2C = 3;
                I2C1TRN = (tramaDeTreball.slaveAddress << 1) & 0xfffe;					//Load byte to I2C1 Transmit buffer
            }
            break;
        case 3: //WriteData
            if (I2C1STATbits.TBF == 0){		//wait for data transmission
                estatI2C = 4;
            }
            break;
        case 4://Segon Idle
            if (I2C1STATbits.TRSTAT == 0){
                aux = I2C1STATbits.ACKSTAT;
                I2C1TRN = tramaDeTreball.Address;					//Load byte to I2C1 Transmit buffer
                estatI2C = 5;
            }
            break;
        case 5:
            if (I2C1STATbits.TBF == 0){//wait for data transmission del segon write
                estatI2C = 6;
            }
            break;
        case 6:
            if (I2C1STATbits.TRSTAT == 0){ //Tercer Idle
                estatI2C = 7;
                aux = I2C1STATbits.ACKSTAT;
                I2C1TRN = tramaDeTreball.Dada;  //Enviem la tercera dada
            }
            break;
        case 7:
            if (I2C1STATbits.TBF == 0){		//wait for data transmission
                estatI2C = 8;
            }
            break;
        case 8:
            if (I2C1STATbits.TRSTAT == 0){		//Wait for bus Idle
                estatI2C = 9;
                I2C1CONbits.PEN = 1;		//Generate Stop Condition
            }
            break;
        case 9:
            if (I2C1CONbits.PEN == 0){	//Wait for Stop
                estatI2C = 0;
                quantes--;
            
            }
            break;
    }

}


char i2cFree(){
    // Post: Retorna True si no hi ha ordres que tramitar,
    // es per no desbordar el nombre d'ordres de L'I2C
    return quantes==0;
}
int i2WriteData(char SlaveAddress, unsigned char Address, unsigned char Dada) {
// Pre: Bytes >= 0;
// Pre: El bit més baix de Address no té informació (serà read o write)
// Post: Encua les instruccions d'I2C

    trames[ultim].slaveAddress = SlaveAddress;
    trames[ultim].Address = Address;
    trames[ultim].Dada = Dada;
    ultim = (ultim +1) % MAXTRAMESI2C;
    quantes++;
        return 0;
}

int i2WriteDataFast(char SlaveAddress, unsigned char Address, unsigned char Dada) {
// Pre: Bytes >= 0;
// Pre: El bit més baix de Address no té informació (serà… read o write)
// Post: retorna 0 si tot OK o 1 si hi ha un timeout d'un segon
// en un ack
// No és coperatiu, uitilitzar-la per inicialitzar
	unsigned int ErrorCode;

	IdleI2C();						//Ensure Module is Idle
	StartI2C();						//Generate Start COndition
	WriteI2C((SlaveAddress << 1) & 0xfffe);			//Write Control byte
	IdleI2C();

	ErrorCode = ACKStatus();		//Return ACK Status

	WriteI2C(Address);				//Write Low Address
	IdleI2C();

	ErrorCode = ACKStatus();		//Return ACK Status

	WriteI2C(Dada);					//Write Data
	IdleI2C();
	StopI2C();						//Initiate Stop Condition
	return(ErrorCode);

}


int i2ReadData(char SlaveAddress, unsigned char Address, int Bytes, unsigned char *Dades) {
// Pre: Bytes >= 0; Dades t‚ espai per a NumBytes
// Pre: El bit més baix de Address no té informació (serà read o write)
// Post: retorna 0 si tot OK o 1 si hi ha un timeout d'un msegon
// en un ack
    while (1);
	IdleI2C();					//wait for bus Idle
	StartI2C();					//Generate Start Condition
	WriteI2C((SlaveAddress<<1));		//Write Control Byte
	IdleI2C();					//wait for bus Idle
	WriteI2C(Address);			//Write start address
	IdleI2C();					//wait for bus Idle

	RestartI2C();				//Generate restart condition
	WriteI2C((SlaveAddress<<1 | 0x01));	//Write control byte for read
	IdleI2C();					//wait for bus Idle

	getsI2C(Dades, Bytes);		//read Length number of bytes
	NotAckI2C();				//Send Not Ack
	StopI2C();					//Generate Stop
	return 0;
}




//--------------------------END--PUBLIQUES-------------------------

//-------------------------------PRIVADES--------------------------

unsigned int StartI2C(void)
{
	//This function generates an I2C start condition and returns status
	//of the Start.

	I2C1CONbits.SEN = 1;		//Generate Start COndition
	while (I2C1CONbits.SEN);	//Wait for Start COndition
	return(I2C1STATbits.S);	//Optionally return status
}


/*********************************************************************
* Function:        RestartI2C()
*
* Input:		None.
*
* Output:		None.
*
* Overview:		Generates a restart condition and optionally returns status
*
* Note:			None
********************************************************************/
unsigned int RestartI2C(void)
{
	//This function generates an I2C Restart condition and returns status
	//of the Restart.

	I2C1CONbits.RSEN = 1;		//Generate Restart
	while (I2C1CONbits.RSEN);	//Wait for restart
	return(I2C1STATbits.S);	//Optional - return status
}


/*********************************************************************
* Function:        StopI2C()
*
* Input:		None.
*
* Output:		None.
*
* Overview:		Generates a bus stop condition
*
* Note:			None
********************************************************************/
unsigned int StopI2C(void)
{
	//This function generates an I2C stop condition and returns status
	//of the Stop.

	I2C1CONbits.PEN = 1;		//Generate Stop Condition
	while (I2C1CONbits.PEN);	//Wait for Stop
	return(I2C1STATbits.P);	//Optional - return status
}


/*********************************************************************
* Function:        WriteI2C()
*
* Input:		Byte to write.
*
* Output:		None.
*
* Overview:		Writes a byte out to the bus
*
* Note:			None
********************************************************************/
unsigned int WriteI2C(unsigned char byte)
{
	//This function transmits the byte passed to the function
	//while (I2C1STATbits.TRSTAT);	//Wait for bus to be idle
	I2C1TRN = byte;					//Load byte to I2C1 Transmit buffer
	while (I2C1STATbits.TBF);		//wait for data transmission
	return 0;
}


/*********************************************************************
* Function:        IdleI2C()
*
* Input:		None.
*
* Output:		None.
*
* Overview:		Waits for bus to become Idle
*
* Note:			None
********************************************************************/
unsigned int IdleI2C(void)
{
	while (I2C1STATbits.TRSTAT);		//Wait for bus Idle
	return 0;
}

unsigned int ACKStatus(void)
{
	return (!I2C1STATbits.ACKSTAT);		//Return Ack Status
}


/*********************************************************************
* Function:        NotAckI2C()
*
* Input:		None.
*
* Output:		None.
*
* Overview:		Generates a NO Acknowledge on the Bus
*
* Note:			None
********************************************************************/
unsigned int NotAckI2C(void)
{
	I2C1CONbits.ACKDT = 1;			//Set for NotACk
	I2C1CONbits.ACKEN = 1;
	while(I2C1CONbits.ACKEN);		//wait for ACK to complete
	I2C1CONbits.ACKDT = 0;			//Set for NotACk
	return 0;
}


/*********************************************************************
* Function:        AckI2C()
*
* Input:		None.
*
* Output:		None.
*
* Overview:		Generates an Acknowledge.
*
* Note:			None
********************************************************************/
unsigned int AckI2C(void)
{
	I2C1CONbits.ACKDT = 0;			//Set for ACk
	I2C1CONbits.ACKEN = 1;
	while(I2C1CONbits.ACKEN);		//wait for ACK to complete
	return 0;
}


/*********************************************************************
* Function:       getsI2C()
*
* Input:		array pointer, Length.
*
* Output:		None.
*
* Overview:		read Length number of Bytes into array
*
* Note:			None
********************************************************************/
unsigned int getsI2C(unsigned char *rdptr, unsigned char Length)
{
	while (Length --)
	{
		*rdptr++ = getI2C();		//get a single byte

		if(I2C1STATbits.BCL)		//Test for Bus collision
		{
			return(-1);
		}

		if(Length)
		{
			AckI2C();				//Acknowledge until all read
		}
	}
	return(0);
}

/*********************************************************************
* Function:        getI2C()
*
* Input:		None.
*
* Output:		contents of I2C1 receive buffer.
*
* Overview:		Read a single byte from Bus
*
* Note:			None
********************************************************************/
unsigned int getI2C(void)
{
	I2C1CONbits.RCEN = 1;			//Enable Master receive
	Nop();
	while(!I2C1STATbits.RBF);		//Wait for receive bufer to be full
	return(I2C1RCV);			//Return data in buffer
}

//--------------------------END--PRIVADES--------------------------


