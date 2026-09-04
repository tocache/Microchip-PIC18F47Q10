;Este es un comentario en assembler
    ;Directiva de procesador
    PROCESSOR 18F47Q10
    ;Llamado al archivo cabecera
    #include "cabecera.inc"
    
    ;Declaracion de PSECT
    PSECT upcinos, class=CODE, reloc=2, abs
upcinos:
    ORG 000000H		    ;Vector de RESET
    bra configuro	    ;salto a label configuro
    
    ORG 000080H		    ;Zona de programa de usuario
configuro:
    ;configuracion de la fuente de reloj
    movlb 0EH		    ;Me voy al Bank14
    movlw 60H
    movwf OSCCON1, 1	    ;NOSC=HFINTOSC, NDIV=1:1
    movlw 02H
    movwf OSCFRQ, 1	    ;HFINTOSC a 4MHz
    movlw 40H
    movwf OSCEN, 1	    ;HFINTOSC enabled
    ;configuracion de los puertos de E/S
    movlb 0FH		    ;Me voy al Bank15
    bsf TRISA, 0, 1	    ;puerto RA0 como entrada
    bcf ANSELA, 0, 1	    ;puerto RA0 como digital
    bsf WPUA, 0, 1	    ;puerto RA0 con pullup
    bcf TRISC, 0, 1	    ;puerto RC0 como salida
    bcf ANSELC, 0, 1	    ;puerto RC0 como digital
    
inicio:			    ;aplicacion de la latched
    btfsc PORTA, 0, 1	    ;pregunto si el puerto RA0 es cero
    bra $-2		    ;rpta falso regresa a linea anterior
    btg LATC, 0, 1	    ;rpta verdad, basculacion RC0
    btfss PORTA, 0, 1	    ;pregunto si RA0 es uno
    bra $-2		    ;rpta falso retorna una linea arriba
    bra inicio		    ;salto a label inicio
    
    ;fin del PSECT
    end upcinos
    





