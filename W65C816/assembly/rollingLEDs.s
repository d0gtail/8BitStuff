PORTA=$6001
PORTB=$6000
DDRA=$6003
DDRB=$6002

	.org $8000
initialPorts:
	lda #$FF
	sta DDRA
	sta DDRB

	lda #$01
	sta PORTB
loop:
	ror
	sta PORTB

	jmp loop

	.org $FFFC
	.word initialPorts
	.word $0000
