PHONY+=dep
dep: $B.deps

PHONY+=dep
dep: $B.deps

$B.deps: $(ONAMES:%=$B%.d)
	$(PERL) $Smergedeps.pl $@ $^

-include $B.deps
