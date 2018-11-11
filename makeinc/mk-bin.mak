$K.elf: $(KERN).ld $(OBJS)
	$E ld $@
	$V$(CC) $(CFLAGS) $(LDFLAGS) -T $(KERN).ld -o $@ $^

$K.bin: $K.elf
	$E objcopy $@
	$V$(OBJCOPY) -O binary -R .note -R .comment -S $< $@

$K.bim: $(KERN).rul $(ASKOBJS)
	$E obj2bim $@
	$V$(OBJ2BIM) @$N.rul out:$@ stack:3136k $K.map $(ASKOBJS)

$K.hrb: $K.bim
	$E bim2hrb $@
	$V$(BIM2HRB) $@ $< 0
