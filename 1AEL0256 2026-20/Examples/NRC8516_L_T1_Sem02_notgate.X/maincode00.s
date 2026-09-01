;Este es un comentario, los comentarios van antecedidos por el ";"
    PROCESSOR 18F47Q10		;Directiva de procesador
    ;Llamado al archivo cabecera.inc
    #include "cabecera.inc"	
    
    ;Inicio del PSECT
    PSECT upcino, class=CODE, reloc=2, abs
upcino:
    ORG 000000H			;Vector de RESET
    bra configuro		;Salto a label configuro
    
    ORG 000250H			;Zona de programa del usuario
configuro:
    ;Primero configurar la fuente de reloj
    movlb 0EH			;Voy al Bank14
    movlw 60H
    movwf OSCCON1, 1		;NOSC=HFINTOSC y NDIV=1:1
    movlw 02H
    movwf OSCFRQ, 1		;HFINTOSC a 4MHz
    movlw 40H
    movwf OSCEN, 1		;HFINTOSC enabled
    ;Segundo configurar las E/S
    movlb 0FH			;Voy al Bank15
    bsf TRISB, 0, 1		;RB0 como entrada
    bcf ANSELB, 0, 1		;RB0 como digital
    bcf TRISC, 0, 1		;RC0 como salida
    bcf ANSELC, 0, 1		;RC0 como digital
    
inicio:				;Inicio del algoritmo
    btfss PORTB, 0, 1		;Pregunto si RB0=1
    bra falso			;Rpta es falsa salto a label falso
    bcf LATC, 0, 1		;Rpta es verdad apago el LED
    bra inicio			;Regreso a label inicio
falso:
    bsf LATC, 0, 1		;Enciendo el LED
    bra inicio			;Regreso a label inicio
    
    ;Fin del PSECT
    end upcino


