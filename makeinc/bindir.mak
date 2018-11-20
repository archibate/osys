ifneq (B,)
$B%: %
	@mkdir -p $(@D)
	$Vcat $< > $@

CLEAN+=$B
$B:
	$Vmkdir -p $@
endif
