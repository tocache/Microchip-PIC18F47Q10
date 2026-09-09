    PROCESSOR 18F47Q10
    #include "cabecera.inc"
    
    PSECT upcinos, class=CODE, reloc=2, abs
upcinos:
    ORG 000000H		;vector de reset
    bra configuro
    
    ORG 000080H		;zona de programa de usuario
configuro:
    movlb 0EH		;Bank14
    movlw 60H
    movwf OSCCON1, 1	;NOSC=HFINTOSC, NDIV=1:1
    movlw 02H
    movwf OSCFRQ, 1	;HFINTOSC a 4MHz
    movlw 40H
    movwf OSCEN, 1	;HFINTOSC enabled
    movlb 0FH		;Bank15
    movlw 0FCH
    movwf TRISD, 1	;RD1 y RD0 como salidas
    movwf ANSELD, 1	;RD1 y RD0 como digitales

inicio:			;aplicacion de la señal de tren
    movlw 01H
    movwf LATD, 1	;escribo 01H en LATD
    nop			;retardo de 1us
    movlw 02H
    movwf LATD, 1	;escribo 02H en LATD
    nop			;retardo de 1us
    bra inicio
    
    end upcinos

