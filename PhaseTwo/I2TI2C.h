//-----------------------------------------------------------------
//TAD         :I2C del robot OMNIA
//Autor       :FEC
//Descripció  :Gestiona l'I2C del PIC24FJ64GA006
//Prefix      :i2
//Data        :Wednesday, March 2, 2010
//-----------------------------------------------------------------
//
//-----------------------------------------------------------------


//-------------------------------PUBLIQUES-------------------------
int i2Init(void);
// Post: retorna 0 si tot ok, 2 si no queden timers

void i2End(void);

int i2WriteData(char SlaveAddress, unsigned char Address, unsigned char Dada);
// Pre: Bytes >= 0;
// Pre: El bit més baix de Address no téinformació (serà… read o write)
// Post: encua l'ordre  dins de la llista d'ordres I2C

int i2ReadData(char SlaveAddress, unsigned char Address, int Bytes, unsigned char *Dades);
// Pre: Bytes >= 0; Dades té‚ espai per a NumBytes
// Pre: El bit més baix de Address no té informació (serà… read o write)
// Post: retorna 0 si tot OK
//--------------------------END--PUBLIQUES-------------------------
int i2WriteDataFast(char SlaveAddress, unsigned char Address, unsigned char Dada);
// Post: tramita ordre al moment. No cooperatiu

void MotorI2C();
// Tramita ordres I2C de forma coperativa


char i2cFree();
//Post: retorna True si el I2C està lliure