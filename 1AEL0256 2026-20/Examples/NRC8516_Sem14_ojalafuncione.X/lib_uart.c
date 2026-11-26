//Librería LIB_UART desarrollada por Kalun Lau para el PIC18F47Q10
//Curso de Microcontroladores
//Universidad Peruana de Ciencias Aplicadas
//Ultima edicion 23/11/2026

//Changelog:
//10/06/2024: Se esta considerando el uso del EUSART2
//como períferico a emplearse en la comunicacion serial.
//La velocidad de comunicacion establecida es 9600
//La frecuencia de trabajo es de 32MHz
//Tener en cuenta que para la recepcion de datos se esta
//empleando interrupciones del USART para recepcion

//Instrucciones para usar la librería
//1.Llamar a EUSART_INIT() para configurar el EUSART2
//2.Para la parte de recepcion deberan de habilitar
//la funcion de interrupcion en el codigo principal

#include <xc.h>
#include <string.h>
#include "lib_uart.h"

void EUSART2_INIT(unsigned int velocidad){
    //configuracion del UART
    BAUD2CONbits.BRG16 = 1;         //BRG 8 bit mode
    TX2STAbits.BRGH = 1;
    SP2BRGH = (velocidad >> 8) & 0x00FF;
    SP2BRGL = velocidad & 0x00FF;
    TRISDbits.TRISD0 = 0;
    ANSELDbits.ANSELD0 = 0;
    RD0PPS = 0x0B;          //asignación de TX a RD0
    RX2PPS = 0x19;          //asingacion de RX a RD1 
    TX2STAbits.SYNC = 0;
    RC2STAbits.SPEN = 1;
    TX2STAbits.TXEN = 1;
    PIR3bits.TX2IF = 0;
}

/*funcion para enviar un dato de 8 bits*/
void EUSART2_BYTE_SEND(unsigned char dato){
    TX2REG = dato;
    while(TX2STAbits.TRMT == 0);
}

/*funcion para enviar una cadena de caracteres*/
void EUSART2_STRING_SEND(const char *cadena)
{
    unsigned char tam;
    tam = strlen(cadena);
	unsigned char i = 0;
	for(i = 0; i<tam; i++)
	{
		EUSART2_BYTE_SEND(cadena[i]);
	}
}

void EUSART2_PrintString(const char *str) {
    while (*str) {
        EUSART2_BYTE_SEND(*str++);
    }
}

/*funcion para visualizar una variable de 8 bits en formato decimal*/
void EUSART2_VAR_CHAR(unsigned char numero, unsigned char n_digitos){
    unsigned char centena, decena, unidad;  //variables de funcion convierte
    centena = (numero % 1000) / 100;
    decena =  (numero % 100) / 10;
    unidad = numero % 10;
    switch(n_digitos){
        case 1:
            EUSART2_BYTE_SEND(unidad+0x30);
            break;
        case 2:
            EUSART2_BYTE_SEND(decena+0x30);
            EUSART2_BYTE_SEND(unidad+0x30);
            break;
        case 3:
            EUSART2_BYTE_SEND(centena+0x30);
            EUSART2_BYTE_SEND(decena+0x30);
            EUSART2_BYTE_SEND(unidad+0x30);
            break;
    }
}

/*funcion para visualizar una variable de 16 bits en formato decimal*/
void EUSART2_VAR_INT(unsigned int numero, unsigned char n_digitos, unsigned char punto){
    unsigned char d_millar, millar, centena, decena, unidad;  //variables de funcion convierte
    d_millar = numero / 10000;
    millar = (numero % 10000) / 1000;
    centena = (numero % 1000) / 100;
    decena =  (numero % 100) / 10;
    unidad = numero % 10;
    switch(n_digitos){
        case 1:
            EUSART2_BYTE_SEND(unidad+0x30);
            break;
        case 2:
            if(punto == 0){
                EUSART2_BYTE_SEND(decena+0x30);
                EUSART2_BYTE_SEND(unidad+0x30);
            }
            else if(punto == 1){
                EUSART2_BYTE_SEND(decena+0x30);
                EUSART2_BYTE_SEND('.');
                EUSART2_BYTE_SEND(unidad+0x30);
            }
            break;
        case 3:
            switch(punto){
                case 0:
                    EUSART2_BYTE_SEND(centena+0x30);
                    EUSART2_BYTE_SEND(decena+0x30);
                    EUSART2_BYTE_SEND(unidad+0x30);
                    break;
                case 1:
                    EUSART2_BYTE_SEND(centena+0x30);
                    EUSART2_BYTE_SEND(decena+0x30);
                    EUSART2_BYTE_SEND('.');
                    EUSART2_BYTE_SEND(unidad+0x30);
                    break;
                case 2:
                    EUSART2_BYTE_SEND(centena+0x30);
                    EUSART2_BYTE_SEND('.');
                    EUSART2_BYTE_SEND(decena+0x30);
                    EUSART2_BYTE_SEND(unidad+0x30);
                    break;
            }
            break;
        case 4:
            switch(punto){
                case 0:
                    EUSART2_BYTE_SEND(millar+0x30);
                    EUSART2_BYTE_SEND(centena+0x30);
                    EUSART2_BYTE_SEND(decena+0x30);
                    EUSART2_BYTE_SEND(unidad+0x30);
                    break;
                case 1:
                    EUSART2_BYTE_SEND(millar+0x30);
                    EUSART2_BYTE_SEND(centena+0x30);
                    EUSART2_BYTE_SEND(decena+0x30);
                    EUSART2_BYTE_SEND('.');
                    EUSART2_BYTE_SEND(unidad+0x30);
                    break;
                case 2:
                    EUSART2_BYTE_SEND(millar+0x30);
                    EUSART2_BYTE_SEND(centena+0x30);
                    EUSART2_BYTE_SEND('.');
                    EUSART2_BYTE_SEND(decena+0x30);
                    EUSART2_BYTE_SEND(unidad+0x30);
                    break;
                case 3:
                    EUSART2_BYTE_SEND(millar+0x30);
                    EUSART2_BYTE_SEND('.');
                    EUSART2_BYTE_SEND(centena+0x30);
                    EUSART2_BYTE_SEND(decena+0x30);
                    EUSART2_BYTE_SEND(unidad+0x30);
                    break;
            }
            break;
        case 5:
            switch(punto){
                case 0:
                    EUSART2_BYTE_SEND(d_millar+0x30);
                    EUSART2_BYTE_SEND(millar+0x30);
                    EUSART2_BYTE_SEND(centena+0x30);
                    EUSART2_BYTE_SEND(decena+0x30);
                    EUSART2_BYTE_SEND(unidad+0x30);
                    break;
                case 1:
                    EUSART2_BYTE_SEND(d_millar+0x30);
                    EUSART2_BYTE_SEND(millar+0x30);
                    EUSART2_BYTE_SEND(centena+0x30);
                    EUSART2_BYTE_SEND(decena+0x30);
                    EUSART2_BYTE_SEND('.');
                    EUSART2_BYTE_SEND(unidad+0x30);
                    break;
                case 2:
                    EUSART2_BYTE_SEND(d_millar+0x30);
                    EUSART2_BYTE_SEND(millar+0x30);
                    EUSART2_BYTE_SEND(centena+0x30);
                    EUSART2_BYTE_SEND('.');
                    EUSART2_BYTE_SEND(decena+0x30);
                    EUSART2_BYTE_SEND(unidad+0x30);
                    break;
                case 3:
                    EUSART2_BYTE_SEND(d_millar+0x30);
                    EUSART2_BYTE_SEND(millar+0x30);
                    EUSART2_BYTE_SEND('.');
                    EUSART2_BYTE_SEND(centena+0x30);
                    EUSART2_BYTE_SEND(decena+0x30);
                    EUSART2_BYTE_SEND(unidad+0x30);
                    break;
                case 4:
                    EUSART2_BYTE_SEND(d_millar+0x30);
                    EUSART2_BYTE_SEND('.');
                    EUSART2_BYTE_SEND(millar+0x30);
                    EUSART2_BYTE_SEND(centena+0x30);
                    EUSART2_BYTE_SEND(decena+0x30);
                    EUSART2_BYTE_SEND(unidad+0x30);
                    break;
            }
            break;
    }
}

void EUSART2_NEWLINE(void){
    EUSART2_BYTE_SEND(0x0A);
    EUSART2_BYTE_SEND(0x0D);
}

//funcion de ejemplo para la recepcion de datos, debera de ser
//colocado en el archivo fuente
//el dato recepcionado se encuentra en el registro U1RXB
/*void __interrupt(irq(IRQ_U1RX)) U1RX_ISR(void){
    PIR4bits.U1RXIF = 0;
    switch(U1RXB){
        case '1':
            LATCbits.LATC0 = 1;
            break;
        case '2':
            LATCbits.LATC0 = 0;
            break;
        case '3':
            LATCbits.LATC1 = 1;
            break;
        case '4':
            LATCbits.LATC1 = 0;
            break;
    }
}
*/