;Este es un comentario
    ;Directiva de procesador
    PROCESSOR 18F47Q10
    ;Llamado a cabecera
    #include "cabecera.inc"
    
    ;Declaracion del PSECT
    PSECT upcino, class=CODE, reloc=2, abs
upcino:
    ORG 000000H		;Vector de RESET
    bra configuro	;salto a label configuro
    
    ORG 000080H		;Zona de programa de usuario
configuro:
    ;configuracion de la fuente de reloj
    movlb 0EH		;me voy al Bank14
    movlw 60H
    movwf OSCCON1, 1	;NOSC=HFINTOSC, NDIV=1:1
    movlw 02H
    movwf OSCFRQ, 1	;HFINTOSC a 4MHz
    movlw 40H
    movwf OSCEN, 1	;HFINTOSC enabled
    ;configuracion de los puertos de E/S
    movlb 0FH		;me voy al Bank15
    bsf TRISB, 0, 1	;RB0 como entrada
    bcf ANSELB, 0, 1	;RB0 como digital
    bcf TRISC, 0, 1	;RC0 como salida
    bcf ANSELC, 0, 1	;RC0 como digital
inicio:			;codigo de la aplicacion
    btfss PORTB, 0, 1	;pregunto si RB0 es uno
    bra es_falso	;rpta falso, salta a label es_falso
    bcf LATC, 0, 1	;rpta verdad, RC0 en cero
    bra inicio		;salto a label inicio
es_falso:
    bsf LATC, 0, 1	;RC0 en uno
    bra inicio		;salto a label inicio
    ;Fin del PSECT
    end upcino
    


