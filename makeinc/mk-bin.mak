ifndef LDSCRIPT
LDSCRIPT=$Suser.ld
endif

$K.elf: $(LDSCRIPT) $(OBJS)
	$E ld $@
	$V$(CC) $(CFLAGS) $(LDFLAGS) -T $(LDSCRIPT) -o $@ $(OBJS)

$K.bin: $K.elf
	$E strip $@
	$V$(STRIP) -O binary -o $@ $<

$K.exf: $K.bin
	$E bin2exf $@
	$V$(MKEXFHDR) $@ $$((5)) `size $K.elf | grep -v filename | awk '{print $$4}'`
	$Vcat $< >> $@

$K.bim: $(NAME).rul $(ASKOBJS)
	$E obj2bim $@
	$V$(OBJ2BIM) @$N.rul out:$@ stack:3136k $K.map $(ASKOBJS)

$K.hrb: $K.bim
	$E bim2hrb $@
	$V$(BIM2HRB) $@ $< 0
