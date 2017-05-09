#define POLS_CARREGA 0x05
#define POLS_ENVIA_RF 0x04
#define BIT_RESPOSTA_PC 0x08
#define FLAG_DESAR_MSG 0x81
#define FLAG_DESAR_MSG 0x85
#define FLAG_ENVIAR_RF_MSG 0x82
#define FLAG_CONFIRMACIO_MSG 0x83
#define FLAG_DESAR_SENSE_CONFIRMACIO_MSG 0x84
#define ESTAT_LEDS_CIRCULAR 0x04
#define ESTAT_POLSADOR 0x01
#define ESTAT_BLINKING_10HZ 0x08
#define ESTAT_BLINKING_5HZ 0x09
#define POSICIO_A_DESAR_RAM 0x80
#define END_BYTE 0x88
#define MAX_LEDS 0x0B
#define LED0 0x04
#define LED1 0x05
#define LED2 0x06
#define LED3 0x07
#define LED4 0x00
#define LED5 0x01
#define LED6 0x02
#define LED7 0x03
#define LED8 0x04
#define LED9 0x05

int comptaBytes = 0, ESTAT = 0, aux = 0x00, LEDs = 0x01;


void main(){
	while(1){
		if(PIR1.RCIF == 1){
			rebut();
		}
		if(LATC.POLS_CARREGA == 1){
			pols_carrega_missatge();
		}
		if(LATC.POLS_ENVIA_RF == 1){
			pols_enviar_rf();
		}
		if(ESTAT == ESTAT_LEDS_CIRCULAR){
			ledsCircular();
		}
		if(ESTAT == ESTAT_POLSADOR){
			espera_polsador();
		}
		if(status == ESTAT_BLINKING_10HZ){
			blinking_10hz();
		}
	}
}

//Fet
void espera_polsador(){
	if(tempsUn >= 200){
		auxiliar++;
		if(auxiliar >= 50){
			activar_leds_circular();
		}
	}
}

//Fet
void pols_carrega_missatge(){
	status = ESTAT_POLSADOR;
	tempsUn = 0;
	FLAG_DESAR_SENSE_CONFIRMACIO_MSG -> TXREG;
}
//Fet
void pols_enviar_rf(){
	enviaRF();
}

//Fet
void enviarConfirmacioRf(){
	FLAG_ENVIAR_RF_MSG -> TXREG
}

//Fet
void enviarConfirmacioDesar(){
	FLAG_DESAR_MSG -> TXREG
}

//Fet
void enviarConfirmacioDesaT(){
	FLAG_DESAT_MSG -> TXREG
}

//Fet
void blinking_10hz(){
	if(tempsUn == 0x14){
		encendreLEDS(); //Encendre i apagar els leds
	}
	if(tempsUn == 0x28){
		apagarLEDS();
	}
}

//Fet
void blinking_5hz(){
	if(tempsUn == 0x28){
		encendreLEDS(); //Encendre i apagar els leds
	}
	if(tempsUn == 0x50){
		apagarLEDS();
	}
}

//Fet
void encendreLEDS(){
	0xFF -> LATB;
	0xFF -> LATD;
}

//Fet
void apagarLEDS(){
	0xC0 -> LATB;
	0xF0 -> LATD;
	tempsUn = 0;
}

//Fet
void rebut(){
	tempsUn = 0x00;
	if(RCREG.BIT_RESPOSTA_PC == 1){
		if(RCREG < FLAG_DESAR_MSG){ 
			desar();
		}
		if(RCREG == FLAG_ENVIAR_RF_MSG){ 
			enviaRF();
		}
		if(RCREG == FLAG_SENSE_CONFIRMACIO_MSG){
			desar_sense_confirmacio();
		}
	}else{
		return;
	}
}

//Fet
void HighInt(){
	resetTimer();
	tempsUn++;
}

//Fet
void enviaRF(){
	if(comptaBytes != 0x00){
		LATC = 0x00;
		LATD = 0x00;
		LEDs = 0x01; //Ja que es del 1-10
		enviaConfirmacio();
		result = dividir10();
		restant = comptaBytes;
		char i = 0x00;
		while(restant != 0){ // Result es el numero de bytes total entre 10
			POSTINC0 -> AUX; //Afegim el valor de la ram al auxiliar
			tempsUn = 0x00; // Per tal de fer el temps a 0 i 1 amb la codificacio manchester, fiquem 
							//tempsUn a 0 per quan salti la interrupcio
			while(enviat<0x08){ //Enviat son els 8 bits que hem denviar
				if(tempsUn == 0x01){
					enviar_bit_pos(); //Primers 5 mseg
				}
				if(tempsUn == 0x02){
					enviar_bit_neg(); //Segon 5 mseg
					AUX >> 1; //Posem la seguent dada 
					tempsUn = 0x00; //Reiniciem el comptador
				}
			}
			restant--;
			enviat = 0;
			i++;
			if(i==result){
				LEDs++; 
				activar_leds();
				i=0;
			}
		}
		while(LEDS<10){
			LEDs++;
			activar_leds();
		}
		ESTAT = ESTAT_BLINKING_10HZ;
	}else{
		activacio_leds_circular();
	}
}


//Fet
void enviaRF(){
	if(comptaBytes != 0x00){
		LATC = 0x00;
		LATD = 0x00;
		LEDs = 0x01; //Ja que es del 1-10
		enviaConfirmacio();
		result = dividir10();
		restant = comptaBytes;
		char i = 0x00;
		while(LEDs < MAX_LEDS){ //Hi hauran 10 leds i anira 10-1
			while(i<result){ // Result es el numero de bytes total entre 10
				POSTINC0 -> AUX; //Afegim el valor de la ram al auxiliar
				tempsUn = 0x00; // Per tal de fer el temps a 0 i 1 amb la codificacio manchester, fiquem 
								//tempsUn a 0 per quan salti la interrupcio
				while(enviat<0x08){ //Enviat son els 8 bits que hem denviar
					if(tempsUn == 0x01){
						enviar_bit_pos(); //Primers 5 mseg
					}
					if(tempsUn == 0x02){
						enviar_bit_neg(); //Segon 5 mseg
						AUX >> 1; //Posem la seguent dada 
						tempsUn = 0x00; //Reiniciem el comptador
					}
				}
				restant--;
				enviat = 0;
				i++;
			}
			if(restant !=0)i = 0;
			LEDs++; 
			activar_leds();
		}
		ESTAT = ESTAT_BLINKING_10HZ;
	}else{
		activacio_leds_circular();
	}
}

//Fet
void ENVIAR_BIT_PRIMERA_MEITAT(){
	if(AUX.Bit0){ //Si el bit a enviar es 1
		clrBit(LATC5.Bit5); //La primera part a 0
	}else{
		setBit(LATC5.Bit5); //La primera part a 1
	}
}

//Fet
void ENVIAR_BIT_SEGONA_MEITAT(){
	if(AUX.Bit0){ //Si el bit a enviar es 1
		setBit(LATC.Bit5); //La segona part a 1
	}else{
		clrBit(LATC.Bit5); //La segona part a 0
	}
	enviat++;//Hem de tenir un recompte dels bits enviats per anar a 
			 //la seguent dada en la ram un cop enviats 8 
}

//Fet
void activar_leds(){ 
	if(LEDs > 1){
		if(!LATD.LED3){
			LATD >> 1;
			LATD.LED0 = 1;
		}else{
			LATB >> 1;
			LATB.LED4 = 1;
		}
	}else{
		LATD.LED0
	}
}

//Fet
void activacio_leds_circular(){
	ESTAT = ESTAT_LEDS_CIRCULAR;
	LATB = 0;
	LATD = 0;
	tempsUn = 0;
	index = 1;
}

//Fet
void activar_leds_circular(){ 
	if(tempsUn > 100){
		if(dreta == 1){
			if(LATD > 0){
				LATD >> 1;
				LATD.LED3 = 0;
				if(LATD.Bit3){
					dreta = 0;
					LATD << 1;
				}
			}else{
				LATB >> 1;
				if(STATUS.C){
					LATD.LED3 = 1;
				}
			}
		}else{
			if(LATD > 0){
				LATD << 1;
				LATD.Bit3 = 0;
				if(STATUS.C){
					LATB.LED4 = 1;
					LATD = 0;
				}
			}else{
				LATB << 1;
				LATB.LED4 = 0;
				if(LATB.Bit6){
					LATB >> 1;
					dreta = 1;
				}
			}
		}
	}
}

//Fet
void dividir10(int num){
	num = ((num * 205).High); //Donat que 205/2048 dona 0,10009765, es una aproximacio a 0,10. Ens permet dividir qualsevol numero de 0-300 entre 10 amb un error d'un bit sense us de taules.
	(num >> 3); //El número maxim sera ‭1111 0000 0011 1100 (300d) ‬Agafarem el byte high i shiftarem 3 posicions es a dir: 1111 0000 (240d) -> 0001 1111 (30d) (Ja que 240/8 -> 30)
}

//Fet
void desar(){
	enviaConfirmacio();
	comptaBytes = 0;
	FSR0 = POSICIO_A_DESAR_RAM;
	goto bucle_desar();
	
}

//Fet
void bucle_desar(){
	if(PIR1.RCIF == 0){goto bucle_desar();} //Mentres no hem rebut res esperar
	if(RCREG == END_BYTE){ //Quan el ordinador ens envii el end byte pararem
		ESTAT = ESTAT_BLINKING_5HZ;
		return //Retornara al bucle principal ja que hem vingut a desar amb goto
	}
	POSTINC0 = RCREG;
	goto bucle_desar();
}

//Fet
void desar_sense_confirmacio(){
	comptaBytes = 0;
	FSR0 = POSICIO_A_DESAR_RAM;
	goto bucle_desar();
}
