LIST P=18F4321, F=INHX32
#include <P18F4321.INC>	

;******************
;* CONFIGURACIONS *
;******************
CONFIG	OSC = HSPLL          
CONFIG	PBADEN = DIG
CONFIG	WDT = OFF

;*************
;* VARIABLES *
;*************
    
ESTAT ORG 0x02   
TEMPS_UN ORG 0x03     

 
;*************
;* CONSTANTS *
;*************
POLS_CARREGA ORG 0x05   
POLS_ENVIA_RF ORG 0x04 
BIT_RESPOSTA_PC ORG 0x08   
FLAG_DESAR_MSG ORG 0x81
FLAG_ENVIAR_RF_MSG ORG 0x82 
FLAG_CONFIRMACIO_MSG ORG 0x83
ESTAT_LEDS_CIRCULAR ORG 0x04
ESTAT_POLSADOR ORG 0x01
ESTAT_BLINKING_10HZ ORG 0x08
POSICIO_A_DESAR_RAM ORG 0x80
END_BYTE ORG 0x88
MAX_LEDS ORG 0x0B
LED0 ORG 0x04
LED1 ORG 0x05
LED2 ORG 0x06
LED3 ORG 0x07
LED4 ORG 0x00
LED5 ORG 0x01
LED6 ORG 0x02
LED7 ORG 0x03
LED8 ORG 0x04
LED9 ORG 0x05

;*********************************
; VECTORS DE RESET I INTERRUPCIï¿½ *
;*********************************
    ORG 0x000000
RESET_VECTOR
    goto MAIN		

    ORG 0x000008
HI_INT_VECTOR
    goto HIGH_INT	

    ORG 0x000018
LOW_INT_VECTOR
    retfie FAST		


;***********************************
;* RUTINES DE SERVEI D'INTERRUPCIï¿½ *
;***********************************
HIGH_INT
    call RESET_TIMER
    
    retfie FAST;2
    
ENVIAR_BIT_RF
    btfsc AUX,0,0
    call POSITIVE_BIT
    btfss AUX,0,0
    call NEGATIVE_BIT
    
    
;****************************
;* MAIN I RESTA DE FUNCIONS *
;****************************

    
INIT_VARS
    return;2
    
INIT_SIO
    
    ;1001 0000 - 0x90
    movlw 0x90
    movwf TXSTA,0
    ;0010 0110 - 0x26
    movlw 0x26
    movwf RCSTA,0
    movlw 0x81
    movwf SPBRG,0
    clrf SPBRGH,0
    clrf BAUDCON, 0
    bcf PIE1, RCIE, 0
    bcf PIE1, TXIE, 0
    
    
    
    return
    
INIT_PORTS
    ;clrf TRISA, 0
    bsf TRISC, 7, 0	;RX Eusart
    bcf TRISC, 6, 0	;TX Eusart
    
    bcf TRISD, 4, 0	;LED0
    bcf TRISD, 5, 0	;LED1
    bcf TRISD, 6, 0	;LED2
    bcf TRISD, 7, 0	;LED3
    
    bcf TRISB, 0, 0	;LED4
    bcf TRISB, 1, 0	;LED5
    bcf TRISB, 2, 0	;LED6
    bcf TRISB, 3, 0	;LED7
    bcf TRISB, 4, 0	;LED8
    bcf TRISB, 5, 0	;LED9
        
    return
    
INIT_TIMER
    ;10001000
    movlw 0x88
    movwf T0CON,0
    bcf RCON, IPEN, 0
    
    ;11100000
    movlw 0xE0
    movwf INTCON, 0
    
    call RESET_TIMER
    return
    
RESET_TIMER
    movlw 0x3C
    movwf TMR0H, 0
    movlw 0xB0
    movwf TMR0L, 0
    bcf INTCON, TMR0IF, 0
    return
    
;********
;* MAIN *
;******** 

    
MAIN
    call INIT_VARS
    call INIT_PORTS
    call INIT_TIMER
    
LOOP
    btfsc PIR1,RCIF,0
    call REBUT
    
    btfsc LATC,POLS_CARREGA,0
    call POLS_CARREGA_MISSATGE
    
    btfsc LATC,POLS_RF,0
    call POLS_ENVIAR_RF
    
    btfsc ESTAT,ESTAT_LEDS_CIRCULAR,0
    call LEDS_CIRCULAR
    
    btfsc ESTAT,ESTAT_POLSADOR,0
    call ESPERA_POLSADOR
    
    btfsc ESTAT,ESTAT_BLINKING_10HZ,0
    call BLINKING_10HZ
    
    goto LOOP
    
ENVIAR_RF
      
    
REBUT
    clrf TEMPS_UN
    btfsc RCREG, BIT_RESPOSTA_PC,0 //Qualsevol byte rebut que tingui aquest bit a 1 es refereix a una resposta del pc
    return //Si rebem un byte i no esta activat no hauriem dentrar aqui
    movlw FLAG_ENVIAR_RF_MSG //Valor del RCREG per enviar per RF
    cpfslt RCREG,0
    goto DESA //Desem els bytes
    movlw FLAG_CONFIRMACIO_MSG //Valor del RCREG com a confirmacio del PC per enviar dades
    cpfseq RCREG,0
    goto ENVIAR_RF //Enviar dades per RF
    goto DESA_SENSE_CONFIRMACIO //Desar dades sense enviar confirmacio al PC ja que ja l'hem rebut
    return
 
	
DESA
    call ENVIAR_CONFIRMACIO

DESA_SENSE_CONFIRMACIO
    clrf COMPTA_BYTES;
    movlw POSICIO_A_DESAR_RAM
    movwf FSR0,0
    
BUCLE_DESAR
    btfss PIR1,RCIF,0
	goto BUCLE_DESAR //Mentres no valgui 1 el bit RCIF que ens indica que hi ha un byte ens esperem
    movlw END_BYTE 
    cpfslt RCREG, 0 //Si rebem el byte de final del ordinador sortim, no el desem
	return
    movff RCREG, POSTINC0,0 //Movem el caracter a la posicio de la ram corresponent
    incf COMPTA_BYTES //Incrementem en numero de bytes rebut
    goto BUCLE_DESAR //Esperem una nova dada
    
    
    
;*******
;* END *
;*******    
    END