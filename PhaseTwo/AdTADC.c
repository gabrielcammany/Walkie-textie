#include "AdTADC.h"

void AdInit(void){
    TRISAbits.TRISA0 = 1;
    //AD1PCFGbits.PCFG0=1; //Per provar si escolta l'entrada digital

    AD1CON1bits.ADSIDL=1;   //Sempre converteix
    AD1CON1bits.FORM1=AD1CON1bits.FORM0=0; //Right aligned
    AD1CON1bits.SSRC0= AD1CON1bits.SSRC1=AD1CON1bits.SSRC2= 1; //Auto convert

    AD1CON2bits.VCFG = 0; // Referències VDD i VSS
    AD1CON2bits.CSCNA = 0; //No escanejis, concentra't en RA0!
    AD1CON2bits.BUFM = 1; //El buffer són 2 paraules de 8 bits

    AD1CON3 = 0x0100;

    AD1CHS= 0;
    AD1CSSL = 0x0001;

    AD1CON1bits.ADON=1;     //Activa't
    AD1CON1bits.SAMP = 1;
    AD1CON1bits.ASAM = 1;  //Després d'una conversió la següent

}

int AdGetMostra(void){
    return ADC1BUF0;
}