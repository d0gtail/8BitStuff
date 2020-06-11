VASM=~/vasm/vasm6502_oldstyle
VASMFLAGS=-dotdir -Fbin
lcdHW: lcdHW.s
	$(VASM) $(VASMFLAGS) lcdHW.s -o lcdHW.hex
lcdHWnoStack: lcdHWnoStack.s
	$(VASM) $(VASMFLAGS) lcdHWnoStack.s -o lcdHWnoStack.hex

clean:
	rm -rf *.hex

