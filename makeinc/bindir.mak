ifneq (B,)
$B%: $B

$B%: %
	cat $< > $@

CLEAN+=$B
$B:
	mkdir -p $@
endif
