/******************************************************************************

  EPICS device support for the APS/Micro-Research (Diamond & PSI) Event Receiver
 
 *****************************************************************************/
#define ER_DEBUG 
/*
 *      Original author: 	John Winans
 *      Date from original:     07-21-93
 *      Current author:         Timo Korhonen
 *      Date                    06/28/01
 *      Modification		Babak Kalantari
 *  	Date			02/11/03
 *      Modification		Babak Kalantari
 *  	Date			07/31/03
 *	Modification		Timo Korhonen
 *	Date			07/25/05
 *  
 * *****************************************************************
 *                           COPYRIGHT NOTIFICATION
 * *****************************************************************
 *  
 * THE FOLLOWING IS A NOTICE OF COPYRIGHT, AVAILABILITY OF THE CODE,
 * AND DISCLAIMER WHICH MUST BE INCLUDED IN THE PROLOGUE OF THE CODE
 * AND IN ALL SOURCE LISTINGS OF THE CODE.
 *
 */
/*
 * Modification Log:
 * -----------------
 * New version for the 200-series (Micro-Research) event system ).
 *
 */
static char cvsid[]="$Id: devApsEr.c,v 1.1.1.1 2006/10/17 00:58:05 leige Exp $";
#include <vxWorks.h>
#include <stdlib.h>
#include <stdio.h>
#include <iosLib.h>
#include <taskLib.h>
#include <semLib.h>
#include <memLib.h>
#include <logLib.h>
#include <rebootLib.h>
#include <intLib.h>
#include <lstLib.h>
#include <vme.h>
#include <sysLib.h>
#include <iv.h>

#include <module_types.h>
#include <devSup.h>
#include <recGbl.h>
#include <dbDefs.h>
#include <dbAccess.h>
#include <epicsPrint.h>
#include <link.h>
#include <dbScan.h>
#include <eventRecord.h>
#include <dbCommon.h>

#include <erDefs.h>
#include <erRecord.h>
#include <ereventRecord.h>
#include <devApsEr.h>

#include <vme64x_cr.h>


#ifdef ER_DEBUG
#define STATIC
#else
#define STATIC static
#endif

typedef struct MrfErStruct
{
  unsigned short	Control;
  unsigned short	EventRamAddr;
  unsigned short	EventRamData;
  unsigned short	OutputPulseEnables;
  unsigned short	OutputLevelEnables;
  unsigned short	TriggerEventEnables;
  unsigned short	EventCounterLo;
  unsigned short	EventCounterHi;
  unsigned short	TimeStampLo;
  unsigned short	TimeStampHi;

  unsigned short	EventFifo; /* LSB = event #, MSB = LSB of time stamp counter */
  unsigned short	EventTimeHi; /* bits 23-8 of the time stamp counter */
  unsigned short	DelayPulseEnables;
  unsigned short	DelayPulseSelect; 
  /* OTP pulses have delays and widths. See HW technical reference */
  unsigned short	PulseDelay;
  unsigned short	PulseWidth;
  unsigned short	IrqVector;
  unsigned short	IrqEnables;
  /* Extended registers */
  unsigned short	DBusEnables; /* Distributed bus enable register */
  unsigned short	DBusData; /* Distributed bus data register (read only) */
  unsigned short	DelayPrescaler; /* Programmable delay pulse clock prescaler */
  unsigned short	EventCounterClock; /* Event counter clock prescaler */
  unsigned short	Resvd1; /* 0x2c */
  unsigned short	FPGAVersion; /* FPGA Firmware version number */
  unsigned short 	Resvd3; /* 0x30 */
  unsigned short 	Resvd4; /* 0x32 */
  unsigned short 	Resvd5; /* 0x34 */
  unsigned short 	Resvd6; /* 0x36 */
  unsigned short 	Resvd7; /* 0x38 */
  unsigned short 	Resvd8; /* 0x3a */
  unsigned short 	Resvd9; /* 0x3c */
  unsigned short 	Resvd10; /* 0x3e */
  unsigned short 	FP0Map; /* 0x040, Front panel output mapping register */
  unsigned short 	FP1Map; /* 0x042, Front panel output mapping register */
  unsigned short 	FP2Map; /* 0x044, Front panel output mapping register */
  unsigned short 	FP3Map; /* 0x046, Front panel output mapping register */
  unsigned short 	FP4Map; /* 0x048, Front panel output mapping register */
  unsigned short 	FP5Map; /* 0x04a, Front panel output mapping register */
  unsigned short 	FP6Map; /* 0x04c, Front panel output mapping register */
  unsigned short 	uSecDivider; /*Resvd11; 0x4e*/
  unsigned short 	ExtEventCode;  /* 0x50 */
  unsigned short	ClockControl;
  unsigned int		SecondsSR;
  unsigned int		TSSec;
  unsigned int		Resvd12;
  unsigned int		EvFIFOSec;
  unsigned int		EvFIFOEvCnt;
  unsigned int		OutputPol;
  unsigned int		ExtDelay; /*extended/32-bit delay register*/
  unsigned int		ExtWidth; /*extended/32-bit width register*/
  unsigned short 	Presc0;   /* 0x074, Front panel clock #0 prescaler */
  unsigned short 	Presc1;	  /* 0x076, Front panel clock #1 prescaler */
  unsigned short 	Presc2;   /* 0x078, Front panel clock #2 prescaler */
  /*unsigned int	 	Resvd13;*/ /* 0x7a*/
  unsigned short	DataBufCtrl; /* 0x7a*/ /*leige */
  unsigned int	 	RFpattern; /* 0x7c*/ /*leige set RFpattern */
  unsigned int		FracDiv; /* 0x80 leige */
  /* These registers are only of special interest and
     left outside EPICS support for the time being. TK, 25-JUL-05. */
  unsigned int		RfDelay;
  unsigned int		RxDelay;
  unsigned int		Resvd14; /* 0x8c leige */
  unsigned int		FBRFFrac; /* 0x90 leige */
  unsigned int		FbRxFrac;
  unsigned int		RFDelyInit;
  unsigned int		RxDelyInit; /* 09C */
  unsigned int          CML4Pat00; /* 0A0 */
  unsigned int          CML4Pat01; /* 0A4 */
  unsigned int          CML4Pat10; /* 0A8 */
  unsigned int          CML4Pat11; /* 0AC */
  unsigned int          CML4Ena; /* 0B0 */
  unsigned int          CML4EnaResv_1; /* 0B4 */
  unsigned int          CML4EnaResv_2; /* 0B8 */
  unsigned int          CML4EnaResv_3; /* 0BC */
  unsigned int          CML5Pat00; /* 0C0 */
  unsigned int          CML5Pat01; /* 0C4 */
  unsigned int          CML5Pat10; /* 0C8 */
  unsigned int          CML5Pat11; /* 0CC */
  unsigned int          CML5Ena; /* 0D0 */
  unsigned int          CML5EnaResv_1; /* 0B4 */
  unsigned int          CML5EnaResv_2; /* 0B8 */
  unsigned int          CML5EnaResv_3; /* 0BC */
  unsigned int          CML6Pat00; /* 0E0 */
  unsigned int          CML6Pat01; /* 0E4 */
  unsigned int          CML6Pat10; /* 0E8 */
  unsigned int          CML6Pat11; /* 0EC */
  unsigned int          CML6Ena; /* 0F0 */
  unsigned int          CML6EnaResv_1; /* 0B4 */
  unsigned int          CML6EnaResv_2; /* 0B8 */
  unsigned int          CML6EnaResv_3; /* 0BC */

} MrfErStruct;



/* 
 * CONTROL_REG_OR_STRIP = bits that we should never leave set when or-ing
 * on other bits in the control register.
 */
#define CONTROL_REG_OR_STRIP	(~0x3c9f)

#define NUM_EVENTS	257

#define ER_IRQ_ALL	0x001f
#define ER_IRQ_OFF	0x0000
#define ER_IRQ_TELL	0xffff
#define ER_IRQ_EVENT	0x0008
#define ER_IRQ_HEART	0x0004
#define ER_IRQ_FIFO	0x0002
#define ER_IRQ_TAXI	0x0001
#define ER_IRQ_DVME	0x0010
#define ER_OT0_POL	0x0800
/* Fractional synthesizer word setting. From HW doc. */
#define FR_SYNTH_WORD   0x0c928166

/* leige add FR_SYNTH_WORD definition for BEPCII */
#define BEPCII_FRAC_4 0x00FE816D

typedef struct ErLinkStruct
{
  struct erRecord	*pRec;		/**< Pointer to the ER record */
  int			Card;		/**< Card number */
  long			IrqVector;	/**< IRQ Vector */
  long			IrqLevel;	/**< VME Interrupt level */
  MrfErStruct		*pEr;		/**< pointer to the register map struct */
  SEM_ID		LinkLock;	/**< Lock for the card */
  unsigned short	ErEventTab[NUM_EVENTS]; /**< current view of the RAM */
  IOSCANPVT		IoScanPvt[NUM_EVENTS];	/**< event-based rec processing */
  EVENT_FUNC		EventFunc;		/**< Pointer to registerable event handling function */
  ERROR_FUNC		ErrorFunc;		/**< Pointer to registerable error handling function */
  unsigned short	RxvioCount;	/**<  RXVIO error counter */
} ErLinkStruct;

/* leige test evg200 and evr200. 2005 Oct 12 */
STATIC MrfErStruct *lgEr;
/*end of leige test*/


#define NUM_ER_LINKS	21
static int              ErNumLinks = 0;	/* User configurable link limit */
static int		ConfigureLock = 0;
STATIC ErLinkStruct	ErLink[NUM_ER_LINKS];
int			ErDebug = 0;  /*leige move it to erDefs.h*/
int			ErStallIrqs = 1;

STATIC void func(int Card, int ErrorNum);	/* Error function for RX violation interrupt */

STATIC long ErMasterEnableGet(MrfErStruct *pParm);
STATIC long ErMasterEnableSet(MrfErStruct *pParm, int Enable);
STATIC long ErSetTrg(MrfErStruct *pParm, int Channel, int Enable);
STATIC long ErSetOtp(MrfErStruct *pParm, int Channel, int Enable,
		     unsigned short Width, unsigned short Pol,unsigned int Delay);
STATIC long ErSetOtl(MrfErStruct *pParm, int Channel, int Enable);
STATIC long ErSetDg(MrfErStruct *pParm, int Channel, int Enable,
		    unsigned int Delay, unsigned int Width, 
		    unsigned short Prescaler, unsigned short Pol);
STATIC long ErSetOtb(MrfErStruct *pParm, int Channel, int Enable);
STATIC long ErSetDVME(MrfErStruct *pParm, int Enable, unsigned short Delay,
		    unsigned short Prescaler);
STATIC unsigned short ErEnableIrq(MrfErStruct *pParm, unsigned short Mask);
STATIC int ErRegisterIrq(ErLinkStruct *pLink, int IrqVector, int IrqLevel);
STATIC long ErCheckTaxi(MrfErStruct *pParm);
STATIC int ErGetRamStatus(MrfErStruct *pParm, int Ram);
STATIC int ErResetAll(MrfErStruct *pParm);
STATIC int ErUpdateRam(MrfErStruct *pParm, unsigned short *RamBuf);
STATIC int ErProgramRam(MrfErStruct *pParm, unsigned short *RamBuf, int Ram);
STATIC int ErEnableRam(MrfErStruct *pParm, int Ram);
STATIC long proc(struct erRecord  *pRec);
STATIC long ErGetFpgaVersion(MrfErStruct *pParm);
STATIC long ErInitDev(int pass);
STATIC long init_record(struct erRecord *pRec);
STATIC long ErSetFPMap(MrfErStruct *pEvr, int channel, int map);

STATUS vmeCRSSetIRQLevel(int slot, UINT16 level);

ErDsetStruct devEr={ 5, NULL, ErInitDev, init_record, NULL, proc};

/**
 *
 * Routine used to verify that a given card number is valid.
 * Returns the number of events if valid, nonzero otherwise.
 *
 **/
long ErHaveReceiver(int Card)
{
  if ((Card < 0)||(Card >= ErNumLinks)) {
    return(-1);
  }
  if (ErLink[Card].pEr == NULL)
    {
      return(-1);
    }
  return(256);	/* Return the number of possible event codes, 0-255. The extra code here is for the
  	delayed interrupt to be able to timestamp from that. */
}

/**
 *
 * Register a listener for the event system.  Every time we get an event from
 * the event receiver, we will call the registered listener and pass in the
 * event number received and the tick counter value when that event was
 * received.
 *
 **/
long ErRegisterEventHandler(int Card, EVENT_FUNC Eventfunc)
{
  if (ErDebug){
    logMsg("ErRegisterEventHandler(%d, %p)\n", Card, (int)Eventfunc,0,0,0,0);
  }
  ErLink[Card].EventFunc = Eventfunc;
  return(0);
}
/**
 *
 * Register an error handler for the event system. 
 * Every time we get an error (rxvio, etc.) from
 * the event receiver, we will call the registered listener and pass in the
 * event number received and the tick counter value when that event was
 * received.
 *
 **/
long ErRegisterErrorHandler(int Card, ERROR_FUNC errorfunc)
{
  if (ErDebug){
    logMsg("ErRegisterErrorHandler(%d, %p\n", Card, (int)errorfunc,0,0,0,0);
  }
  ErLink[Card].ErrorFunc = errorfunc;
  return(0);
}

/**
 *
 * Return the current tick counter value.
 *
 **/
long ErGetTicks(int Card, unsigned long *Ticks)
{
  int key;
  if (ErHaveReceiver(Card) < 0){
    return(-1);
  }
  /*-- Do we read the HI first or the low? 
  	There is a very small probability that the low counter would
	roll over just between reading lo and hi bytes.
  */

  key = intLock();
  *Ticks = ErLink[Card].pEr->EventCounterLo;
  *Ticks += ErLink[Card].pEr->EventCounterHi << 16;
  intUnlock(key);

  return(0);
}

/**
 *
 * This routine is to be called in the startup script in order to init the
 * card addresses and the associated IRQ vectors and levels.
 *
 * By default there are no cards configured.
 *
 **/
int ErConfigure(int Card, unsigned long CardAddress, unsigned int IrqVector, 
		unsigned int IrqLevel)
{
  short	Junk;

  if (ConfigureLock != 0)
  {
    epicsPrintf("devApsEr: Cannot change configuration after init.  Request ignored\n");
    return(-1);
  }
  if (Card >= NUM_ER_LINKS)
  {
    epicsPrintf("devApsEr: Card number invalid, must be 0-%d\n", NUM_ER_LINKS);
    return(-1);
  }
  if (CardAddress > 0xffff)
  {
    epicsPrintf("devApsEr: Card address invalid, must be 0x0000-0xffff\n");
    return(-1);
  }
  /* CSR handling here */
  vmeCSRWriteADER(Card,0,(CardAddress&0xff00)|0xa4);
  vmeCRSSetIRQLevel(Card,IrqLevel);

  if(sysBusToLocalAdrs(VME_AM_SUP_SHORT_IO, (char*)CardAddress,
		       (char**)&ErLink[Card].pEr)!=OK)
  {
    epicsPrintf("devApsEr: Failure mapping requested A16 address\n");
    ErLink[Card].pEr = NULL;
    return(-1);
  }
  if (vxMemProbe((char*)&ErLink[Card].pEr->Control, READ,
		 sizeof(short), (char*)&Junk) != OK)
  {
    epicsPrintf("devApsEr: Failure probing for event receiver... Card disabled\n");
    ErLink[Card].pEr = NULL;
    return(-1);
  }
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

  ErLink[Card].IrqVector = IrqVector;
  ErLink[Card].IrqLevel = IrqLevel;
  /*ErLink[Card].pEr->FracDiv=FR_SYNTH_WORD;*/ /* program the correct frequency */
  ErLink[Card].pEr->FracDiv=BEPCII_FRAC_4; 

  if (Card >= ErNumLinks){
    ErNumLinks = Card+1;
  }

  /*leige test*/
  printf("ErConfigure: address: %X\n", (int)(&(ErLink[Card].pEr)));
  printf("ErConfigure: address: %X\n", (int)((ErLink[Card].pEr)));
  printf("%d\n", (int)Junk);
  lgEr=ErLink[Card].pEr;

  printf("get RFpattern - %8X\n", ErLink[Card].pEr->RFpattern);

   printf("set 0x7c to 0xC6318, divider is 4\n"); /* leige 2005 Oct 19*/
   ErLink[Card].pEr->RFpattern=0xC6318 ; /* divider 4 . leige */
  printf("get current RFpattern - %8X\n", ErLink[Card].pEr->RFpattern);
  /*end of leige test */
  printf("get EVR uSecDivider - %8X\n", ErLink[Card].pEr->uSecDivider);
  printf("get EVR ExtEvent - %8X\n", ErLink[Card].pEr->ExtEventCode);
  printf("get EVR ClockControl - %8X\n", ErLink[Card].pEr->ClockControl);

  return(0);
}

int ErConfigure1(int Card, unsigned long CardAddress, unsigned int IrqVector, 
                unsigned int IrqLevel, unsigned int divider)
{
  short Junk;

  if (ConfigureLock != 0)
  {
    epicsPrintf("devApsEr: Cannot change configuration after init.  Request ignored\n");
    return(-1);
  }
  if (Card >= NUM_ER_LINKS)
  {
    epicsPrintf("devApsEr: Card number invalid, must be 0-%d\n", NUM_ER_LINKS);
    return(-1);
  }
  if (CardAddress > 0xffff)
  {
    epicsPrintf("devApsEr: Card address invalid, must be 0x0000-0xffff\n");
    return(-1);
  }
  /* CSR handling here */
  vmeCSRWriteADER(Card,0,(CardAddress&0xff00)|0xa4);
  vmeCRSSetIRQLevel(Card,IrqLevel);

  if(sysBusToLocalAdrs(VME_AM_SUP_SHORT_IO, (char*)CardAddress,
                       (char**)&ErLink[Card].pEr)!=OK)
  {
    epicsPrintf("devApsEr: Failure mapping requested A16 address\n");
    ErLink[Card].pEr = NULL;
    return(-1);
  }
  if (vxMemProbe((char*)&ErLink[Card].pEr->Control, READ,
                 sizeof(short), (char*)&Junk) != OK)
  {
    epicsPrintf("devApsEr: Failure probing for event receiver... Card disabled\n");
    ErLink[Card].pEr = NULL;
    return(-1);
  }
  ErLink[Card].IrqVector = IrqVector;
  ErLink[Card].IrqLevel = IrqLevel;
  /*ErLink[Card].pEr->FracDiv=FR_SYNTH_WORD;*/ /* program the correct frequency */
 /* ErLink[Card].pEr->FracDiv=BEPCII_FRAC_4; */

  if (Card >= ErNumLinks){
    ErNumLinks = Card+1;
  }

  /*leige test*/
  printf("ErConfigure: address: %X\n", (int)(&(ErLink[Card].pEr)));
  printf("ErConfigure: address: %X\n", (int)((ErLink[Card].pEr)));
  printf("%d\n", (int)Junk);
  lgEr=ErLink[Card].pEr;

  printf("get original RFpattern - %8X\n", ErLink[Card].pEr->RFpattern);
  printf("get original FracDiv - %8X\n", ErLink[Card].pEr->FracDiv);

  switch (divider) {
    case 0: 
      printf("use card default\n");
      break;
    case 4:
      printf("set 0xC6318 to register 0x7c, divider is 4\n"); /* leige 2005 Oct 19*/
      ErLink[Card].pEr->RFpattern=0xC6318 ; /* divider 4 . leige */
      ErLink[Card].pEr->FracDiv=BEPCII_FRAC_4; 
      ErLink[Card].pEr->uSecDivider = 125;
      break;
    case 5:
      printf("set 0xCCCCC to register 0x7c, divider is 5\n"); /* leige 2006 Apr 11*/
      ErLink[Card].pEr->RFpattern=0xCCCCC ; /* divider 5 . leige */
      ErLink[Card].pEr->FracDiv=0x025b41ed; 
      ErLink[Card].pEr->uSecDivider = 100;
      break;
    case 10:
      printf("set 0xAAAAA to register 0x7c, divider is 10\n"); /* leige 2006 Mar 17*/
      ErLink[Card].pEr->RFpattern=0xAAAAA ; /* divider 10 . leige */
      ErLink[Card].pEr->FracDiv=0x025b43ad; 
      ErLink[Card].pEr->uSecDivider = 50;
      break;
  }
  /*end of leige test */

  sysUsDelay(1000000);
  
  printf("get current RFpattern - %8X\n", ErLink[Card].pEr->RFpattern);
  printf("get current FracDiv - %8X\n", ErLink[Card].pEr->FracDiv);
  printf("get current uSecDivider - %8X\n", ErLink[Card].pEr->uSecDivider);
  printf("get EVR ExtEvent - %8X\n", ErLink[Card].pEr->ExtEventCode);
  printf("get EVR ClockControl - %8X\n", ErLink[Card].pEr->ClockControl);

  return(0);
}

/* 2007 Apr 4, to to set RfPattern to get 100MHz RF */
int ErConfigure2(int Card, unsigned long CardAddress, unsigned int IrqVector,
                unsigned int IrqLevel, unsigned int divider )
{
  short Junk;

  if (ConfigureLock != 0)
  {
    epicsPrintf("devApsEr: Cannot change configuration after init.  Request ignored\n");
    return(-1);
  }
  if (Card >= NUM_ER_LINKS)
  {
    epicsPrintf("devApsEr: Card number invalid, must be 0-%d\n", NUM_ER_LINKS);
    return(-1);
  }
  if (CardAddress > 0xffff)
  {
    epicsPrintf("devApsEr: Card address invalid, must be 0x0000-0xffff\n");
    return(-1);
  }
  /* CSR handling here */
  vmeCSRWriteADER(Card,0,(CardAddress&0xff00)|0xa4);
  vmeCRSSetIRQLevel(Card,IrqLevel);

  if(sysBusToLocalAdrs(VME_AM_SUP_SHORT_IO, (char*)CardAddress,
                       (char**)&ErLink[Card].pEr)!=OK)
  {
    epicsPrintf("devApsEr: Failure mapping requested A16 address\n");
    ErLink[Card].pEr = NULL;
    return(-1);
  }
  if (vxMemProbe((char*)&ErLink[Card].pEr->Control, READ,
                 sizeof(short), (char*)&Junk) != OK)
  {
    epicsPrintf("devApsEr: Failure probing for event receiver... Card disabled\n");
    ErLink[Card].pEr = NULL;
    return(-1);
  }
  ErLink[Card].IrqVector = IrqVector;
  ErLink[Card].IrqLevel = IrqLevel;
  /*ErLink[Card].pEr->FracDiv=FR_SYNTH_WORD;*/ /* program the correct frequency */
 /* ErLink[Card].pEr->FracDiv=BEPCII_FRAC_4; */

  if (Card >= ErNumLinks){
    ErNumLinks = Card+1;
  }

  /*leige test*/
  printf("ErConfigure: address: %X\n", (int)(&(ErLink[Card].pEr)));
  printf("ErConfigure: address: %X\n", (int)((ErLink[Card].pEr)));
  printf("%d\n", (int)Junk);
  lgEr=ErLink[Card].pEr;

  printf("get original RFpattern - %8X\n", ErLink[Card].pEr->RFpattern);
  printf("get original FracDiv - %8X\n", ErLink[Card].pEr->FracDiv);

  switch (divider) {
    case 0:
      printf("use card default\n");
      break;
    case 4:
      printf("set 0xC6318 to register 0x7c, divider is 4\n"); /* leige 2005 Oct 19*/
      ErLink[Card].pEr->RFpattern=0xC6318 ; /* divider 4 . leige */
      ErLink[Card].pEr->FracDiv=BEPCII_FRAC_4;
      ErLink[Card].pEr->uSecDivider = 125;
      break;
    case 5:
      printf("set 0xCCCCC to register 0x7c, divider is 5\n"); /* leige 2006 Apr 11*/
      ErLink[Card].pEr->RFpattern=0xCCCCC ; /* divider 5 . leige */
      ErLink[Card].pEr->FracDiv=0x025b41ed;
      ErLink[Card].pEr->uSecDivider = 100;
      break;
    case 10:
      printf("set 0xAAAAA to register 0x7c, divider is 10\n"); /* leige 2006 Mar 17*/
      ErLink[Card].pEr->RFpattern=0xAAAAA ; /* divider 10 . leige */
      ErLink[Card].pEr->FracDiv=0x025b43ad;
      ErLink[Card].pEr->uSecDivider = 50;
      break;
  }
  /*end of leige test */
  ErLink[Card].pEr->RFpattern=0xFFC00; /* generate event clock rate RF */

  sysUsDelay(1000000);
 
  printf("get current RFpattern - %8X\n", ErLink[Card].pEr->RFpattern);
  printf("get current FracDiv - %8X\n", ErLink[Card].pEr->FracDiv);
  printf("get current uSecDivider - %8X\n", ErLink[Card].pEr->uSecDivider);
  printf("get EVR ExtEvent - %8X\n", ErLink[Card].pEr->ExtEventCode);
  printf("get EVR ClockControl - %8X\n", ErLink[Card].pEr->ClockControl);

  return(0);
} /* end of ErConfigure2 */


/* 2007 Sep. leige created ErConfigure230 */
/* FPS6 - 499.8MHz, FPS5 - 99.96MHz */
int ErConfigure230(int Card, unsigned long CardAddress, unsigned int IrqVector, 
                unsigned int IrqLevel, unsigned int divider)
{
  short Junk;

printf("ErConfigure230 begin.\n");

  if (ConfigureLock != 0)
  {
    epicsPrintf("devApsEr: Cannot change configuration after init.  Request ignored\n");
    return(-1);
  }
  if (Card >= NUM_ER_LINKS)
  {
    epicsPrintf("devApsEr: Card number invalid, must be 0-%d\n", NUM_ER_LINKS);
    return(-1);
  }
  if (CardAddress > 0xffff)
  {
    epicsPrintf("devApsEr: Card address invalid, must be 0x0000-0xffff\n");
    return(-1);
  }
  /* CSR handling here */
  vmeCSRWriteADER(Card,0,(CardAddress&0xff00)|0xa4);
  vmeCRSSetIRQLevel(Card,IrqLevel);

  if(sysBusToLocalAdrs(VME_AM_SUP_SHORT_IO, (char*)CardAddress,
                       (char**)&ErLink[Card].pEr)!=OK)
  {
    epicsPrintf("devApsEr: Failure mapping requested A16 address\n");
    ErLink[Card].pEr = NULL;
    return(-1);
  }
  if (vxMemProbe((char*)&ErLink[Card].pEr->Control, READ,
                 sizeof(short), (char*)&Junk) != OK)
  {
    epicsPrintf("devApsEr: Failure probing for event receiver... Card disabled\n");
    ErLink[Card].pEr = NULL;
    return(-1);
  }
  ErLink[Card].IrqVector = IrqVector;
  ErLink[Card].IrqLevel = IrqLevel;
  /*ErLink[Card].pEr->FracDiv=FR_SYNTH_WORD;*/ /* program the correct frequency */
 /* ErLink[Card].pEr->FracDiv=BEPCII_FRAC_4; */

  if (Card >= ErNumLinks){
    ErNumLinks = Card+1;
  }

  /*leige test*/
  printf("ErConfigure: address: %X\n", (int)(&(ErLink[Card].pEr)));
  printf("ErConfigure: address: %X\n", (int)((ErLink[Card].pEr)));
  printf("%d\n", (int)Junk);
  lgEr=ErLink[Card].pEr;

  printf("get original RFpattern - %8X\n", ErLink[Card].pEr->RFpattern);
  printf("get original FracDiv - %8X\n", ErLink[Card].pEr->FracDiv);

  switch (divider) {
    case 0: 
      printf("use card default\n");
      break;
    case 4:
      ErLink[Card].pEr->FracDiv=BEPCII_FRAC_4; 
      ErLink[Card].pEr->uSecDivider = 125;
      ErLink[Card].pEr->CML6Pat00 = 0x000c6318;
      break;
    case 5:
      ErLink[Card].pEr->FracDiv=0x025b41ed; 
      ErLink[Card].pEr->uSecDivider = 100;
      ErLink[Card].pEr->CML6Pat00 = 0x00033333; /* 5 multiplex */
      break;
    case 10:
      ErLink[Card].pEr->FracDiv=0x025b43ad; 
      ErLink[Card].pEr->uSecDivider = 50;
      ErLink[Card].pEr->CML6Pat00 = 0x000aaaaa; /* 10 multiplex */
      break;
  }

  /* to link FPS5 and FPS6 to 0x003f, i.e. tied low */
  ErLink[Card].pEr->FP5Map = 0x003f;
  ErLink[Card].pEr->FP6Map = 0x003f;

  /* CML5 give the event clock rate */
  ErLink[Card].pEr->CML5Pat00 = 0x000003ff;

  /* to enable CML4, CML5, CML6 */
  ErLink[Card].pEr->CML4Ena = 0x00000001;
  ErLink[Card].pEr->CML5Ena = 0x00000001;
  ErLink[Card].pEr->CML6Ena = 0x00000001;

  sysUsDelay(1000000);
  
  printf("get current FracDiv - %8X\n", ErLink[Card].pEr->FracDiv);
  printf("get current uSecDivider - %8X\n", ErLink[Card].pEr->uSecDivider);
  printf("get current FP3Map - %4X\n", ErLink[Card].pEr->FP3Map);
  printf("get current FP4Map - %4X\n", ErLink[Card].pEr->FP4Map);
  printf("get current FP5Map - %4X\n", ErLink[Card].pEr->FP5Map);
  printf("get current FP6Map - %4X\n", ErLink[Card].pEr->FP6Map);

  return(0);
} /* end of ErConfigure230 */


int ErConfigureT(int Card, unsigned long CardAddress, unsigned int IrqVector, 
                unsigned int IrqLevel)
{
  short Junk;

  if (ConfigureLock != 0)
  {
    epicsPrintf("devApsEr: Cannot change configuration after init.  Request ignored\n");
    return(-1);
  }
  if (Card >= NUM_ER_LINKS)
  {
    epicsPrintf("devApsEr: Card number invalid, must be 0-%d\n", NUM_ER_LINKS);
    return(-1);
  }
  if (CardAddress > 0xffff)
  {
    epicsPrintf("devApsEr: Card address invalid, must be 0x0000-0xffff\n");
    return(-1);
  }
  /* CSR handling here */
  vmeCSRWriteADER(Card,0,(CardAddress&0xff00)|0xa4);
  vmeCRSSetIRQLevel(Card,IrqLevel);

  if(sysBusToLocalAdrs(VME_AM_SUP_SHORT_IO, (char*)CardAddress,
                       (char**)&ErLink[Card].pEr)!=OK)
  {
    epicsPrintf("devApsEr: Failure mapping requested A16 address\n");
    ErLink[Card].pEr = NULL;
    return(-1);
  }
  if (vxMemProbe((char*)&ErLink[Card].pEr->Control, READ,
                 sizeof(short), (char*)&Junk) != OK)
  {
    epicsPrintf("devApsEr: Failure probing for event receiver... Card disabled\n");
    ErLink[Card].pEr = NULL;
    return(-1);
  }
  ErLink[Card].IrqVector = IrqVector;
  ErLink[Card].IrqLevel = IrqLevel;
  /*ErLink[Card].pEr->FracDiv=FR_SYNTH_WORD;*/ /* program the correct frequency */
/*  ErLink[Card].pEr->FracDiv=BEPCII_FRAC_4; */

  if (Card >= ErNumLinks){
    ErNumLinks = Card+1;
  }

  /*leige test*/
  printf("ErConfigure: address: %X\n", (int)(&(ErLink[Card].pEr)));
  printf("ErConfigure: address: %X\n", (int)((ErLink[Card].pEr)));
  printf("Junk, %d\n", (int)Junk);
  lgEr=ErLink[Card].pEr;

  printf("get RFpattern - %8X\n", ErLink[Card].pEr->RFpattern);
  printf("get current FracDiv - %8X\n", ErLink[Card].pEr->FracDiv);
  printf("get EVR uSecDivider - %8X\n", ErLink[Card].pEr->uSecDivider);
  /*end of leige test */

  return(0);
} /*end of ErConfigureT 2006 Aug 7, leige */

/**
 *
 * A function to shut up an event receiver in case we get a ^X style reboot.
 *
 **/
static void ErRebootFunc(void)
{
  int	j = 0;

  while (j < ErNumLinks)
  {
    if (ErLink[j].pEr != NULL) {
      ErEnableIrq(ErLink[j].pEr, ER_IRQ_OFF);
    }
    ++j;
  }
  return;
}
/**
 *
 * Register and init the IRQ handlers for each ER card.
 *
 **/
STATIC long ErInitDev(int pass)
{
  int	j;
  int	k;
  static int OneShotFlag = 1;

  if (ErDebug){
    printf("ErInitDev(%d)\n", pass);
  }
  if (OneShotFlag)
  {
    OneShotFlag = 0;
    rebootHookAdd((FUNCPTR)ErRebootFunc);

    ConfigureLock = 1;	/* Prevent any future ErConfigure's */

    /* Clear the record pointers */
    for (j=0;j<ErNumLinks;j++)
    {
      if (ErHaveReceiver(j) >= 0)
      {
        if ((ErLink[j].LinkLock = semBCreate(SEM_Q_PRIORITY,SEM_FULL)) == NULL){
	  return(-1);
	}
        for (k=0;k<NUM_EVENTS;k++)
        {
	  ErLink[j].Card = j;
	  ErLink[j].ErEventTab[k] = 0;
	  /* ErLink[j].EventFunc = NULL; Should be NULL because is static */
	  /* ErLink[j].ErrorFunc = NULL; */
	  scanIoInit(&ErLink[j].IoScanPvt[k]);
        }
        ErLink[j].pRec = NULL;
        ErResetAll(ErLink[j].pEr);
        ErRegisterIrq(&ErLink[j], ErLink[j].IrqVector, ErLink[j].IrqLevel);
      }
    }
  }
  if (pass == 1)
  {
    for (j=0;j<ErNumLinks;j++) {
      if (ErHaveReceiver(j) >= 0)
	{
	 ErEnableIrq(ErLink[j].pEr, ER_IRQ_EVENT);
	}		     		
    }
  }
  return(0);
}
/**
 *
 * Init routine for the ER record type.
 *
 **/
STATIC long init_record(struct erRecord *pRec)
{
  ErLinkStruct  *pLink;

  if (ErDebug){
    printf("devApsEr::init_record() entered\n");
  }
  if (ErHaveReceiver(pRec->out.value.vmeio.card) < 0)
  {
    recGblRecordError(S_db_badField, (void *)pRec, 
		      "devApsEr::ErInitErRec() Invalid card number");
    return(S_db_badField);
  }
  pLink = &ErLink[pRec->out.value.vmeio.card];

  /* Make sure we only have one record for a given ER card */
  if (pLink->pRec != NULL)
  {
    recGblRecordError(S_db_badField, (void *)pRec, 
		      "devApsEr::ErInitErRec() onlyone record allowed per card");
    return(S_db_badField);
  }
  pLink->pRec = pRec;

  proc(pRec);

  return(0);
}
/**
 *
 * Process routine for the ER record type.
 *
 **/
STATIC long proc(struct erRecord  *pRec)
{
  ErLinkStruct  *pLink = &ErLink[pRec->out.value.vmeio.card];

  if (pRec->tpro > 10){
    printf("devApsEr::proc(%s) entered\n", pRec->name);
  }
  /* Make sure the card is present */
  if (ErHaveReceiver(pRec->out.value.vmeio.card) < 0) {
    return(0);
  }
  semTake(pLink->LinkLock, WAIT_FOREVER);

  if (pRec->enab != ErMasterEnableGet(pLink->pEr)) {
    ErMasterEnableSet(pLink->pEr, pRec->enab);
  }
  
  ErSetTrg(pLink->pEr, 0, pRec->trg0);
  ErSetTrg(pLink->pEr, 1, pRec->trg1);
  ErSetTrg(pLink->pEr, 2, pRec->trg2);
  ErSetTrg(pLink->pEr, 3, pRec->trg3);
  ErSetTrg(pLink->pEr, 4, pRec->trg4);
  ErSetTrg(pLink->pEr, 5, pRec->trg5);
  ErSetTrg(pLink->pEr, 6, pRec->trg6);

  /* Set OTP output parameters (enable, width, polarity) 
     for each channel */
  ErSetOtp(pLink->pEr, 0, pRec->otp0, pRec->ot0w, pRec->ot0p,pRec->ot0d);
  ErSetOtp(pLink->pEr, 1, pRec->otp1, pRec->ot1w, pRec->ot1p,pRec->ot1d);
  ErSetOtp(pLink->pEr, 2, pRec->otp2, pRec->ot2w, pRec->ot2p,pRec->ot2d);
  ErSetOtp(pLink->pEr, 3, pRec->otp3, pRec->ot3w, pRec->ot3p,pRec->ot3d);
  ErSetOtp(pLink->pEr, 4, pRec->otp4, pRec->ot4w, pRec->ot4p,pRec->ot4d);
  ErSetOtp(pLink->pEr, 5, pRec->otp5, pRec->ot5w, pRec->ot5p,pRec->ot5d);
  ErSetOtp(pLink->pEr, 6, pRec->otp6, pRec->ot6w, pRec->ot6p,pRec->ot6d);
  ErSetOtp(pLink->pEr, 7, pRec->otp7, pRec->ot7w, pRec->ot7p,pRec->ot7d);
  ErSetOtp(pLink->pEr, 8, pRec->otp8, pRec->ot8w, pRec->ot8p,pRec->ot8d);
  ErSetOtp(pLink->pEr, 9, pRec->otp9, pRec->ot9w, pRec->ot9p,pRec->ot9d);
  ErSetOtp(pLink->pEr, 10, pRec->otpa, pRec->otaw, pRec->otap,pRec->otad);
  ErSetOtp(pLink->pEr, 11, pRec->otpb, pRec->otbw, pRec->otbp,pRec->otbd);
  ErSetOtp(pLink->pEr, 12, pRec->otpc, pRec->otcw, pRec->otcp,pRec->otcd);
  ErSetOtp(pLink->pEr, 13, pRec->otpd, pRec->otdw, pRec->otdp,pRec->otdd);

  /* OTnB distributed bus enables */
  ErSetOtb(pLink->pEr, 0, pRec->ot0b);
  ErSetOtb(pLink->pEr, 1, pRec->ot1b);
  ErSetOtb(pLink->pEr, 2, pRec->ot2b);
  ErSetOtb(pLink->pEr, 3, pRec->ot3b);
  ErSetOtb(pLink->pEr, 4, pRec->ot4b);
  ErSetOtb(pLink->pEr, 5, pRec->ot5b);
  ErSetOtb(pLink->pEr, 6, pRec->ot6b);
  ErSetOtb(pLink->pEr, 7, pRec->ot7b);
  pRec->fpgv = ErGetFpgaVersion(pLink->pEr);
  pRec->rxvc = pLink->RxvioCount;
 
  /* OTL (level output) enables */
  ErSetOtl(pLink->pEr, 0, pRec->otl0);
  ErSetOtl(pLink->pEr, 1, pRec->otl1);
  ErSetOtl(pLink->pEr, 2, pRec->otl2);
  ErSetOtl(pLink->pEr, 3, pRec->otl3);
  ErSetOtl(pLink->pEr, 4, pRec->otl4);
  ErSetOtl(pLink->pEr, 5, pRec->otl5);
  ErSetOtl(pLink->pEr, 6, pRec->otl6);

  /* delay generator channel parameters */
  ErSetDg(pLink->pEr, 0, pRec->dg0e, pRec->dg0d, pRec->dg0w, pRec->dg0c, pRec->dg0p);
  ErSetDg(pLink->pEr, 1, pRec->dg1e, pRec->dg1d, pRec->dg1w, pRec->dg1c, pRec->dg1p);
  ErSetDg(pLink->pEr, 2, pRec->dg2e, pRec->dg2d, pRec->dg2w, pRec->dg2c, pRec->dg2p);
  ErSetDg(pLink->pEr, 3, pRec->dg3e, pRec->dg3d, pRec->dg3w, pRec->dg3c, pRec->dg3p);
  ErSetDVME(pLink->pEr, pRec->dvme, pRec->dvmd, pRec->dvmc);

  /* front panel output configuration */

  ErSetFPMap(pLink->pEr,0,pRec->fps0);
  ErSetFPMap(pLink->pEr,1,pRec->fps1);
  ErSetFPMap(pLink->pEr,2,pRec->fps2);
  ErSetFPMap(pLink->pEr,3,pRec->fps3);
  ErSetFPMap(pLink->pEr,4,pRec->fps4);
  ErSetFPMap(pLink->pEr,5,pRec->fps5);
  ErSetFPMap(pLink->pEr,6,pRec->fps6);

 /******************************************************************************/

  /* Enable/Disable Interrupts */
  (pRec->rxve) ? ErEnableIrq(pLink->pEr, ErEnableIrq(pLink->pEr,
  ER_IRQ_TELL)|ER_IRQ_TAXI) : ErEnableIrq(pLink->pEr, ErEnableIrq(pLink->pEr, ER_IRQ_TELL)&~ER_IRQ_TAXI);
 
  if(pLink->pEr->Control & 0x0008) 
  			(pRec->plok)=0; 
  		else 
			(pRec->plok)=1;
  
  if (ErCheckTaxi(pLink->pEr) != 0)
    {
      pRec->taxi = 1;
    }
  else 
    {
      pRec->taxi = 0;
    }
  semGive(pLink->LinkLock);

  pRec->udf = 0;
  return(0);
}

/**
 *
 * Init routine for the EREVENT record type.
 *
 **/
STATIC long ErEventInitRec(struct ereventRecord *pRec)
{
  if (ErDebug){
    printf("ErEventInitRec(%s)\n", pRec->name);
  }
  if (ErHaveReceiver(pRec->out.value.vmeio.card) < 0)
  {
    recGblRecordError(S_db_badField, (void *)pRec, 
		      "devApsEr::ErEventInitRec() invalid card number in OUT field");
    return(S_db_badField);
  }
  if ((pRec->out.value.vmeio.signal < 0)||(pRec->out.value.vmeio.signal > 255))
  {
    recGblRecordError(S_db_badField, (void *)pRec, 
		      "devApsEr::ErEventInitRec() invalid signal number in OUT field");
    return(S_db_badField);
  }

  pRec->lenm = 300;	/* Force setting on first process */
  pRec->lout = 0;	/* Clear the 'last' event mask */
  return(0);
}
/**
 *
 * Process routine for the EREVENT record type.
 *
 **/
STATIC long ErEventProc(struct ereventRecord  *pRec)
{

  ErLinkStruct  *pLink = &ErLink[pRec->out.value.vmeio.card];
  int		DownLoadRam = 0;
  unsigned 	short Mask;
  
  if (pRec->tpro > 10){
    printf("ErEventProc(%s) entered\n", pRec->name);
  }
  /* Make sure the card is present */
  if (ErHaveReceiver(pRec->out.value.vmeio.card) < 0){
    return(0);
  }
  if (pRec->enab != 0)
  {
    if (pRec->tpro > 10){
      printf("ErEventProc(%s) enable=true\n", pRec->name);
    }
    semTake(pLink->LinkLock, WAIT_FOREVER);
    if (pRec->enm != pRec->lenm)
    {
      if (pRec->tpro > 10) {
	printf("ErEventProc(%s) event number changed %ld-%ld\n", 
	       pRec->name, pRec->lenm, pRec->enm);
      }
      /* Transfer the event info to new position */ 
      if ((pRec->lenm < 256) && (pRec->lenm >= 0)) {
        pLink->ErEventTab[pRec->lenm] = 0;
      }
      DownLoadRam = 1;
      pRec->lenm = pRec->enm;
    }
    /* Build event mask */
    /* NOTE: that we never latch the time in the 32 bit latch */

    Mask = 0;
    if (pRec->out0 != 0){ Mask |= 0x0001;}
    if (pRec->out1 != 0){ Mask |= 0x0002;}
    if (pRec->out2 != 0){ Mask |= 0x0004;}
    if (pRec->out3 != 0){ Mask |= 0x0008;}
    if (pRec->out4 != 0){ Mask |= 0x0010;}
    if (pRec->out5 != 0){ Mask |= 0x0020;}
    if (pRec->out6 != 0){ Mask |= 0x0040;}
    if (pRec->out7 != 0){ Mask |= 0x0080;}
    if (pRec->out8 != 0){ Mask |= 0x0100;}
    if (pRec->out9 != 0){ Mask |= 0x0200;}
    if (pRec->outa != 0){ Mask |= 0x0400;}
    if (pRec->outb != 0){ Mask |= 0x0800;}
    if (pRec->outc != 0){ Mask |= 0x1000;}
    if (pRec->outd != 0){ Mask |= 0x2000;}
    if (pRec->vme != 0) { Mask |= 0x8000;}

    if (pRec->tpro > 10){
      printf("ErEventProc(%s) New RAM mask is 0x%4.4X\n", pRec->name, Mask);
    }
    if (Mask != pRec->lout) 
      {
	DownLoadRam = 1;
      }
    if (DownLoadRam != 0)
    {
      if (pRec->tpro > 10) {
	printf("ErEventProc(%s) updating evt RAM \n", pRec->name);
      }
      if (pRec->enm != 0)
      {
        pLink->ErEventTab[pRec->enm] = Mask;
        pRec->lout = Mask;
      }
      ErUpdateRam(pLink->pEr, pLink->ErEventTab);
    }
    if (pRec->tpro > 10){
      printf("ErEventProc(%s) I/O operations complete\n", pRec->name);
    }
    semGive(pLink->LinkLock);
  }
  else
  {
    /* BUG kill event if was ever enabled! */
  }

  return(0);
}

/******************************************************************************/

ErDsetStruct devErevent={ 5, NULL, NULL, ErEventInitRec, NULL, ErEventProc};
/**
 *
 * Routine to initialize an event record.  This is a regular EPICS event 
 * record!! Not an ER or EG event record.  We allow the user to use the
 * regular epics event records to request processing on an event that is
 * received from the event receiver.
 *
 * All we gotta do here is set the dpvt field to point to the proper 
 * IOSCANEVENT field so that we can later return it on getIoScanInfo() calls.
 *
 **/
STATIC long ErEpicsEventInit(struct eventRecord *pRec)
{
  pRec->dpvt = NULL;	/* In case problems arise below */

  if(ErDebug) {
    printf("ErEpicsEventInit(%s) %d %d\n", pRec->name, 
	   pRec->inp.value.vmeio.card, pRec->inp.value.vmeio.signal);
  }
  if (ErHaveReceiver(pRec->inp.value.vmeio.card) < 0)
  {
    recGblRecordError(S_db_badField, (void *)pRec, 
		      "devApsEr::ErEpicsEventInitRec() invalid card number in INP field");
    return(S_db_badField);
  }
  if ((pRec->inp.value.vmeio.signal < 0)||(pRec->inp.value.vmeio.signal > NUM_EVENTS))
  {
    recGblRecordError(S_db_badField, (void *)pRec, 
		      "devApsEr::ErEpicsEventInitRec() invalid signal number in INP field");
    return(S_db_badField);
  }

  pRec->dpvt = &ErLink[pRec->inp.value.vmeio.card].IoScanPvt[pRec->inp.value.vmeio.signal];
  return(0);
}
/**
 *
 * Routine to get the IOSCANPVT value for an EVENT record type.
 *
 **/
STATIC long ErEpicsEventGetIoScan(int cmd, struct eventRecord *pRec, 
				  IOSCANPVT *pPvt)
{
  *pPvt = *((IOSCANPVT*)(pRec->dpvt));
  return(0);
}
ErDsetStruct devErEpicsEvent={5, NULL, NULL, ErEpicsEventInit, ErEpicsEventGetIoScan, NULL};

/**
 *
 * Inquire as to if the card is enabled or not.
 *
 **/
STATIC long ErMasterEnableGet(MrfErStruct *pParm)
{
  volatile MrfErStruct  *pEr = pParm;
  return((pEr->Control & 0x8000) != 0);
}
/**
 *
 * Enable/disable the card.
 *
 **/
STATIC long ErMasterEnableSet(MrfErStruct *pParm, int Enable)
{
  volatile MrfErStruct  *pEr = pParm;

  if (Enable != 0) {
    pEr->Control = (pEr->Control&CONTROL_REG_OR_STRIP)|0x8000;
  }
  else {
    pEr->Control = (pEr->Control&CONTROL_REG_OR_STRIP)&~0x8000;
  }
  return(0);
}
/**
 *
 * Check to see if we have a taxi violation and reset it if so.
 *
 **/
STATIC long ErCheckTaxi(MrfErStruct *pParm)
{
  volatile MrfErStruct  *pEr = pParm;

  if (pEr->Control & 0x0001)
  {
    pEr->Control = (pEr->Control&CONTROL_REG_OR_STRIP)|0x0001;
    return(1);
  }
  return(0);
}
/**
 *
 * Set/clear an enable bit in the trigger mask.
 *
 **/
STATIC long ErSetTrg(MrfErStruct *pParm, int Channel, int Enable)
{
  volatile MrfErStruct  *pEr = pParm;
  unsigned short	mask;

  mask = 1;
  mask <<= Channel;

  if (Enable != 0){
    pEr->TriggerEventEnables |= mask;
  }  else {
    pEr->TriggerEventEnables &= ~mask;
  }
  return(0);
}
/**
 *
 * Set/clear an enable bit in the one-shot mask.
 *
 **/
STATIC long ErSetOtp(MrfErStruct *pParm, int Channel, int Enable, 
		     unsigned short Width, unsigned short Pol, unsigned int Delay)
{
  volatile MrfErStruct  *pEr = pParm;
  unsigned short	mask;
  unsigned int          polMask;

  mask = 1;
  mask <<= Channel;
  /* set the pulse width */
  pEr->DelayPulseSelect = Channel | 0x10;
  pEr->PulseWidth = Width;
  pEr->ExtDelay = Delay;
  polMask = (ER_OT0_POL << Channel);
  if (Pol==0) {
    pEr->OutputPol = (pEr->OutputPol & ~polMask); /*reset the corresponding bit */
  } else {
   /*epicsPrintf ("set polarity reg: 0x%x\n",polMask | pEr->OutputPol);*/
   pEr->OutputPol = polMask | pEr->OutputPol; /*set the bit*/
   /*epicsPrintf ("polarity reg val: 0x%x\n", pEr->OutputPol);*/
  }
  if(ErDebug >9){
    epicsPrintf("devApsEr: Reg addr %x, set to %x\n", 
		pEr->DelayPulseSelect,pEr->PulseWidth);
  }
  if (Enable != 0){
    pEr->OutputPulseEnables |= mask;
  } else {
    pEr->OutputPulseEnables &= ~mask;
  }
  return(0);
}
/**
 *
 * Set/clear an enable bit in the level-trigger mask.
 *
 **/
STATIC long ErSetOtl(MrfErStruct *pParm, int Channel, int Enable)
{
  volatile MrfErStruct  *pEr = pParm;
  unsigned short	mask;

  mask = 1;
  mask <<= Channel;

  if (Enable != 0) {
    pEr->OutputLevelEnables |= mask;
  } else {
    pEr->OutputLevelEnables &= ~mask;
  }
  return(0);
}
/**
 *
 * Enable/Disable distributed bus channels on the output.
 *
 **/
STATIC long ErSetOtb(MrfErStruct *pParm, int Channel, int Enable)
{
  volatile MrfErStruct  *pEr = pParm;
  unsigned short	mask;

  mask = 1;
  mask <<= Channel;

  if (Enable != 0) {
    pEr->DBusEnables |= mask;
  } else {
    pEr->DBusEnables &= ~mask;
  }
  return(0);
}
/**
 *
 *  get FPGA version 
 **/

STATIC long ErGetFpgaVersion(MrfErStruct *pParm)
{
  volatile MrfErStruct *pEr = pParm;

  return(pEr -> FPGAVersion & 0x0fff);
}

/**
 *
 * Set the tick counter prescaler (0=count '124' events, other value= value
 * to divide the internal clock with.)
 *
 **/
STATIC long ErSetTickPre(MrfErStruct *pParm, int ScalerVal)
{
  volatile MrfErStruct  *pEr = pParm;

  if (!(ScalerVal >= 0 || ScalerVal < 65535)) 
    {
      return (-1);
    } else {
      pEr->EventCounterClock = ScalerVal;
    }
  return(0);
}
/**
 *
 * Set the front panel output map
 *
 **/

STATIC long ErSetFPMap(MrfErStruct *pEvr, int channel, int map)
{
  volatile MrfErStruct *pEr = pEvr;

  if (channel < 0 || channel > 6)
    return ERROR;

  if (map < 0 || map > EVR_FPMAP_MAX)
    return ERROR;

  switch (channel)
    {
    case 0:
      pEr->FP0Map = map;
      break;
    case 1:
      pEr->FP1Map = map;
      break;
    case 2:
      pEr->FP2Map = map;
      break;
    case 3:
      pEr->FP3Map = map;
      break;
    case 4:
      pEr->FP4Map = map;
      break;
    case 5:
      pEr->FP5Map = map;
      break;
    case 6:
      pEr->FP6Map = map;
      break;
    }

  return OK;
}

/**
 *
 * Set/clear an enable bit in the programmable pulse delay mask.  If enabling
 * a channel, the delay and width are also set.
 *
 **/
STATIC long ErSetDg(MrfErStruct *pParm, int Channel, int Enable, unsigned int Delay, 
		    unsigned int Width, unsigned short Prescaler, unsigned short Pol)
{
  volatile MrfErStruct  *pEr = pParm;
  unsigned short	mask, polMask;

  mask = 1;
  mask <<= Channel;
  polMask = 1;
  polMask <<= Channel+4;

  if (Enable != 0)
  {
    pEr->DelayPulseSelect = Channel;
    pEr->ExtDelay = Delay;
    pEr->ExtWidth = Width;
    pEr->DelayPrescaler = Prescaler;
    pEr->DelayPulseEnables |= mask;
    if (Pol != 0){
      pEr->DelayPulseEnables |= polMask;
    } else {
      pEr->DelayPulseEnables &= ~polMask;
    }
  }
  else {
    pEr->DelayPulseEnables &= ~mask;
  }
  return(0);
}
/**
 *
 * Set/clear an enable bit for the delayed VME interrupt.  If enabling,
 * the delay and prescaler are also set.
 *
 **/
STATIC long ErSetDVME(MrfErStruct *pParm, int Enable, 
		      unsigned short Delay, unsigned short Prescaler)
{
  volatile MrfErStruct  *pEr = pParm;
  unsigned short	mask;

  mask = 0x10;

  if (Enable != 0)
  {
    pEr->DelayPulseSelect = 0x4; /*select DIRQ settings */
    pEr->PulseDelay = Delay;
    pEr->DelayPrescaler = Prescaler;
    pEr->IrqEnables |= mask;
  }
  else {
    pEr->IrqEnables &= ~mask;
  }
  return(0);
}
/**
 *
 * A debugging aid that dumps out all the regs on the ER board.
 *
 **/
STATIC int ErDumpRegs(MrfErStruct *pParm)
{
  volatile MrfErStruct	*pEr = pParm;

  printf("Control reg:    %4.4X\n", pEr->Control);
  printf("RAM addr&data:  %4.4X %4.4X\n", pEr->EventRamAddr, 
	 pEr->EventRamData);
  printf("OTP, TEV, LVL:  %4.4X %4.4X %4.4X\n",
	 pEr->OutputPulseEnables, pEr->OutputLevelEnables, 
	 pEr->TriggerEventEnables);
  printf("PDP enab:       %4.4X\n", pEr->DelayPulseEnables);
  printf("Evt ctr hi,lo:  %4.4X %4.4X \n",
        pEr->EventCounterHi, pEr->EventCounterLo);
  printf("TS ctr hi, lo:  %4.4X %4.4X \n", pEr->TimeStampLo, pEr->TimeStampHi);
  printf("Evt fifo,time:  %4.4X %4.4X \n", pEr->EventFifo, pEr->EventTimeHi);
  printf("PDP sel, dly,w: %4.4X %4.4X %4.4X \n",
        pEr->DelayPulseSelect, pEr->PulseDelay, pEr->PulseWidth);
  printf("PDP prescaler:  %4.4X \n", pEr->DelayPrescaler);
  printf("EVT ctr presc:  %4.4X \n", pEr->EventCounterClock);
  printf("IRQ vec, enab:  %4.4X %4.4X \n", pEr->IrqVector, pEr->IrqEnables);
  printf("DB enab, data:  %4.4X %4.4X \n", pEr->DBusEnables, pEr->DBusData);
  printf("FPGA version :  %4.4X\n", pEr->FPGAVersion);

  printf("Card status info: \n");
  if(pEr->Control&0x8000) {
    printf("*Card enabled \n") ;
  } else {
    printf("*Card disabled \n");
  }
  if(pEr->Control&0x4000) {
    printf("*IRQs enabled \n") ;
  } else {
    printf("*IRQs disabled \n");
  }
  if(pEr->Control&0x1000) {
    printf("*Heartbeat lost \n") ;
  } else {
    printf("*Heartbeat OK \n");
  }
  if(pEr->Control&0x0800) {
    printf("*Interrupt arrived \n") ;
  } else {
    printf("*No interrupt \n");
  }
  if(pEr->Control&0x0100) {
    printf("*RAM 2 selected \n") ;
  } else {
    printf("*RAM 1 selected \n");
  }
  if(pEr->Control&0x0200) {
    printf("*RAM enabled \n") ;
  } else {
    printf("*RAM disabled \n");
  }
  if(pEr->Control&0x0040) {
    printf("*RAM 2 selected for VME access \n") ;
  } else { 
    printf("*RAM 1 selected for VME access \n");
  }
  if(pEr->Control&0x0008) {
    printf("*Datastream sync (was) lost \n") ;
  } else {
    printf("*Datastream sync OK \n");
  }
  if(pEr->Control&0x0001) {
    printf("*RXVIO detected \n") ;
  } else {
    printf("*RXVIO clear \n");
  }
  if(pEr->Control&0x0004) {
    printf("*event FIFO (was) full \n") ;
  } else {
    printf("*event FIFO ok \n");
  }
  printf("--\n");
  return(0);
}

/**
 *
 * Reset, clear and disable everything on the board.
 *
 **/
STATIC int ErResetAll(MrfErStruct *pParm)
{
  volatile MrfErStruct  *pEr = pParm;

  pEr->Control = 0x201d;
  pEr->Control = 0x208d;

/*  
while(pEr->Control & 0x0080) {
    taskDelay(1);
  }
*/  
  pEr->OutputPulseEnables = 0;
  pEr->OutputLevelEnables = 0;
  pEr->DelayPulseEnables = 0;
  pEr->DelayPulseSelect = 0;
  pEr->IrqEnables = 0;

  pEr->Control = (pEr->Control&CONTROL_REG_OR_STRIP)|0x8000;

  return(0);
}
/**
 *
 * Receive a hardware IRQ from an ER board.
 *
 * BUG -- should add errMessage calls for problem events & include storm
 *        control on them.
 *
 **/
STATIC void ErIrqHandler(ErLinkStruct *pLink)
{
  volatile MrfErStruct  *pEr = pLink->pEr;
  volatile erRecord  	*pRec = pLink->pRec;
  unsigned short	mask;
  unsigned short	j;
  unsigned short	k;
  short			z;
  unsigned long		Time;

  mask = pEr->Control;
  if(ErDebug > 9) {
    /* epicsPrintf("ErIrqHandler() control =%4.4X\n", mask); */
    logMsg("ErIrqHandler() control =%4.4X\n", mask,0,0,0,0,0); 
  }
  
  if (mask&0x0001)
  {
    pEr->Control = (mask&CONTROL_REG_OR_STRIP)|0x0001;

    if (pLink->ErrorFunc != NULL) {
		func(pLink->Card, ERROR_TAXI);
        if(mask&0x0008)	{					/* PLL lost, means fiber optic link disconnected */
      		(*pLink->ErrorFunc)(pLink->Card, ERROR_TAXI);	/* This is where drvTS intr.  called */
		pEr->Control = (mask&CONTROL_REG_OR_STRIP)|0x0008;
       		pEr->IrqEnables = ~(ER_IRQ_TAXI) & pEr->IrqEnables; 
		pRec->rxve = 0; 				/* Indiciation of RXVE disable to record */
      }
    }
    if(ErDebug){
     logMsg("ErIrqHandler() taxi violation %4.4X\n", 
		  pEr->Control = (mask&CONTROL_REG_OR_STRIP)|0x0001,0,0,0,0,0); 
    }
  }
  
  if (mask&0x1000)
  { /* Got a lost heart beat */
    pEr->Control = (mask&CONTROL_REG_OR_STRIP)|0x1000;

    if (pLink->ErrorFunc != NULL){
     (*pLink->ErrorFunc)(pLink->Card, ERROR_HEART);
    }
    else 
    if(ErDebug)	logMsg("ErrorFunc is NULL ....\n",0,0,0,0,0,0);
    if(ErDebug){
      logMsg("ErIrqHandler() lost heart beat\n",0,0,0,0,0,0); 
    }
  }

  /* Read any stuff from the FIFO */
  if (mask&0x0800)
  {
    pEr->Control = (mask&CONTROL_REG_OR_STRIP)|0x0800;
    if(ErDebug > 10){
     logMsg("ErIrqHandler() clearing event with %4.4X\n", 
		  (mask&CONTROL_REG_OR_STRIP)|0x0800,0,0,0,0,0); 
    }
    z = 10;	/* Make sure we don't get into a major spin loop */
    while ((pEr->Control & 0x0002)&&(--z > 0))
    {
      unsigned short	EventNum;

      j = pEr->EventFifo;
      k = pEr->EventTimeHi;

      /* Mark the event time in the proper array place */
      EventNum = j & 0x00ff;
      Time = (k<<8)|(j>>8);

      if (pLink->EventFunc != NULL)
      {
	(*pLink->EventFunc)(pLink->Card, EventNum, Time);
        if(ErDebug){
        logMsg("ErIrqHandler() Calling %p", (int)pLink->EventFunc,0,0,0,0,0); 
	}
      }

      if(ErDebug){
        logMsg("ErIrqHandler() %6.6X-%2.2X\n", Time, EventNum,0,0,0,0); 
      }
      /* Schedule processing for any event-driven records */
      scanIoRequest(pLink->IoScanPvt[EventNum]);
    }
  }
  /* Kill any fifo overflow status */
  if (mask&0x0004)
  {
    pEr->Control = (mask&CONTROL_REG_OR_STRIP)|0x0004;

    if (pLink->ErrorFunc != NULL) {
      (*pLink->ErrorFunc)(pLink->Card, ERROR_LOST);
    }
    if(ErDebug){
    logMsg("ErIrqHandler() FIFO overflow %4.4X\n", 
		  (mask&CONTROL_REG_OR_STRIP)|0x0004,0,0,0,0,0); 
    }
  }

  if (mask&0x0010) /* delayed IRQs */
  {
    pEr->Control = (mask&CONTROL_REG_OR_STRIP)|0x0002;
    if(ErDebug > 10) {
     logMsg("ErIrqHandler() got delayed IRQ with %4.4X\n", 
		  (mask&CONTROL_REG_OR_STRIP)|0x0002,0,0,0,0,0); 
    }    
    scanIoRequest(pLink->IoScanPvt[256]); /*special event*/
  }
  /*
   * Disable and then re-enable the IRQs so board re-issues any 
   * that are still pending
   */

  pEr->Control = (mask&CONTROL_REG_OR_STRIP)&~0x4000;
  pEr->Control = (mask&CONTROL_REG_OR_STRIP)| 0x4000;

  return;
}

/**
 *
 * Set up the IRQs for the receiver.
 * (Should only be called once.)
 *
 **/
STATIC int ErRegisterIrq(ErLinkStruct *pLink, int IrqVector, int IrqLevel)
{
  volatile MrfErStruct  *pEr = pLink->pEr;

  if (IrqVector != -1)
  {
    intConnect(INUM_TO_IVEC(IrqVector), ErIrqHandler, (int)pLink);
    sysIntEnable(IrqLevel);
    pEr->IrqVector = IrqVector;
  }
  return(0);
}

/**
 *
 * Enable or disable the IRQs from the Event Receiver.
 *
 **/
STATIC unsigned short ErEnableIrq(MrfErStruct *pParm, unsigned short Mask)
{
  volatile MrfErStruct  *pEr = pParm;
  if (Mask == ER_IRQ_OFF)
  {
    pEr->IrqEnables = 0;
    pEr->Control = (pEr->Control&CONTROL_REG_OR_STRIP) & ~0x4000;
  }
  else if (Mask == ER_IRQ_TELL)
  {
    if (pEr->Control & 0x4000) {
      return(pEr->IrqEnables & ER_IRQ_ALL);
    }
  }
  else
  {
    pEr->IrqEnables = Mask;
    pEr->Control = (pEr->Control&CONTROL_REG_OR_STRIP)|0x4000;
  }

  return(0);
}

/**
 *
 * We load what ever RAM is not running. And Enable it.
 *
 **/
STATIC int ErUpdateRam(MrfErStruct *pParm, unsigned short *RamBuf)
{
  int	ChosenRam = 2;

  /* Find the idle RAM */
  if (ErGetRamStatus(pParm, 1) == 0)
    {
      ChosenRam = 1;	/* RAM 1 is currently idle */
    }
  ErProgramRam(pParm, RamBuf, ChosenRam);

  ErEnableRam(pParm, ChosenRam);
  return(0);
}
/**
 *
 * Download the entire RAM from a buffer.
 *
 **/
STATIC int ErProgramRam(MrfErStruct *pParm, unsigned short *RamBuf, int Ram)
{
  volatile MrfErStruct  *pEr = pParm;
  int			j;

  if(ErDebug){
    printf("RAM download for RAM %d\n", Ram);
  }
  if (Ram == 1)
  {
    if ((pEr->Control&0x0300) == 0x0200) {
      return(-1);		/* RAM 1 is enabled */
    }
    /* select VME access to ram 1 */
    pEr->Control = (pEr->Control&CONTROL_REG_OR_STRIP) & ~0x0040;	
  }
  else
  if (Ram == 2)
  {
    if ((pEr->Control&0x0300) == 0x0300){
      return(-1);		/* RAM 2 is enabled */
    }
    /* select VME access to ram 2 */
    pEr->Control = (pEr->Control&CONTROL_REG_OR_STRIP)|0x0040;	
  }
  else {
    return(-1);
  }
  /* Reset RAM address */
  pEr->Control = (pEr->Control&CONTROL_REG_OR_STRIP)|0x0010;

  /* enable auto increment */
  /*pEr->Control = (pEr->Control&CONTROL_REG_OR_STRIP)|0x0020;	*/

  pEr->EventRamAddr = 0;

  for (j=0; j<256; j++)
  {
    pEr->EventRamAddr=j;
    pEr->EventRamData = RamBuf[j];
    if((ErDebug)&&(RamBuf[j] != 0)) {
      printf("%d: write %d  read %d \n", j, RamBuf[j],pEr->EventRamData);
    }
  }
  pEr->EventRamAddr = 0;

  /* Turn off auto-increment */
  pEr->Control = (pEr->Control&CONTROL_REG_OR_STRIP) & ~0x0020;	
  return(0);
}

/**
 *
 * Print the entire RAM from a buffer.  We skip the null events.
 *
 **/
STATIC int ErDumpRam(MrfErStruct *pParm, int Ram)
{
  volatile MrfErStruct  *pEr = pParm;
  int			j;
  int			x, y;

  if (Ram == 1)
  {
    if ((pEr->Control&0x0300) == 0x0200) {
      printf("RAM %d is enabled!\n",Ram);
      /*return(-1);*/		/* RAM 1 is enabled */
    }
    /* select VME access to ram 1 */
    pEr->Control = (pEr->Control&CONTROL_REG_OR_STRIP) & ~0x0040;	
  }
  else
    {
      if (Ram == 2)
	{
	  if ((pEr->Control&0x0300) == 0x0300) {
	    printf("RAM %d is enabled!\n",Ram);
	    /*return(-1);*/		/* RAM 2 is enabled */
	  }
	  /* select VME access to ram 2 */
	  pEr->Control = (pEr->Control&CONTROL_REG_OR_STRIP)|0x0040;	
	}
      else
	{
	  return(-1);
	}
    }
  /* Reset RAM address */
  pEr->Control = (pEr->Control&CONTROL_REG_OR_STRIP)|0x0010;

  /* enable auto increment */
  pEr->Control = (pEr->Control&CONTROL_REG_OR_STRIP)|0x0020;	

  pEr->EventRamAddr = 0;

  for (j=0; j<256; j++)
  {
    x = pEr->EventRamAddr & 0x00ff;
    y = pEr->EventRamData;
    if (y != 0)
      {
	printf("RAM 0x%2.2X (=%3d), 0x%4.4X\n", x, x, y);
      }
  }

  /* turn off auto-increment */
  pEr->Control = (pEr->Control&CONTROL_REG_OR_STRIP) & ~0x0020;	
  
  return(0);
}

/**
 *
 * Select the specified RAM for operation
 *
 **/
STATIC int ErEnableRam(MrfErStruct *pParm, int Ram)
{
  volatile MrfErStruct  *pEr = pParm;

  if (Ram == 0)
  {
    pEr->Control = (pEr->Control&CONTROL_REG_OR_STRIP) & ~0x0200;
    return(0);
  }
  if (Ram == 1) {
    pEr->Control = (pEr->Control&CONTROL_REG_OR_STRIP) & ~0x0100;
  }
  else if (Ram == 2) {
    pEr->Control = (pEr->Control&CONTROL_REG_OR_STRIP) | 0x0100;
  }
  else {
    return(-1);
  }
  /* Set the map enable bit */
  pEr->Control = (pEr->Control&CONTROL_REG_OR_STRIP) | 0x0200;	

  return(0);
}
/**
 *
 * Return 1 if the requested RAM is enabled, 0 otherwise.  We return -1 on
 * error.
 *
 **/
STATIC int ErGetRamStatus(MrfErStruct *pParm, int Ram)
{
  volatile MrfErStruct  *pEr = pParm;

  /* Check if RAM map is not enabled */
  if ((pEr->Control & 0x0200) == 0) {
    return(0);
  }
  if (Ram == 1) {
    return((pEr->Control & 0x0100) == 0);
  }
  if (Ram == 2) {
    return((pEr->Control & 0x0100) != 0);
  }
  return(-1);	/* We should never get here, invalid RAM number */
}


/**
 *
 * Restart EVR after reset (or live insertion.)
 *
 **/
STATIC int ErRestart(ErLinkStruct *pLink)
{
  volatile MrfErStruct  *pEr = pLink->pEr;
  pEr->IrqVector = pLink->IrqVector;
  ErEnableIrq(pLink->pEr, ER_IRQ_EVENT);
  return(0);
}
/**
 *  Dump the contents of the event FIFO to terminal. Will empty the FIFO
 **/
static int DumpEventFifo(MrfErStruct *pParm)
{
  unsigned short        j;
  unsigned short        k;
  volatile MrfErStruct  *pEr = pParm;

  while (pEr->Control & 0x0002)
  {
    j = pEr->EventFifo;
    k = pEr->EventTimeHi;
    printf("FIFO event: %4.4X%4.4X\n", k, j);
  }
  return(0);
}
/**
 *
 * Screen menu for the simple debugger
 *
 **/

static void MainMenu(void)
{
  printf("r - Reset everything on the event receiver\n");
  printf("f - drain the event FIFO\n");
  printf("1 - dump event RAM 1\n");
  printf("2 - dump event RAM 2\n");
  printf("z - Dump event receiver regs\n");
  printf("a - Enable RAM 1\n");
  printf("b - Enable RAM 2\n");
  printf("d - Disable RAMs\n");
  printf("s - Show RXVIO counter and clear\n");
  printf("v - Clear RXVIO\n");

  printf("q - quit\n");
}
/**
 *simple interactive debugging shell
 **/
int ER(void)
{
  static int	Card = 0;
  char 		buf[100];

  printf("Event receiver testerizer\n");
  printf("Enter Card number [%d]:", Card);
  fgets(buf, sizeof(buf), stdin);
  sscanf(buf, "%d", &Card);

  if (ErHaveReceiver(Card) < 0)
  {
    printf("Invalid card number specified\n");
    return(-1);
  }
  printf("Card address: %p\n", ErLink[Card].pEr);

  while(1)
  {
    MainMenu();

    if (fgets(buf, sizeof(buf), stdin) == NULL)
      break;

    switch (buf[0])
    {
    case 'r': ErResetAll(ErLink[Card].pEr); break;
    case 'a': ErEnableRam(ErLink[Card].pEr, 1); break;
    case 'b': ErEnableRam(ErLink[Card].pEr, 2); break;
    case 'd': ErEnableRam(ErLink[Card].pEr, 0); break;
    case 'f': DumpEventFifo(ErLink[Card].pEr); break;
    case '1': ErDumpRam(ErLink[Card].pEr, 1); break;
    case '2': ErDumpRam(ErLink[Card].pEr, 2); break;
    case 'z': ErDumpRegs(ErLink[Card].pEr); break;
    case 'v': ErCheckTaxi(ErLink[Card].pEr); break;
    case 's': printf("RXVIO_COUNT=%d\n",ErLink[Card].RxvioCount);
    	      ErLink[Card].RxvioCount=0 ; break;
    case 'q': return(0);
    default:break;
    }
  }
  return(0);
}

/**
 *
 * Error function
 *
 **/

void func(int Card, int ErrorNum)
{
	if(ErrorNum==ERROR_TAXI) ErLink[Card].RxvioCount++;
	
	if(ErDebug) logMsg("Card# %d has Error# %d, Error repeatation# %d....\n"
	, Card,ErrorNum,ErLink[Card].RxvioCount,0,0,0);

	return;
} 
