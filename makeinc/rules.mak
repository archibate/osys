$B%.o: $B%.c
	$E cc $(<F)
	$V$(CC) -c $(CFLAGS) -o $@ $<

$B%.d: $B%.c
	$E dep $(<F)
	$V$(CC) -E -M -MT $B$*.o $(CFLAGS) $< > $@

$B%.o: $B%.s
	$E as $(<F)
	$V$(CC) -c $(CFLAGS) $(ASFLAGS) -o $@ $<

$B%.o: $B%.asm
	$E nasm $(<F)
	$V$(NASM) -f elf $(NASMFLAGS) -o $@ $<

$B%.d: $B%.asm
	$E dep $(<F)
	$V$(NASM) -M $(NASMFLAGS) $< > $@

$B%.gas: $B%.c
	$E cc1 $(<F)
	$V$(CC1) -o $@ $<

$B%.nas: $B%.gas
	$E g2n $(<F)
	$V$(GAS2NASK) $< $@

$B%.obj: $B%.nas
	$E nask $(<F)
	$V$(NASK) $< $@

$B%.obj: $B%.asm
	$E nask $(<F)
	$V$(NASK) $< $@

$B%.bin: $B%.asm
	$E nasm $(<F)
	$V$(NASM) -f bin $(NASMFLAGS) -o $@ $<

$B%.bin: $B%.nas
	$E nask $(<F)
	$V$(NASK) $< $@

$B%.bin: $B%.nas
	$E nask $(<F)
	$V$(NASK) $< $@
