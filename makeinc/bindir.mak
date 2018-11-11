ifneq (B,)
$B%: % $B
	$Vcat $< > $@

$B%: $B

CLEAN+=$B
$B:
	$Vmkdir -p $@
endif
