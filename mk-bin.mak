CLEAN+=$(ELF)
$(BIN): $(ELF)
	$(OBJCOPY) -O binary -R .note -R .comment -S $< $@
