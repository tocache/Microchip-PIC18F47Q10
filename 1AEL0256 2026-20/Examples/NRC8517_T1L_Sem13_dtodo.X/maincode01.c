/*
 * File:   maincode01.c
 * Author: Alumnos
 *
 * Created on 19 de noviembre de 2026, 09:10 AM
 */
#include <xc.h>
#include "cabecera.h"
#include "i2c_lcd.h"
#define _XTAL_FREQ 4000000UL

void configuro(void){
    //conf mod osc
    OSCCON1 = 0x60;
    OSCFRQ = 0x02;
    OSCEN = 0x40;
    //inicializar i2c_lcd
    LCD_Init();
    //conf el adc
    FVRCON = 0x00;
    ADCON0 = 0x94;
    ADREF = 0x00;
    ADPCH = 0x00;
    //conf del dac
    DAC1CON0 = 0xA0;
}

unsigned int ADC_GetSample(void){
    ADCON0bits.GO_nDONE = 1;
    while(ADCON0bits.GO_nDONE);
    return (ADRESH << 8) + ADRESL;
}

void main(void) {
    configuro();
    LCD_SetCursor(0,0);     //primero es y luego es x
    LCD_PrintString("Hola upcino 2026");
    LCD_SetCursor(1,0);     //primero es y luego es x
    LCD_PrintString("Kalun Lau");
    __delay_ms(2000);
    LCD_Clear();
    while(1){
        LCD_SetCursor(0,0);     //primero es y luego es x
        LCD_PrintString("ADC CH0:");
        LCD_PrintInt(ADC_GetSample(),4);
        DAC1CON1 = ADC_GetSample() >> 5;
    }
}
