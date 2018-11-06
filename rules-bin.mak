%.bin: %.asm
	$(NASM) -f bin $(NASMFLAGS) -o $@ $<
