#include <vxWorks.h>
#include <semLib.h>
#include "ellLib.h"
#include "fast_lock.h"
#include "link.h"
#include "tsDefs.h"

#ifndef INCerTRG0H
#define INCerTRG0H
typedef enum {
	erTRG0_Disabled,
	erTRG0_Enabled
}erTRG0;
#endif /*INCerTRG0H*/

#ifndef INCerOPPH
#define INCerOPPH
typedef enum {
	erOPP_Normal,
	erOPP_Inverted
}erOPP;
#endif /*INCerOPPH*/
#ifndef INCerH
#define INCerH
typedef struct erRecord {
	char		name[29]; /*Record Name*/
	char		desc[29]; /*Descriptor*/
	char		asg[29]; /*Access Security Group*/
	unsigned short	scan;	/*Scan Mechanism*/
	unsigned short	pini;	/*Process at iocInit*/
	short		phas;	/*Scan Phase*/
	short		evnt;	/*Event Number*/
	short		tse;	/*Time Stamp Event*/
	DBLINK		tsel;	/*Time Stamp Link*/
	unsigned short	dtyp;	/*Device Type*/
	short		disv;	/*Disable Value*/
	short		disa;	/*Disable*/
	DBLINK		sdis;	/*Scanning Disable*/
	FAST_LOCK	mlok;	/*Monitor fastlock*/
	ELLLIST		mlis;	/*Monitor List*/
	unsigned char	disp;	/*Disable putField*/
	unsigned char	proc;	/*Force Processing*/
	unsigned short	stat;	/*Alarm Status*/
	unsigned short	sevr;	/*Alarm Severity*/
	unsigned short	nsta;	/*New Alarm Status*/
	unsigned short	nsev;	/*New Alarm Severity*/
	unsigned short	acks;	/*Alarm Ack Severity*/
	unsigned short	ackt;	/*Alarm Ack Transient*/
	unsigned short	diss;	/*Disable Alarm Sevrty*/
	unsigned char	lcnt;	/*Lock Count*/
	unsigned char	pact;	/*Record active*/
	unsigned char	putf;	/*dbPutField process*/
	unsigned char	rpro;	/*Reprocess */
	void		*asp;	/*Access Security Pvt*/
	struct putNotify *ppn;	/*addr of PUTNOTIFY*/
	struct pnWaitNode *ppnn;	/*next record PUTNOTIFY*/
	struct scan_element *spvt;	/*Scan Private*/
	struct rset	*rset;	/*Address of RSET*/
	struct dset	*dset;	/*DSET address*/
	void		*dpvt;	/*Device Private*/
	struct dbRecordType *rdes;	/*Address of dbRecordType*/
	struct lockRecord *lset;	/*Lock Set*/
	unsigned short	prio;	/*Scheduling Priority*/
	unsigned char	tpro;	/*Trace Processing*/
	char bkpt;	/*Break Point*/
	unsigned char	udf;	/*Undefined*/
	TS_STAMP	time;	/*Time*/
	DBLINK		flnk;	/*Forward Process Link*/
	char		val;	/*Worthless Value*/
	DBLINK		out;	/*Output Specification*/
	unsigned short	enab;	/*Master Enable*/
	long		taxi;	/*Taxi Violation*/
	long		ltax;	/*Last Taxi Violation*/
	unsigned short	trg0;	/*Trigger 0 Enable*/
	unsigned short	trg1;	/*Trigger 1 Enable*/
	unsigned short	trg2;	/*Trigger 2 Enable*/
	unsigned short	trg3;	/*Trigger 3 Enable*/
	unsigned short	trg4;	/*Trigger 4 Enable*/
	unsigned short	trg5;	/*Trigger 5 Enable*/
	unsigned short	trg6;	/*Trigger 6 Enable*/
	unsigned short	ot0b;	/*OTB 0 Enable*/
	unsigned short	ot1b;	/*OTB 1 Enable*/
	unsigned short	ot2b;	/*OTB 2 Enable*/
	unsigned short	ot3b;	/*OTB 3 Enable*/
	unsigned short	ot4b;	/*OTB 4 Enable*/
	unsigned short	ot5b;	/*OTB 5 Enable*/
	unsigned short	ot6b;	/*OTB 6 Enable*/
	unsigned short	ot7b;	/*OTB 7 Enable*/
	long		fpgv;	/*Virtex Configuration Version*/
	unsigned short	otp0;	/*OTP 0 Enable*/
	unsigned short	otp1;	/*OTP 1 Enable*/
	unsigned short	otp2;	/*OTP 2 Enable*/
	unsigned short	otp3;	/*OTP 3 Enable*/
	unsigned short	otp4;	/*OTP 4 Enable*/
	unsigned short	otp5;	/*OTP 5 Enable*/
	unsigned short	otp6;	/*OTP 6 Enable*/
	unsigned short	otp7;	/*OTP 7 Enable*/
	unsigned short	otp8;	/*OTP 8 Enable*/
	unsigned short	otp9;	/*OTP 9 Enable*/
	unsigned short	otpa;	/*OTP 10 Enable*/
	unsigned short	otpb;	/*OTP 11 Enable*/
	unsigned short	otpc;	/*OTP 12 Enable*/
	unsigned short	otpd;	/*OTP 13 Enable*/
	unsigned short	ot0w;	/*OTP 0 Width*/
	unsigned long	ot1w;	/*OTP 1 Width*/
	unsigned long	ot2w;	/*OTP 2 Width*/
	unsigned long	ot3w;	/*OTP 3 Width*/
	unsigned long	ot4w;	/*OTP 4 Width*/
	unsigned long	ot5w;	/*OTP 5 Width*/
	unsigned long	ot6w;	/*OTP 6 Width*/
	unsigned long	ot7w;	/*OTP 7 Width*/
	unsigned long	ot8w;	/*OTP 8 Width*/
	unsigned long	ot9w;	/*OTP 9 Width*/
	unsigned long	otaw;	/*OTP 10 Width*/
	unsigned long	otbw;	/*OTP 11 Width*/
	unsigned long	otcw;	/*OTP 12 Width*/
	unsigned long	otdw;	/*OTP 13 Width*/
	unsigned long	ot0d;	/*OTP 0 Delay*/
	unsigned long	ot1d;	/*OTP 1 Delay*/
	unsigned long	ot2d;	/*OTP 2 Delay*/
	unsigned long	ot3d;	/*OTP 3 Delay*/
	unsigned long	ot4d;	/*OTP 4 Delay*/
	unsigned long	ot5d;	/*OTP 5 Delay*/
	unsigned long	ot6d;	/*OTP 6 Delay*/
	unsigned long	ot7d;	/*OTP 7 Delay*/
	unsigned long	ot8d;	/*OTP 8 Delay*/
	unsigned long	ot9d;	/*OTP 9 Delay*/
	unsigned long	otad;	/*OTP 10 Delay*/
	unsigned long	otbd;	/*OTP 11 Delay*/
	unsigned long	otcd;	/*OTP 12 Delay*/
	unsigned long	otdd;	/*OTP 13 Delay*/
	unsigned short	ot0p;	/*OTP 0 Polarity*/
	unsigned short	ot1p;	/*OTP 1 Polarity*/
	unsigned short	ot2p;	/*OTP 2 Polarity*/
	unsigned short	ot3p;	/*OTP 3 Polarity*/
	unsigned short	ot4p;	/*OTP 4 Polarity*/
	unsigned short	ot5p;	/*OTP 5 Polarity*/
	unsigned short	ot6p;	/*OTP 6 Polarity*/
	unsigned short	ot7p;	/*OTP 7 Polarity*/
	unsigned short	ot8p;	/*OTP 8 Polarity*/
	unsigned short	ot9p;	/*OTP 9 Polarity*/
	unsigned short	otap;	/*OTP 10 Polarity*/
	unsigned short	otbp;	/*OTP 11 Polarity*/
	unsigned short	otcp;	/*OTP 12 Polarity*/
	unsigned short	otdp;	/*OTP 13 Polarity*/
	unsigned short	otl0;	/*OTL 0 Enable*/
	unsigned short	otl1;	/*OTL 1 Enable*/
	unsigned short	otl2;	/*OTL 2 Enable*/
	unsigned short	otl3;	/*OTL 3 Enable*/
	unsigned short	otl4;	/*OTL 4 Enable*/
	unsigned short	otl5;	/*OTL 5 Enable*/
	unsigned short	otl6;	/*OTL 6 Enable*/
	long		dgcm;	/*DG Change Mask*/
	unsigned short	dg0e;	/*DG 0 Enable*/
	unsigned long	dg0d;	/*DG 0 Delay*/
	unsigned long	dg0w;	/*DG 0 Width*/
	unsigned short	dg0c;	/*DG 0 Clock prescaler*/
	unsigned short	dg0p;	/*DG 0 Parity*/
	unsigned short	dg1e;	/*DG 1 Enable*/
	unsigned long	dg1d;	/*DG 1 Delay*/
	unsigned long	dg1w;	/*DG 1 Width*/
	unsigned short	dg1c;	/*DG 1 Clock prescaler*/
	unsigned short	dg1p;	/*DG 1 Parity*/
	unsigned short	dg2e;	/*DG 2 Enable*/
	unsigned long	dg2d;	/*DG 2 Delay*/
	unsigned long	dg2w;	/*DG 2 Width*/
	unsigned short	dg2c;	/*DG 2 Clock prescaler*/
	unsigned short	dg2p;	/*DG 2 Parity*/
	unsigned short	dg3e;	/*DG 3 Enable*/
	unsigned long	dg3d;	/*DG 3 Delay*/
	unsigned long	dg3w;	/*DG 3 Width*/
	unsigned short	dg3c;	/*DG 3 Clock prescaler*/
	unsigned short	dg3p;	/*DG 3 Parity*/
	unsigned short	dvme;	/*Delayed IRQ Enable*/
	unsigned short	dvmd;	/*Delayed IRQ Delay*/
	unsigned short	dvmc;	/*Delayed IRQ Clock prescaler*/
	unsigned short	rxve;	/*RX violation IRQ Enable*/
	unsigned short	rxvc;	/*RX violation counter*/
	unsigned short	plok;	/*PLL lock lost*/
	unsigned short	fps0;	/*Front Panel output 0 select*/
	unsigned short	fps1;	/*Front Panel output 1 select*/
	unsigned short	fps2;	/*Front Panel output 2 select*/
	unsigned short	fps3;	/*Front Panel output 3 select*/
	unsigned short	fps4;	/*Front Panel output 4 select*/
	unsigned short	fps5;	/*Front Panel output 5 select*/
	unsigned short	fps6;	/*Front Panel output 6 select*/
} erRecord;
#define erRecordNAME	0
#define erRecordDESC	1
#define erRecordASG	2
#define erRecordSCAN	3
#define erRecordPINI	4
#define erRecordPHAS	5
#define erRecordEVNT	6
#define erRecordTSE	7
#define erRecordTSEL	8
#define erRecordDTYP	9
#define erRecordDISV	10
#define erRecordDISA	11
#define erRecordSDIS	12
#define erRecordMLOK	13
#define erRecordMLIS	14
#define erRecordDISP	15
#define erRecordPROC	16
#define erRecordSTAT	17
#define erRecordSEVR	18
#define erRecordNSTA	19
#define erRecordNSEV	20
#define erRecordACKS	21
#define erRecordACKT	22
#define erRecordDISS	23
#define erRecordLCNT	24
#define erRecordPACT	25
#define erRecordPUTF	26
#define erRecordRPRO	27
#define erRecordASP	28
#define erRecordPPN	29
#define erRecordPPNN	30
#define erRecordSPVT	31
#define erRecordRSET	32
#define erRecordDSET	33
#define erRecordDPVT	34
#define erRecordRDES	35
#define erRecordLSET	36
#define erRecordPRIO	37
#define erRecordTPRO	38
#define erRecordBKPT	39
#define erRecordUDF	40
#define erRecordTIME	41
#define erRecordFLNK	42
#define erRecordVAL	43
#define erRecordOUT	44
#define erRecordENAB	45
#define erRecordTAXI	46
#define erRecordLTAX	47
#define erRecordTRG0	48
#define erRecordTRG1	49
#define erRecordTRG2	50
#define erRecordTRG3	51
#define erRecordTRG4	52
#define erRecordTRG5	53
#define erRecordTRG6	54
#define erRecordOT0B	55
#define erRecordOT1B	56
#define erRecordOT2B	57
#define erRecordOT3B	58
#define erRecordOT4B	59
#define erRecordOT5B	60
#define erRecordOT6B	61
#define erRecordOT7B	62
#define erRecordFPGV	63
#define erRecordOTP0	64
#define erRecordOTP1	65
#define erRecordOTP2	66
#define erRecordOTP3	67
#define erRecordOTP4	68
#define erRecordOTP5	69
#define erRecordOTP6	70
#define erRecordOTP7	71
#define erRecordOTP8	72
#define erRecordOTP9	73
#define erRecordOTPA	74
#define erRecordOTPB	75
#define erRecordOTPC	76
#define erRecordOTPD	77
#define erRecordOT0W	78
#define erRecordOT1W	79
#define erRecordOT2W	80
#define erRecordOT3W	81
#define erRecordOT4W	82
#define erRecordOT5W	83
#define erRecordOT6W	84
#define erRecordOT7W	85
#define erRecordOT8W	86
#define erRecordOT9W	87
#define erRecordOTAW	88
#define erRecordOTBW	89
#define erRecordOTCW	90
#define erRecordOTDW	91
#define erRecordOT0D	92
#define erRecordOT1D	93
#define erRecordOT2D	94
#define erRecordOT3D	95
#define erRecordOT4D	96
#define erRecordOT5D	97
#define erRecordOT6D	98
#define erRecordOT7D	99
#define erRecordOT8D	100
#define erRecordOT9D	101
#define erRecordOTAD	102
#define erRecordOTBD	103
#define erRecordOTCD	104
#define erRecordOTDD	105
#define erRecordOT0P	106
#define erRecordOT1P	107
#define erRecordOT2P	108
#define erRecordOT3P	109
#define erRecordOT4P	110
#define erRecordOT5P	111
#define erRecordOT6P	112
#define erRecordOT7P	113
#define erRecordOT8P	114
#define erRecordOT9P	115
#define erRecordOTAP	116
#define erRecordOTBP	117
#define erRecordOTCP	118
#define erRecordOTDP	119
#define erRecordOTL0	120
#define erRecordOTL1	121
#define erRecordOTL2	122
#define erRecordOTL3	123
#define erRecordOTL4	124
#define erRecordOTL5	125
#define erRecordOTL6	126
#define erRecordDGCM	127
#define erRecordDG0E	128
#define erRecordDG0D	129
#define erRecordDG0W	130
#define erRecordDG0C	131
#define erRecordDG0P	132
#define erRecordDG1E	133
#define erRecordDG1D	134
#define erRecordDG1W	135
#define erRecordDG1C	136
#define erRecordDG1P	137
#define erRecordDG2E	138
#define erRecordDG2D	139
#define erRecordDG2W	140
#define erRecordDG2C	141
#define erRecordDG2P	142
#define erRecordDG3E	143
#define erRecordDG3D	144
#define erRecordDG3W	145
#define erRecordDG3C	146
#define erRecordDG3P	147
#define erRecordDVME	148
#define erRecordDVMD	149
#define erRecordDVMC	150
#define erRecordRXVE	151
#define erRecordRXVC	152
#define erRecordPLOK	153
#define erRecordFPS0	154
#define erRecordFPS1	155
#define erRecordFPS2	156
#define erRecordFPS3	157
#define erRecordFPS4	158
#define erRecordFPS5	159
#define erRecordFPS6	160
#endif /*INCerH*/
#ifdef GEN_SIZE_OFFSET
int erRecordSizeOffset(dbRecordType *pdbRecordType)
{
    erRecord *prec = 0;
  pdbRecordType->papFldDes[0]->size=sizeof(prec->name);
  pdbRecordType->papFldDes[0]->offset=(short)((char *)&prec->name - (char *)prec);
  pdbRecordType->papFldDes[1]->size=sizeof(prec->desc);
  pdbRecordType->papFldDes[1]->offset=(short)((char *)&prec->desc - (char *)prec);
  pdbRecordType->papFldDes[2]->size=sizeof(prec->asg);
  pdbRecordType->papFldDes[2]->offset=(short)((char *)&prec->asg - (char *)prec);
  pdbRecordType->papFldDes[3]->size=sizeof(prec->scan);
  pdbRecordType->papFldDes[3]->offset=(short)((char *)&prec->scan - (char *)prec);
  pdbRecordType->papFldDes[4]->size=sizeof(prec->pini);
  pdbRecordType->papFldDes[4]->offset=(short)((char *)&prec->pini - (char *)prec);
  pdbRecordType->papFldDes[5]->size=sizeof(prec->phas);
  pdbRecordType->papFldDes[5]->offset=(short)((char *)&prec->phas - (char *)prec);
  pdbRecordType->papFldDes[6]->size=sizeof(prec->evnt);
  pdbRecordType->papFldDes[6]->offset=(short)((char *)&prec->evnt - (char *)prec);
  pdbRecordType->papFldDes[7]->size=sizeof(prec->tse);
  pdbRecordType->papFldDes[7]->offset=(short)((char *)&prec->tse - (char *)prec);
  pdbRecordType->papFldDes[8]->size=sizeof(prec->tsel);
  pdbRecordType->papFldDes[8]->offset=(short)((char *)&prec->tsel - (char *)prec);
  pdbRecordType->papFldDes[9]->size=sizeof(prec->dtyp);
  pdbRecordType->papFldDes[9]->offset=(short)((char *)&prec->dtyp - (char *)prec);
  pdbRecordType->papFldDes[10]->size=sizeof(prec->disv);
  pdbRecordType->papFldDes[10]->offset=(short)((char *)&prec->disv - (char *)prec);
  pdbRecordType->papFldDes[11]->size=sizeof(prec->disa);
  pdbRecordType->papFldDes[11]->offset=(short)((char *)&prec->disa - (char *)prec);
  pdbRecordType->papFldDes[12]->size=sizeof(prec->sdis);
  pdbRecordType->papFldDes[12]->offset=(short)((char *)&prec->sdis - (char *)prec);
  pdbRecordType->papFldDes[13]->size=sizeof(prec->mlok);
  pdbRecordType->papFldDes[13]->offset=(short)((char *)&prec->mlok - (char *)prec);
  pdbRecordType->papFldDes[14]->size=sizeof(prec->mlis);
  pdbRecordType->papFldDes[14]->offset=(short)((char *)&prec->mlis - (char *)prec);
  pdbRecordType->papFldDes[15]->size=sizeof(prec->disp);
  pdbRecordType->papFldDes[15]->offset=(short)((char *)&prec->disp - (char *)prec);
  pdbRecordType->papFldDes[16]->size=sizeof(prec->proc);
  pdbRecordType->papFldDes[16]->offset=(short)((char *)&prec->proc - (char *)prec);
  pdbRecordType->papFldDes[17]->size=sizeof(prec->stat);
  pdbRecordType->papFldDes[17]->offset=(short)((char *)&prec->stat - (char *)prec);
  pdbRecordType->papFldDes[18]->size=sizeof(prec->sevr);
  pdbRecordType->papFldDes[18]->offset=(short)((char *)&prec->sevr - (char *)prec);
  pdbRecordType->papFldDes[19]->size=sizeof(prec->nsta);
  pdbRecordType->papFldDes[19]->offset=(short)((char *)&prec->nsta - (char *)prec);
  pdbRecordType->papFldDes[20]->size=sizeof(prec->nsev);
  pdbRecordType->papFldDes[20]->offset=(short)((char *)&prec->nsev - (char *)prec);
  pdbRecordType->papFldDes[21]->size=sizeof(prec->acks);
  pdbRecordType->papFldDes[21]->offset=(short)((char *)&prec->acks - (char *)prec);
  pdbRecordType->papFldDes[22]->size=sizeof(prec->ackt);
  pdbRecordType->papFldDes[22]->offset=(short)((char *)&prec->ackt - (char *)prec);
  pdbRecordType->papFldDes[23]->size=sizeof(prec->diss);
  pdbRecordType->papFldDes[23]->offset=(short)((char *)&prec->diss - (char *)prec);
  pdbRecordType->papFldDes[24]->size=sizeof(prec->lcnt);
  pdbRecordType->papFldDes[24]->offset=(short)((char *)&prec->lcnt - (char *)prec);
  pdbRecordType->papFldDes[25]->size=sizeof(prec->pact);
  pdbRecordType->papFldDes[25]->offset=(short)((char *)&prec->pact - (char *)prec);
  pdbRecordType->papFldDes[26]->size=sizeof(prec->putf);
  pdbRecordType->papFldDes[26]->offset=(short)((char *)&prec->putf - (char *)prec);
  pdbRecordType->papFldDes[27]->size=sizeof(prec->rpro);
  pdbRecordType->papFldDes[27]->offset=(short)((char *)&prec->rpro - (char *)prec);
  pdbRecordType->papFldDes[28]->size=sizeof(prec->asp);
  pdbRecordType->papFldDes[28]->offset=(short)((char *)&prec->asp - (char *)prec);
  pdbRecordType->papFldDes[29]->size=sizeof(prec->ppn);
  pdbRecordType->papFldDes[29]->offset=(short)((char *)&prec->ppn - (char *)prec);
  pdbRecordType->papFldDes[30]->size=sizeof(prec->ppnn);
  pdbRecordType->papFldDes[30]->offset=(short)((char *)&prec->ppnn - (char *)prec);
  pdbRecordType->papFldDes[31]->size=sizeof(prec->spvt);
  pdbRecordType->papFldDes[31]->offset=(short)((char *)&prec->spvt - (char *)prec);
  pdbRecordType->papFldDes[32]->size=sizeof(prec->rset);
  pdbRecordType->papFldDes[32]->offset=(short)((char *)&prec->rset - (char *)prec);
  pdbRecordType->papFldDes[33]->size=sizeof(prec->dset);
  pdbRecordType->papFldDes[33]->offset=(short)((char *)&prec->dset - (char *)prec);
  pdbRecordType->papFldDes[34]->size=sizeof(prec->dpvt);
  pdbRecordType->papFldDes[34]->offset=(short)((char *)&prec->dpvt - (char *)prec);
  pdbRecordType->papFldDes[35]->size=sizeof(prec->rdes);
  pdbRecordType->papFldDes[35]->offset=(short)((char *)&prec->rdes - (char *)prec);
  pdbRecordType->papFldDes[36]->size=sizeof(prec->lset);
  pdbRecordType->papFldDes[36]->offset=(short)((char *)&prec->lset - (char *)prec);
  pdbRecordType->papFldDes[37]->size=sizeof(prec->prio);
  pdbRecordType->papFldDes[37]->offset=(short)((char *)&prec->prio - (char *)prec);
  pdbRecordType->papFldDes[38]->size=sizeof(prec->tpro);
  pdbRecordType->papFldDes[38]->offset=(short)((char *)&prec->tpro - (char *)prec);
  pdbRecordType->papFldDes[39]->size=sizeof(prec->bkpt);
  pdbRecordType->papFldDes[39]->offset=(short)((char *)&prec->bkpt - (char *)prec);
  pdbRecordType->papFldDes[40]->size=sizeof(prec->udf);
  pdbRecordType->papFldDes[40]->offset=(short)((char *)&prec->udf - (char *)prec);
  pdbRecordType->papFldDes[41]->size=sizeof(prec->time);
  pdbRecordType->papFldDes[41]->offset=(short)((char *)&prec->time - (char *)prec);
  pdbRecordType->papFldDes[42]->size=sizeof(prec->flnk);
  pdbRecordType->papFldDes[42]->offset=(short)((char *)&prec->flnk - (char *)prec);
  pdbRecordType->papFldDes[43]->size=sizeof(prec->val);
  pdbRecordType->papFldDes[43]->offset=(short)((char *)&prec->val - (char *)prec);
  pdbRecordType->papFldDes[44]->size=sizeof(prec->out);
  pdbRecordType->papFldDes[44]->offset=(short)((char *)&prec->out - (char *)prec);
  pdbRecordType->papFldDes[45]->size=sizeof(prec->enab);
  pdbRecordType->papFldDes[45]->offset=(short)((char *)&prec->enab - (char *)prec);
  pdbRecordType->papFldDes[46]->size=sizeof(prec->taxi);
  pdbRecordType->papFldDes[46]->offset=(short)((char *)&prec->taxi - (char *)prec);
  pdbRecordType->papFldDes[47]->size=sizeof(prec->ltax);
  pdbRecordType->papFldDes[47]->offset=(short)((char *)&prec->ltax - (char *)prec);
  pdbRecordType->papFldDes[48]->size=sizeof(prec->trg0);
  pdbRecordType->papFldDes[48]->offset=(short)((char *)&prec->trg0 - (char *)prec);
  pdbRecordType->papFldDes[49]->size=sizeof(prec->trg1);
  pdbRecordType->papFldDes[49]->offset=(short)((char *)&prec->trg1 - (char *)prec);
  pdbRecordType->papFldDes[50]->size=sizeof(prec->trg2);
  pdbRecordType->papFldDes[50]->offset=(short)((char *)&prec->trg2 - (char *)prec);
  pdbRecordType->papFldDes[51]->size=sizeof(prec->trg3);
  pdbRecordType->papFldDes[51]->offset=(short)((char *)&prec->trg3 - (char *)prec);
  pdbRecordType->papFldDes[52]->size=sizeof(prec->trg4);
  pdbRecordType->papFldDes[52]->offset=(short)((char *)&prec->trg4 - (char *)prec);
  pdbRecordType->papFldDes[53]->size=sizeof(prec->trg5);
  pdbRecordType->papFldDes[53]->offset=(short)((char *)&prec->trg5 - (char *)prec);
  pdbRecordType->papFldDes[54]->size=sizeof(prec->trg6);
  pdbRecordType->papFldDes[54]->offset=(short)((char *)&prec->trg6 - (char *)prec);
  pdbRecordType->papFldDes[55]->size=sizeof(prec->ot0b);
  pdbRecordType->papFldDes[55]->offset=(short)((char *)&prec->ot0b - (char *)prec);
  pdbRecordType->papFldDes[56]->size=sizeof(prec->ot1b);
  pdbRecordType->papFldDes[56]->offset=(short)((char *)&prec->ot1b - (char *)prec);
  pdbRecordType->papFldDes[57]->size=sizeof(prec->ot2b);
  pdbRecordType->papFldDes[57]->offset=(short)((char *)&prec->ot2b - (char *)prec);
  pdbRecordType->papFldDes[58]->size=sizeof(prec->ot3b);
  pdbRecordType->papFldDes[58]->offset=(short)((char *)&prec->ot3b - (char *)prec);
  pdbRecordType->papFldDes[59]->size=sizeof(prec->ot4b);
  pdbRecordType->papFldDes[59]->offset=(short)((char *)&prec->ot4b - (char *)prec);
  pdbRecordType->papFldDes[60]->size=sizeof(prec->ot5b);
  pdbRecordType->papFldDes[60]->offset=(short)((char *)&prec->ot5b - (char *)prec);
  pdbRecordType->papFldDes[61]->size=sizeof(prec->ot6b);
  pdbRecordType->papFldDes[61]->offset=(short)((char *)&prec->ot6b - (char *)prec);
  pdbRecordType->papFldDes[62]->size=sizeof(prec->ot7b);
  pdbRecordType->papFldDes[62]->offset=(short)((char *)&prec->ot7b - (char *)prec);
  pdbRecordType->papFldDes[63]->size=sizeof(prec->fpgv);
  pdbRecordType->papFldDes[63]->offset=(short)((char *)&prec->fpgv - (char *)prec);
  pdbRecordType->papFldDes[64]->size=sizeof(prec->otp0);
  pdbRecordType->papFldDes[64]->offset=(short)((char *)&prec->otp0 - (char *)prec);
  pdbRecordType->papFldDes[65]->size=sizeof(prec->otp1);
  pdbRecordType->papFldDes[65]->offset=(short)((char *)&prec->otp1 - (char *)prec);
  pdbRecordType->papFldDes[66]->size=sizeof(prec->otp2);
  pdbRecordType->papFldDes[66]->offset=(short)((char *)&prec->otp2 - (char *)prec);
  pdbRecordType->papFldDes[67]->size=sizeof(prec->otp3);
  pdbRecordType->papFldDes[67]->offset=(short)((char *)&prec->otp3 - (char *)prec);
  pdbRecordType->papFldDes[68]->size=sizeof(prec->otp4);
  pdbRecordType->papFldDes[68]->offset=(short)((char *)&prec->otp4 - (char *)prec);
  pdbRecordType->papFldDes[69]->size=sizeof(prec->otp5);
  pdbRecordType->papFldDes[69]->offset=(short)((char *)&prec->otp5 - (char *)prec);
  pdbRecordType->papFldDes[70]->size=sizeof(prec->otp6);
  pdbRecordType->papFldDes[70]->offset=(short)((char *)&prec->otp6 - (char *)prec);
  pdbRecordType->papFldDes[71]->size=sizeof(prec->otp7);
  pdbRecordType->papFldDes[71]->offset=(short)((char *)&prec->otp7 - (char *)prec);
  pdbRecordType->papFldDes[72]->size=sizeof(prec->otp8);
  pdbRecordType->papFldDes[72]->offset=(short)((char *)&prec->otp8 - (char *)prec);
  pdbRecordType->papFldDes[73]->size=sizeof(prec->otp9);
  pdbRecordType->papFldDes[73]->offset=(short)((char *)&prec->otp9 - (char *)prec);
  pdbRecordType->papFldDes[74]->size=sizeof(prec->otpa);
  pdbRecordType->papFldDes[74]->offset=(short)((char *)&prec->otpa - (char *)prec);
  pdbRecordType->papFldDes[75]->size=sizeof(prec->otpb);
  pdbRecordType->papFldDes[75]->offset=(short)((char *)&prec->otpb - (char *)prec);
  pdbRecordType->papFldDes[76]->size=sizeof(prec->otpc);
  pdbRecordType->papFldDes[76]->offset=(short)((char *)&prec->otpc - (char *)prec);
  pdbRecordType->papFldDes[77]->size=sizeof(prec->otpd);
  pdbRecordType->papFldDes[77]->offset=(short)((char *)&prec->otpd - (char *)prec);
  pdbRecordType->papFldDes[78]->size=sizeof(prec->ot0w);
  pdbRecordType->papFldDes[78]->offset=(short)((char *)&prec->ot0w - (char *)prec);
  pdbRecordType->papFldDes[79]->size=sizeof(prec->ot1w);
  pdbRecordType->papFldDes[79]->offset=(short)((char *)&prec->ot1w - (char *)prec);
  pdbRecordType->papFldDes[80]->size=sizeof(prec->ot2w);
  pdbRecordType->papFldDes[80]->offset=(short)((char *)&prec->ot2w - (char *)prec);
  pdbRecordType->papFldDes[81]->size=sizeof(prec->ot3w);
  pdbRecordType->papFldDes[81]->offset=(short)((char *)&prec->ot3w - (char *)prec);
  pdbRecordType->papFldDes[82]->size=sizeof(prec->ot4w);
  pdbRecordType->papFldDes[82]->offset=(short)((char *)&prec->ot4w - (char *)prec);
  pdbRecordType->papFldDes[83]->size=sizeof(prec->ot5w);
  pdbRecordType->papFldDes[83]->offset=(short)((char *)&prec->ot5w - (char *)prec);
  pdbRecordType->papFldDes[84]->size=sizeof(prec->ot6w);
  pdbRecordType->papFldDes[84]->offset=(short)((char *)&prec->ot6w - (char *)prec);
  pdbRecordType->papFldDes[85]->size=sizeof(prec->ot7w);
  pdbRecordType->papFldDes[85]->offset=(short)((char *)&prec->ot7w - (char *)prec);
  pdbRecordType->papFldDes[86]->size=sizeof(prec->ot8w);
  pdbRecordType->papFldDes[86]->offset=(short)((char *)&prec->ot8w - (char *)prec);
  pdbRecordType->papFldDes[87]->size=sizeof(prec->ot9w);
  pdbRecordType->papFldDes[87]->offset=(short)((char *)&prec->ot9w - (char *)prec);
  pdbRecordType->papFldDes[88]->size=sizeof(prec->otaw);
  pdbRecordType->papFldDes[88]->offset=(short)((char *)&prec->otaw - (char *)prec);
  pdbRecordType->papFldDes[89]->size=sizeof(prec->otbw);
  pdbRecordType->papFldDes[89]->offset=(short)((char *)&prec->otbw - (char *)prec);
  pdbRecordType->papFldDes[90]->size=sizeof(prec->otcw);
  pdbRecordType->papFldDes[90]->offset=(short)((char *)&prec->otcw - (char *)prec);
  pdbRecordType->papFldDes[91]->size=sizeof(prec->otdw);
  pdbRecordType->papFldDes[91]->offset=(short)((char *)&prec->otdw - (char *)prec);
  pdbRecordType->papFldDes[92]->size=sizeof(prec->ot0d);
  pdbRecordType->papFldDes[92]->offset=(short)((char *)&prec->ot0d - (char *)prec);
  pdbRecordType->papFldDes[93]->size=sizeof(prec->ot1d);
  pdbRecordType->papFldDes[93]->offset=(short)((char *)&prec->ot1d - (char *)prec);
  pdbRecordType->papFldDes[94]->size=sizeof(prec->ot2d);
  pdbRecordType->papFldDes[94]->offset=(short)((char *)&prec->ot2d - (char *)prec);
  pdbRecordType->papFldDes[95]->size=sizeof(prec->ot3d);
  pdbRecordType->papFldDes[95]->offset=(short)((char *)&prec->ot3d - (char *)prec);
  pdbRecordType->papFldDes[96]->size=sizeof(prec->ot4d);
  pdbRecordType->papFldDes[96]->offset=(short)((char *)&prec->ot4d - (char *)prec);
  pdbRecordType->papFldDes[97]->size=sizeof(prec->ot5d);
  pdbRecordType->papFldDes[97]->offset=(short)((char *)&prec->ot5d - (char *)prec);
  pdbRecordType->papFldDes[98]->size=sizeof(prec->ot6d);
  pdbRecordType->papFldDes[98]->offset=(short)((char *)&prec->ot6d - (char *)prec);
  pdbRecordType->papFldDes[99]->size=sizeof(prec->ot7d);
  pdbRecordType->papFldDes[99]->offset=(short)((char *)&prec->ot7d - (char *)prec);
  pdbRecordType->papFldDes[100]->size=sizeof(prec->ot8d);
  pdbRecordType->papFldDes[100]->offset=(short)((char *)&prec->ot8d - (char *)prec);
  pdbRecordType->papFldDes[101]->size=sizeof(prec->ot9d);
  pdbRecordType->papFldDes[101]->offset=(short)((char *)&prec->ot9d - (char *)prec);
  pdbRecordType->papFldDes[102]->size=sizeof(prec->otad);
  pdbRecordType->papFldDes[102]->offset=(short)((char *)&prec->otad - (char *)prec);
  pdbRecordType->papFldDes[103]->size=sizeof(prec->otbd);
  pdbRecordType->papFldDes[103]->offset=(short)((char *)&prec->otbd - (char *)prec);
  pdbRecordType->papFldDes[104]->size=sizeof(prec->otcd);
  pdbRecordType->papFldDes[104]->offset=(short)((char *)&prec->otcd - (char *)prec);
  pdbRecordType->papFldDes[105]->size=sizeof(prec->otdd);
  pdbRecordType->papFldDes[105]->offset=(short)((char *)&prec->otdd - (char *)prec);
  pdbRecordType->papFldDes[106]->size=sizeof(prec->ot0p);
  pdbRecordType->papFldDes[106]->offset=(short)((char *)&prec->ot0p - (char *)prec);
  pdbRecordType->papFldDes[107]->size=sizeof(prec->ot1p);
  pdbRecordType->papFldDes[107]->offset=(short)((char *)&prec->ot1p - (char *)prec);
  pdbRecordType->papFldDes[108]->size=sizeof(prec->ot2p);
  pdbRecordType->papFldDes[108]->offset=(short)((char *)&prec->ot2p - (char *)prec);
  pdbRecordType->papFldDes[109]->size=sizeof(prec->ot3p);
  pdbRecordType->papFldDes[109]->offset=(short)((char *)&prec->ot3p - (char *)prec);
  pdbRecordType->papFldDes[110]->size=sizeof(prec->ot4p);
  pdbRecordType->papFldDes[110]->offset=(short)((char *)&prec->ot4p - (char *)prec);
  pdbRecordType->papFldDes[111]->size=sizeof(prec->ot5p);
  pdbRecordType->papFldDes[111]->offset=(short)((char *)&prec->ot5p - (char *)prec);
  pdbRecordType->papFldDes[112]->size=sizeof(prec->ot6p);
  pdbRecordType->papFldDes[112]->offset=(short)((char *)&prec->ot6p - (char *)prec);
  pdbRecordType->papFldDes[113]->size=sizeof(prec->ot7p);
  pdbRecordType->papFldDes[113]->offset=(short)((char *)&prec->ot7p - (char *)prec);
  pdbRecordType->papFldDes[114]->size=sizeof(prec->ot8p);
  pdbRecordType->papFldDes[114]->offset=(short)((char *)&prec->ot8p - (char *)prec);
  pdbRecordType->papFldDes[115]->size=sizeof(prec->ot9p);
  pdbRecordType->papFldDes[115]->offset=(short)((char *)&prec->ot9p - (char *)prec);
  pdbRecordType->papFldDes[116]->size=sizeof(prec->otap);
  pdbRecordType->papFldDes[116]->offset=(short)((char *)&prec->otap - (char *)prec);
  pdbRecordType->papFldDes[117]->size=sizeof(prec->otbp);
  pdbRecordType->papFldDes[117]->offset=(short)((char *)&prec->otbp - (char *)prec);
  pdbRecordType->papFldDes[118]->size=sizeof(prec->otcp);
  pdbRecordType->papFldDes[118]->offset=(short)((char *)&prec->otcp - (char *)prec);
  pdbRecordType->papFldDes[119]->size=sizeof(prec->otdp);
  pdbRecordType->papFldDes[119]->offset=(short)((char *)&prec->otdp - (char *)prec);
  pdbRecordType->papFldDes[120]->size=sizeof(prec->otl0);
  pdbRecordType->papFldDes[120]->offset=(short)((char *)&prec->otl0 - (char *)prec);
  pdbRecordType->papFldDes[121]->size=sizeof(prec->otl1);
  pdbRecordType->papFldDes[121]->offset=(short)((char *)&prec->otl1 - (char *)prec);
  pdbRecordType->papFldDes[122]->size=sizeof(prec->otl2);
  pdbRecordType->papFldDes[122]->offset=(short)((char *)&prec->otl2 - (char *)prec);
  pdbRecordType->papFldDes[123]->size=sizeof(prec->otl3);
  pdbRecordType->papFldDes[123]->offset=(short)((char *)&prec->otl3 - (char *)prec);
  pdbRecordType->papFldDes[124]->size=sizeof(prec->otl4);
  pdbRecordType->papFldDes[124]->offset=(short)((char *)&prec->otl4 - (char *)prec);
  pdbRecordType->papFldDes[125]->size=sizeof(prec->otl5);
  pdbRecordType->papFldDes[125]->offset=(short)((char *)&prec->otl5 - (char *)prec);
  pdbRecordType->papFldDes[126]->size=sizeof(prec->otl6);
  pdbRecordType->papFldDes[126]->offset=(short)((char *)&prec->otl6 - (char *)prec);
  pdbRecordType->papFldDes[127]->size=sizeof(prec->dgcm);
  pdbRecordType->papFldDes[127]->offset=(short)((char *)&prec->dgcm - (char *)prec);
  pdbRecordType->papFldDes[128]->size=sizeof(prec->dg0e);
  pdbRecordType->papFldDes[128]->offset=(short)((char *)&prec->dg0e - (char *)prec);
  pdbRecordType->papFldDes[129]->size=sizeof(prec->dg0d);
  pdbRecordType->papFldDes[129]->offset=(short)((char *)&prec->dg0d - (char *)prec);
  pdbRecordType->papFldDes[130]->size=sizeof(prec->dg0w);
  pdbRecordType->papFldDes[130]->offset=(short)((char *)&prec->dg0w - (char *)prec);
  pdbRecordType->papFldDes[131]->size=sizeof(prec->dg0c);
  pdbRecordType->papFldDes[131]->offset=(short)((char *)&prec->dg0c - (char *)prec);
  pdbRecordType->papFldDes[132]->size=sizeof(prec->dg0p);
  pdbRecordType->papFldDes[132]->offset=(short)((char *)&prec->dg0p - (char *)prec);
  pdbRecordType->papFldDes[133]->size=sizeof(prec->dg1e);
  pdbRecordType->papFldDes[133]->offset=(short)((char *)&prec->dg1e - (char *)prec);
  pdbRecordType->papFldDes[134]->size=sizeof(prec->dg1d);
  pdbRecordType->papFldDes[134]->offset=(short)((char *)&prec->dg1d - (char *)prec);
  pdbRecordType->papFldDes[135]->size=sizeof(prec->dg1w);
  pdbRecordType->papFldDes[135]->offset=(short)((char *)&prec->dg1w - (char *)prec);
  pdbRecordType->papFldDes[136]->size=sizeof(prec->dg1c);
  pdbRecordType->papFldDes[136]->offset=(short)((char *)&prec->dg1c - (char *)prec);
  pdbRecordType->papFldDes[137]->size=sizeof(prec->dg1p);
  pdbRecordType->papFldDes[137]->offset=(short)((char *)&prec->dg1p - (char *)prec);
  pdbRecordType->papFldDes[138]->size=sizeof(prec->dg2e);
  pdbRecordType->papFldDes[138]->offset=(short)((char *)&prec->dg2e - (char *)prec);
  pdbRecordType->papFldDes[139]->size=sizeof(prec->dg2d);
  pdbRecordType->papFldDes[139]->offset=(short)((char *)&prec->dg2d - (char *)prec);
  pdbRecordType->papFldDes[140]->size=sizeof(prec->dg2w);
  pdbRecordType->papFldDes[140]->offset=(short)((char *)&prec->dg2w - (char *)prec);
  pdbRecordType->papFldDes[141]->size=sizeof(prec->dg2c);
  pdbRecordType->papFldDes[141]->offset=(short)((char *)&prec->dg2c - (char *)prec);
  pdbRecordType->papFldDes[142]->size=sizeof(prec->dg2p);
  pdbRecordType->papFldDes[142]->offset=(short)((char *)&prec->dg2p - (char *)prec);
  pdbRecordType->papFldDes[143]->size=sizeof(prec->dg3e);
  pdbRecordType->papFldDes[143]->offset=(short)((char *)&prec->dg3e - (char *)prec);
  pdbRecordType->papFldDes[144]->size=sizeof(prec->dg3d);
  pdbRecordType->papFldDes[144]->offset=(short)((char *)&prec->dg3d - (char *)prec);
  pdbRecordType->papFldDes[145]->size=sizeof(prec->dg3w);
  pdbRecordType->papFldDes[145]->offset=(short)((char *)&prec->dg3w - (char *)prec);
  pdbRecordType->papFldDes[146]->size=sizeof(prec->dg3c);
  pdbRecordType->papFldDes[146]->offset=(short)((char *)&prec->dg3c - (char *)prec);
  pdbRecordType->papFldDes[147]->size=sizeof(prec->dg3p);
  pdbRecordType->papFldDes[147]->offset=(short)((char *)&prec->dg3p - (char *)prec);
  pdbRecordType->papFldDes[148]->size=sizeof(prec->dvme);
  pdbRecordType->papFldDes[148]->offset=(short)((char *)&prec->dvme - (char *)prec);
  pdbRecordType->papFldDes[149]->size=sizeof(prec->dvmd);
  pdbRecordType->papFldDes[149]->offset=(short)((char *)&prec->dvmd - (char *)prec);
  pdbRecordType->papFldDes[150]->size=sizeof(prec->dvmc);
  pdbRecordType->papFldDes[150]->offset=(short)((char *)&prec->dvmc - (char *)prec);
  pdbRecordType->papFldDes[151]->size=sizeof(prec->rxve);
  pdbRecordType->papFldDes[151]->offset=(short)((char *)&prec->rxve - (char *)prec);
  pdbRecordType->papFldDes[152]->size=sizeof(prec->rxvc);
  pdbRecordType->papFldDes[152]->offset=(short)((char *)&prec->rxvc - (char *)prec);
  pdbRecordType->papFldDes[153]->size=sizeof(prec->plok);
  pdbRecordType->papFldDes[153]->offset=(short)((char *)&prec->plok - (char *)prec);
  pdbRecordType->papFldDes[154]->size=sizeof(prec->fps0);
  pdbRecordType->papFldDes[154]->offset=(short)((char *)&prec->fps0 - (char *)prec);
  pdbRecordType->papFldDes[155]->size=sizeof(prec->fps1);
  pdbRecordType->papFldDes[155]->offset=(short)((char *)&prec->fps1 - (char *)prec);
  pdbRecordType->papFldDes[156]->size=sizeof(prec->fps2);
  pdbRecordType->papFldDes[156]->offset=(short)((char *)&prec->fps2 - (char *)prec);
  pdbRecordType->papFldDes[157]->size=sizeof(prec->fps3);
  pdbRecordType->papFldDes[157]->offset=(short)((char *)&prec->fps3 - (char *)prec);
  pdbRecordType->papFldDes[158]->size=sizeof(prec->fps4);
  pdbRecordType->papFldDes[158]->offset=(short)((char *)&prec->fps4 - (char *)prec);
  pdbRecordType->papFldDes[159]->size=sizeof(prec->fps5);
  pdbRecordType->papFldDes[159]->offset=(short)((char *)&prec->fps5 - (char *)prec);
  pdbRecordType->papFldDes[160]->size=sizeof(prec->fps6);
  pdbRecordType->papFldDes[160]->offset=(short)((char *)&prec->fps6 - (char *)prec);
    pdbRecordType->rec_size = sizeof(*prec);
    return(0);
}
#endif /*GEN_SIZE_OFFSET*/
