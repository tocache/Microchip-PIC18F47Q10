;Este es un comentario
    ;Directiva de procesador
    PROCESSOR 18F47Q10
    ;Llamado a la cabecera
    #include "cabecera.inc"
    
    ;Declaracion del PSECT
    PSECT upcino, class=CODE, reloc=2, abs
upcino:
    ORG 000000H		;direccion del vector de RESET
    bra configuro	;salto a label configuro
    
    ORG 000080H		;Zona de programa de usuario
configuro:
    ;Configuracion de fuente de reloj a 4MHz
    movlb 0EH		;Me voy al Bank14
    movlw 60H
    movwf OSCCON1, 1	;NOSC=HFINTOSC, NDIV=1:1
    movlw 02H
    movwf OSCFRQ, 1	;HFINTOSC a 4MHz
    movlw 40H
    movwf OSCEN, 1	;HFINTOSC enabled
    
    ;Configuracion de puertos de E/S
    movlb 0FH		;Me voy al Bank15
    ;RA0 como entrada digital:
    bsf TRISA, 0, 1	;puerto RA0 como entrada
    bcf ANSELA, 0, 1	;puerto RA0 como digital
    ;RD2 como salida digital
    bcf TRISD, 2, 1	;puerto RD2 como salida
    bcf ANSELD, 2, 1	;puerto RD2 como digital
    
inicio:			;codigo de la aplicacion
    btfss PORTA, 0, 1	;pregunto si RA0 es uno
    bra falsssso	;rpta falso, salta a label falsssso
    bcf LATD, 2, 1	;rpta verdad, puerto RD2 en cero (LED off)
    bra inicio		;salto a label inicio
falsssso:
    bsf LATD, 2, 1	;puerto RD2 en uno (LED on)
    bra inicio		;salto a label inicio

    ;cierre del PSECT
    end upcino
    


