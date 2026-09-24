    PROCESSOR 18F47Q10
    #include "cabecera.inc"
    
    PSECT bioslocos, class=CODE, reloc=2, abs
bioslocos:
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
    clrf TRISD, 1
    clrf ANSELD, 1
    clrf LATD, 1	    ;a cero los habilitadores de los digitos
    bcf TRISE, 0, 1
    bcf ANSELE, 0, 1
    ;asignacion de direccion al TBLPTR
    clrf TBLPTRU, 1
    clrf TBLPTRH, 1
loop:
    movlw 0F0H	
    movwf TBLPTRL, 1	    ;TBLPTR apuntando a direccion 0000F0H
    TBLRD*+		    ;accion de lectura y posterior incremento
    movff TABLAT, LATC	    ;muevo contenido de TABLAT a RC
    bsf LATD, 0, 1	    ;enciendo digito
    call nopes		    ;espero un ratito
    bcf LATD, 0, 1	    ;apago digito
    TBLRD*+		    ;accion de lectura y posterior incremento
    movff TABLAT, LATC	    ;muevo contenido de TABLAT a RC
    bsf LATD, 1, 1	    ;enciendo digito
    call nopes		    ;espero un ratito
    bcf LATD, 1, 1	    ;apago digito
    TBLRD*+		    ;accion de lectura y posterior incremento
    movff TABLAT, LATC	    ;muevo contenido de TABLAT a RC
    bsf LATD, 2, 1	    ;enciendo digito
    call nopes		    ;espero un ratito
    bcf LATD, 2, 1	    ;apago digito
    TBLRD*+		    ;accion de lectura y posterior incremento
    movff TABLAT, LATC	    ;muevo contenido de TABLAT a RC
    bsf LATD, 3, 1	    ;enciendo digito
    call nopes		    ;espero un ratito
    bcf LATD, 3, 1	    ;apago digito
    movwf TBLPTRL, 1	    ;TBLPTR apuntando a direccion 0000F0H
    TBLRD*+		    ;accion de lectura y posterior incremento
    movff TABLAT, LATC	    ;muevo contenido de TABLAT a RC
    bsf LATD, 4, 1	    ;enciendo digito
    call nopes		    ;espero un ratito
    bcf LATD, 4, 1	    ;apago digito
    TBLRD*+		    ;accion de lectura y posterior incremento
    movff TABLAT, LATC	    ;muevo contenido de TABLAT a RC
    bsf LATD, 5, 1	    ;enciendo digito
    call nopes		    ;espero un ratito
    bcf LATD, 5, 1	    ;apago digito
    TBLRD*+		    ;accion de lectura y posterior incremento
    movff TABLAT, LATC	    ;muevo contenido de TABLAT a RC
    bsf LATD, 6, 1	    ;enciendo digito
    call nopes		    ;espero un ratito
    bcf LATD, 6, 1	    ;apago digito
    TBLRD*		    ;accion de lectura y posterior incremento
    movff TABLAT, LATC	    ;muevo contenido de TABLAT a RC
    bsf LATD, 7, 1	    ;enciendo digito
    call nopes		    ;espero un ratito
    bcf LATD, 7, 1	    ;apago digito    
    bra loop

nopes:
    nop
    nop
    nop
    nop
    return
    
    ORG 0000F0H
datos:	DB  38H, 30H, 15H, 77H, 38H, 30H, 15H, 77H
	
    end bioslocos


