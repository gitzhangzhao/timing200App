record(bo, "$(SUBSYS):$(DEVICE):bo:start")
{
	field(SCAN, "Passive")
        field(VAL, "0")
	field(FLNK, "$(SUBSYS):$(DEVICE):fout:start")
}
record(dfanout, "$(SUBSYS):$(DEVICE):fout:start")
{
	field(SCAN, "Passive")
	field(OMSL, "closed_loop")
	field(DOL, "$(SUBSYS):$(DEVICE):bo:start")
	field(OUTA, "$(SUBSYS):$(DEVICE):erEvt:gunT.OUT1 PP")
	field(OUTB, "$(SUBSYS):$(DEVICE):erEvt:gunT.OUT2 PP")
	field(OUTC, "$(SUBSYS):$(DEVICE):erEvt:gunT.OUT3 PP")
	field(OUTD, "$(SUBSYS):$(DEVICE):erEvt:gunT.OUT4 PP")
	field(OUTE, "$(SUBSYS):$(DEVICE):erEvt:gunT.OUT5 PP")
	field(OUTF, "$(SUBSYS):$(DEVICE):erEvt:gunT.OUT6 PP")
	field(OUTG, "$(SUBSYS):$(DEVICE):erEvt:gunT.OUT7 PP")
}

