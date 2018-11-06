PHONY+=run
run: $(DISK)
	$(QEMU) $(QEMUFLAGS) -boot a -fda $(DISK)

bochsrun: $(DISK)
	$(BOCHS) $(BOCHSFLAGS) -qf bochsrc.txt
