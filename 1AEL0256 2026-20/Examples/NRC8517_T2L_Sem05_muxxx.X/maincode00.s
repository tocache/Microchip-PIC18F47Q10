    PROCESSOR 18F47Q10
    #include "cabecera.inc"
    
    PSECT mecatron, class=CODE, reloc=2, abs
mecatron:
    ORG 000000H
    bra configuro
    
    ORG 00001AH
configuro:
    ;conf el modulo de oscilador
    movlb 0EH
    movlw 54H
    movwf OSCCON1, 1	    ;LFINTOSC y 1:512
    movlw 02H
    movwf OSCFRQ, 1	    ;HFINTOSC a 4MHz
    movlw 50H
    movwf OSCEN, 1	    ;LFINTOSC y HFINTOSC enabled
    ;conf las E/S
    movlb 0FH
    bsf TRISA, 0, 1
    bcf ANSELA, 0, 1
    bsf WPUA, 0, 1
    bsf TRISB, 0, 1
    bcf ANSELB, 0, 1
    bsf WPUB, 0, 1
    bcf TRISE, 0, 1
    bcf ANSELE, 0, 1
    movlw 0F0H
    movwf TRISD, 1
    movwf ANSELD, 1
    movlw 80H
    movwf TRISC, 1
    movwf ANSELC, 1
    ;condiciones iniciales
    clrf LATD, 1	;habilitadores de digitos en cero
    clrf TBLPTRU, 1
    movlw 01H
    movwf TBLPTRH, 1

loop:
    movlw 50H
    movwf TBLPTRL, 1	;TBLPTR apuntando a 000150H
    TBLRD*+		;accion de lectura y posterior inc
    movff TABLAT, LATC	;movemos TABLAT hacia LATC
    bsf LATD, 0, 1	;habilitamos digito
    call nopes		;un microretardo
    bcf LATD, 0, 1	;deshabilitamos digito
    TBLRD*+
    movff TABLAT, LATC	;movemos TABLAT hacia LATC
    bsf LATD, 1, 1	;habilitamos digito
    call nopes		;un microretardo
    bcf LATD, 1, 1	;deshabilitamos digito
    TBLRD*+
    movff TABLAT, LATC	;movemos TABLAT hacia LATC
    bsf LATD, 2, 1	;habilitamos digito
    call nopes		;un microretardo
    bcf LATD, 2, 1	;deshabilitamos digito
    TBLRD*
    movff TABLAT, LATC	;movemos TABLAT hacia LATC
    bsf LATD, 3, 1	;habilitamos digito
    call nopes		;un microretardo
    bcf LATD, 3, 1	;deshabilitamos digito
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
    
    ORG 000150H
datos:	DB 73H, 7BH, 50H, 1CH
	
    end mecatron	


