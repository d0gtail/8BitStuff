PORTA=$6001
PORTB=$6000
DDRA=$6003
DDRB=$6002

E  = %10000000 ; Enable Bit
RW = %01000000 ; Read/Write Bit
RS = %00100000 ; Register Select

	.org $8000

reset:
	ldx #$FF	; initalize the stack pointer to sit on 0x1FF
	txs
	jsr initVIA
	jsr initLCD
	jsr helloWorld
loop:
	jmp loop

initVIA:
	lda #%11111111 ; Set all pins on PORT B to output
	sta DDRB
	
	lda #%11100000 ; Set top 3 pins on PORT A to output
	sta DDRA
	rts

initLCD:
	lda #%00111000 ; Set 8-Bit mode, 2-line Display, 5x8 Font
	sta PORTB
	jsr lcdInstruction
	
	lda #%00001110 ; Display on, cursor on, blink off
	sta PORTB
	jsr lcdInstruction
	
	lda #%00000110 ; Inc and shift cursor, don't shift display
	sta PORTB
	jsr lcdInstruction
	
	lda #%00000001 ; Clear Display
	sta PORTB
	jsr lcdInstruction
	rts

helloWorld:
	lda #"H"
	sta PORTB
	jsr lcdSendData

	lda #"a"
	sta PORTB
	jsr lcdSendData

	lda #"l"
	sta PORTB
	jsr lcdSendData

	lda #"l"
	sta PORTB
	jsr lcdSendData

	lda #"o"
	sta PORTB
	jsr lcdSendData

	lda #" "
	sta PORTB
	jsr lcdSendData

	lda #"W"
	sta PORTB
	jsr lcdSendData

	lda #"e"
	sta PORTB
	jsr lcdSendData

	lda #"l"
	sta PORTB
	jsr lcdSendData

	lda #"t"
	sta PORTB
	jsr lcdSendData	
	
	lda #"!"
	sta PORTB
	jsr lcdSendData
	rts

lcdInstruction:
	lda #0 ; Clear RS/RW/E bits
	sta PORTA
	lda #E ; Toggle Enable bit
	sta PORTA
	lda #0 ; Clear RS/RW/E bits
	sta PORTA
	rts

lcdSendData:
	lda #RS ; Clear RS/RW/E bits
	sta PORTA
	lda #(RS | E) ; Toggle Enable bit
	sta PORTA
	lda #RS ; Clear RS/RW/E bits
	sta PORTA
	rts


	.org $FFFC
	.word reset
	.word $0000
