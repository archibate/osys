%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

%.o: %.s
	$(CC) -c $(CFLAGS) $(ASFLAGS) -o $@ $<

%.o: %.asm
	$(NASM) -f elf $(NASMFLAGS) -o $@ $<
