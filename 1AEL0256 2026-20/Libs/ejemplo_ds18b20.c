#include <xc.h>
#include <stdio.h>
#include "cabecera.h"
#include "lib_uart.h"
#include "ds18b20.h"

#define _XTAL_FREQ 32000000UL

void configuro(void) {
    OSCCON1 = 0x60;
    OSCFRQ  = 0x06;
    OSCEN   = 0x40;
    while (!OSCCON3bits.ORDY);

    EUSART2_INIT(BAUD_9600);
    DS18B20_Init();
}

void main(void) {
    uint8_t rom[8];
    int16_t temp_entero = 0;
    float   temp_flotante = 0.0f;
    char    buffer[32];

    configuro();
    EUSART2_PrintString("Iniciando DS18B20...\r\n");

    // 1. Leer e imprimir ROM (número de serie)
    if (DS18B20_ReadROM(rom) == DS18B20_OK) {
        EUSART2_PrintString("ROM: ");
        for (uint8_t i = 0; i < 8; i++) {
            sprintf(buffer, "%02X", rom[i]);
            EUSART2_PrintString(buffer);
            if (i < 7) EUSART2_BYTE_SEND(':');
        }
        EUSART2_NEWLINE();
    } else {
        EUSART2_PrintString("Error: DS18B20 no detectado\r\n");
    }

    // 2. Configurar resolución
    DS18B20_SetResolution(DS18B20_RES_12BIT);

    while (1) {
        // Lectura en formato entero usando EUSART2_VAR_INT
        if (DS18B20_ReadTempInt(&temp_entero) == DS18B20_OK) {
            EUSART2_PrintString("Temp (Int): ");
            if (temp_entero < 0) {
                EUSART2_BYTE_SEND('-');
                temp_entero = -temp_entero;
            }
            EUSART2_VAR_INT((unsigned int)temp_entero, 3, 0);
            EUSART2_PrintString(" C\r\n");
        }

        // Lectura en formato flotante
        if (DS18B20_ReadTempFloat(&temp_flotante) == DS18B20_OK) {
            sprintf(buffer, "Temp (Float): %.2f C\r\n", temp_flotante);
            EUSART2_PrintString(buffer);
        } else {
            EUSART2_PrintString("Error de lectura DS18B20\r\n");
        }

        EUSART2_NEWLINE();
        __delay_ms(1000);
    }
}
