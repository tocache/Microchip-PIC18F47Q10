;Este es un primer comentario
    ;Directiva de seleccion de procesador
    PROCESSOR 18F47Q10
    ;Llamada a la cabecera
    #include "cabecera.inc"
    
    ;Declaración del PSECT
    PSECT upcino, class=CODE, reloc=2, abs
upcino:
    ORG 000000H		;vector de RESET
    bra configuro	;salto a label configuro
    
    ORG 000050H		;zona de programa de usuario
configuro:
    ;configuracion de la fuente de reloj
    movlb 0EH		;me voy al Bank14
    movlw 60H
    movwf OSCCON1, 1	;NOSC=HFINTOSC, NDIV=1:1
    movlw 02H
    movwf OSCFRQ, 1	;HFINTOSC trabaje a 4MHz
    movlw 40H
    movwf OSCEN, 1	;HFINTOSC enabled
    ;configuracion las E/S
    movlb 0FH		;me voy al Bank15
    bsf TRISA, 0, 1	;puerto RA0 como entrada
    bcf ANSELA, 0, 1	;puerto RA0 como digital
    bcf TRISC, 0, 1	;puerto RC0 como salida
    bcf ANSELC, 0, 1	;puerto RC0 como digital
    
inicio:			;el programa de la aplicación
    btfss PORTA, 0, 1	;pregunto si RA0 es 1
    bra falso		;rpta falsa, salto a label falso
    bcf LATC, 0, 1	;rpta verdad, RC0 = 0
    bra inicio		;salto a label inicio
falso:
    bsf LATC, 0, 1	;RC0 = 1
    bra inicio		;salto a label inicio
    ;Cierre del PSECT
    end upcino
    
    


