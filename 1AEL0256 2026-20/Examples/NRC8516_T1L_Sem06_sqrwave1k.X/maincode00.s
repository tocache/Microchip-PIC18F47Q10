    PROCESSOR 18F47Q10
    #include "cabecera.inc"
    
    PSECT hacecalor, class=CODE, reloc=2, abs
hacecalor:
    ORG 000000H
    bra configuro
    
    ORG 00001AH
configuro:
    movlb 0EH
    movlw 60H
    movwf OSCCON1, 1
    movlw 02H
    movwf OSCFRQ, 1
    movlw 40H
    movwf OSCEN, 1
    movlb 0FH
    bcf TRISC, 0, 1
    bcf ANSELC, 0, 1
;    bcf TRISE, 0, 1
;    bcf ANSELE,0, 1
;    bsf LATE, 0, 1
    movlw 80H
    movwf T0CON0, 1
    movlw 4BH
    movwf T0CON1, 1
    movlw 250
    movwf TMR0H, 1

loop:
    movlb 0EH
    btfss PIR0, 5, 1	    ;Pregunto si TMR0IF es uno
    bra loop		    ;Falso, regreso a preguntar de nuevo
    bcf PIR0, 5, 1	    ;Bajamos al bandera
    movlb 0FH
    btg LATC, 0, 1	    ;Complementamos RC0
    bra loop
    
    end hacecalor


