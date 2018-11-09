%.o: %.c
%.o: %.s

$B%.o: $B%.c
	$(CC) -c $(CFLAGS) -o $@ $<

$B%.o: $B%.s
	$(CC) -c $(CFLAGS) $(ASFLAGS) -o $@ $<

$B%.o: $B%.asm
	$(NASM) -f elf $(NASMFLAGS) -o $@ $<

$B%.gas: $B%.c
	$(CC1) -o $@ $<

$B%.nas: $B%.gas
	$(GAS2NASK) $< $@

$B%.obj: $B%.nas
	$(NASK) $< $@

$B%.obj: $B%.asm
	$(NASK) $< $@

$B%.bin: $B%.asm
	$(NASM) -f bin $(NASMFLAGS) -o $@ $<

$B%.bin: $B%.nas
	$(NASK) $< $@

$B%.bin: $B%.nas
	$(NASK) $< $@
