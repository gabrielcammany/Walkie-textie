

//Interficie 1,2,3,4,6

int TiGetTimer (void);
	/*********************************************************************
	//Precondicions: Hi ha algun timer lliure. Maxim TI_NUMTIMERS
	//Postcondicions: Retorna un handle per usar les funcions TiGetTics i
	//TiResetTics. Retorna -1 si no hi ha cap timer disponible.
	\\*********************************************************************/

void TiResetTics (unsigned char Handle);
	/*********************************************************************
	//Precondicions: Handle ha estat retornat per Ti_OpenTimer.
	//Postcondicions: Engega la temporitzaci¢ associada a 'Handle'.
	//              i agafa la referencia temporal del sistema
	\\*********************************************************************/

unsigned int  TiGetTics (unsigned char Handle);
	/*********************************************************************
	//Precondicions: Handle ha estat retornat per TiGetTimer.
	// La distància temmporal entre GetTics i ResetTics ha de ser menor
	// de TI_MAXTICS ms (actualment, 30 segons)
	//Postcondicions: Retorna els milisegons transcorreguts des de la crida
	//                a l'StartTimer del 'Handle'.
	\\*********************************************************************/
	
//Interficie 0 

char changeAudioStatus();
    //Post Canvia l'estat d'audio
	
void setAudioPeriode(char nouPeriode);
// Pre: nouPeriode >= 1


//Interficie 9

int getLength();
//Pre: -
//Post: Retorna la mida del missatge tenim desat

char* getTramesTotals();
//Pre: -
//Post: Retorna el numero de trames totals

char* getTramesPropies();
//Pre: -
//Post: Retorna el numero de trames propies

unsigned char* getMessage(unsigned char offset);
//Pre: 0<= offset <= MAX_MESSAGE
//Post: Retorna el missatge que hi ha actualment amb el offset especificat



//Interficie 5

void turnOffAudio();
//Post: Apaga el Altaveu

void seguentFrequencia();
//Post: Canvia el periode del altaveu amb una desviació maxima del 
//original de MAX_PERIODES


//Interficie 10

void LcClear(void);
// Post: Esborra el display i posa el cursor a la posiciÛ zero en
// l'estat en el que estava.
// Post: La propera ordre pot trigar fins a 1.6ms

void LcGotoXY(char Columna, char Fila);
// Pre : Columna entre 0 i 39, Fila entre 0 i 3
// Post: Posiciona el cursor en aquestes coordenades
// Post: La propera ordre pot trigar fins a 40us

void LcPutChar(char c);
// Post: Pinta C en l'actual posciciÛ del cursor i incrementa
// la seva posiciÛ. Si la columna arriba a 39, salta a 0 tot
// incrementant la fila si el LCD Ès de dues files.
// Si es de 4 files, incrementa de fila en arribar a la columna 20
// AixÌ mateix, la fila 4 passa a la zero.
// En els LCDs d'una fila, quan la columna arriba a 39, torna
// a zero. No s'incrementa mai la fila

//Interficie 11

char getIDPos(unsigned char pos);
//Pre: 0 <= pos < MAX_ID_STRING
//Post: Retorna el digit del ID que es demani

//Interficie 8

int AdGetMostra(void);
//Post: Retorna la mostra convertida en 10 bits

//Interficie 7

int SiCharAvail(void);
// Pre: retorna el nombre de car?cters rebuts que no s'han recollit
// amb la funciÛ GetChar encara

char SiGetChar(void);
// Pre: SiCharAvail() Ès major que zero
// Post: Treu i retorna el primer car?cter de la cua de recepciÛ.

void SiSendChar(char c);
// Post: espera que el car?cter anterior s'hagi enviat i envia aquest

void SiPutsCooperatiu(char *s);
    //Pre: La referència de char *s és o bé un const char o bé puc garantir que
    //     no es sobreescriurà fins que no l'hagi enviat...
    //Post: Encua *s a la cua de cadenes per enviar...

