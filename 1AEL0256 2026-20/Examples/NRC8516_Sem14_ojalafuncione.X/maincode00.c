/*
 * File:   maincode00.c
 * Author: SURFACE
 *
 * Created on 23 de noviembre de 2026, 05:34 PM
 */
#include <xc.h>
#include <stdio.h>
#include <string.h>
#include "cabecera.h"
#include "i2c_lcd.h"
#include "lib_uart.h"
#include "dht22.h"
#include "ds18b20.h"
#define _XTAL_FREQ 32000000UL

void configuro(void){
    //conf del modulo de oscilador
    OSCCON1 = 0x60;
    OSCFRQ = 0x06;
    OSCEN = 0x40;
    while (!OSCCON3bits.ORDY);  //para segurarse de que el HFINTOSC este funcioonando correctamente
    //inicializacion del EUSART2
    EUSART2_INIT(BAUD_9600);
    //inicializacion del DHT22
    LCD_Init();
    DS18B20_Init();
    DHT22_Init();
}

void main(void) {
    float temp = 0.0f, hum = 0.0f;
    char buffer[48];
    int16_t raw_temp;
    uint8_t rom[8];
    char serial_str[13];
    configuro();
    EUSART2_STRING_SEND("Hola mundo!");
    EUSART2_NEWLINE();
    LCD_SetCursor(0,0);
    LCD_PrintString("Hola ultimo lab");
    LCD_SetCursor(1,0);
    LCD_PrintString("UPC MecaEleBio");
    __delay_ms(3000);
    LCD_Clear();
    while(1){
        uint8_t estado = DHT22_Read(&temp, &hum);
        if (estado == DHT22_OK) {
            sprintf(buffer, "DHT22 Temp: %.1f C | Hum: %.1f %%\r\n", temp, hum);
            EUSART2_PrintString(buffer);
            sprintf(buffer, "T:%.1f%cC", temp, 0xDF);
            LCD_SetCursor(0,0);
            LCD_PrintString(buffer);
            sprintf(buffer, "H:%.1f%%", hum);
            LCD_SetCursor(0,9);
            LCD_PrintString(buffer);
        } else if (estado == DHT22_ERR_TIMEOUT) {
            EUSART2_PrintString("Error: DHT22 no responde\r\n");
        } else {
            EUSART2_PrintString("Error: Checksum invalido\r\n");
        }
        if (DS18B20_ReadTempRaw(&raw_temp)) {
            char signo = ' ';
            uint16_t abs_raw;

            if (raw_temp < 0) {
                signo = '-';
                abs_raw = (uint16_t)(-raw_temp);
            } else {
                abs_raw = (uint16_t)raw_temp;
            }

            uint16_t parte_entera  = abs_raw >> 4;
            uint16_t parte_decimal = ((abs_raw & 0x000F) * 625) / 100;

            sprintf(buffer, "DS18-T:%c%u.%02u%cC   ", signo, parte_entera, parte_decimal, 0xDF);
            LCD_SetCursor(1, 0);
            LCD_PrintString(buffer);
            sprintf(buffer, "DS18B20 Temp: %c%u.%02u C\r\n", signo, parte_entera, parte_decimal);
            EUSART2_PrintString(buffer);
        } else {
            EUSART2_PrintString("Error Sensor DS18B20 no responde!   \r\n");
            __delay_ms(500);
        }        
        __delay_ms(200);
    }
}
