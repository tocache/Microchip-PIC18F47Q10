    PROCESSOR 18F47Q10
    #include "cabecera.inc"
    
    PSECT noentiendo, class=CODE, reloc=2, abs
noentiendo:
    ORG 000000H
    bra configuro
    
    ORG 00001AH
configuro:
    ;conf el modulo de oscilador
    movlb 0EH
    movlw 60H
    movwf OSCCON1, 1
    movlw 02H
    movwf OSCFRQ, 1
    movlw 40H
    movwf OSCEN, 1
    ;conf los puertos de E/S
    movlb 0FH
    bcf TRISC, 0, 1
    bcf ANSELC, 0, 1
    ;conf el Timer0
    movlw 80H
    movwf T0CON0, 1
    movlw 0DAH
    movwf T0CON1, 1
    movlw 249
    movwf TMR0H, 1	;valor de referencia de comparacion

loop:
    movlb 0EH
    btfss PIR0, 5, 1	;pregunto si se levanto TMR0IF
    bra loop		;no se levanto TMR0IF
    bcf PIR0, 5, 1	;se levantó TMR0IF y la bajamos
    movlb 0FH
    btg LATC, 0, 1	;aplicamos complemento a RC0
    bra loop
    
    end noentiendo
    
    


