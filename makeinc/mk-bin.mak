ifndef LDSCRIPT
LDSCRIPT=$Suser.ld
endif

$K.elf: $(LDSCRIPT) $(OBJS)
	$E ld $@
	$V$(CC) $(CFLAGS) $(LDFLAGS) -T $(LDSCRIPT) -o $@ $(OBJS)

$K.bin: $K.elf
	@#$E objcopy $@
	@#$V$(OBJCOPY) -O binary -S $< $@
	$E strip $@
	$V$(STRIP) -O binary -o $@ $<

$K.bim: $(NAME).rul $(ASKOBJS)
	$E obj2bim $@
	$V$(OBJ2BIM) @$N.rul out:$@ stack:3136k $K.map $(ASKOBJS)

$K.hrb: $K.bim
	$E bim2hrb $@
	$V$(BIM2HRB) $@ $< 0
