PHONY+=dep
dep: $B.deps

PHONY+=dep
dep: $B.deps

$B.deps: $(ONAMES:%=$B%.d)
	$V$(PERL) $Smergedeps.pl $@ $^

ifndef NODEP
-include $B.deps
endif
