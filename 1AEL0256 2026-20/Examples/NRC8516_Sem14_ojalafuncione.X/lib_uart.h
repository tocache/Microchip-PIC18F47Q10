#ifndef LIB_UART_H_
//Librería LIB_UART desarrollada por Kalun Lau
//Curso de Microcontroladores
//Universidad Peruana de Ciencias Aplicadas
//Ultima edicion 10/06/2024

//Changelog:
//10/06/2024: Se esta considerando el uso del EUSART2
//como períferico a emplearse en la comunicacion serial.
//La velocidad de comunicacion establecida es 9600
//La frecuencia de trabajo es de 32MHz
//Tener en cuenta que para la recepcion de datos se esta
//empleando interrupciones del USART para recepcion

//Instrucciones para usar la librería
//1.Llamar a EUSART2_INIT() para configurar el UART EUSART2
//2.Para la parte de recepcion deberan de habilitar
//la funcion de interrupcion en el codigo principal

#define	LIB_UART_H_
#define _XTAL_FREQ 32000000UL

#define BAUD_4800 1666   //No va a funcionar
#define BAUD_9600 832
#define BAUD_19200 416
#define BAUD_57600 138
#define BAUD_115200 68

/*Función para inicializar el periférico EUSART2
 Opciones:  BAUD_4800,
            BAUD_9600,
            BAUD_19200,
            BAUD_57600,
            BAUD_115200*/
void EUSART2_INIT(unsigned int velocidad);

/*Funcion para enviar un dato de 8 bits*/
void EUSART2_BYTE_SEND(unsigned char dato);

/*Funcion para enviar una cadena de caracteres*/
void EUSART2_STRING_SEND(const char *cadena);

/*Funcion para enviar una cadena de caracteres version mejorada*/
void EUSART2_PrintString(const char *str);

/*Funcion para visualizar una variable de 8 bits en formato decimal*/
void EUSART2_VAR_CHAR(unsigned char numero, unsigned char n_digitos);

/*Funcion para visualizar una variable de 16 bits en formato decimal*/
void EUSART2_VAR_INT(unsigned int numero, unsigned char n_digitos, unsigned char punto);

/*Funcion para irse una linea abajo en el terminal*/
void EUSART2_NEWLINE(void);

#endif	/* XC_HEADER_TEMPLATE_H */