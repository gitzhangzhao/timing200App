#include <vxWorks.h>
#include <semLib.h>
#include "ellLib.h"
#include "fast_lock.h"
#include "link.h"
#include "tsDefs.h"

#ifndef INCegMOD1H
#define INCegMOD1H
typedef enum {
	egMOD1_Off,
	egMOD1_Normal,
	egMOD1_Normal_Recycle,
	egMOD1_Single,
	egMOD1_Alternate
}egMOD1;
#endif /*INCegMOD1H*/
#ifndef INCegH
#define INCegH
typedef struct egRecord {
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
	DBLINK		out;	/*Output Specification*/
	unsigned short	mod1;	/*RAM 1 Operating Mode*/
	double		r1sp;	/*RAM 1 Clock (Hz)*/
	unsigned short	mod2;	/*RAM 2 Operating Mode*/
	double		r2sp;	/*RAM 2 Clock (Hz)*/
	unsigned short	lmd1;	/*Last Operating Mode1*/
	unsigned short	lmd2;	/*Last Operating Mode2*/
	unsigned short	fifo;	/*Input FIFO Enable*/
	unsigned short	lffo;	/*Last FIFO Enable*/
	char		clr1;	/*Clear Sequence 1*/
	char		clr2;	/*Clear Sequence 2*/
	char		trg1;	/*Trigger Sequence 1*/
	char		trg2;	/*Trigger Sequence 2*/
	unsigned short	enab;	/*Master Enable*/
	long		lena;	/*Last Master Enable*/
	long		taxi;	/*Taxi Violation*/
	long		ltax;	/*Last Taxi Violation*/
	long		vme;	/*Generate VME Event*/
	unsigned short	ete0;	/*Trigger 0 Enable*/
	long		et0;	/*Trigger 0 Event*/
	long		let0;	/*Last Trigger 0 Event*/
	unsigned short	ete1;	/*Trigger 1 Enable*/
	long		et1;	/*Trigger 1 Event*/
	long		let1;	/*Last Trigger 1 Event*/
	unsigned short	ete2;	/*Trigger 2 Enable*/
	long		et2;	/*Trigger 2 Event*/
	long		let2;	/*Last Trigger 2 Event*/
	unsigned short	ete3;	/*Trigger 3 Enable*/
	long		et3;	/*Trigger 3 Event*/
	long		let3;	/*Last Trigger 3 Event*/
	unsigned short	ete4;	/*Trigger 4 Enable*/
	long		et4;	/*Trigger 4 Event*/
	long		let4;	/*Last Trigger 4 Event*/
	unsigned short	ete5;	/*Trigger 5 Enable*/
	long		et5;	/*Trigger 5 Event*/
	long		let5;	/*Last Trigger 5 Event*/
	unsigned short	ete6;	/*Trigger 6 Enable*/
	long		et6;	/*Trigger 6 Event*/
	long		let6;	/*Last Trigger 6 Event*/
	unsigned short	ete7;	/*Trigger 7 Enable*/
	long		et7;	/*Trigger 7 Event*/
	long		let7;	/*Last Trigger 7 Event*/
	unsigned short	md0e;	/*Mux. Counter dist. Bus 0 Enable*/
	unsigned short	md1e;	/*Mux. Counter dist. Bus 1 Enable*/
	unsigned short	md2e;	/*Mux. Counter dist. Bus 2 Enable*/
	unsigned short	md3e;	/*Mux. Counter dist. Bus 3 Enable*/
	unsigned short	md4e;	/*Mux. Counter dist. Bus 4 Enable*/
	unsigned short	md5e;	/*Mux. Counter dist. Bus 5 Enable*/
	unsigned short	md6e;	/*Mux. Counter dist. Bus 6 Enable*/
	unsigned short	md7e;	/*Mux. Counter dist. Bus 7 Enable*/
	unsigned short	me0e;	/*Mux. Counter Event trigger 0 Enable*/
	unsigned short	me1e;	/*Mux. Counter Event trigger 1 Enable*/
	unsigned short	me2e;	/*Mux. Counter Event trigger 2 Enable*/
	unsigned short	me3e;	/*Mux. Counter Event trigger 3 Enable*/
	unsigned short	me4e;	/*Mux. Counter Event trigger 4 Enable*/
	unsigned short	me5e;	/*Mux. Counter Event trigger 5 Enable*/
	unsigned short	me6e;	/*Mux. Counter Event trigger 6 Enable*/
	unsigned short	me7e;	/*Mux. Counter Event trigger 7 Enable*/
	unsigned short	asyn;	/*AC input and Mux. counter 7 Synch.*/
	unsigned short	aev0;	/*External event 0 trigger by AC input*/
	unsigned short	asq1;	/*External event 0 trigger by AC input*/
	unsigned short	asq2;	/*SEQRAM 1 trigger by AC input divider*/
	long		adiv;	/*AC input divisor*/
	long		aphs;	/*AC input phase shifter*/
	unsigned short	msq1;	/*SEQRAM 1 trigger by Mux. counter 7*/
	unsigned short	msq2;	/*SEQRAM 2 trigger by Mux. counter 7*/
	long		mc0p;	/*Mux. Counter 0 Clock Prescaler*/
	long		mc1p;	/*Mux. Counter 1 Clock Prescaler*/
	long		mc2p;	/*Mux. Counter 2 Clock Prescaler*/
	long		mc3p;	/*Mux. Counter 3 Clock Prescaler*/
	long		mc4p;	/*Mux. Counter 4 Clock Prescaler*/
	long		mc5p;	/*Mux. Counter 5 Clock Prescaler*/
	long		mc6p;	/*Mux. Counter 6 Clock Prescaler*/
	long		mc7p;	/*Mux. Counter 7 Clock Prescaler*/
	long		fpgv;	/*Virtex Configuration Version*/
	char		val;	/*Worthless Value*/
	long		r1ck;	/*RAM 1 Clock Prescaler*/
	long		r2ck;	/*RAM 2 Clock Prescaler*/
	long		rmax;	/*Maximum allowed RAM*/
	long		lr1c;	/*Last RAM 1 Clock Prescaler*/
	long		lr2c;	/*Last RAM 2 Clock Prescaler*/
	long		mcrs;	/*Reset Multiplexed Counters*/
} egRecord;
#define egRecordNAME	0
#define egRecordDESC	1
#define egRecordASG	2
#define egRecordSCAN	3
#define egRecordPINI	4
#define egRecordPHAS	5
#define egRecordEVNT	6
#define egRecordTSE	7
#define egRecordTSEL	8
#define egRecordDTYP	9
#define egRecordDISV	10
#define egRecordDISA	11
#define egRecordSDIS	12
#define egRecordMLOK	13
#define egRecordMLIS	14
#define egRecordDISP	15
#define egRecordPROC	16
#define egRecordSTAT	17
#define egRecordSEVR	18
#define egRecordNSTA	19
#define egRecordNSEV	20
#define egRecordACKS	21
#define egRecordACKT	22
#define egRecordDISS	23
#define egRecordLCNT	24
#define egRecordPACT	25
#define egRecordPUTF	26
#define egRecordRPRO	27
#define egRecordASP	28
#define egRecordPPN	29
#define egRecordPPNN	30
#define egRecordSPVT	31
#define egRecordRSET	32
#define egRecordDSET	33
#define egRecordDPVT	34
#define egRecordRDES	35
#define egRecordLSET	36
#define egRecordPRIO	37
#define egRecordTPRO	38
#define egRecordBKPT	39
#define egRecordUDF	40
#define egRecordTIME	41
#define egRecordFLNK	42
#define egRecordOUT	43
#define egRecordMOD1	44
#define egRecordR1SP	45
#define egRecordMOD2	46
#define egRecordR2SP	47
#define egRecordLMD1	48
#define egRecordLMD2	49
#define egRecordFIFO	50
#define egRecordLFFO	51
#define egRecordCLR1	52
#define egRecordCLR2	53
#define egRecordTRG1	54
#define egRecordTRG2	55
#define egRecordENAB	56
#define egRecordLENA	57
#define egRecordTAXI	58
#define egRecordLTAX	59
#define egRecordVME	60
#define egRecordETE0	61
#define egRecordET0	62
#define egRecordLET0	63
#define egRecordETE1	64
#define egRecordET1	65
#define egRecordLET1	66
#define egRecordETE2	67
#define egRecordET2	68
#define egRecordLET2	69
#define egRecordETE3	70
#define egRecordET3	71
#define egRecordLET3	72
#define egRecordETE4	73
#define egRecordET4	74
#define egRecordLET4	75
#define egRecordETE5	76
#define egRecordET5	77
#define egRecordLET5	78
#define egRecordETE6	79
#define egRecordET6	80
#define egRecordLET6	81
#define egRecordETE7	82
#define egRecordET7	83
#define egRecordLET7	84
#define egRecordMD0E	85
#define egRecordMD1E	86
#define egRecordMD2E	87
#define egRecordMD3E	88
#define egRecordMD4E	89
#define egRecordMD5E	90
#define egRecordMD6E	91
#define egRecordMD7E	92
#define egRecordME0E	93
#define egRecordME1E	94
#define egRecordME2E	95
#define egRecordME3E	96
#define egRecordME4E	97
#define egRecordME5E	98
#define egRecordME6E	99
#define egRecordME7E	100
#define egRecordASYN	101
#define egRecordAEV0	102
#define egRecordASQ1	103
#define egRecordASQ2	104
#define egRecordADIV	105
#define egRecordAPHS	106
#define egRecordMSQ1	107
#define egRecordMSQ2	108
#define egRecordMC0P	109
#define egRecordMC1P	110
#define egRecordMC2P	111
#define egRecordMC3P	112
#define egRecordMC4P	113
#define egRecordMC5P	114
#define egRecordMC6P	115
#define egRecordMC7P	116
#define egRecordFPGV	117
#define egRecordVAL	118
#define egRecordR1CK	119
#define egRecordR2CK	120
#define egRecordRMAX	121
#define egRecordLR1C	122
#define egRecordLR2C	123
#define egRecordMCRS	124
#endif /*INCegH*/
#ifdef GEN_SIZE_OFFSET
int egRecordSizeOffset(dbRecordType *pdbRecordType)
{
    egRecord *prec = 0;
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
  pdbRecordType->papFldDes[43]->size=sizeof(prec->out);
  pdbRecordType->papFldDes[43]->offset=(short)((char *)&prec->out - (char *)prec);
  pdbRecordType->papFldDes[44]->size=sizeof(prec->mod1);
  pdbRecordType->papFldDes[44]->offset=(short)((char *)&prec->mod1 - (char *)prec);
  pdbRecordType->papFldDes[45]->size=sizeof(prec->r1sp);
  pdbRecordType->papFldDes[45]->offset=(short)((char *)&prec->r1sp - (char *)prec);
  pdbRecordType->papFldDes[46]->size=sizeof(prec->mod2);
  pdbRecordType->papFldDes[46]->offset=(short)((char *)&prec->mod2 - (char *)prec);
  pdbRecordType->papFldDes[47]->size=sizeof(prec->r2sp);
  pdbRecordType->papFldDes[47]->offset=(short)((char *)&prec->r2sp - (char *)prec);
  pdbRecordType->papFldDes[48]->size=sizeof(prec->lmd1);
  pdbRecordType->papFldDes[48]->offset=(short)((char *)&prec->lmd1 - (char *)prec);
  pdbRecordType->papFldDes[49]->size=sizeof(prec->lmd2);
  pdbRecordType->papFldDes[49]->offset=(short)((char *)&prec->lmd2 - (char *)prec);
  pdbRecordType->papFldDes[50]->size=sizeof(prec->fifo);
  pdbRecordType->papFldDes[50]->offset=(short)((char *)&prec->fifo - (char *)prec);
  pdbRecordType->papFldDes[51]->size=sizeof(prec->lffo);
  pdbRecordType->papFldDes[51]->offset=(short)((char *)&prec->lffo - (char *)prec);
  pdbRecordType->papFldDes[52]->size=sizeof(prec->clr1);
  pdbRecordType->papFldDes[52]->offset=(short)((char *)&prec->clr1 - (char *)prec);
  pdbRecordType->papFldDes[53]->size=sizeof(prec->clr2);
  pdbRecordType->papFldDes[53]->offset=(short)((char *)&prec->clr2 - (char *)prec);
  pdbRecordType->papFldDes[54]->size=sizeof(prec->trg1);
  pdbRecordType->papFldDes[54]->offset=(short)((char *)&prec->trg1 - (char *)prec);
  pdbRecordType->papFldDes[55]->size=sizeof(prec->trg2);
  pdbRecordType->papFldDes[55]->offset=(short)((char *)&prec->trg2 - (char *)prec);
  pdbRecordType->papFldDes[56]->size=sizeof(prec->enab);
  pdbRecordType->papFldDes[56]->offset=(short)((char *)&prec->enab - (char *)prec);
  pdbRecordType->papFldDes[57]->size=sizeof(prec->lena);
  pdbRecordType->papFldDes[57]->offset=(short)((char *)&prec->lena - (char *)prec);
  pdbRecordType->papFldDes[58]->size=sizeof(prec->taxi);
  pdbRecordType->papFldDes[58]->offset=(short)((char *)&prec->taxi - (char *)prec);
  pdbRecordType->papFldDes[59]->size=sizeof(prec->ltax);
  pdbRecordType->papFldDes[59]->offset=(short)((char *)&prec->ltax - (char *)prec);
  pdbRecordType->papFldDes[60]->size=sizeof(prec->vme);
  pdbRecordType->papFldDes[60]->offset=(short)((char *)&prec->vme - (char *)prec);
  pdbRecordType->papFldDes[61]->size=sizeof(prec->ete0);
  pdbRecordType->papFldDes[61]->offset=(short)((char *)&prec->ete0 - (char *)prec);
  pdbRecordType->papFldDes[62]->size=sizeof(prec->et0);
  pdbRecordType->papFldDes[62]->offset=(short)((char *)&prec->et0 - (char *)prec);
  pdbRecordType->papFldDes[63]->size=sizeof(prec->let0);
  pdbRecordType->papFldDes[63]->offset=(short)((char *)&prec->let0 - (char *)prec);
  pdbRecordType->papFldDes[64]->size=sizeof(prec->ete1);
  pdbRecordType->papFldDes[64]->offset=(short)((char *)&prec->ete1 - (char *)prec);
  pdbRecordType->papFldDes[65]->size=sizeof(prec->et1);
  pdbRecordType->papFldDes[65]->offset=(short)((char *)&prec->et1 - (char *)prec);
  pdbRecordType->papFldDes[66]->size=sizeof(prec->let1);
  pdbRecordType->papFldDes[66]->offset=(short)((char *)&prec->let1 - (char *)prec);
  pdbRecordType->papFldDes[67]->size=sizeof(prec->ete2);
  pdbRecordType->papFldDes[67]->offset=(short)((char *)&prec->ete2 - (char *)prec);
  pdbRecordType->papFldDes[68]->size=sizeof(prec->et2);
  pdbRecordType->papFldDes[68]->offset=(short)((char *)&prec->et2 - (char *)prec);
  pdbRecordType->papFldDes[69]->size=sizeof(prec->let2);
  pdbRecordType->papFldDes[69]->offset=(short)((char *)&prec->let2 - (char *)prec);
  pdbRecordType->papFldDes[70]->size=sizeof(prec->ete3);
  pdbRecordType->papFldDes[70]->offset=(short)((char *)&prec->ete3 - (char *)prec);
  pdbRecordType->papFldDes[71]->size=sizeof(prec->et3);
  pdbRecordType->papFldDes[71]->offset=(short)((char *)&prec->et3 - (char *)prec);
  pdbRecordType->papFldDes[72]->size=sizeof(prec->let3);
  pdbRecordType->papFldDes[72]->offset=(short)((char *)&prec->let3 - (char *)prec);
  pdbRecordType->papFldDes[73]->size=sizeof(prec->ete4);
  pdbRecordType->papFldDes[73]->offset=(short)((char *)&prec->ete4 - (char *)prec);
  pdbRecordType->papFldDes[74]->size=sizeof(prec->et4);
  pdbRecordType->papFldDes[74]->offset=(short)((char *)&prec->et4 - (char *)prec);
  pdbRecordType->papFldDes[75]->size=sizeof(prec->let4);
  pdbRecordType->papFldDes[75]->offset=(short)((char *)&prec->let4 - (char *)prec);
  pdbRecordType->papFldDes[76]->size=sizeof(prec->ete5);
  pdbRecordType->papFldDes[76]->offset=(short)((char *)&prec->ete5 - (char *)prec);
  pdbRecordType->papFldDes[77]->size=sizeof(prec->et5);
  pdbRecordType->papFldDes[77]->offset=(short)((char *)&prec->et5 - (char *)prec);
  pdbRecordType->papFldDes[78]->size=sizeof(prec->let5);
  pdbRecordType->papFldDes[78]->offset=(short)((char *)&prec->let5 - (char *)prec);
  pdbRecordType->papFldDes[79]->size=sizeof(prec->ete6);
  pdbRecordType->papFldDes[79]->offset=(short)((char *)&prec->ete6 - (char *)prec);
  pdbRecordType->papFldDes[80]->size=sizeof(prec->et6);
  pdbRecordType->papFldDes[80]->offset=(short)((char *)&prec->et6 - (char *)prec);
  pdbRecordType->papFldDes[81]->size=sizeof(prec->let6);
  pdbRecordType->papFldDes[81]->offset=(short)((char *)&prec->let6 - (char *)prec);
  pdbRecordType->papFldDes[82]->size=sizeof(prec->ete7);
  pdbRecordType->papFldDes[82]->offset=(short)((char *)&prec->ete7 - (char *)prec);
  pdbRecordType->papFldDes[83]->size=sizeof(prec->et7);
  pdbRecordType->papFldDes[83]->offset=(short)((char *)&prec->et7 - (char *)prec);
  pdbRecordType->papFldDes[84]->size=sizeof(prec->let7);
  pdbRecordType->papFldDes[84]->offset=(short)((char *)&prec->let7 - (char *)prec);
  pdbRecordType->papFldDes[85]->size=sizeof(prec->md0e);
  pdbRecordType->papFldDes[85]->offset=(short)((char *)&prec->md0e - (char *)prec);
  pdbRecordType->papFldDes[86]->size=sizeof(prec->md1e);
  pdbRecordType->papFldDes[86]->offset=(short)((char *)&prec->md1e - (char *)prec);
  pdbRecordType->papFldDes[87]->size=sizeof(prec->md2e);
  pdbRecordType->papFldDes[87]->offset=(short)((char *)&prec->md2e - (char *)prec);
  pdbRecordType->papFldDes[88]->size=sizeof(prec->md3e);
  pdbRecordType->papFldDes[88]->offset=(short)((char *)&prec->md3e - (char *)prec);
  pdbRecordType->papFldDes[89]->size=sizeof(prec->md4e);
  pdbRecordType->papFldDes[89]->offset=(short)((char *)&prec->md4e - (char *)prec);
  pdbRecordType->papFldDes[90]->size=sizeof(prec->md5e);
  pdbRecordType->papFldDes[90]->offset=(short)((char *)&prec->md5e - (char *)prec);
  pdbRecordType->papFldDes[91]->size=sizeof(prec->md6e);
  pdbRecordType->papFldDes[91]->offset=(short)((char *)&prec->md6e - (char *)prec);
  pdbRecordType->papFldDes[92]->size=sizeof(prec->md7e);
  pdbRecordType->papFldDes[92]->offset=(short)((char *)&prec->md7e - (char *)prec);
  pdbRecordType->papFldDes[93]->size=sizeof(prec->me0e);
  pdbRecordType->papFldDes[93]->offset=(short)((char *)&prec->me0e - (char *)prec);
  pdbRecordType->papFldDes[94]->size=sizeof(prec->me1e);
  pdbRecordType->papFldDes[94]->offset=(short)((char *)&prec->me1e - (char *)prec);
  pdbRecordType->papFldDes[95]->size=sizeof(prec->me2e);
  pdbRecordType->papFldDes[95]->offset=(short)((char *)&prec->me2e - (char *)prec);
  pdbRecordType->papFldDes[96]->size=sizeof(prec->me3e);
  pdbRecordType->papFldDes[96]->offset=(short)((char *)&prec->me3e - (char *)prec);
  pdbRecordType->papFldDes[97]->size=sizeof(prec->me4e);
  pdbRecordType->papFldDes[97]->offset=(short)((char *)&prec->me4e - (char *)prec);
  pdbRecordType->papFldDes[98]->size=sizeof(prec->me5e);
  pdbRecordType->papFldDes[98]->offset=(short)((char *)&prec->me5e - (char *)prec);
  pdbRecordType->papFldDes[99]->size=sizeof(prec->me6e);
  pdbRecordType->papFldDes[99]->offset=(short)((char *)&prec->me6e - (char *)prec);
  pdbRecordType->papFldDes[100]->size=sizeof(prec->me7e);
  pdbRecordType->papFldDes[100]->offset=(short)((char *)&prec->me7e - (char *)prec);
  pdbRecordType->papFldDes[101]->size=sizeof(prec->asyn);
  pdbRecordType->papFldDes[101]->offset=(short)((char *)&prec->asyn - (char *)prec);
  pdbRecordType->papFldDes[102]->size=sizeof(prec->aev0);
  pdbRecordType->papFldDes[102]->offset=(short)((char *)&prec->aev0 - (char *)prec);
  pdbRecordType->papFldDes[103]->size=sizeof(prec->asq1);
  pdbRecordType->papFldDes[103]->offset=(short)((char *)&prec->asq1 - (char *)prec);
  pdbRecordType->papFldDes[104]->size=sizeof(prec->asq2);
  pdbRecordType->papFldDes[104]->offset=(short)((char *)&prec->asq2 - (char *)prec);
  pdbRecordType->papFldDes[105]->size=sizeof(prec->adiv);
  pdbRecordType->papFldDes[105]->offset=(short)((char *)&prec->adiv - (char *)prec);
  pdbRecordType->papFldDes[106]->size=sizeof(prec->aphs);
  pdbRecordType->papFldDes[106]->offset=(short)((char *)&prec->aphs - (char *)prec);
  pdbRecordType->papFldDes[107]->size=sizeof(prec->msq1);
  pdbRecordType->papFldDes[107]->offset=(short)((char *)&prec->msq1 - (char *)prec);
  pdbRecordType->papFldDes[108]->size=sizeof(prec->msq2);
  pdbRecordType->papFldDes[108]->offset=(short)((char *)&prec->msq2 - (char *)prec);
  pdbRecordType->papFldDes[109]->size=sizeof(prec->mc0p);
  pdbRecordType->papFldDes[109]->offset=(short)((char *)&prec->mc0p - (char *)prec);
  pdbRecordType->papFldDes[110]->size=sizeof(prec->mc1p);
  pdbRecordType->papFldDes[110]->offset=(short)((char *)&prec->mc1p - (char *)prec);
  pdbRecordType->papFldDes[111]->size=sizeof(prec->mc2p);
  pdbRecordType->papFldDes[111]->offset=(short)((char *)&prec->mc2p - (char *)prec);
  pdbRecordType->papFldDes[112]->size=sizeof(prec->mc3p);
  pdbRecordType->papFldDes[112]->offset=(short)((char *)&prec->mc3p - (char *)prec);
  pdbRecordType->papFldDes[113]->size=sizeof(prec->mc4p);
  pdbRecordType->papFldDes[113]->offset=(short)((char *)&prec->mc4p - (char *)prec);
  pdbRecordType->papFldDes[114]->size=sizeof(prec->mc5p);
  pdbRecordType->papFldDes[114]->offset=(short)((char *)&prec->mc5p - (char *)prec);
  pdbRecordType->papFldDes[115]->size=sizeof(prec->mc6p);
  pdbRecordType->papFldDes[115]->offset=(short)((char *)&prec->mc6p - (char *)prec);
  pdbRecordType->papFldDes[116]->size=sizeof(prec->mc7p);
  pdbRecordType->papFldDes[116]->offset=(short)((char *)&prec->mc7p - (char *)prec);
  pdbRecordType->papFldDes[117]->size=sizeof(prec->fpgv);
  pdbRecordType->papFldDes[117]->offset=(short)((char *)&prec->fpgv - (char *)prec);
  pdbRecordType->papFldDes[118]->size=sizeof(prec->val);
  pdbRecordType->papFldDes[118]->offset=(short)((char *)&prec->val - (char *)prec);
  pdbRecordType->papFldDes[119]->size=sizeof(prec->r1ck);
  pdbRecordType->papFldDes[119]->offset=(short)((char *)&prec->r1ck - (char *)prec);
  pdbRecordType->papFldDes[120]->size=sizeof(prec->r2ck);
  pdbRecordType->papFldDes[120]->offset=(short)((char *)&prec->r2ck - (char *)prec);
  pdbRecordType->papFldDes[121]->size=sizeof(prec->rmax);
  pdbRecordType->papFldDes[121]->offset=(short)((char *)&prec->rmax - (char *)prec);
  pdbRecordType->papFldDes[122]->size=sizeof(prec->lr1c);
  pdbRecordType->papFldDes[122]->offset=(short)((char *)&prec->lr1c - (char *)prec);
  pdbRecordType->papFldDes[123]->size=sizeof(prec->lr2c);
  pdbRecordType->papFldDes[123]->offset=(short)((char *)&prec->lr2c - (char *)prec);
  pdbRecordType->papFldDes[124]->size=sizeof(prec->mcrs);
  pdbRecordType->papFldDes[124]->offset=(short)((char *)&prec->mcrs - (char *)prec);
    pdbRecordType->rec_size = sizeof(*prec);
    return(0);
}
#endif /*GEN_SIZE_OFFSET*/
