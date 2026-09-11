    PROCESSOR 18F47Q10
    #include "cabecera.inc"
    
    PSECT upcinos, class=CODE, reloc=2, abs
upcinos:
    ORG 000000H
    bra configuro
    
    ORG 00001AH
configuro:
    movlb 0EH
    movlw 59H
    movwf OSCCON1, 1
    movlw 02H
    movwf OSCFRQ, 1
    movlw 40H
    movwf OSCEN, 1
    movlb 0FH
    movlw 0FCH
    movwf ANSELD, 1
    movwf TRISD, 1
    
inicio:    
    movlw 01H
    movwf LATD, 1
bucle:
    nop
    nop
    nop
    nop
    nop
    comf LATD, 1, 1
    bra bucle
    
    end upcinos

