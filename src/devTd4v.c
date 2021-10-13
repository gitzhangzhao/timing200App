/* devVmeTd4v.c */
/* devVmeTd4v.c - Device Support Routines for  VME TD-4V */
/*
 *      Original Author: Akiyama
 *      Current Author:
 *      Date:            2th May 1997
 *
 *      Experimental Physics and Industrial Control System (EPICS)
 *
 *      Copyright 1997, KEKB.
 *
 *
 * Modification Log:
 * -----------------
 *	5-8-97		ak
 *      27-Oct-97       Makoto Tobiyama 
 *      14-Feb-98       MT for extended access(32bit)
 *       5-Mar-98       MT for (final-1) version.
 *       3-Aug-98       Makoto Tobiyama Final version.
 *      20-Nov-1998     Makoto Tobiyama for distribution
 */


#include	<vxWorks.h>
#include	<types.h>
#include	<stdioLib.h>
#include	<string.h>

#include        <vme.h>
#include	<dbDefs.h>
#include	<dbAccess.h>
#include        <recSup.h>
#include	<devSup.h>
#include	<devCamac.h>
#include	<link.h>
#include	<module_types.h>
#include	<longinRecord.h>
#include	<longoutRecord.h>
#include        <boRecord.h>
#include        <biRecord.h>
#include        <mbbiDirectRecord.h>


/* #define	Base_IO		0x20000 */ /* Std. I/O Address */


struct DvmeTd4v {
  unsigned short	PreSet;
  unsigned short	Dummy0;
  unsigned short	sio;
  unsigned short	Dummy1;
  unsigned short	Last_PreSet;
  unsigned short	Dummy2;
  unsigned short        Dummy3;
  unsigned short        Dummy4;
};


#define	Status	Enable

static long init();
static long init_record();
static long read_longin();
static long write_longout();
static long init_bi_record();
static long init_bo_record();
static long read_bi();
static long write_bo();
static long init_mi_record();
static long read_mbbiDirect();
static int  checkLink();

struct {
	long		number;
	DEVSUPFUN	report;
	DEVSUPFUN	init;
	DEVSUPFUN	init_record;
	DEVSUPFUN	get_ioint_info;
	DEVSUPFUN	read_longin;
	DEVSUPFUN	special_linconv;
}devLiTd4v={
	6,
	NULL,
	init,
	init_record,
	NULL,
	read_longin,
        NULL};

/* Create the dset for devLoTd4v */
struct {
	long		number;
	DEVSUPFUN	report;
	DEVSUPFUN	init;
	DEVSUPFUN	init_record;
	DEVSUPFUN	get_ioint_info;
	DEVSUPFUN	write_longout;
	DEVSUPFUN	special_linconv;
}devLoTd4v={
	6,
	NULL,
	NULL,
	NULL,
	NULL,
	write_longout,
        NULL};


/* Create the dset for devBoTd4v */
struct {
        long            number;
        DEVSUPFUN       report;         /* used by dbior */
        DEVSUPFUN       init;           /* called 1 time before & after all records */
        DEVSUPFUN       init_record;    /* called 1 time for each record */
        DEVSUPFUN       get_ioint_info; /* used for COS processing (not used for outputs)*/
        DEVSUPFUN       write_bo;       /* output command goes here */
}devBoTd4v={
        5,
        NULL,
        NULL,
        init_bo_record,
        NULL,
        write_bo
};

/* Create the dset for devBiTd4V */
struct {
        long            number;
        DEVSUPFUN       report;         /* used by dbior */
        DEVSUPFUN       init;           /* called 1 time before & after all records */
        DEVSUPFUN       init_record;    /* called 1 time for each record */
        DEVSUPFUN       get_ioint_info; /* used for COS processing */
        DEVSUPFUN       read_bi;        /* input command goes here */
}devBiTd4v={
        5,
        NULL,
        NULL,
        init_bi_record,
        NULL,
        read_bi
};
struct{
        long		number;
	DEVSUPFUN	report;
	DEVSUPFUN	init;
	DEVSUPFUN	init_mi_record;
	DEVSUPFUN	get_ioint_info;
	DEVSUPFUN	read_mbbiDirect;
}devMbbiTd4v={
	5,
	NULL,
	NULL,
	init_mi_record,
	NULL,
	read_mbbiDirect};

struct ioCard {
        volatile struct DvmeTd4v     *card; /* address of this card */
        FAST_LOCK                    lock; /* semaphore */
      };

/*#define CONST_NUM_LINKS 10*/
#define CONST_NUM_LINKS 1
/* #define STATIC  */
#define DEBUG_ON

static int           debug_flag = 1;
static unsigned long Base_IO = 0x200;
int                  td4v_num_links =1;

static struct ioCard cards[CONST_NUM_LINKS];
static int           init_flag = 0;


int devTd4vConfig(ncards,a32base)
int ncards;
long a32base;
{
  td4v_num_links = ncards;
  Base_IO = a32base;
  logMsg("VmeTd4 NumLink= %d BaseIO= %x\n",td4v_num_links,Base_IO);
  init(0);
}


static long init(after)
int after;
{
  int                          cardNum, chanNum;
  unsigned char                probeVal[2];
  volatile  struct DvmeTd4v    *p;

  if (init_flag != 0 ) 
   return(OK);

  init_flag = 1;

  if (sysBusToLocalAdrs(VME_AM_EXT_SUP_DATA,(char *)Base_IO,(char **)&p) == ERROR)
	{
		logMsg("VmeTd4v: cannot find extended address space\n");
		return(ERROR);
	}
#ifdef DEBUG_ON  
    if ( CDEBUG)logMsg("devLiTd4v (init) Called. pass = %d\n", after);
#endif
  for (cardNum=0; cardNum< td4v_num_links; cardNum++)
   {
     if (vxMemProbe((char*) &(p->PreSet), READ, 2, &probeVal[0])< OK)
       {
	 if (debug_flag >0 ) 
         logMsg("No TD4V with cardNum= %d\n probe= %x\n",cardNum,p);
	 cards[cardNum].card = NULL;
       }
     else
       {
	 if (debug_flag >0)
         logMsg("Found TD4V with cardNum= %d\n address= %x\n",cardNum,p);
	 cards[cardNum].card = p;  /* Remember address of the board */
	 FASTLOCKINIT(&(cards[cardNum].lock));
	 FASTUNLOCK(&(cards[cardNum].lock)); /* Init the board lock */
       }
     p++;
   }
  return(OK);
}

static long init_record(plongin)
struct longinRecord	*plongin;
{
  return(0);
}

static long read_longin(plongin)
struct longinRecord	*plongin;
{
  short cardN;

  if (debug_flag >10)
    logMsg("read_longin called...\n");

  cardN = plongin->inp.value.vmeio.card;
  if (debug_flag >10)
    logMsg("read_longin called with card number of %d\n",cardN);

  if (checkLink(cardN) == ERROR)
    return(ERROR);

		switch(plongin->inp.value.vmeio.signal){
			case 0:
		               plongin->val = cards[cardN].card->PreSet;
                               if (debug_flag >10)
                                logMsg("read_longin signal 0 Preset %d\n",
                                        plongin->val);

				break;
			case 1:
			  	plongin->val = cards[cardN].card->PreSet;
                                if (debug_flag >10)
                                logMsg("read_longin signal 1 Last Preset %d\n",
                                        plongin->val);
				break;
			default:
				return(-1);
		}
        return(CONVERT);
}

static long write_longout(plongout)
struct longoutRecord	*plongout;
{

  short cardN;
  long  setpreset;
  
  cardN = plongout->out.value.vmeio.card;

  if (debug_flag >10) 
   logMsg("write_longout called with card %d\n",cardN);

  if (checkLink(cardN) == ERROR)
    return(ERROR);

  FASTLOCK(&(cards[cardN].lock));
  if (debug_flag >10)
   logMsg("card locked...\n");
  setpreset = plongout->val;
  if (setpreset<=0)
    cards[cardN].card->PreSet = 1;
  else if (setpreset>65535)
    cards[cardN].card->PreSet = 65535;
  else
    cards[cardN].card->PreSet = (unsigned short)(plongout->val);
  if (debug_flag >10)
   logMsg("write complete \n");
  FASTUNLOCK(&(cards[cardN].lock));
  if (debug_flag >10)
   logMsg("unlock card ...\n");

        return(CONVERT);
}

/**************************************************************************
 *
 * BO Initialization (Called one time for each BO PowerUT card record)
 *
 **************************************************************************/
static long init_bo_record(pbo)
struct boRecord *pbo;
{

        pbo->mask = 1;
		return(0);
}


/**************************************************************************
 *
 * Perform a write operation from a BO record
 *
 **************************************************************************/
static long
write_bo(pbo)
struct boRecord *pbo;
{

  short cardN;

  cardN = pbo->out.value.vmeio.card;
  if (checkLink(cardN) == ERROR)
    {
      logMsg("Error--- No TD4V for card %d\n",cardN);
      return(ERROR);
    }

  FASTLOCK(&(cards[cardN].lock));
  cards[cardN].card->sio = pbo->rval & pbo->mask;
  FASTUNLOCK(&(cards[cardN].lock));
	return(0);
}

/**************************************************************************
 *
 * BI Initialization (Called one time for each BI PowerUT card record)
 *
 **************************************************************************/
static long 
init_bi_record(pbi)
struct biRecord *pbi;
{

        pbi->mask = 1;
		return(0);
}

/**************************************************************************
 *
 * Perform a read operation from a BI record
 *
 **************************************************************************/
static long 
read_bi(pbi)
struct biRecord *pbi;
{
  short cardN;

  cardN = pbi->inp.value.vmeio.card;
  if (checkLink(cardN) == ERROR)
    return(ERROR);
  
  switch(pbi->inp.value.vmeio.signal){
	case 0:
	       pbi->rval = cards[cardN].card->sio & pbi->mask;
               if (debug_flag >10)
                    logMsg("read_bi signal 0 enable %d\n",pbi->rval);
	       break;
	case 1:
	       pbi->rval = (cards[cardN].card->sio)>>2 & pbi->mask;
	       if (debug_flag >10)
                    logMsg("read_bi signal 1 RF OK %d\n",pbi->rval);
	       break;
	case 2:
	       pbi->rval = (cards[cardN].card->sio)>>3 & pbi->mask;
	       if (debug_flag >10)
                    logMsg("read_bi signal 2 Start OK %d\n",pbi->rval);
	       break;
        case 3:
	       pbi->rval = (cards[cardN].card->sio)>>1 & pbi->mask;
	       if (debug_flag >10)
                    logMsg("read_bi signal 3 INH %d\n",pbi->rval);
	       break;
        case 4: 
	       pbi->rval = (cards[cardN].card->sio)>>4 & pbi->mask;
	       if (debug_flag > 10)
                    logMsg("read_bi signal 4 OUT state%d\n",pbi->rval);
	       break;

        default:
		return(-1);
  }		
  return(0);
}
static long init_mi_record(pmbbi)
struct MbbiDirectRecord	*pmbbi;
{
  return(0);
}

static long read_mbbiDirect(pmbbi)
struct mbbiDirectRecord *pmbbi;
{
  short cardN;
  
  cardN = pmbbi->inp.value.vmeio.card;
  if (debug_flag >5) 
   printf("read_mbbiDirect called with card %d\n",cardN);

  if (checkLink(cardN) == ERROR)
    return(ERROR);

  pmbbi->rval = cards[cardN].card->sio;
  if (debug_flag >5)
   printf("read complete \n");

        return(CONVERT);
}


/**************************************************************************
 *
 * Make sure card number is valid
 *
 **************************************************************************/
static int checkLink(cardN)
short   cardN;
{
  if (cardN >= td4v_num_links)
    return(ERROR);
  if (cards[cardN].card == NULL)
    {
      logMsg("No TD4V with this number = %d\n",cardN);
      return(ERROR);
    }

  if (debug_flag >10)
    logMsg("Yes you have TD4V with card No= %d\n",cardN);
  return(OK);
}
