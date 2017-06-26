#include "SwTSwitch.h"

void SwInit(){
    TRISBbits.TRISB13 = 1;
    TRISBbits.TRISB12 = 1;
    AD1PCFGbits.PCFG11 = 1; //No vull entrada analògica (AN11)!!
    AD1PCFGbits.PCFG12 = 1; //No vull entrada analògica (AN12)!!
    CNPU1bits.CN13PUE = 1;
    CNPU1bits.CN14PUE = 1;
}

char getSwitch1(){
    return (PORTBbits.RB12 == 1);
}

char getSwitch2(){
    return (PORTBbits.RB13 == 1);
}
