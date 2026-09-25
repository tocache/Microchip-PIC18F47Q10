    PROCESSOR 18F47Q10
    #include "cabecera.inc"
    
    PSECT electromania, class=CODE, reloc=2, abs
electromania:
    ORG 000000H
    bra configuro
    
    ORG 00001AH
configuro:
    ;conf del modulo de oscilador
    movlb 0EH
    movlw 59H
    movwf OSCCON1, 1
    movlw 02H
    movwf OSCFRQ, 1
    movlw 50H
    movwf OSCEN, 1
    ;conf de las E/S
    movlb 0FH
    bsf TRISA, 0, 1
    bcf ANSELA, 0, 1
    bsf WPUA, 0, 1
    bsf TRISB, 0, 1
    bcf ANSELB, 0, 1
    bsf WPUB, 0, 1
    movlw 80H
    movwf TRISC, 1
    movwf ANSELC, 1
    movlw 0F0H
    movwf TRISD, 1
    movwf ANSELD, 1
    ;condicion inicial de los habilitadores
    clrf LATD, 1
    ;conf del TBLPTR
    clrf TBLPTRU, 1
    movlw 01H
    movwf TBLPTRH, 1
loop:
    movlw 80H
    movwf TBLPTRL, 1
    TBLRD*+
    movff TABLAT, LATC
    bsf LATD, 0, 1
    call nopes
    bcf LATD, 0, 1
    TBLRD*+
    movff TABLAT, LATC
    bsf LATD, 1, 1
    call nopes
    bcf LATD, 1, 1
    TBLRD*+
    movff TABLAT, LATC
    bsf LATD, 2, 1
    call nopes
    bcf LATD, 2, 1
    TBLRD*
    movff TABLAT, LATC
    bsf LATD, 3, 1
    call nopes
    bcf LATD, 3, 1
    bra loop
    
nopes:
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    return
    ORG 000180H
datos: DB 73H, 5FH, 6DH, 7BH
 
    end electromania


