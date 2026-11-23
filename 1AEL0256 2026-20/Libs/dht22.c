#include "dht22.h"

// Espera a que el pin cambie al estado esperado con límite de tiempo (~us)
static uint8_t DHT22_WaitState(uint8_t state, uint16_t timeout_us) {
    while (DHT22_PIN != state) {
        if (timeout_us-- == 0) return 0;
        __delay_us(1);
    }
    return 1;
}

void DHT22_Init(void) {
    DHT22_ANSEL = 0;    // RD3 como pin digital
    DHT22_WPU   = 0;    // Usar pull-up externo (4.7k - 10k)
    DHT22_TRIS  = 1;    // Bus liberado en alta impedancia
}

uint8_t DHT22_Read(float *temperatura, float *humedad) {
    uint8_t data[5] = {0, 0, 0, 0, 0};
    uint8_t gie = INTCONbits.GIE;

    // 1. Señal de inicio del MCU (al menos 1 ms en BAJO)
    DHT22_LAT  = 0;
    DHT22_TRIS = 0;
    __delay_ms(2);

    // Deshabilitar interrupciones durante la lectura crítica
    INTCONbits.GIE = 0;

    // 2. Liberar bus y esperar respuesta del DHT22
    DHT22_TRIS = 1;
    __delay_us(30);

    // El DHT22 responde con ~80 us en BAJO y ~80 us en ALTO
    if (!DHT22_WaitState(0, 100)) { INTCONbits.GIE = gie; return DHT22_ERR_TIMEOUT; }
    if (!DHT22_WaitState(1, 100)) { INTCONbits.GIE = gie; return DHT22_ERR_TIMEOUT; }
    if (!DHT22_WaitState(0, 100)) { INTCONbits.GIE = gie; return DHT22_ERR_TIMEOUT; }

    // 3. Lectura de los 40 bits (5 bytes)
    for (uint8_t i = 0; i < 5; i++) {
        for (uint8_t j = 0; j < 8; j++) {
            // Cada bit inicia con ~50 us en BAJO
            if (!DHT22_WaitState(1, 80)) { INTCONbits.GIE = gie; return DHT22_ERR_TIMEOUT; }

            // Esperar 40 us: si sigue en ALTO es '1' (~70 us), si bajó es '0' (~26 us)
            __delay_us(40);
            data[i] <<= 1;
            if (DHT22_PIN == 1) {
                data[i] |= 1;
                if (!DHT22_WaitState(0, 80)) { INTCONbits.GIE = gie; return DHT22_ERR_TIMEOUT; }
            }
        }
    }

    INTCONbits.GIE = gie;

    // 4. Verificar Checksum
    uint8_t checksum = (uint8_t)(data[0] + data[1] + data[2] + data[3]);
    if (checksum != data[4]) {
        return DHT22_ERR_CHECKSUM;
    }

    // 5. Convertir datos crudos (resolución de 0.1)
    uint16_t raw_hum = ((uint16_t)data[0] << 8) | data[1];
    *humedad = (float)raw_hum / 10.0f;

    uint16_t raw_temp = ((uint16_t)(data[2] & 0x7F) << 8) | data[3];
    *temperatura = (float)raw_temp / 10.0f;
    if (data[2] & 0x80) {
        *temperatura = -(*temperatura); // Bit de signo para temperaturas negativas
    }

    return DHT22_OK;
}