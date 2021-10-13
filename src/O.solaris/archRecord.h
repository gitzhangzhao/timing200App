#include <vxWorks.h>
#include <semLib.h>
#include "ellLib.h"
#include "fast_lock.h"
#include "link.h"
#include "tsDefs.h"

#ifndef INCarchPCABH
#define INCarchPCABH
typedef enum {
	archPCAB_absolute,
	archPCAB_relative,
	archPCAB_abs_and_rel,
	archPCAB_abs_or_rel,
	archPCAB_on_change,
	archPCAB_allways,
	archPCAB_never
}archPCAB;
#endif /*INCarchPCABH*/
#ifndef INCarchH
#define INCarchH
typedef struct archRecord {
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
	void *val;	/*Array Value*/
	double		cval;	/*Current Value*/
	double		rval;	/*Raw value*/
	void *tim;	/*Array Times sec*/
	void *nsc;	/*Array Times nsec*/
	void *svy;	/*Array Severities*/
	float *vals;	/*Values Array*/
	long *tims;	/*Times Array sec*/
	long *nscs;	/*Times Array nsec*/
	short *svys;	/*Severities Array*/
	void *arch;	/*Archive Array*/
	double		lval;	/*Last Value stored*/
	unsigned long	ccnt;	/*Curr. Buffer Counter*/
	unsigned long	flsh;	/*Flush flag*/
	DBLINK		inp;	/*Input Specification*/
	unsigned long	nval;	/*Number of Values*/
	unsigned long	stim;	/*Save time in sec*/
	unsigned long	ftim;	/*Flush time in sec*/
	unsigned long	ftof;	/*Flush time offset*/
	float		avar;	/*Diff. last-curr. abs */
	float		rvar;	/*Diff. last-curr. in %*/
	unsigned short	pcab;	/*Absolute or Percent*/
	float		hopr;	/*High Oper.Range Value*/
	float		lopr;	/*Low  Oper.Range Value*/
	short		prec;	/*Display Precis. Value*/
	char		egu[16]; /*Eng. Units for VALUE*/
	unsigned short	mask;	/*Bit Mask*/
	unsigned long	nuse;	/*Number Used*/
	unsigned long	nusb;	/*Number Used in Buf.*/
	unsigned long	ltim;	/*Last Time Archived*/
	unsigned long	ltmm;	/*Last Time Monitored*/
	short		res;	/*Reset*/
} archRecord;
#define archRecordNAME	0
#define archRecordDESC	1
#define archRecordASG	2
#define archRecordSCAN	3
#define archRecordPINI	4
#define archRecordPHAS	5
#define archRecordEVNT	6
#define archRecordTSE	7
#define archRecordTSEL	8
#define archRecordDTYP	9
#define archRecordDISV	10
#define archRecordDISA	11
#define archRecordSDIS	12
#define archRecordMLOK	13
#define archRecordMLIS	14
#define archRecordDISP	15
#define archRecordPROC	16
#define archRecordSTAT	17
#define archRecordSEVR	18
#define archRecordNSTA	19
#define archRecordNSEV	20
#define archRecordACKS	21
#define archRecordACKT	22
#define archRecordDISS	23
#define archRecordLCNT	24
#define archRecordPACT	25
#define archRecordPUTF	26
#define archRecordRPRO	27
#define archRecordASP	28
#define archRecordPPN	29
#define archRecordPPNN	30
#define archRecordSPVT	31
#define archRecordRSET	32
#define archRecordDSET	33
#define archRecordDPVT	34
#define archRecordRDES	35
#define archRecordLSET	36
#define archRecordPRIO	37
#define archRecordTPRO	38
#define archRecordBKPT	39
#define archRecordUDF	40
#define archRecordTIME	41
#define archRecordFLNK	42
#define archRecordVAL	43
#define archRecordCVAL	44
#define archRecordRVAL	45
#define archRecordTIM	46
#define archRecordNSC	47
#define archRecordSVY	48
#define archRecordVALS	49
#define archRecordTIMS	50
#define archRecordNSCS	51
#define archRecordSVYS	52
#define archRecordARCH	53
#define archRecordLVAL	54
#define archRecordCCNT	55
#define archRecordFLSH	56
#define archRecordINP	57
#define archRecordNVAL	58
#define archRecordSTIM	59
#define archRecordFTIM	60
#define archRecordFTOF	61
#define archRecordAVAR	62
#define archRecordRVAR	63
#define archRecordPCAB	64
#define archRecordHOPR	65
#define archRecordLOPR	66
#define archRecordPREC	67
#define archRecordEGU	68
#define archRecordMASK	69
#define archRecordNUSE	70
#define archRecordNUSB	71
#define archRecordLTIM	72
#define archRecordLTMM	73
#define archRecordRES	74
#endif /*INCarchH*/
#ifdef GEN_SIZE_OFFSET
int archRecordSizeOffset(dbRecordType *pdbRecordType)
{
    archRecord *prec = 0;
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
  pdbRecordType->papFldDes[44]->size=sizeof(prec->cval);
  pdbRecordType->papFldDes[44]->offset=(short)((char *)&prec->cval - (char *)prec);
  pdbRecordType->papFldDes[45]->size=sizeof(prec->rval);
  pdbRecordType->papFldDes[45]->offset=(short)((char *)&prec->rval - (char *)prec);
  pdbRecordType->papFldDes[46]->size=sizeof(prec->tim);
  pdbRecordType->papFldDes[46]->offset=(short)((char *)&prec->tim - (char *)prec);
  pdbRecordType->papFldDes[47]->size=sizeof(prec->nsc);
  pdbRecordType->papFldDes[47]->offset=(short)((char *)&prec->nsc - (char *)prec);
  pdbRecordType->papFldDes[48]->size=sizeof(prec->svy);
  pdbRecordType->papFldDes[48]->offset=(short)((char *)&prec->svy - (char *)prec);
  pdbRecordType->papFldDes[49]->size=sizeof(prec->vals);
  pdbRecordType->papFldDes[49]->offset=(short)((char *)&prec->vals - (char *)prec);
  pdbRecordType->papFldDes[50]->size=sizeof(prec->tims);
  pdbRecordType->papFldDes[50]->offset=(short)((char *)&prec->tims - (char *)prec);
  pdbRecordType->papFldDes[51]->size=sizeof(prec->nscs);
  pdbRecordType->papFldDes[51]->offset=(short)((char *)&prec->nscs - (char *)prec);
  pdbRecordType->papFldDes[52]->size=sizeof(prec->svys);
  pdbRecordType->papFldDes[52]->offset=(short)((char *)&prec->svys - (char *)prec);
  pdbRecordType->papFldDes[53]->size=sizeof(prec->arch);
  pdbRecordType->papFldDes[53]->offset=(short)((char *)&prec->arch - (char *)prec);
  pdbRecordType->papFldDes[54]->size=sizeof(prec->lval);
  pdbRecordType->papFldDes[54]->offset=(short)((char *)&prec->lval - (char *)prec);
  pdbRecordType->papFldDes[55]->size=sizeof(prec->ccnt);
  pdbRecordType->papFldDes[55]->offset=(short)((char *)&prec->ccnt - (char *)prec);
  pdbRecordType->papFldDes[56]->size=sizeof(prec->flsh);
  pdbRecordType->papFldDes[56]->offset=(short)((char *)&prec->flsh - (char *)prec);
  pdbRecordType->papFldDes[57]->size=sizeof(prec->inp);
  pdbRecordType->papFldDes[57]->offset=(short)((char *)&prec->inp - (char *)prec);
  pdbRecordType->papFldDes[58]->size=sizeof(prec->nval);
  pdbRecordType->papFldDes[58]->offset=(short)((char *)&prec->nval - (char *)prec);
  pdbRecordType->papFldDes[59]->size=sizeof(prec->stim);
  pdbRecordType->papFldDes[59]->offset=(short)((char *)&prec->stim - (char *)prec);
  pdbRecordType->papFldDes[60]->size=sizeof(prec->ftim);
  pdbRecordType->papFldDes[60]->offset=(short)((char *)&prec->ftim - (char *)prec);
  pdbRecordType->papFldDes[61]->size=sizeof(prec->ftof);
  pdbRecordType->papFldDes[61]->offset=(short)((char *)&prec->ftof - (char *)prec);
  pdbRecordType->papFldDes[62]->size=sizeof(prec->avar);
  pdbRecordType->papFldDes[62]->offset=(short)((char *)&prec->avar - (char *)prec);
  pdbRecordType->papFldDes[63]->size=sizeof(prec->rvar);
  pdbRecordType->papFldDes[63]->offset=(short)((char *)&prec->rvar - (char *)prec);
  pdbRecordType->papFldDes[64]->size=sizeof(prec->pcab);
  pdbRecordType->papFldDes[64]->offset=(short)((char *)&prec->pcab - (char *)prec);
  pdbRecordType->papFldDes[65]->size=sizeof(prec->hopr);
  pdbRecordType->papFldDes[65]->offset=(short)((char *)&prec->hopr - (char *)prec);
  pdbRecordType->papFldDes[66]->size=sizeof(prec->lopr);
  pdbRecordType->papFldDes[66]->offset=(short)((char *)&prec->lopr - (char *)prec);
  pdbRecordType->papFldDes[67]->size=sizeof(prec->prec);
  pdbRecordType->papFldDes[67]->offset=(short)((char *)&prec->prec - (char *)prec);
  pdbRecordType->papFldDes[68]->size=sizeof(prec->egu);
  pdbRecordType->papFldDes[68]->offset=(short)((char *)&prec->egu - (char *)prec);
  pdbRecordType->papFldDes[69]->size=sizeof(prec->mask);
  pdbRecordType->papFldDes[69]->offset=(short)((char *)&prec->mask - (char *)prec);
  pdbRecordType->papFldDes[70]->size=sizeof(prec->nuse);
  pdbRecordType->papFldDes[70]->offset=(short)((char *)&prec->nuse - (char *)prec);
  pdbRecordType->papFldDes[71]->size=sizeof(prec->nusb);
  pdbRecordType->papFldDes[71]->offset=(short)((char *)&prec->nusb - (char *)prec);
  pdbRecordType->papFldDes[72]->size=sizeof(prec->ltim);
  pdbRecordType->papFldDes[72]->offset=(short)((char *)&prec->ltim - (char *)prec);
  pdbRecordType->papFldDes[73]->size=sizeof(prec->ltmm);
  pdbRecordType->papFldDes[73]->offset=(short)((char *)&prec->ltmm - (char *)prec);
  pdbRecordType->papFldDes[74]->size=sizeof(prec->res);
  pdbRecordType->papFldDes[74]->offset=(short)((char *)&prec->res - (char *)prec);
    pdbRecordType->rec_size = sizeof(*prec);
    return(0);
}
#endif /*GEN_SIZE_OFFSET*/
