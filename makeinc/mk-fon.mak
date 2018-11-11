$(FONBIN) : $(FONTXT)
	$E makefont $@
	$V$(MAKEFONT) $< $@

$(FONOBJ) : $(FONBIN)
	$E bin2obj $@
	$V$(BIN2OBJ) $< $@ $(FONSYM)
