    PROCESSOR 18F47Q10
    #include "cabecera.inc"
    
    PSECt upcino, class=CODE, reloc=2, abs
upcino:
    ORG 000000H
    bra configuro
    
    ORG 000080H
configuro:
    ;configuracion de la fuente de reloj
    movlb 0EH		;Me voy al Bank14
    movlw 60H
    movwf OSCCON1, 1	;NOSC=HFINTOSC, NDIV=1:1
    movlw 02H
    movwf OSCFRQ, 1	;HFINTOSC a 4MHz
    movlw 40H
    movwf OSCEN, 1	;HFINTOSC enabled
    ;configuracion de los puertos de entrada/salida
    movlb 0FH		;me voy al Bank15
    bsf TRISA, 0, 1	;RA0 como entrada
    bcf ANSELA, 0, 1	;RA0 como digital
    bcf TRISC, 0, 1	;RC0 como salida
    bcf ANSELC, 0, 1	;RC0 como digital
    ;codigo de la aplicacion de la NOTGATE
inicio:
    btfss PORTA, 0, 1	;pregunto si RA0 es uno
    bra es_falso	;rpta falso, salta a label es_falso
    bcf LATC, 0, 1	;rpta verdad, RC0 en cero
    bra inicio		;salto a label inicio
es_falso:
    bsf LATC, 0, 1	;RC0 en uno
    bra inicio		;salto a label inicio
    
    end upcino
    


