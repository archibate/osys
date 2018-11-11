PHONY+=dep
dep: $B.deps

$B.deps: $(ONAMES:%=$B%.d)
	$(PERL) $Smergedeps.pl $@ $^
