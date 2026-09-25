    PROCESSOR 18F47Q10
    #include "cabecera.inc"
    
    PSECT tortugas, class=CODE, reloc=2, abs
tortugas:
    ORG 000000H
    bra configuro
    
    ORG 00001AH
configuro:
    ;conf del modulo de oscilador
    movlb 0EH
    movlw 60H
    movwf OSCCON1, 1
    movlw 02H
    movwf OSCFRQ, 1
    movlw 50H
    movwf OSCEN, 1
    ;conf de las entradas y salidas
    movlb 0FH
    bsf TRISA, 0, 1
    bcf ANSELA, 0, 1
    bsf WPUA, 0, 1
    bsf TRISB, 0, 1
    bcf ANSELB, 0, 1    
    bsf WPUB, 0, 1
    bcf TRISE, 0, 1
    bcf ANSELE, 0, 1
    movlw 80H
    movwf TRISC, 1
    movwf ANSELC, 1
    movlw 0F0H
    movwf TRISD, 1
    movwf ANSELD, 1
    clrf LATD, 1
    ;configuracion del TBLPTR
    clrf TBLPTRU, 1
    movlw HIGH datos
    movwf TBLPTRH, 1

loop:
    clrf TBLPTRL, 1
    TBLRD*+
    movff TABLAT, LATC
    bsf LATD, 0, 1
    call nopx8
    bcf LATD, 0, 1
    TBLRD*+
    movff TABLAT, LATC
    bsf LATD, 1, 1
    call nopx8
    bcf LATD, 1, 1
    TBLRD*+
    movff TABLAT, LATC
    bsf LATD, 2, 1
    call nopx8
    bcf LATD, 2, 1
    TBLRD*
    movff TABLAT, LATC
    bsf LATD, 3, 1
    call nopx8
    bcf LATD, 3, 1
    bra loop
    
nopx8:
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    return
    
    ORG 002B00H
datos: DB 5FH, 38H, 78H, 5CH
 
    end tortugas


