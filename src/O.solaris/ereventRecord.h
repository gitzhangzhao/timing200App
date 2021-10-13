#include <vxWorks.h>
#include <semLib.h>
#include "ellLib.h"
#include "fast_lock.h"
#include "link.h"
#include "tsDefs.h"

#ifndef INCereventENABH
#define INCereventENABH
typedef enum {
	ereventENAB_Disabled,
	ereventENAB_Enabled
}ereventENAB;
#endif /*INCereventENABH*/
#ifndef INCereventH
#define INCereventH
typedef struct ereventRecord {
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
	unsigned short	enab;	/*Enable*/
	long		enm;	/*Event Number*/
	long		lenm;	/*Last Event Number*/
	long		lout;	/*Last Out Enable Mask*/
	unsigned short	out0;	/*Out 0 Enable*/
	unsigned short	out1;	/*Out 1 Enable*/
	unsigned short	out2;	/*Out 2 Enable*/
	unsigned short	out3;	/*Out 3 Enable*/
	unsigned short	out4;	/*Out 4 Enable*/
	unsigned short	out5;	/*Out 5 Enable*/
	unsigned short	out6;	/*Out 6 Enable*/
	unsigned short	out7;	/*Out 7 Enable*/
	unsigned short	out8;	/*Out 8 Enable*/
	unsigned short	out9;	/*Out 9 Enable*/
	unsigned short	outa;	/*Out 10 Enable*/
	unsigned short	outb;	/*Out 11 Enable*/
	unsigned short	outc;	/*Out 12 Enable*/
	unsigned short	outd;	/*Out 13 Enable*/
	unsigned short	vme;	/*VME IRQ Enable*/
} ereventRecord;
#define ereventRecordNAME	0
#define ereventRecordDESC	1
#define ereventRecordASG	2
#define ereventRecordSCAN	3
#define ereventRecordPINI	4
#define ereventRecordPHAS	5
#define ereventRecordEVNT	6
#define ereventRecordTSE	7
#define ereventRecordTSEL	8
#define ereventRecordDTYP	9
#define ereventRecordDISV	10
#define ereventRecordDISA	11
#define ereventRecordSDIS	12
#define ereventRecordMLOK	13
#define ereventRecordMLIS	14
#define ereventRecordDISP	15
#define ereventRecordPROC	16
#define ereventRecordSTAT	17
#define ereventRecordSEVR	18
#define ereventRecordNSTA	19
#define ereventRecordNSEV	20
#define ereventRecordACKS	21
#define ereventRecordACKT	22
#define ereventRecordDISS	23
#define ereventRecordLCNT	24
#define ereventRecordPACT	25
#define ereventRecordPUTF	26
#define ereventRecordRPRO	27
#define ereventRecordASP	28
#define ereventRecordPPN	29
#define ereventRecordPPNN	30
#define ereventRecordSPVT	31
#define ereventRecordRSET	32
#define ereventRecordDSET	33
#define ereventRecordDPVT	34
#define ereventRecordRDES	35
#define ereventRecordLSET	36
#define ereventRecordPRIO	37
#define ereventRecordTPRO	38
#define ereventRecordBKPT	39
#define ereventRecordUDF	40
#define ereventRecordTIME	41
#define ereventRecordFLNK	42
#define ereventRecordVAL	43
#define ereventRecordOUT	44
#define ereventRecordENAB	45
#define ereventRecordENM	46
#define ereventRecordLENM	47
#define ereventRecordLOUT	48
#define ereventRecordOUT0	49
#define ereventRecordOUT1	50
#define ereventRecordOUT2	51
#define ereventRecordOUT3	52
#define ereventRecordOUT4	53
#define ereventRecordOUT5	54
#define ereventRecordOUT6	55
#define ereventRecordOUT7	56
#define ereventRecordOUT8	57
#define ereventRecordOUT9	58
#define ereventRecordOUTA	59
#define ereventRecordOUTB	60
#define ereventRecordOUTC	61
#define ereventRecordOUTD	62
#define ereventRecordVME	63
#endif /*INCereventH*/
#ifdef GEN_SIZE_OFFSET
int ereventRecordSizeOffset(dbRecordType *pdbRecordType)
{
    ereventRecord *prec = 0;
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
  pdbRecordType->papFldDes[46]->size=sizeof(prec->enm);
  pdbRecordType->papFldDes[46]->offset=(short)((char *)&prec->enm - (char *)prec);
  pdbRecordType->papFldDes[47]->size=sizeof(prec->lenm);
  pdbRecordType->papFldDes[47]->offset=(short)((char *)&prec->lenm - (char *)prec);
  pdbRecordType->papFldDes[48]->size=sizeof(prec->lout);
  pdbRecordType->papFldDes[48]->offset=(short)((char *)&prec->lout - (char *)prec);
  pdbRecordType->papFldDes[49]->size=sizeof(prec->out0);
  pdbRecordType->papFldDes[49]->offset=(short)((char *)&prec->out0 - (char *)prec);
  pdbRecordType->papFldDes[50]->size=sizeof(prec->out1);
  pdbRecordType->papFldDes[50]->offset=(short)((char *)&prec->out1 - (char *)prec);
  pdbRecordType->papFldDes[51]->size=sizeof(prec->out2);
  pdbRecordType->papFldDes[51]->offset=(short)((char *)&prec->out2 - (char *)prec);
  pdbRecordType->papFldDes[52]->size=sizeof(prec->out3);
  pdbRecordType->papFldDes[52]->offset=(short)((char *)&prec->out3 - (char *)prec);
  pdbRecordType->papFldDes[53]->size=sizeof(prec->out4);
  pdbRecordType->papFldDes[53]->offset=(short)((char *)&prec->out4 - (char *)prec);
  pdbRecordType->papFldDes[54]->size=sizeof(prec->out5);
  pdbRecordType->papFldDes[54]->offset=(short)((char *)&prec->out5 - (char *)prec);
  pdbRecordType->papFldDes[55]->size=sizeof(prec->out6);
  pdbRecordType->papFldDes[55]->offset=(short)((char *)&prec->out6 - (char *)prec);
  pdbRecordType->papFldDes[56]->size=sizeof(prec->out7);
  pdbRecordType->papFldDes[56]->offset=(short)((char *)&prec->out7 - (char *)prec);
  pdbRecordType->papFldDes[57]->size=sizeof(prec->out8);
  pdbRecordType->papFldDes[57]->offset=(short)((char *)&prec->out8 - (char *)prec);
  pdbRecordType->papFldDes[58]->size=sizeof(prec->out9);
  pdbRecordType->papFldDes[58]->offset=(short)((char *)&prec->out9 - (char *)prec);
  pdbRecordType->papFldDes[59]->size=sizeof(prec->outa);
  pdbRecordType->papFldDes[59]->offset=(short)((char *)&prec->outa - (char *)prec);
  pdbRecordType->papFldDes[60]->size=sizeof(prec->outb);
  pdbRecordType->papFldDes[60]->offset=(short)((char *)&prec->outb - (char *)prec);
  pdbRecordType->papFldDes[61]->size=sizeof(prec->outc);
  pdbRecordType->papFldDes[61]->offset=(short)((char *)&prec->outc - (char *)prec);
  pdbRecordType->papFldDes[62]->size=sizeof(prec->outd);
  pdbRecordType->papFldDes[62]->offset=(short)((char *)&prec->outd - (char *)prec);
  pdbRecordType->papFldDes[63]->size=sizeof(prec->vme);
  pdbRecordType->papFldDes[63]->offset=(short)((char *)&prec->vme - (char *)prec);
    pdbRecordType->rec_size = sizeof(*prec);
    return(0);
}
#endif /*GEN_SIZE_OFFSET*/
