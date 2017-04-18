LIST P=18F4321, F=INHX32
#include <P18F4321.INC>

;******************
;* CONFIGURACIONS *
;******************
CONFIG OSC = HSPLL          
CONFIG PBADEN = DIG
CONFIG WDT = OFF

;*************
;* VARIABLES *
;*************
    
ESTAT EQU 0x02   
TEMPS_UN EQU 0x03    
COMPTA_BYTES EQU 0x04 
LEDS EQU 0x05  
RESULTAT_DIVISIO EQU 0x06
INDEX EQU 0x07   
AUXILIAR EQU 0x08   
ENVIAT EQU 0x09  

 
;*************
;* CONSTANTS *
;*************
ZERO EQU 0x00
POLS_CARREGA EQU 0x05   
POLS_ENVIA_RF EQU 0x04 
BIT_RESPOSTA_PC EQU 0x08  
 
FLAG_DESAR_MSG EQU 0x81 
FLAG_DESAT_MSG EQU 0x85
FLAG_BYTE_ENVIAT_MSG EQU 0x86
FLAG_ENVIAR_RF_MSG EQU 0x82 
FLAG_CONFIRMACIO_MSG EQU 0x83
FLAG_DESAR_SENSE_CONFIRMACIO_MSG EQU 0x84

ESTAT_LEDS_CIRCULAR EQU 0x02
ESTAT_POLSADOR EQU 0x01
ESTAT_BLINKING_10HZ EQU 0x04
ESTAT_BLINKING_5HZ EQU 0x08

POSICIO_A_DESAR_RAM EQU 0x80
END_BYTE EQU 0x88
MAX_LEDS EQU 0x0B
 
VALOR_A_MULTIPLICAR EQU 0xCD ;205
 
TEMPS_100_MSEG EQU 0x14 ;20
TEMPS_200_MSEG EQU 0x28 ;40
TEMPS_400_MSEG EQU 0x50 ;80
TEMPS_500_MSEG EQU 0x34 ;100 - 0x64
TEMPS_1000_MSEG EQU 0xC8 ;200
 
LED0 EQU 0x04
LED1 EQU 0x05
LED2 EQU 0x06
LED3 EQU 0x07
LED4 EQU 0x00
LED5 EQU 0x01
LED6 EQU 0x02
LED7 EQU 0x03
LED8 EQU 0x04
LED9 EQU 0x05 
 
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
    incf TEMPS_UN,1,0
    ;bsf LATB,5,0
    retfie FAST;2
  
    
;****************************
;* MAIN I RESTA DE FUNCIONS *
;****************************

    
INIT_VARS
    clrf ESTAT,0
    ;clrf TEMPS_UN,0
    ;clrf COMPTA_BYTES,0
    ;clrf LEDS,0  
    ;clrf RESULTAT_DIVISIO,0
    ;clrf INDEX,0  
    ;clrf AUXILIAR,0   
    ;clrf ENVIAT,0 
    ;clrf TEMPS_UN,0
    return;2
    
INIT_SIO
    ;1001 0000 - 0x90
    movlw 0x90
    movwf RCSTA,0
    ;0010 0110 - 0x26
    movlw 0x26
    movwf TXSTA,0
    movlw 0x81
    movwf SPBRG,0
    clrf SPBRGH,0
    clrf BAUDCON, 0
    return
    
INIT_PORTS
    ;clrf TRISA, 0
    bcf TRISC, 5, 0	;Bit RF
    bsf TRISC, 7, 0	;RX Eusart
    bsf TRISC, 6, 0	;TX Eusart
    
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
    ;88
    movlw 0x88
    movwf T0CON,0
    bcf RCON, IPEN, 0
    
    ;10100000
    ;E0
    movlw 0xE0
    movwf INTCON, 0
    
    call RESET_TIMER
    return
    
RESET_TIMER
    ;CF
    ;3A
    movlw 0x3C
    movwf TMR0H, 0
    ;30
    ;98
    movlw 0xA6
    movwf TMR0L, 0
    bcf INTCON, TMR0IF, 0
    return
    
;********
;* MAIN *
;******** 

    
MAIN
    call INIT_VARS
    call INIT_PORTS
    call INIT_SIO
    call INIT_TIMER
    
LOOP
    btfsc PIR1,RCIF,0
    call REBUT
    
    btfsc LATC,4,0
    call POLS_CARREGA_MISSATGE
    
    btfsc LATC,3,0
    call POLS_ENVIAR_RF
    
    btfsc ESTAT, 1,0
    call LEDS_CIRCULAR
    
    btfsc ESTAT, 0,0
    call ESPERA_POLSADOR
    
    btfsc ESTAT, 2,0
    call BLINKING_10HZ
    
    btfsc ESTAT, 3,0
    call BLINKING_5HZ
    
    goto LOOP

;**************** - BLOC RF - ****************************
    
ENVIAR_RF
    movlw ZERO
    cpfsgt COMPTA_BYTES, 0
    goto ACTIVACIO_LEDS_CIRCULAR ;Activar leds circular
    clrf LATB, 0
    clrf LATD, 0
    clrf LEDS, 0
    clrf ENVIAT,0
    clrf AUXILIAR,0
    incf LEDS, 1, 0 
    call ENVIAR_CONFIRMACIO_RF
    call DIVIDIR_10
    clrf INDEX,0
    movlw POSICIO_A_DESAR_RAM
    movwf FSR0,0

BUCLE_LEDS_ENVIAR_RF
    movlw MAX_LEDS
    cpfslt LEDS,0
    goto FINAL_RF

BUCLE_INDEX_DIVISOR_ENVIAR_RF 
    movlw 0x00
    cpfsgt RESULTAT_DIVISIO,0
    call ENVIA_WORK
    movf RESULTAT_DIVISIO,0,0
    cpfslt INDEX,0 ;Mentre INDEX < RESULTAT_DIVISIO ens quedem aqui
    goto FINAL_BUCLE_INDEX_DIVISOR ;Quan sigui igual, incrementarem variable leds i activarem leds.
    movff POSTINC0, AUXILIAR
    clrf TEMPS_UN,0
    
BUCLE_ENVIAR_8_BITS ;Ens quedarem aqui fins que no hem enviat 8 bits
    movlw 0x08 
    cpfslt ENVIAT,0
    goto FINAL_BUCLE_ENVIAR ;Incrementarem index
    btfsc TEMPS_UN, 0,0 
    goto ENVIAR_BIT_PRIMERA_MEITAT ;Primers 5 mseg
    btfsc TEMPS_UN, 1,0
    goto ENVIAR_BIT_SEGONA_MEITAT ;Segon 5 mseg
    goto BUCLE_ENVIAR_8_BITS ;Tornem al inici del bucle   
    
FINAL_BUCLE_ENVIAR 
    incf INDEX, 1,0
    clrf ENVIAT,0
    goto BUCLE_INDEX_DIVISOR_ENVIAR_RF ;Tornem al seguent 10 percent
    
FINAL_BUCLE_INDEX_DIVISOR
    incf LEDS,1,0
    call ACTIVAR_LEDS_PROCES
    clrf INDEX, 0
    goto BUCLE_LEDS_ENVIAR_RF

ENVIAR_BIT_PRIMERA_MEITAT
    btfsc AUXILIAR,0,0 ;Mirem si el primer bit es 0, si ho es fiquem la primera part a 0
    bcf LATC, 5, 0
    btfss AUXILIAR,0,0 ;Si el primer es 0 fiquem la primera part a 1 
    bsf LATC, 5, 0
    goto BUCLE_ENVIAR_8_BITS
    
ENVIAR_BIT_SEGONA_MEITAT
    btfsc AUXILIAR,0,0 ;Mirem si el primer bit es 0, si ho es fiquem la segona part a 1
    bsf LATC, 5, 0
    btfss AUXILIAR,0,0 ;Si el primer es 1 fiquem la segona part a 0
    bcf LATC, 5, 0
    incf ENVIAT,1,0
    rrncf AUXILIAR,1,0
    call ENVIAR_CONFIRMACIO_BYTE_ENVIAT
    clrf TEMPS_UN,0
    goto BUCLE_ENVIAR_8_BITS
    
ACTIVAR_LEDS_PROCES    
    movlw 0x01
    cpfslt LEDS,0
    goto LEDS_PROCES_MAJOR_1
    bsf LATD, LED0, 0
    return

LEDS_PROCES_MAJOR_1
    btfsc LATD, LED3,0
    goto LEDS_PROCES_LATB
    rlncf LATD, 1,0
    bsf LATD, LED0, 0
    return 

LEDS_PROCES_LATB
    btfss LATB, LED4,0
    goto LEDS_PROCES_LATB_INICI
    rlncf LATB, 1,0
    bsf LATB, LED4, 0
    return
    
LEDS_PROCES_LATB_INICI
    bsf LATB, LED4, 0
    return

FINAL_RF
    movlw ESTAT_BLINKING_10HZ 
    movwf ESTAT,0
    call ENVIAR_END_BYTE
    clrf TEMPS_UN,0
    return
    
    
;***********************************************************   
    
REBUT
    ;clrf TEMPS_UN
    btfss RCREG, 7,0 ;Qualsevol byte rebut que tingui aquest bit a 1 es refereix a una resposta del pc
    return ;Si rebem un byte i no esta activat no hauriem dentrar aqui
    movlw FLAG_DESAR_MSG ;Valor del RCREG per enviar per RF
    cpfsgt RCREG,0 ;Si es mes gran no desara el missatge
    goto DESA ;Desem els bytes
    movlw FLAG_ENVIAR_RF_MSG ;Valor del RCREG com a confirmacio del PC per enviar dades
    cpfsgt RCREG,0
    goto ENVIAR_RF ;Enviar dades per RF
    movlw FLAG_DESAR_SENSE_CONFIRMACIO_MSG ;Valor del RCREG com a confirmacio del PC per enviar dades
    cpfsgt RCREG,0
    goto DESA_SENSE_CONFIRMACIO ;Desar dades sense enviar confirmacio al PC ja que ja l'hem rebut
    return
 
;**************** - BLOC DESAR - ****************************
	
DESA
    clrf COMPTA_BYTES; 
    call ENVIAR_CONFIRMACIO_DESAR

DESA_SENSE_CONFIRMACIO ;Quan ens apreten el boto no necessitem enviar al pc la confirmacio
    movlw POSICIO_A_DESAR_RAM
    movwf FSR0,0
    
BUCLE_DESAR
    btfss PIR1,RCIF,0
	goto BUCLE_DESAR ;Mentres no valgui 1 el bit RCIF que ens indica que hi ha un byte ens esperem
    movlw END_BYTE 
    cpfslt RCREG, 0 ;Si rebem el byte de final del ordinador sortim, no el desem
	goto RETORNA_DESAR 
    movff RCREG, POSTINC0 ;Movem el caracter a la posicio de la ram corresponent
    incf COMPTA_BYTES,1,0 ;Incrementem en numero de bytes rebut
    call ENVIAR_CONFIRMACIO_DESAT ;Confirmarem al ordinador que ho hem desat
    goto BUCLE_DESAR ;Esperem una nova dada
    
RETORNA_DESAR
    movlw ESTAT_BLINKING_5HZ 
    movwf ESTAT,0
    clrf TEMPS_UN,0
    ;call ACTIVACIO_LEDS_CIRCULAR
    return
    
;***********************************************************    
    

;*********** - BLOC COMUNICACIO PC - *****************
    
ENVIAR_CONFIRMACIO_RF 
    movlw FLAG_ENVIAR_RF_MSG
    movwf TXREG,0
    btfss TXSTA,TRMT,0
    call ESPERA
    return
    
ENVIAR_CONFIRMACIO_DESAT
    ;movlw FLAG_DESAT_MSG
    movf COMPTA_BYTES,0,0
    movwf TXREG,0
    call ESPERA
    return
    
ENVIA_WORK
    movlw 0x13
    movwf TXREG,0
    call ESPERA
    movf RESULTAT_DIVISIO,0,0
    movwf TXREG,0
    call ESPERA
    movlw 0x14
    movwf TXREG,0
    call ESPERA
    return
    
    
ENVIAR_CONFIRMACIO_BYTE_ENVIAT
    movlw FLAG_BYTE_ENVIAT_MSG
    movwf TXREG,0
    call ESPERA
    return
    
ENVIAR_CONFIRMACIO_DESAR
    movlw FLAG_DESAR_MSG
    movwf TXREG,0
    call ESPERA
    return
    
ENVIAR_END_BYTE
    movlw END_BYTE
    movwf TXREG,0
    call ESPERA
    return
    
ESPERA
    btfss TXSTA,TRMT,0
    goto ESPERA
    return
    
ENVIAR_PETICIO_DESAR
    clrf AUXILIAR
    clrf TEMPS_UN
    movlw FLAG_DESAR_SENSE_CONFIRMACIO_MSG
    movwf TXREG,0
    btfss TXSTA,TRMT,0
    call ESPERA
    movlw ESTAT_POLSADOR
    movwf ESTAT,0
    return
    
;***********************************************************    


;***************** - BLOC DIVIDIR - ************************
    
DIVIDIR_10
    clrf RESULTAT_DIVISIO,0
    movlw VALOR_A_MULTIPLICAR
    mulwf COMPTA_BYTES,0
    movff PRODH, RESULTAT_DIVISIO
    rrncf RESULTAT_DIVISIO,1,0
    rrncf RESULTAT_DIVISIO,1,0
    rrncf RESULTAT_DIVISIO,1,0
    movlw 0x1F ;Assegurarnos que els bits que hem afegit son 0
    andwf RESULTAT_DIVISIO,1,0
    return
    
;***********************************************************  
    

;***************** - BLOC LEDS - ***************************
    

ACTIVACIO_LEDS_CIRCULAR
    movlw ESTAT_LEDS_CIRCULAR
    movwf ESTAT,0
    clrf LATB,0
    clrf LATD,0
    clrf TEMPS_UN,0
    clrf INDEX,0
    bsf LATD, LED0, 0 ;Activem el primer bit del LATB
    ;bcf INDEX, 0,0
    return
    
LEDS_CIRCULAR
   movlw TEMPS_500_MSEG
   cpfslt TEMPS_UN,0
   goto ACTIVAR_LEDS_CIRCULAR
   return
 
ACTIVAR_LEDS_CIRCULAR
   clrf TEMPS_UN,0
   btfss INDEX,0,0 ;Comprovem si estem dreta o no
   goto LEDS_CIRCULAR_ESQUERRA
   
LEDS_CIRCULAR_DRETA
   ;movff INDEX, LATB
   movlw 0x00
   cpfsgt LATD,0 ;Comprovem que latd no estigui a 0 
   goto LEDS_CIRCULAR_DRETA_LATB ;Si ho esta hem de canviar de registre
   rrcf LATD,1,0 ;Rotem el LATD cap a la dreta
   bcf LATD, LED3, 0
   btfsc LATD, 3,0 ;Si ja tenim carry del primer registre hem danar al seguent
   goto LEDS_CIRCULAR_DRETA_ESQUERRA
   return

LEDS_CIRCULAR_DRETA_LATB
    rrcf LATB,1,0 
    bcf LATB,LED9,0
    btfsc STATUS, C,0 ;Si ja hem arribat al final ja no podem anar mes cap a la dreta
    goto LEDS_CIRCULAR_DRETA_SEGONA_PART  
    return ;Tornem al loop principal
    
LEDS_CIRCULAR_DRETA_ESQUERRA
    clrf INDEX,0 ;Borrem index per decidir que anirem cap a lesquerra
    clrf LATD, 0 
    bsf LATD,LED0,0
    return ;Tornem al bucle principal
   
LEDS_CIRCULAR_DRETA_SEGONA_PART
   bsf LATD, LED3, 0
   clrf LATB,0
   return    
    
    
    
LEDS_CIRCULAR_ESQUERRA
   movlw 0x00
   cpfsgt LATD,0
   goto LEDS_CIRCULAR_ESQUERRA_LATB
   rlcf LATD,1,0
   bcf LATD, 3, 0
   btfsc STATUS, C,0
   goto LEDS_CIRCULAR_ESQ_SEGONA_PART
   return
   
LEDS_CIRCULAR_ESQUERRA_DRETA
    bsf INDEX,0,0 ;Ativem index per decidir que anirem cap a la dreta
    rrcf LATB,1,0
    clrf LATD,0
    return ;Tornem al bucle principal
   
LEDS_CIRCULAR_ESQUERRA_LATB
   rlcf LATB,1,0
   bcf LATB,LED4,0
   btfsc LATB,6,0
   goto LEDS_CIRCULAR_ESQUERRA_DRETA
   return
   
LEDS_CIRCULAR_ESQ_SEGONA_PART
   bsf LATB, LED4,0
   clrf LATD,0
   return
   
   
BLINKING_10HZ
    movlw TEMPS_100_MSEG
    cpfseq TEMPS_UN,0 ;Si estem als 100 no els encendrem
    goto COMPROVA_APAGAR_10HZ ;En cas de estar per sota o sobre entrarem aqui
    goto ENCENDRE_LEDS
    return
   
COMPROVA_APAGAR_10HZ
    movlw TEMPS_200_MSEG
    cpfslt TEMPS_UN,0 ;Si estem en menys de 200 no farem res, sino els apagarem
    goto APAGAR_LEDS
    return
    
BLINKING_5HZ
    movlw TEMPS_200_MSEG
    cpfseq TEMPS_UN,0 ;Si encara no estem als 200 mseg no els encendrem
    goto COMPROVA_APAGAR_5HZ ;Si estem per sobre o sota entrarem aqui
    goto ENCENDRE_LEDS
    return

COMPROVA_APAGAR_5HZ
    movlw TEMPS_400_MSEG
    cpfslt TEMPS_UN,0 ;En cas de arribar ja als 400 els apagarem
    goto APAGAR_LEDS
    return
  
ENCENDRE_LEDS
    ;movlw 0x3F
    setf LATB,0
    ;movlw 0x0F
    setf LATD,0
    return
    
APAGAR_LEDS
    clrf TEMPS_UN,0
    clrf LATB,0
    clrf LATD,0
    return
    
;***********************************************************  
    
;***************** - BLOC POLSADORS - ***************************
    
POLS_CARREGA_MISSATGE
    ;goto ENVIAR_PETICIO_DESAR
    return
    
POLS_ENVIAR_RF
    ;goto ENVIAR_RF
    return

ESPERA_POLSADOR
    movlw TEMPS_1000_MSEG
    cpfslt TEMPS_UN
    goto COMPROVACIO_10_SEG
    return
    
COMPROVACIO_10_SEG
    incf AUXILIAR,1,0
    movlw 0x32 ;50 ja que 50*200 son 10k
    cpfslt AUXILIAR,0 ;Si encara no hem arribat a 50 encara ens hem desperar
    call ACTIVACIO_LEDS_CIRCULAR
    return
    
INCREMENTAR_SENSE_LEDS
    incf AUXILIAR,1,0
    return
    
;***********************************************************  
    
;*******
;* END *
;*******    
    END