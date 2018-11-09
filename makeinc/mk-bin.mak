$K.elf: $(KERN).ld $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -T $(KERN).ld -o $@ $^

$K.bin: $K.elf
	$(OBJCOPY) -O binary -R .note -R .comment -S $< $@

$K.bim: $(KERN).rul $(ASKOBJS)
	$(OBJ2BIM) @$N.rul out:$@ stack:3136k $K.map $(ASKOBJS)

$K.hrb: $K.bim
	$(BIM2HRB) $@ $< 0
