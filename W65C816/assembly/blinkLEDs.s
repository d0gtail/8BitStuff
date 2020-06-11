PORTA=$6001
PORTB=$6000
DDRA=$6003
DDRB=$6002

	.org $8000
.initialPorts
	lda #$FF
	sta DDRA
	sta DDRB
.loop
	lda #$55
	sta PORTA
	sta PORTB

	lda #$AA
	sta PORTA
	sta PORTB

	jmp .loop

	.org $FFFC
	.word $8000
	.word $0000
