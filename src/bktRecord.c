/*************************************************************************\
* Copyright (c) 2002 The University of Chicago, as Operator of Argonne
*     National Laboratory.
* Copyright (c) 2002 The Regents of the University of California, as
*     Operator of Los Alamos National Laboratory.
* EPICS BASE Versions 3.13.7
* and higher are distributed subject to a Software License Agreement found
* in file LICENSE that is included with this distribution. 
\*************************************************************************/
/* bkt.c - Record Support Routines for calc with output records */
/*
 *   Author : Ned Arnold
 *   Based on recCalc.c, by ...
 *      Original Author: Julie Sander and Bob Dalesio
 *      Current  Author: Marty Kraimer
 *      Date:            7-27-87
 */



#include	<vxWorks.h>
#include        <stdlib.h>
#include        <stdarg.h>
#include        <stdio.h>
#include        <string.h>
#include        <math.h>

#include        <tickLib.h>
#include        <wdLib.h>
#include        <sysLib.h>

#include	"alarm.h"
#include	"dbDefs.h"
#include	"dbAccess.h"
#include	"dbEvent.h"
#include	"dbScan.h"
#include	"errMdef.h"
#include	"recSup.h"
#include	"special.h"
#include        "callback.h"
#include        "taskwd.h"
#include        "postfix.h"

#define GEN_SIZE_OFFSET
#include	"bktRecord.h"
#undef  GEN_SIZE_OFFSET
#include        "menuIvoa.h"

/* Create RSET - Record Support Entry Table*/
#define report NULL
#define initialize NULL
static long init_record();
static long process();
static long special();
#define get_value NULL
#define cvt_dbaddr NULL
#define get_array_info NULL
#define put_array_info NULL
static long get_units();
static long get_precision();
#define get_enum_str NULL
#define get_enum_strs NULL
#define put_enum_str NULL
static long get_graphic_double();
static long get_control_double();
static long get_alarm_double();

struct rset bktRSET={
	RSETNUMBER,
	report,
	initialize,
	init_record,
	process,
	special,
	get_value,
	cvt_dbaddr,
	get_array_info,
	put_array_info,
	get_units,
	get_precision,
	get_enum_str,
	get_enum_strs,
	put_enum_str,
	get_graphic_double,
	get_control_double,
	get_alarm_double
};


/* To provide feedback to the user as to the connection status of the 
 * links (.INxV and .OUTV), the following algorithm has been implemented ...
 *
 * A new PV_LINK [either in init() or special()] is searched for using
 * dbNameToAddr. If local, it is so indicated. If not, a checkLinkCb
 * callback is scheduled to check the connectivity later using 
 * dbCaIsLinkConnected(). Anytime there are unconnected CA_LINKs, another
 * callback is scheduled. Once all connections are established, the CA_LINKs
 * are checked whenever the record processes. 
 *
 */

#define NO_CA_LINKS     0
#define CA_LINKS_ALL_OK 1
#define CA_LINKS_NOT_OK 2

struct rpvtStruct {
        CALLBACK  doOutCb;
        WDOG_ID   wd_id_0;
        CALLBACK  checkLinkCb;
        WDOG_ID   wd_id_1;
        short     wd_id_1_LOCK;
        short     caLinkStat; /* NO_CA_LINKS,CA_LINKS_ALL_OK,CA_LINKS_NOT_OK */
};

/* leige define for bucket selection */
int i, n, bktRecordTmp, bktRecordTmp_i, bktRecord_pCount;
FILE *fpBkt;
#define TOTAL_BUCKET_SYNC 402
#define TOTAL_BUCKET_CLI 396
#define TOTAL_BUCKET TOTAL_BUCKET_CLI

  int bktRam[TOTAL_BUCKET];
  int pattern[TOTAL_BUCKET];
  char *syncBktRamFile="/home21/pubrun/consys/timing/bktFiles/bktRamDiv5.txt";
  char *collBktRamFile="/home21/pubrun/consys/timing/bktFiles/collBktRam5.txt";
  char *bsrPattFile="/home21/operator/bucket/bktPattern";
/*  char *berPattFile="/home21/operator/bucket/bktPatternBER.ioc"; */
  char berPattFile[256];
  char bepc_tm_buf[256];
/* end of leige definition */

static void alarm();
static void monitor();
static int fetch_values();
static void execOutput();
static void doOutputCallback();
static void checkLinks();
static void checkLinksCallback();

int    bktRecDebug;

#define ARG_MAX 12


static long init_record(pcalc,pass)
    struct bktRecord	*pcalc;
    int pass;
{
    struct link *plink;
    int bktRecord_i;
    double *pvalue;
    unsigned short *plinkValid;
    short error_number;

    struct dbAddr       dbAddr;
    struct dbAddr       *pAddr = &dbAddr;
    struct rpvtStruct   *prpvt;

    if (pass==0) {
        pcalc->rpvt = (void *)calloc(1, sizeof(struct rpvtStruct));
        return(0);
    }
    
    prpvt = (struct rpvtStruct *)pcalc->rpvt;

    plink = &pcalc->inpa;
    pvalue = &pcalc->a;
    plinkValid = &pcalc->inav;
    for(bktRecord_i=0; bktRecord_i<(ARG_MAX+1); bktRecord_i++, plink++, pvalue++, plinkValid++) {
        if (plink->type == CONSTANT) {
            /* Don't InitConstantLink the .OUT link */
            if(bktRecord_i<ARG_MAX) { 
                recGblInitConstantLink(plink,DBF_DOUBLE,pvalue);
            }
            *plinkValid = bktINAV_CON;
        }
        /* see if the PV resides on this ioc */
        else if(!dbNameToAddr(plink->value.pv_link.pvname, pAddr)) {
            *plinkValid = bktINAV_LOC;
        }
        /* pv is not on this ioc. Callback later for connection stat */
        else {
            *plinkValid = bktINAV_EXT_NC;
             prpvt->caLinkStat = CA_LINKS_NOT_OK;
        }
    }

    pcalc->clcv=postfix(pcalc->calc,pcalc->rpcl,&error_number);
    if(pcalc->clcv){
		recGblRecordError(S_db_badField,(void *)pcalc,
			"bkt: init_record: Illegal CALC field");
    }

    pcalc->oclv=postfix(pcalc->ocal,pcalc->orpc,&error_number);
    if(pcalc->oclv){
		recGblRecordError(S_db_badField,(void *)pcalc,
			"bkt: init_record: Illegal OCAL field");
    }

    prpvt = (struct rpvtStruct *)pcalc->rpvt;
    callbackSetCallback(doOutputCallback, &prpvt->doOutCb);
    callbackSetPriority(pcalc->prio, &prpvt->doOutCb);
    callbackSetUser(pcalc, &prpvt->doOutCb);
    callbackSetCallback(checkLinksCallback, &prpvt->checkLinkCb);
    callbackSetPriority(0, &prpvt->checkLinkCb);
    callbackSetUser(pcalc, &prpvt->checkLinkCb);
    prpvt->wd_id_0 = wdCreate();
    prpvt->wd_id_1 = wdCreate();
    prpvt->wd_id_1_LOCK = 0;

/* Can't do this. Sometimes initialization is not done after 1 second
   and then dbScanLock will complain !!!

    if(prpvt->caLinkStat == CA_LINKS_NOT_OK) {
        wdStart(prpvt->wd_id_1, 60, (FUNCPTR)callbackRequest,
                (int)(&prpvt->checkLinkCb));
        prpvt->wd_id_1_LOCK = 1;
    }
*/

/* leige */
    printf("TMsubBktInit was called\n");
    i = 0; n = 0;

    fpBkt = fopen(collBktRamFile, "rb");
    for (bktRecordTmp_i=0; bktRecordTmp_i<TOTAL_BUCKET && (fgets(bepc_tm_buf,256,fpBkt)!=NULL); bktRecordTmp_i++) {
      if (bepc_tm_buf[0]=='#') {printf("%s\n", bepc_tm_buf); bktRecordTmp_i--;}
      else {
        sscanf(bepc_tm_buf, "%d%d", &bktRecordTmp, &(bktRam[bktRecordTmp_i]));
        if (bktRam[bktRecordTmp_i]<0) {
           printf("illegal ram position\n");
           bktRam[bktRecordTmp_i] = 1;
          }
        else if (bktRam[bktRecordTmp_i]>2000) { /* seqRam length is 2048 */
          printf("illegal ram position\n");
          bktRam[bktRecordTmp_i] = 2000;
          }
        }
      }
    fclose(fpBkt);

   for (bktRecordTmp_i=0; bktRecordTmp_i<TOTAL_BUCKET; bktRecordTmp_i++) {
     printf("bkt, ramUnit %d, %d\n", bktRecordTmp_i, bktRam[bktRecordTmp_i]); 
     }

   bktRecordTmp_i=0;
   bktRecord_pCount = 0;
   printf("ptrn %s, pdir %s \n", pcalc->ptrn, pcalc->pdir);
   strcpy(berPattFile, pcalc->pdir);
   strcat(berPattFile, pcalc->ptrn);
   fpBkt = fopen(berPattFile, "rb");
   if (fpBkt==NULL) {
     printf("pattern file open error\n");
       }
   else {
     for (bktRecordTmp_i=0; (fgets(bepc_tm_buf,256,fpBkt)!=NULL)&&(bktRecordTmp_i<TOTAL_BUCKET); bktRecordTmp_i++ ) {
     sscanf(bepc_tm_buf, "%d", &pattern[bktRecordTmp_i]);
     if ((pattern[bktRecordTmp_i]>401)||(pattern[bktRecordTmp_i]<0)) {
       printf("illegal bucket number. Use bucker 0\n");
       pattern[bktRecordTmp_i]=0;
       }
     bktRecord_pCount ++;
   }
   fclose(fpBkt);
   printf("total buckets in pattern file %d\n", bktRecord_pCount);
     for (bktRecordTmp_i=0; bktRecordTmp_i<bktRecord_pCount; bktRecordTmp_i++) {
     printf("%d , ", pattern[bktRecordTmp_i]);
     }
   } /* end of fpBkt==NULL else */

   bktRecordTmp_i=0;   
/* leige */

    return(0);
}

static long process(pcalc)
    struct bktRecord     *pcalc;
{
    struct rpvtStruct   *prpvt = (struct rpvtStruct *)pcalc->rpvt;
    int      wdDelay;
    short    doOutput = 0;
  
    if(!pcalc->pact) { 
	pcalc->pact = TRUE;

        /* if some links are CA, check connections */
        if(prpvt->caLinkStat != NO_CA_LINKS) {
            checkLinks(pcalc);
        }

	if(fetch_values(pcalc)==0) {
	    if(calcPerform(&pcalc->a,&pcalc->val,pcalc->rpcl)) {
		recGblSetSevr(pcalc,CALC_ALARM,INVALID_ALARM);
	    } else pcalc->udf = FALSE;
	}
	recGblGetTimeStamp(pcalc);
	/* check for alarms */
	alarm(pcalc);

        /* check for output link execution */
        switch(pcalc->oopt) {
          case bktOOPT_Every_Time:
                doOutput = 1;
            break;
          case bktOOPT_On_Change:
            if(fabs(pcalc->pval - pcalc->val) > pcalc->mdel)  {
                doOutput = 1;
            }
            break;
          case bktOOPT_Transition_To_Zero:
            if((pcalc->pval != 0) && (pcalc->val == 0)) {
                doOutput = 1;
            }
            break;         
          case bktOOPT_Transition_To_Non_zero:
            if((pcalc->pval == 0) && (pcalc->val != 0)) {
                doOutput = 1;
            }
            break;
          case bktOOPT_When_Zero:
            if(!pcalc->val) {
                doOutput = 1;
            }
            break;
          case bktOOPT_When_Non_zero:
            if(pcalc->val) {
                doOutput = 1;
            }
            break;
          default:
            break;
        }
        pcalc->pval = pcalc->val;

/* leige */
 if (pcalc->rlpf==1) {
   printf("reload pattern file %s\n", pcalc->ptrn);
   bktRecordTmp_i=0;
   bktRecord_pCount = 0;
   strcpy(berPattFile, pcalc->pdir);
   strcat(berPattFile, pcalc->ptrn);
   printf("berPattFile %s \n", berPattFile);
   fpBkt = fopen(berPattFile, "rb");
   if (fpBkt==NULL) {
     printf("pattern file open error\n");
       }
   else {
     for (bktRecordTmp_i=0; (fgets(bepc_tm_buf,256,fpBkt)!=NULL)&&(bktRecordTmp_i<TOTAL_BUCKET); bktRecordTmp_i++ ) {
     sscanf(bepc_tm_buf, "%d", &pattern[bktRecordTmp_i]);
     if ((pattern[bktRecordTmp_i]>TOTAL_BUCKET)||(pattern[bktRecordTmp_i]<0)) {
       printf("illegal bucket number. Use bucker 0\n");
       pattern[bktRecordTmp_i]=0;
       }
     bktRecord_pCount ++;
   }
   fclose(fpBkt);
   printf("total buckets in pattern file %d\n", bktRecord_pCount);
     for (bktRecordTmp_i=0; bktRecordTmp_i<bktRecord_pCount; bktRecordTmp_i++) {
     printf("%d , ", pattern[bktRecordTmp_i]);
     }
      } /*end of fpBkt==NULL else */
    }
    pcalc->rlpf=0;

/* printf("bktRecordTmp_i %d, pattern bktRecordTmp_i %d, bktRam %d\n", bktRecordTmp_i, pattern[bktRecordTmp_i], bktRam[pattern[bktRecordTmp_i]]); */
    pcalc->val=bktRam[pattern[bktRecordTmp_i]]+3;
/*   printf("bkt val -- %d\n",(int)( pcalc->val)); */

    bktRecordTmp_i++;
    if (bktRecordTmp_i>=bktRecord_pCount) bktRecordTmp_i=0;
/* end of leige bkt */

        if(doOutput) {
            if(pcalc->odly > 0.0) {
                pcalc->dlya = 1;
                db_post_events(pcalc,&pcalc->dlya,DBE_VALUE);
                wdDelay = pcalc->odly * sysClkRateGet();
                callbackSetPriority(pcalc->prio, &prpvt->doOutCb);
                wdStart(prpvt->wd_id_0, wdDelay, (FUNCPTR)callbackRequest,
                        (int)(&prpvt->doOutCb));
            }
            else {
                execOutput(pcalc);
            }
        }

	/* check event list */
	monitor(pcalc);

        /* if no delay requested, finish processing */ 
        if(!pcalc->dlya) {
            /* process the forward scan link record */
            recGblFwdLink(pcalc);
            pcalc->pact = FALSE;
        }
    }
    else {
        execOutput(pcalc);
        pcalc->dlya = 0;
        db_post_events(pcalc,&pcalc->dlya,DBE_VALUE);

	/* process the forward scan link record */
        recGblFwdLink(pcalc);

        pcalc->pact = FALSE;
    }

    return(0);
}

static long special(paddr,after)
    struct dbAddr *paddr;
    int	   	  after;
{
    struct bktRecord *pcalc = (struct bktRecord *)(paddr->precord);
    struct rpvtStruct   *prpvt = (struct rpvtStruct *)pcalc->rpvt;
    struct dbAddr       dbAddr;
    struct dbAddr       *pAddr = &dbAddr;
    short error_number;
    int                 fieldIndex = dbGetFieldIndex(paddr);
    int                 lnkIndex;
    struct link         *plink;
    double              *pvalue;
    unsigned short      *plinkValid;

    if(!after) return(0);
    switch(fieldIndex) {
      case(bktRecordCALC):
        pcalc->clcv=postfix(pcalc->calc,pcalc->rpcl,&error_number);
        if(pcalc->clcv){
              recGblRecordError(S_db_badField,(void *)pcalc,
                        "bkt: special(): Illegal CALC field");
        }
        db_post_events(pcalc,&pcalc->clcv,DBE_VALUE);
        return(0);
        break;

      case(bktRecordOCAL):
        pcalc->oclv=postfix(pcalc->ocal,pcalc->orpc,&error_number);
        if(pcalc->oclv){
                recGblRecordError(S_db_badField,(void *)pcalc,
                        "bkt: special(): Illegal OCAL field");
        }
        db_post_events(pcalc,&pcalc->oclv,DBE_VALUE);

        return(0);
        break;

      case(bktRecordINPA):
      case(bktRecordINPB):
      case(bktRecordINPC):
      case(bktRecordINPD):
      case(bktRecordINPE):
      case(bktRecordINPF):
      case(bktRecordINPG):
      case(bktRecordINPH):
      case(bktRecordINPI):
      case(bktRecordINPJ):
      case(bktRecordINPK):
      case(bktRecordINPL):
      case(bktRecordOUT):
        lnkIndex = fieldIndex - bktRecordINPA;
        plink   = &pcalc->inpa + lnkIndex;
        pvalue  = &pcalc->a    + lnkIndex;
        plinkValid = &pcalc->inav + lnkIndex;

        if (plink->type == CONSTANT) {
            if(fieldIndex != bktRecordOUT) {
                recGblInitConstantLink(plink,DBF_DOUBLE,pvalue);
                db_post_events(pcalc,pvalue,DBE_VALUE);
            }
            *plinkValid = bktINAV_CON;
        }
        /* see if the PV resides on this ioc */
        else if(!dbNameToAddr(plink->value.pv_link.pvname, pAddr)) {
            *plinkValid = bktINAV_LOC;
        }
        /* pv is not on this ioc. Callback later for connection stat */
        else {
            *plinkValid = bktINAV_EXT_NC;
            /* DO_CALLBACK, if not already scheduled */
            if(!prpvt->wd_id_1_LOCK) {
                wdStart(prpvt->wd_id_1, 30, (FUNCPTR)callbackRequest,
                    (int)(&prpvt->checkLinkCb));
                prpvt->wd_id_1_LOCK = 1;
                prpvt->caLinkStat = CA_LINKS_NOT_OK;
            }
        }
        db_post_events(pcalc,plinkValid,DBE_VALUE);

        
        return(0);
        break;

      default:
	recGblDbaddrError(S_db_badChoice,paddr,"calc: special");
	return(S_db_badChoice);
    }
}

static long get_units(paddr,units)
    struct dbAddr *paddr;
    char	  *units;
{
    struct bktRecord	*pcalc=(struct bktRecord *)paddr->precord;

    strncpy(units,pcalc->egu,DB_UNITS_SIZE);
    return(0);
}

static long get_precision(paddr,precision)
    struct dbAddr *paddr;
    long	  *precision;
{
    struct bktRecord	*pcalc=(struct bktRecord *)paddr->precord;

    *precision = pcalc->prec;
    if(paddr->pfield == (void *)&pcalc->val) return(0);
    recGblGetPrec(paddr,precision);
    return(0);
}

static long get_graphic_double(paddr,pgd)
    struct dbAddr *paddr;
    struct dbr_grDouble	*pgd;
{
    struct bktRecord	*pcalc=(struct bktRecord *)paddr->precord;

    if(paddr->pfield==(void *)&pcalc->val
    || paddr->pfield==(void *)&pcalc->hihi
    || paddr->pfield==(void *)&pcalc->high
    || paddr->pfield==(void *)&pcalc->low
    || paddr->pfield==(void *)&pcalc->lolo){
        pgd->upper_disp_limit = pcalc->hopr;
        pgd->lower_disp_limit = pcalc->lopr;
       return(0);
    } 

    if(paddr->pfield>=(void *)&pcalc->a && paddr->pfield<=(void *)&pcalc->l){
        pgd->upper_disp_limit = pcalc->hopr;
        pgd->lower_disp_limit = pcalc->lopr;
        return(0);
    }
    if(paddr->pfield>=(void *)&pcalc->la && paddr->pfield<=(void *)&pcalc->ll){
        pgd->upper_disp_limit = pcalc->hopr;
        pgd->lower_disp_limit = pcalc->lopr;
        return(0);
    }
    recGblGetGraphicDouble(paddr,pgd);
    return(0);
}

static long get_control_double(paddr,pcd)
    struct dbAddr *paddr;
    struct dbr_ctrlDouble *pcd;
{
    struct bktRecord	*pcalc=(struct bktRecord *)paddr->precord;

    if(paddr->pfield==(void *)&pcalc->val
    || paddr->pfield==(void *)&pcalc->hihi
    || paddr->pfield==(void *)&pcalc->high
    || paddr->pfield==(void *)&pcalc->low
    || paddr->pfield==(void *)&pcalc->lolo){
        pcd->upper_ctrl_limit = pcalc->hopr;
        pcd->lower_ctrl_limit = pcalc->lopr;
       return(0);
    } 

    if(paddr->pfield>=(void *)&pcalc->a && paddr->pfield<=(void *)&pcalc->l){
        pcd->upper_ctrl_limit = pcalc->hopr;
        pcd->lower_ctrl_limit = pcalc->lopr;
        return(0);
    }
    if(paddr->pfield>=(void *)&pcalc->la && paddr->pfield<=(void *)&pcalc->ll){
        pcd->upper_ctrl_limit = pcalc->hopr;
        pcd->lower_ctrl_limit = pcalc->lopr;
        return(0);
    }
    recGblGetControlDouble(paddr,pcd);
    return(0);
}
static long get_alarm_double(paddr,pad)
    struct dbAddr *paddr;
    struct dbr_alDouble	*pad;
{
    struct bktRecord	*pcalc=(struct bktRecord *)paddr->precord;

    if(paddr->pfield==(void *)&pcalc->val){
         pad->upper_alarm_limit = pcalc->hihi;
         pad->upper_warning_limit = pcalc->high;
         pad->lower_warning_limit = pcalc->low;
         pad->lower_alarm_limit = pcalc->lolo;
    } else recGblGetAlarmDouble(paddr,pad);
    return(0);
}


static void alarm(pcalc)
    struct bktRecord	*pcalc;
{
	double		hyst, lalm, val;
	double		hihi, high, low, lolo;
	unsigned short	hhsv, llsv, hsv, lsv;

	if(pcalc->udf == TRUE ){
 		recGblSetSevr(pcalc,UDF_ALARM,INVALID_ALARM);
		return;
	}
	hihi = pcalc->hihi; 
        lolo = pcalc->lolo; 
        high = pcalc->high;  
        low = pcalc->low;
	hhsv = pcalc->hhsv; 
        llsv = pcalc->llsv; 
        hsv = pcalc->hsv; 
        lsv = pcalc->lsv;
	val = pcalc->val; 
        hyst = pcalc->hyst; 
        lalm = pcalc->lalm;

	/* alarm condition hihi */
	if (hhsv && (val >= hihi || ((lalm==hihi) && (val >= hihi-hyst)))){
	        if (recGblSetSevr(pcalc,HIHI_ALARM,pcalc->hhsv)) 
                    pcalc->lalm = hihi;
		return;
	}

	/* alarm condition lolo */
	if (llsv && (val <= lolo || ((lalm==lolo) && (val <= lolo+hyst)))){
	        if (recGblSetSevr(pcalc,LOLO_ALARM,pcalc->llsv)) 
                    pcalc->lalm = lolo;
		return;
	}

	/* alarm condition high */
	if (hsv && (val >= high || ((lalm==high) && (val >= high-hyst)))){
	        if (recGblSetSevr(pcalc,HIGH_ALARM,pcalc->hsv)) 
                    pcalc->lalm = high;
		return;
	}

	/* alarm condition low */
	if (lsv && (val <= low || ((lalm==low) && (val <= low+hyst)))){
	        if (recGblSetSevr(pcalc,LOW_ALARM,pcalc->lsv)) 
                    pcalc->lalm = low;
		return;
	}

	/* we get here only if val is out of alarm by at least hyst */
	pcalc->lalm = val;
	return;
}

static void doOutputCallback(pcallback)
    struct callback *pcallback;
{

    dbCommon    *pcalc;
    struct rset *prset;

    callbackGetUser(pcalc, pcallback);
    prset = (struct rset *)pcalc->rset;
    dbScanLock((struct dbCommon *)pcalc);
    (*prset->process)(pcalc);
    dbScanUnlock((struct dbCommon *)pcalc);
}

    

static void execOutput(pcalc)
    struct bktRecord *pcalc;
{
        long            status;

        /* Determine output data */
        switch(pcalc->dopt) {
          case(bktDOPT_Use_VAL):
              pcalc->oval = pcalc->val;
            break; 
          case(bktDOPT_Use_OVAL):
              if(calcPerform(&pcalc->a,&pcalc->oval,pcalc->orpc)) {
                  recGblSetSevr(pcalc,CALC_ALARM,INVALID_ALARM);
              }
          break;
        }

        /* Check to see what to do if INVALID */
        if (pcalc->nsev < INVALID_ALARM ) {
            /* Output the value */
            status=dbPutLink(&(pcalc->out), DBR_DOUBLE,&(pcalc->oval),1);
            /* post event if output event != 0 */
            if(pcalc->oevt > 0) {
                post_event((int)pcalc->oevt);
            }
        }
        else {
            switch (pcalc->ivoa) {
                case (menuIvoaContinue_normally) :
                    /* write the new value */
                    status=dbPutLink(&(pcalc->out), DBR_DOUBLE,
                                     &(pcalc->oval),1);
                    /* post event if output event != 0 */
                    if(pcalc->oevt > 0) {
                        post_event((int)pcalc->oevt);
                    }
                  break;
                case (menuIvoaDon_t_drive_outputs) :
                  break;
                case (menuIvoaSet_output_to_IVOV) :
                    pcalc->oval=pcalc->ivov;
                    status=dbPutLink(&(pcalc->out), DBR_DOUBLE,
                                     &(pcalc->oval),1);
                    /* post event if output event != 0 */
                    if(pcalc->oevt > 0) {
                        post_event((int)pcalc->oevt);
                    }
                  break;
                default :
                    status=-1;
                    recGblRecordError(S_db_badField,(void *)pcalc,
                            "bkt:process Illegal IVOA field");
            }
        } 
}

static void monitor(pcalc)
    struct bktRecord	*pcalc;
{
	unsigned short	monitor_mask;
	double		delta;
	double		*pnew;
	double		*pprev;
	int		i;

        monitor_mask = recGblResetAlarms(pcalc);
        /* check for value change */
        delta = pcalc->mlst - pcalc->val;
        if(delta<0.0) delta = -delta;
        if (delta > pcalc->mdel) {
                /* post events for value change */
                monitor_mask |= DBE_VALUE;
                /* update last value monitored */
                pcalc->mlst = pcalc->val;
        }
        /* check for archive change */
        delta = pcalc->alst - pcalc->val;
        if(delta<0.0) delta = -delta;
        if (delta > pcalc->adel) {
                /* post events on value field for archive change */
                monitor_mask |= DBE_LOG;
                /* update last archive value monitored */
                pcalc->alst = pcalc->val;
        }

        /* send out monitors connected to the value field */
        if (monitor_mask){
                db_post_events(pcalc,&pcalc->val,monitor_mask);
        }
        /* check all input fields for changes*/
        for(i=0, pnew=&pcalc->a, pprev=&pcalc->la; i<ARG_MAX; 
            i++, pnew++, pprev++) {
             if((*pnew != *pprev) || (monitor_mask&DBE_ALARM)) {
                  db_post_events(pcalc,pnew,monitor_mask|DBE_VALUE|DBE_LOG);
                  *pprev = *pnew;
	     }
	}
        /* Check OVAL field */
        if(pcalc->povl != pcalc->oval) {
            db_post_events(pcalc,&pcalc->oval, monitor_mask|DBE_VALUE|DBE_LOG);
            pcalc->povl = pcalc->oval;
        }
        return;
}

static int fetch_values(pcalc)
    struct bktRecord *pcalc;
{
	struct link	*plink;	/* structure of the link field  */
	double		*pvalue;
	long		status = 0;
	int		i;

	for(i=0, plink=&pcalc->inpa, pvalue=&pcalc->a; i<ARG_MAX; 
            i++, plink++, pvalue++) {

            status = dbGetLink(plink,DBR_DOUBLE, pvalue,0,0);

	    if (!RTN_SUCCESS(status)) return(status);
	}
	return(0);
}

static void checkLinksCallback(pcallback)
    struct callback *pcallback;
{

    struct bktRecord *pcalc;
    struct rpvtStruct   *prpvt;

    callbackGetUser(pcalc, pcallback);
    prpvt = (struct rpvtStruct *)pcalc->rpvt;
    
    dbScanLock((struct dbCommon *)pcalc);
    prpvt->wd_id_1_LOCK = 0;
    checkLinks(pcalc);
    dbScanUnlock((struct dbCommon *)pcalc);

}



static void checkLinks(pcalc)
    struct bktRecord *pcalc;
{

    struct link *plink;
    struct rpvtStruct   *prpvt = (struct rpvtStruct *)pcalc->rpvt;
    int i;
    int stat;
    int caLink   = 0;
    int caLinkNc = 0;
    unsigned short *plinkValid;

    if(bktRecDebug) printf("checkLinks() for %p\n", pcalc);

    plink   = &pcalc->inpa;
    plinkValid = &pcalc->inav;

    for(i=0; i<ARG_MAX+1; i++, plink++, plinkValid++) {
        if (plink->type == CA_LINK) {
            caLink = 1;
            stat = dbCaIsLinkConnected(plink);
            if(!stat && (*plinkValid == bktINAV_EXT_NC)) {
                caLinkNc = 1;
            }
            else if(!stat && (*plinkValid == bktINAV_EXT)) {
                *plinkValid = bktINAV_EXT_NC;
                db_post_events(pcalc,plinkValid,DBE_VALUE);
                caLinkNc = 1;
            } 
            else if(stat && (*plinkValid == bktINAV_EXT_NC)) {
                *plinkValid = bktINAV_EXT;
                db_post_events(pcalc,plinkValid,DBE_VALUE);
            } 
        }
        
    }
    if(caLinkNc)
        prpvt->caLinkStat = CA_LINKS_NOT_OK;
    else if(caLink)
        prpvt->caLinkStat = CA_LINKS_ALL_OK;
    else
        prpvt->caLinkStat = NO_CA_LINKS;

    if(!prpvt->wd_id_1_LOCK && caLinkNc) {
        /* Schedule another CALLBACK */
        prpvt->wd_id_1_LOCK = 1;
        wdStart(prpvt->wd_id_1, 30, (FUNCPTR)callbackRequest,
               (int)(&prpvt->checkLinkCb));
    }
}

