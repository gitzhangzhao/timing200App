#include <vxWorks.h>
#include <semLib.h>
#include "ellLib.h"
#include "fast_lock.h"
#include "link.h"
#include "tsDefs.h"

#ifndef INCegeventUNITH
#define INCegeventUNITH
typedef enum {
	egeventUNIT_Clock_Ticks,
	egeventUNIT_Fortnights,
	egeventUNIT_Weeks,
	egeventUNIT_Days,
	egeventUNIT_Hours,
	egeventUNIT_Minuites,
	egeventUNIT_Seconds,
	egeventUNIT_Milliseconds,
	egeventUNIT_Microseconds,
	egeventUNIT_Nanoseconds
}egeventUNIT;
#endif /*INCegeventUNITH*/

#ifndef INCegeventRAMH
#define INCegeventRAMH
typedef enum {
	egeventRAM_RAM_1,
	egeventRAM_RAM_2
}egeventRAM;
#endif /*INCegeventRAMH*/
#ifndef INCegeventH
#define INCegeventH
typedef struct egeventRecord {
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
	long		enm;	/*Event Number*/
	long		levt;	/*Last Event Number*/
	unsigned short	ram;	/*Event RAM*/
	unsigned short	lram;	/*Last RAM*/
	float		dely;	/*Desired Delay Time*/
	float		adly;	/*Actual Delay*/
	long		dpos;	/*Desired Position*/
	long		apos;	/*Actual Position*/
	float		ldly;	/*Last Desired Delay*/
	unsigned short	unit;	/*Delay Units*/
	char		val;	/*Worthless Value*/
	ELLNODE eln;	/*List Node*/
	struct egeventRecord *self;	/*Self Pointer*/
} egeventRecord;
#define egeventRecordNAME	0
#define egeventRecordDESC	1
#define egeventRecordASG	2
#define egeventRecordSCAN	3
#define egeventRecordPINI	4
#define egeventRecordPHAS	5
#define egeventRecordEVNT	6
#define egeventRecordTSE	7
#define egeventRecordTSEL	8
#define egeventRecordDTYP	9
#define egeventRecordDISV	10
#define egeventRecordDISA	11
#define egeventRecordSDIS	12
#define egeventRecordMLOK	13
#define egeventRecordMLIS	14
#define egeventRecordDISP	15
#define egeventRecordPROC	16
#define egeventRecordSTAT	17
#define egeventRecordSEVR	18
#define egeventRecordNSTA	19
#define egeventRecordNSEV	20
#define egeventRecordACKS	21
#define egeventRecordACKT	22
#define egeventRecordDISS	23
#define egeventRecordLCNT	24
#define egeventRecordPACT	25
#define egeventRecordPUTF	26
#define egeventRecordRPRO	27
#define egeventRecordASP	28
#define egeventRecordPPN	29
#define egeventRecordPPNN	30
#define egeventRecordSPVT	31
#define egeventRecordRSET	32
#define egeventRecordDSET	33
#define egeventRecordDPVT	34
#define egeventRecordRDES	35
#define egeventRecordLSET	36
#define egeventRecordPRIO	37
#define egeventRecordTPRO	38
#define egeventRecordBKPT	39
#define egeventRecordUDF	40
#define egeventRecordTIME	41
#define egeventRecordFLNK	42
#define egeventRecordOUT	43
#define egeventRecordENM	44
#define egeventRecordLEVT	45
#define egeventRecordRAM	46
#define egeventRecordLRAM	47
#define egeventRecordDELY	48
#define egeventRecordADLY	49
#define egeventRecordDPOS	50
#define egeventRecordAPOS	51
#define egeventRecordLDLY	52
#define egeventRecordUNIT	53
#define egeventRecordVAL	54
#define egeventRecordELN	55
#define egeventRecordSELF	56
#endif /*INCegeventH*/
#ifdef GEN_SIZE_OFFSET
int egeventRecordSizeOffset(dbRecordType *pdbRecordType)
{
    egeventRecord *prec = 0;
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
  pdbRecordType->papFldDes[44]->size=sizeof(prec->enm);
  pdbRecordType->papFldDes[44]->offset=(short)((char *)&prec->enm - (char *)prec);
  pdbRecordType->papFldDes[45]->size=sizeof(prec->levt);
  pdbRecordType->papFldDes[45]->offset=(short)((char *)&prec->levt - (char *)prec);
  pdbRecordType->papFldDes[46]->size=sizeof(prec->ram);
  pdbRecordType->papFldDes[46]->offset=(short)((char *)&prec->ram - (char *)prec);
  pdbRecordType->papFldDes[47]->size=sizeof(prec->lram);
  pdbRecordType->papFldDes[47]->offset=(short)((char *)&prec->lram - (char *)prec);
  pdbRecordType->papFldDes[48]->size=sizeof(prec->dely);
  pdbRecordType->papFldDes[48]->offset=(short)((char *)&prec->dely - (char *)prec);
  pdbRecordType->papFldDes[49]->size=sizeof(prec->adly);
  pdbRecordType->papFldDes[49]->offset=(short)((char *)&prec->adly - (char *)prec);
  pdbRecordType->papFldDes[50]->size=sizeof(prec->dpos);
  pdbRecordType->papFldDes[50]->offset=(short)((char *)&prec->dpos - (char *)prec);
  pdbRecordType->papFldDes[51]->size=sizeof(prec->apos);
  pdbRecordType->papFldDes[51]->offset=(short)((char *)&prec->apos - (char *)prec);
  pdbRecordType->papFldDes[52]->size=sizeof(prec->ldly);
  pdbRecordType->papFldDes[52]->offset=(short)((char *)&prec->ldly - (char *)prec);
  pdbRecordType->papFldDes[53]->size=sizeof(prec->unit);
  pdbRecordType->papFldDes[53]->offset=(short)((char *)&prec->unit - (char *)prec);
  pdbRecordType->papFldDes[54]->size=sizeof(prec->val);
  pdbRecordType->papFldDes[54]->offset=(short)((char *)&prec->val - (char *)prec);
  pdbRecordType->papFldDes[55]->size=sizeof(prec->eln);
  pdbRecordType->papFldDes[55]->offset=(short)((char *)&prec->eln - (char *)prec);
  pdbRecordType->papFldDes[56]->size=sizeof(prec->self);
  pdbRecordType->papFldDes[56]->offset=(short)((char *)&prec->self - (char *)prec);
    pdbRecordType->rec_size = sizeof(*prec);
    return(0);
}
#endif /*GEN_SIZE_OFFSET*/
