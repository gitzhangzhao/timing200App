# 2006 Aug 23, leige for bucket selection
# eventNo 0x24=36, gunT, 
#         0x20=32, ram0; 0x21=33, bkt0; 0x28=40, Kly

record(event, "$(SUBSYS):$(DEVICE):event:ram0")
{
field(DESC, "")
field(ASG, "")
field(SCAN, "I/O Intr")
field(PINI, "NO")
field(PHAS, "0")
field(EVNT, "0")
field(TSE, "0")
field(TSEL, "0")
field(DTYP, "APS event receiver")
field(DISV, "0")
field(DISA, "1")
field(SDIS, "0")
field(DISS, "NO_ALARM")
field(PRIO, "LOW")
field(FLNK, "$(SUBSYS):$(DEVICE):calc:ram0 PP MS")
# field(INP, "#C$(ERslot) S32 @")
field(SIOL, "0")
field(SIML, "0")
field(SIMS, "NO_ALARM")
# field(VAL, "32")  
# don't calc this, to save time for IOC
field(VAL, "0")
}

record(calc,"$(SUBSYS):$(DEVICE):calc:ram0") {
   field(PREC, "0")
   field(SCAN,"Passive")
   field(CALC, "(A<B)?(A+C):D")
   field(INPA, "$(SUBSYS):$(DEVICE):calc:ram0.VAL NPP MS")
   field(INPB, "100000")
   field(INPC, "1")
   field(INPD, "0")
}

record(event, "$(SUBSYS):$(DEVICE):event:bkt0")
{
field(DESC, "")
field(ASG, "")
field(SCAN, "I/O Intr")
field(PINI, "NO")
field(PHAS, "0")
field(EVNT, "0")
field(TSE, "0")
field(TSEL, "0")
field(DTYP, "APS event receiver")
field(DISV, "0")
field(DISA, "1")
field(SDIS, "0")
field(DISS, "NO_ALARM")
field(PRIO, "LOW")
field(FLNK, "$(SUBSYS):$(DEVICE):calc:bkt0 PP MS")
# field(INP, "#C$(ERslot) S33 @")
field(SIOL, "0")
field(SIML, "0")
field(SIMS, "NO_ALARM")
#field(VAL, "33")
# don't calc this, to save time for IOC
field(VAL, "0")
}

record(calc,"$(SUBSYS):$(DEVICE):calc:bkt0") {
   field(PREC, "0")
   field(SCAN,"Passive")
   field(CALC, "(A<B)?(A+C):D")
   field(INPA, "$(SUBSYS):$(DEVICE):calc:bkt0.VAL NPP MS")
   field(INPB, "100000")
   field(INPC, "1")
   field(INPD, "0")
}

record(event, "$(SUBSYS):$(DEVICE):event:gunT")
{
field(DESC, "")
field(ASG, "")
field(SCAN, "I/O Intr")
field(PINI, "NO")
field(PHAS, "0")
field(EVNT, "0")
field(TSE, "0")
field(TSEL, "0")
field(DTYP, "APS event receiver")
field(DISV, "0")
field(DISA, "0")
field(SDIS, "0")
field(DISS, "NO_ALARM")
field(PRIO, "LOW")
field(FLNK, "$(SUBSYS):$(DEVICE):calc:gunT PP MS")
field(INP, "#C$(ERslot) S36 @")
field(SIOL, "0")
field(SIML, "0")
field(SIMS, "NO_ALARM")
field(VAL, "36")
}

record(calc,"$(SUBSYS):$(DEVICE):calc:gunT") {
   field(PREC, "0")
   field(SCAN,"Passive")
   field(CALC, "(A<B)?(A+C):D")
   field(INPA, "$(SUBSYS):$(DEVICE):calc:gunT.VAL NPP MS")
   field(INPB, "100000")
   field(INPC, "1")
   field(INPD, "0")
}

record(event, "$(SUBSYS):$(DEVICE):event:Kly")
{
field(DESC, "")
field(ASG, "")
field(SCAN, "I/O Intr")
field(PINI, "NO")
field(PHAS, "0")
field(EVNT, "0")
field(TSE, "0")
field(TSEL, "0")
field(DTYP, "APS event receiver")
field(DISV, "0")
field(DISA, "0")
field(SDIS, "0")
field(DISS, "NO_ALARM")
field(PRIO, "LOW")
field(FLNK, "$(SUBSYS):$(DEVICE):calc:Kly PP MS")
# field(INP, "#C$(ERslot) S40 @")
field(SIOL, "0")
field(SIML, "0")
field(SIMS, "NO_ALARM")
# field(VAL, "40")
# don't calc this, to save time for IOC
field(VAL, "0")
}

record(calc,"$(SUBSYS):$(DEVICE):calc:Kly") {
   field(PREC, "0")
   field(SCAN,"Passive")
   field(CALC, "(A<B)?(A+C):D")
   field(INPA, "$(SUBSYS):$(DEVICE):calc:Kly.VAL NPP MS")
   field(INPB, "100000")
   field(INPC, "1")
   field(INPD, "0")
}

record(event, "$(SUBSYS):$(DEVICE):event:Kck")
{
field(DESC, "")
field(ASG, "")
field(SCAN, "I/O Intr")
field(PINI, "NO")
field(PHAS, "0")
field(EVNT, "0")
field(TSE, "0")
field(TSEL, "0")
field(DTYP, "APS event receiver")
field(DISV, "0")
field(DISA, "0")
field(SDIS, "0")
field(DISS, "NO_ALARM")
field(PRIO, "LOW")
field(FLNK, "$(SUBSYS):$(DEVICE):calc:Kck PP MS")
# field(INP, "#C$(ERslot) S34 @")
field(SIOL, "0")
field(SIML, "0")
field(SIMS, "NO_ALARM")
# field(VAL, "34")
# don't calc this, to save time for IOC
field(VAL, "0")
}

record(calc,"$(SUBSYS):$(DEVICE):calc:Kck") {
   field(PREC, "0")
   field(SCAN,"Passive")
   field(CALC, "(A<B)?(A+C):D")
   field(INPA, "$(SUBSYS):$(DEVICE):calc:Kck.VAL NPP MS")
   field(INPB, "100000")
   field(INPC, "1")
   field(INPD, "0")
}


