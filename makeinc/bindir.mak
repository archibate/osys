ifneq (B,)
$B%: % $B
	@mkdir -p $(@D)
	$Vcat $< > $@

$B%: $B

CLEAN+=$B
$B:
	$Vmkdir -p $@
endif
