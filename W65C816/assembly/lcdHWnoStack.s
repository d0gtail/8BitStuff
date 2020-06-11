PORTA=$6001
PORTB=$6000
DDRA=$6003
DDRB=$6002

E  = %10000000 ; Enable Bit
RW = %01000000 ; Read/Write Bit
RS = %00100000 ; Register Select

	.org $8000

init:
	lda #%11111111 ; Set all pins on PORT B to output
	sta DDRB
	
	lda #%11100000 ; Set top 3 pins on PORT A to output
	sta DDRA

	lda #%00111000 ; Set 8-Bit mode, 2-line Display, 5x8 Font
	sta PORTB
	lda #0 ; Clear RS/RW/E bits
	sta PORTA
	lda #E ; Toggle Enable bit
	sta PORTA
	lda #0 ; Clear RS/RW/E bits
	sta PORTA
	lda #%00001110 ; Display on, cursor on, blink off
	sta PORTB
	lda #0 ; Clear RS/RW/E bits
	sta PORTA
	lda #E ; Toggle Enable bit
	sta PORTA
	lda #0 ; Clear RS/RW/E bits
	sta PORTA
	lda #%00000110 ; Inc and shift cursor, don't shift display
	sta PORTB
	lda #0 ; Clear RS/RW/E bits
	sta PORTA
	lda #E ; Toggle Enable bit
	sta PORTA
	lda #0 ; Clear RS/RW/E bits
	sta PORTA

	lda #"H"
	sta PORTB
	lda #RS ; Clear RS/RW/E bits
	sta PORTA
	lda #(RS | E) ; Toggle Enable bit
	sta PORTA
	lda #RS ; Clear RS/RW/E bits
	sta PORTA
	lda #"a"
	sta PORTB
	lda #RS ; Clear RS/RW/E bits
	sta PORTA
	lda #(RS | E) ; Toggle Enable bit
	sta PORTA
	lda #RS ; Clear RS/RW/E bits
	sta PORTA
	lda #"l"
	sta PORTB
	lda #RS ; Clear RS/RW/E bits
	sta PORTA
	lda #(RS | E) ; Toggle Enable bit
	sta PORTA
	lda #RS ; Clear RS/RW/E bits
	sta PORTA
	lda #"l"
	sta PORTB
	lda #RS ; Clear RS/RW/E bits
	sta PORTA
	lda #(RS | E) ; Toggle Enable bit
	sta PORTA
	lda #RS ; Clear RS/RW/E bits
	sta PORTA
	lda #"o"
	sta PORTB
	lda #RS ; Clear RS/RW/E bits
	sta PORTA
	lda #(RS | E) ; Toggle Enable bit
	sta PORTA
	lda #RS ; Clear RS/RW/E bits
	sta PORTA
	lda #" "
	sta PORTB
	lda #RS ; Clear RS/RW/E bits
	sta PORTA
	lda #(RS | E) ; Toggle Enable bit
	sta PORTA
	lda #RS ; Clear RS/RW/E bits
	sta PORTA
	lda #"W"
	sta PORTB
	lda #RS ; Clear RS/RW/E bits
	sta PORTA
	lda #(RS | E) ; Toggle Enable bit
	sta PORTA
	lda #RS ; Clear RS/RW/E bits
	sta PORTA
	lda #"e"
	sta PORTB
	lda #RS ; Clear RS/RW/E bits
	sta PORTA
	lda #(RS | E) ; Toggle Enable bit
	sta PORTA
	lda #RS ; Clear RS/RW/E bits
	sta PORTA
	lda #"l"
	sta PORTB
	lda #RS ; Clear RS/RW/E bits
	sta PORTA
	lda #(RS | E) ; Toggle Enable bit
	sta PORTA
	lda #RS ; Clear RS/RW/E bits
	sta PORTA
	lda #"t"
	sta PORTB
	lda #RS ; Clear RS/RW/E bits
	sta PORTA
	lda #(RS | E) ; Toggle Enable bit
	sta PORTA
	lda #RS ; Clear RS/RW/E bits
	sta PORTA
	lda #"!"
	sta PORTB
	lda #RS ; Clear RS/RW/E bits
	sta PORTA
	lda #(RS | E) ; Toggle Enable bit
	sta PORTA
	lda #RS ; Clear RS/RW/E bits
	sta PORTA
	
loop:
	jmp loop

	.org $FFFC
	.word init
	.word $0000
