 
int temps = 0x00, comptaBytes = 0, espera = 0, aux = 0x00, LEDs = 0x01;

void main(){
	while(1){
		if(PIE1.RCIF == 1){
			if(RCREG == 0x99){ //Ens diu el ordinador que ens enviaran les dades ara
				enviaConfirmacio();
				comptaBytes = 0;
				FSR0 = 0x80;
			}else if(RCREG == 0x96){ //Ens demana el ordinador enviar per RF
				enviaConfirmacio();
				status = 0x02;
				enviaRF();
				status = 0x03;
			}else if(RCREG == 0x97){ //Rebre confirmacio quan ens apreten el Polsador i el ordinador començara a enviar les dades
				status = 0x00;
				temps = 0x00;
			}else{
				comptaBytes++; 
				RCREG -> POSTINC0;
			}
		}
		if(polsadorCarregaMissatge == 1){
			status = 0x01;
			temps = 0x00;
			enviaPeticioMissatge();
		}
		if(polsadorEnviaRF == 1){
			if(comptaBytes != 0x00){
				enviaRF(comptaBytes);
			}else{
				LEDs = 0x20;
				status = 0x04;
			}
		}
		if(status == 0x04){ //Leds circular
			ledsCircular();
		}
	}
}


void HighInt(){
	resetTimer();
	if(status == 0x02){ //Enviar per RF
		if(AUX.Bit0 == 1){ //Si el bit a enviar es 1
			if(tempsUn == 0){ 
				clrBit(LATC5.Bit5); //La primera part a 0
			}else{
				setBit(LATC5.Bit5); //La segona part a 1
				enviat++; //Hem de tenir un recompte dels bits enviats per anar a la seguent dada en la ram un cop enviats 8 
			}
		}
		if(AUX.Bit0 == 0){
			if(tempsUn == 0){ 
				setBit(LATC5.Bit5); //La primera part a 0
			}else{
				clrBit(LATC5.Bit5); //La segona part a 1
				enviat++; //Hem de tenir un recompte dels bits enviats per anar a la seguent dada en la ram un cop enviats 8 
			}
		}
	}
	tempsUn++;
	if(tempsUn >= 200){ //Compte 1 segon
		if(status == 0x01)temps++; 
		tempsUn = 0;
	}
	if(status == 0x03){ //Blinking 10 Hz
		if(tempsUn == 20){
			encendreLEDS(); //Encendre i apagar els leds
		}
		if(tempsUn == 40){
			apagarLEDS();
			tempsUn = 0x00;
		}
	}

}

void ledsCircular(){
	if(tempsUn
	
}

void encendreLEDS(){
	0xFF -> LATB;
	0xFF -> LATD;
}

void apagarLEDS(){
	0xC0 -> LATB;
	0xF0 -> LATD;
}


void enviaRF(int num){
	result = dividir10(num);
	char i;
	while(LEDs < 0x0B){ //Hi hauran 10 leds i anira 10-1
		while(i<result){ // Result es el numero de bytes total entre 10
			POSTINC0 -> AUX; //Afegim el valor de la ram al auxiliar
			tempsUn = 0x00; // Per tal de fer el temps a 0 i 1 amb la codificacio manchester, fiquem tempsUn a 0 per quan salti la interrupcio
			while(enviat<0x08){ //Enviat son els 8 bits que hem denviar
				if(tempsUn >0x01){ //Si ja ha passat 10 ms hem de enviar el seguent bit
					AUX >> 1; //Rotem al seguent bit
					tempsUn = 0x00; //Reiniciem el tempsUn per tal de enviar el seguent bit amb la codificacio manchester
				}
			}
			i++;
		}
		LEDs++; //Va del 1-10
		i == LEDs;
		while(i>=0){ // Activara els LEDs del 0 - 9
			i--; //Restem al principi per ferho del 0-9
			if(i<6){ //Els leds del LATB 5-4-3-2-1-0
				setBit(LATB(i));
			}else{ //Leds del LATD, 9-8-7-6
				i = i - 2; //Restem 2 ja que els ports sera del 7-6-5-4
				setBit(LATD(i));
				i = i + 2;
			}
		}
	}
	
}

void dividir10(int num){
	num = ((num * 205).High);
	(num >> 3);
}

void enviaPeticioMissatge(){
	enviaPC(0x91);
}

