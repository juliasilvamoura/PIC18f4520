/* 
 * File:   HELOWORD.c
 * Author: Julia e Igor
 *
 * Created on 20 de Setembro de 2019, 09:33
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <pic18f4520.h>
#include "configura.h"
#include "lcd.h"
#include "ARAGUA_Header_File.h"



void regressiva();
int ligando();
void cancelando();
void pausando();
#define _XTAL_FREQ 8000000
//#define cancelar PORTCbits.RC3
#define ligar PORTCbits.RC2
#define cancela PORTCbits.RC3
//#define tempoo PORTCbits.RC1 //conferir botão
#define botao PORTCbits.RC0

int cronometro = 0, incre = 0, dezena = 0;
char valor[5], chato[5];

void main(void) {
    config();
    lcd_init();
    ARAGUA_Init(9600);
    PORTB = 0;
    PORTD = 0;
    PORTE = 0;
    TMR0 = 6;
    LATA5 = 1;
    
    sprintf(valor, "%d:%d", dezena, incre);
    lcd_goto(3);
    lcd_puts(valor);
    
    while (1) {
        if (!PORTCbits.RC1) {
            __delay_ms(5);
            if (!PORTCbits.RC1) {
                while (!PORTCbits.RC1);
            }
        }
        if (!cancela) {
            cancelando();// se apertar o botão cancelar 
            //antes dele estar rodando ele zera tudo
        }
        if (!botao) {
            __delay_ms(5);
            if (!botao) {
                while (!botao);
                lcd_clear();// limpa o LCD
                lcd_goto(3); // deixa a escrita primeira linha na columa do meio
                incremento();//chama incremento 
                //se o botão para add o tempo for apertado
            }
        }
        if (!ligar) {
            incre++; // se apertar o botão ligar a variavel incremento recebe +1
            regressiva();// chama função regressiva
        }
    }
}

__interrupt(high_priority) void ISR1(void) {

    while (RCIF == 0);
    char ju = (char) RCREG;
    TXREG = ju;
    if (ju == 'L') { 
        regressiva();
    }
    if (ju == 'T') {
        incremento();
    }
    if(ju=='P'){
        pausando();
    }
    if(ju=='C'){
        cancelando();
    }
        
    
}

int incremento() {
    GIE = 1;
    incre = incre + 10; //incremento recebe 10 quando botão for apertado
    if (incre == 60) { // se incre for = a 60 
        incre = 0;// zera incre
        dezena++; // adiciona +1 na variavel dezena
    }
    sprintf(valor, "%d:%d", dezena, incre);//aloca o valor de dezena
    //e incre na variavel tipo char valor
    
    lcd_clear(); // limpa LCD
    lcd_goto(3); //posição que vai escrever no LCD
    lcd_puts(valor); // imprime valor no LCD
    return incre; // retorna o valor de incre
}

int ligando() { // quando chamar a função ligando
    incre--; // decrementa 1 na variavel incre 
    return incre; // retorna o valor de incre
}

void cancelando(){
            incre = 0;// zera incre
            dezena = 0; // zera dezena
            lcd_clear(); 
            lcd_goto(3);
            sprintf(valor, "%d:%d", dezena, incre); // passa dezena e incre pra valor
            lcd_puts(valor); // imprime valor que será 0:0
    
}

void pausando(){
    GIE = 1;
    lcd_goto(3);
    lcd_puts(chato);
    int aok = 1; // variavel aok recebe 1
        while (aok == 1) { // enquanto a variavel for 1
            if (!cancela != 1) { // e se o botão cancela for  diferente de 1
                aok = 0; // variavel aok recebe 0
            }
        }
        while (!cancela != 1) // enquanto o botão cancela for diferente de 1
            if (!ligar) { // e se botão liga for apertado
                lcd_clear(); // limpa LCD
                lcd_goto(3);
                break; // volta a contar o valor normalmente
                }
        }
    


void regressiva() {
    {
        GIE = 1;
        while (incre != 0) {
            lcd_clear();
            lcd_goto(3);
            ligando(incre);            
           // cancelando();?            
            if (!cancela) {
               pausando();               
                if (!cancela) {
                    cancelando();
                }
            }
            if (incre == 0) {
                if (dezena >= 1) {
                    dezena--;
                    incre = 59;
                }
            }
            if (incre == 0 && dezena == 0) {
                RB7 = !RB7; // acende led se a comida já terminou de esquentar, ou seja timer = 0
                lcd_clear();
                lcd_goto(3);
                lcd_puts("PRONTO"); // fala que ta pronto 

            }  
            else {
                sprintf(chato, "%d:%d", dezena, incre); // senão continua a contagem recressiva 
                lcd_goto(3);
                lcd_puts(chato); // imprime valor
                __delay_ms(1000); // pra contar em 1 segundo
            }
        }
    }
}


              //            if (incre >= 0 && (dezena - 1) > 0) {
                //
                //                incre = 59;
                //                dezena--;
                //            }