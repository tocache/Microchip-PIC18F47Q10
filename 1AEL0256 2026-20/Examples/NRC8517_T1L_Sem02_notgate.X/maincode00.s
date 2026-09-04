;Este es un comentario
    ;Directiva de procesador
    PROCESSOR 18F47Q10
    ;Llamada a la cabecera
    #include "cabecera.inc"
    
    ;Creacion del PSECT
    PSECT upcino, class=CODE, reloc=2, abs
upcino:
    ORG 000000H		    ;Vector de RESET
    bra configuro	    ;Salto a label configuro
    
    ORG 000080H		    ;Zona de programa de usuario
configuro:
    ;configuracion de la fuente de reloj a 4MHZ con HFINTOSC
    movlb 0EH		    ;me voy al Bank14
    movlw 60H
    movwf OSCCON1, 1	    ;NOSC=HFINTOSC, NDIV=1:1
    movlw 02H
    movwf OSCFRQ, 1	    ;HFINTOSC a 4MHz
    movlw 40H
    movwf OSCEN, 1	    ;HFINTOSC enabled
    ;configuracion de los puertos de E/S
    movlb 0FH		    ;me voy al Bank15
    bsf TRISA, 0, 1	    ;RA0 como entrada
    bcf ANSELA, 0, 1	    ;RA0 como digital
    bcf TRISC, 0, 1	    ;RC0 como salida
    bcf ANSELC, 0, 1	    ;RC0 como digital
inicio:			    ;codigo de la aplicacion
    btfss PORTA, 0, 1	    ;pregunto si RA0 es uno
    bra falso		    ;rpta falso, salto a label falso
    bcf LATC, 0, 1	    ;rpta verdad, pongo RC0 a cero
    bra inicio		    ;salto a label inicio
falso:
    bsf LATC, 0, 1	    ;pongo RC0 a uno
    bra inicio		    ;salto a label inicio
    ;cierre del PSECT
    end upcino


