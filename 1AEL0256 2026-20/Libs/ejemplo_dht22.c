#include <xc.h>
#include <stdio.h>
#include "cabecera.h"
#include "dht22.h"

#define _XTAL_FREQ 32000000UL

static void EUSART2_Init_9600(void) {
    // Configurar RB6 como salida TX2 mediante PPS
    ANSELBbits.ANSELB6 = 0;
    TRISBbits.TRISB6   = 0;
    RB6PPS = 0x0B;          // 0x0B = TX2/CK2 en PIC18F47Q10

    // Configuración 9600 bps @ 32 MHz (BRG16=1, BRGH=1 -> SP2BRG=832)
    BAUD2CONbits.BRG16 = 1;
    TX2STAbits.BRGH    = 1;
    TX2STAbits.SYNC    = 0;
    SP2BRG             = 832;

    RC2STAbits.SPEN    = 1; // Habilitar puerto serial
    TX2STAbits.TXEN    = 1; // Habilitar transmisor
}

static void EUSART2_WriteChar(char c) {
    while (!TX2STAbits.TRMT);
    TX2REG = c;
}

static void EUSART2_PrintString(const char *str) {
    while (*str) {
        EUSART2_WriteChar(*str++);
    }
}

static void configuro(void) {
    // Oscilador interno a 32 MHz
    OSCCON1 = 0x60;
    OSCFRQ  = 0x06;
    OSCEN   = 0x40;
    while (!OSCCON3bits.ORDY);

    EUSART2_Init_9600();
    DHT22_Init();
}

void main(void) {
    float temp = 0.0f, hum = 0.0f;
    char buffer[48];

    configuro();
    EUSART2_PrintString("Sistema DHT22 listo...\r\n");

    while (1) {
        uint8_t estado = DHT22_Read(&temp, &hum);

        if (estado == DHT22_OK) {
            sprintf(buffer, "Temp: %.1f C | Hum: %.1f %%\r\n", temp, hum);
            EUSART2_PrintString(buffer);
        } else if (estado == DHT22_ERR_TIMEOUT) {
            EUSART2_PrintString("Error: DHT22 no responde\r\n");
        } else {
            EUSART2_PrintString("Error: Checksum invalido\r\n");
        }

        // El DHT22 requiere mínimo 2 segundos entre lecturas
        __delay_ms(2000);
    }
}
