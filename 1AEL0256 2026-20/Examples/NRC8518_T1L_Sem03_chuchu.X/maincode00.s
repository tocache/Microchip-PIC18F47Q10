    PROCESSOR 18F47Q10
    #include "cabecera.inc"
    
    PSECT upecenenes, class=CODE, reloc=2, abs
upecenenes:
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
    movlw 0FCH
    movwf TRISD, 1
    movwf ANSELD, 1
    movlw 01H
    movwf LATD, 1
inicio:
    nop
    comf LATD, 1
    bra inicio
    
    end upecenenes

