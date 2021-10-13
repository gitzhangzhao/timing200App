record(bo, "$(SUBSYS):$(DEVICE):bo:start")
{
	field(SCAN, "Passive")
	field(FLNK, "$(SUBSYS):$(DEVICE):fout:ena1")
}
record(dfanout, "$(SUBSYS):$(DEVICE):fout:ena1")
{
	field(SCAN, "Passive")
	field(OMSL, "closed_loop")
	field(DOL, "$(SUBSYS):$(DEVICE):bo:start")
	field(OUTA, "$(SUBSYS):$(DEVICE):event:ram0.DISA PP")
	field(OUTB, "$(SUBSYS):$(DEVICE):event:bkt0.DISA PP")
	field(OUTC, "$(SUBSYS):$(DEVICE):event:gunT.DISA PP")
	field(OUTD, "$(SUBSYS):$(DEVICE):event:Kly.DISA PP")
	field(OUTE, "$(SUBSYS):$(DEVICE):er.OTP0 NPP")
	field(OUTF, "$(SUBSYS):$(DEVICE):er.OTP1 NPP")
	field(OUTG, "$(SUBSYS):$(DEVICE):er.OTP2 NPP")
	field(OUTH, "$(SUBSYS):$(DEVICE):fout:ena2 PP")
}

record(dfanout, "$(SUBSYS):$(DEVICE):fout:ena2")
{
        field(SCAN, "Passive")
        field(OMSL, "closed_loop")
        field(DOL, "$(SUBSYS):$(DEVICE):bo:start")
        field(OUTA, "$(SUBSYS):$(DEVICE):er.OTP3 NPP")
        field(OUTB, "$(SUBSYS):$(DEVICE):er.OTP4 NPP")
        field(OUTC, "$(SUBSYS):$(DEVICE):er.OTP5 NPP")
        field(OUTD, "$(SUBSYS):$(DEVICE):er.OTP6 NPP")
        field(OUTE, "$(SUBSYS):$(DEVICE):er.OTP7 NPP")
        field(OUTF, "$(SUBSYS):$(DEVICE):er.OTP8 NPP")
        field(OUTG, "$(SUBSYS):$(DEVICE):er.OTP9 NPP")
        field(OUTH, "$(SUBSYS):$(DEVICE):fout:ena3 PP")
}

record(dfanout, "$(SUBSYS):$(DEVICE):fout:ena3")
{
        field(SCAN, "Passive")
        field(OMSL, "closed_loop")
        field(DOL, "$(SUBSYS):$(DEVICE):bo:start")
        field(OUTA, "$(SUBSYS):$(DEVICE):er.OTPA NPP")
        field(OUTB, "$(SUBSYS):$(DEVICE):er.OTPB NPP")
        field(OUTC, "$(SUBSYS):$(DEVICE):er.OTPC NPP")
        field(OUTD, "$(SUBSYS):$(DEVICE):er.OTPD PP")
}

#sub-harmonic buncher mode, i.e. dual bunches njection mode
record(bo, "$(SUBSYS):$(DEVICE):bo:SHBmode")
{
        field(SCAN, "Passive")
        field(VAL, "0")
}

record(bo, "$(SUBSYS):$(DEVICE):bo:pInj") {

}
record(bo, "$(SUBSYS):$(DEVICE):bo:sInj") {

}
record(ai, "$(SUBSYS):$(DEVICE):ai:bktSelect")
{
   field(VAL, "1")
}
record(ai, "$(SUBSYS):$(DEVICE):ai:injTimes")
{
   field(VAL, "10")
}

