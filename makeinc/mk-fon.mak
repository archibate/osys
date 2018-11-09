$(FONBIN) : $(FONTXT)
	$(MAKEFONT) $< $@

$(FONOBJ) : $(FONBIN)
	$(BIN2OBJ) $< $@ $(FONSYM)
