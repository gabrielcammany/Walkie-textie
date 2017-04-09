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
    
RAM ORG 0x01    

 
;*************
;* CONSTANTS *
;*************


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
    
    
    movlw 0x00
    cpfseq FINAL, 0
    call BLINKING
    
    
    
    retfie FAST;2
    
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
;todos esp -2!!!

    
MAIN
    call INIT_VARS
    call INIT_PORTS
    call INIT_TIMER
    
LOOP
    goto LOOP
    
      
	
;*******
;* END *
;*******    
    END