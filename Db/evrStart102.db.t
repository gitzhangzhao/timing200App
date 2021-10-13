record(bo, "$(SUBSYS):$(DEVICE):bo:start")
{
	field(SCAN, "Passive")
	field(FLNK, "$(SUBSYS):$(DEVICE):fout:start")
}
record(dfanout, "$(SUBSYS):$(DEVICE):fout:start")
{
	field(SCAN, "Passive")
	field(OMSL, "closed_loop")
	field(DOL, "$(SUBSYS):$(DEVICE):bo:start")
#	field(OUTA, "$(SUBSYS):$(DEVICE):erEvt:ram0.ENAB")
#	field(OUTB, "$(SUBSYS):$(DEVICE):erEvt:bkt0.ENAB")
#	field(OUTC, "$(SUBSYS):$(DEVICE):erEvt:gunT.ENAB")
#	field(OUTD, "$(SUBSYS):$(DEVICE):erEvt:test.ENAB")
	field(OUTA, "$(SUBSYS):$(DEVICE):er.OTP1 PP")
	field(OUTB, "$(SUBSYS):$(DEVICE):er.OTP2 PP")
	field(OUTC, "$(SUBSYS):$(DEVICE):er.OTP3 PP")
	field(OUTD, "$(SUBSYS):$(DEVICE):er.OTP4 PP")
	field(OUTE, "$(SUBSYS):$(DEVICE):er.OTP5 PP")
}
record(bo, "$(SUBSYS):$(DEVICE):bo:stop")
{
        field(SCAN, "Passive")
        field(FLNK, "$(SUBSYS):$(DEVICE):fout:stop")
}
record(fanout, "$(SUBSYS):$(DEVICE):fout:stop")
{
        field(SCAN, "Passive")
        field(LNK1, "$(SUBSYS):$(DEVICE):erEvt:ram0")
        field(LNK2, "$(SUBSYS):$(DEVICE):erEvt:bkt0")
        field(LNK3, "$(SUBSYS):$(DEVICE):erEvt:gunT")
        field(LNK4, "$(SUBSYS):$(DEVICE):erEvt:test")
}

