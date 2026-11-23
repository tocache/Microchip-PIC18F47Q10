#ifndef DHT22_H
#define DHT22_H

#include <xc.h>
#include <stdint.h>

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 32000000UL
#endif

// Configuración de Hardware (PIN RD3)
#define DHT22_PIN       PORTDbits.RD3
#define DHT22_LAT       LATDbits.LATD3
#define DHT22_TRIS      TRISDbits.TRISD3
#define DHT22_ANSEL     ANSELDbits.ANSELD3
#define DHT22_WPU       WPUDbits.WPUD3

// Códigos de retorno
#define DHT22_OK            0
#define DHT22_ERR_TIMEOUT   1
#define DHT22_ERR_CHECKSUM  2

// Prototipos
void DHT22_Init(void);
uint8_t DHT22_Read(float *temperatura, float *humedad);

#endif /* DHT22_H */