/*
 * File:   maincode.c
 * Author: Alumnos
 *
 * Created on 19 de noviembre de 2026, 08:02 AM
 */
#include <xc.h>
#include <string.h>
#include "cabecera.h"
#define _XTAL_FREQ 4000000UL

void configuro(void){
    //conf mod osc
    OSCCON1 = 0x60;
    OSCFRQ = 0x02;
    OSCEN = 0x40;
    //conf e-s
    ANSELB = 0xFC;
    //conf EUSART2 (para el Curiosity Nano)
    TX2STAbits.BRGH = 1;
    SP2BRG = 25;
    TX2STAbits.TXEN = 1;
    RC2STAbits.SPEN = 1;
    RX2PPS = 0x19;
    RD0PPS = 0x0B;
    ANSELDbits.ANSELD1 = 0;
    TRISDbits.TRISD0 = 0;
    ANSELDbits.ANSELD0 = 0;
}

void Serial2_SendByte(unsigned char dato){
    TX2REG = dato;
    while(TX2STAbits.TRMT == 0);
    //__delay_us(10);
}

void Serial2_SendString(const char *cadena){
    unsigned char tam_cad;
    tam_cad = strlen(cadena);
    for(unsigned char x_var=0;x_var<tam_cad;x_var++){
        Serial2_SendByte(cadena[x_var]);
    }
}

void Serial2_NewLine(void){
    Serial2_SendByte(0x0A);
    Serial2_SendByte(0x0D);
}

void main(void) {
    configuro();
    while(1){
        Serial2_SendString("Hola mundo!");
        Serial2_NewLine();
        Serial2_SendString("Programa hecho por Kalun");
        Serial2_NewLine();
        Serial2_SendString("Ingenieria Mecatronica Electronica y Biomedica");
        Serial2_NewLine();
        __delay_ms(1000);
    }
}
