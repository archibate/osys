PHONY+=run
run: $(FDIMG)
	$(QEMU) $(QEMUFLAGS) -boot a -fda $(FDIMG)

bochsrun: $(FDIMG)
	$(BOCHS) $(BOCHSFLAGS) -qf bochsrc.bxrc
