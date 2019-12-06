

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <pic18f4520.h>
#include "ARAGUA_Header_File.h"
#define F_CPU 8000000/64

void ARAGUA_Init(int baud_rate) {
    float kk; 
    TRISC6 = 0; // envia
    TRISC7 = 1; //recebe
    kk = (((float) (F_CPU)/ (float) baud_rate)-1);
    SPBRG = (int) kk;
    TXSTA = 0x20;
    RCSTA = 0x90;
    INTCONbits.GIE =1;
    INTCONbits.PEIE =1;
    PIE1bits.RCIE = 1;
}
void ARAGUA_TxChar(char out)
{
    while(TXIF == 0);
    TXREG = out;
}
void ARAGUA_SendString(const char *out){
    while(*out != '\0'){
        ARAGUA_TxChar(*out);
        out++;
    }
}

