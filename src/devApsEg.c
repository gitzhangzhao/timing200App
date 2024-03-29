/* $Author: leige $ */
/* $Date: 2006/10/17 00:58:05 $ */
/* $Id: devApsEg.c,v 1.1.1.1 2006/10/17 00:58:05 leige Exp $ */
/* $Name:  $ */
/* $Revision: 1.1.1.1 $  */
/* EgConfigure1 by leige for BEPCII, 2015 */
/* EgConfigure230 by leige for BEPCII, 2017-12-30 */
/* EgConfigureA24 by leige for HEPS, 2019-02-25 */

/* Modification: */
/* B. Kalantari */
/* Date: 2003/02/12 */

/*#define EG_DEBUG */
/*leige use EG_DEBUG, and define it in egDefs.h */


/*
 *      Author:		John R. Winans
 *      Date:   	07-21-93
 *
 * PROBLEMS:
 * -When transitioning from ALTERNATE mode to anything else, the mode MUST
 *  be set to OFF long enough to clear/reload the RAMS first.
 * -In SINGLE mode, to rearm, the mode must be set to OFF, then back to SINGLE.
 *
 * Modification Log:
 * -----------------
 * By : Timo Korhonen
 * New version for the upgraded (Micro-Research) event system.
 * Includes clock prescaler settings
 *          RAM size to 512K
 *          distributed bus setup
 *          ...
 *
 * By : Babak Kalantari
 * New version for the new (Micro-Research) event system, EVG-110 plus.
 * supports for: AC line input sequence trigger
 *          	 Multiplexed counter registers
 *          	 Distributed bus ...
 *          
 *          
 * By : Babak Kalantari
 * Support for version 3.0 (EVG firmware 2.03) 
 * Added features: Phase shifer for AC input and 32-bit multiplexed counters
 *
 * By : Timo Korhonen
 * New version for the series 200 event system.
 * 
 *          new RAM sequencer
 *          clock selection
 *          
* 
 *         
*/

#include <vxWorks.h>
#include <stdlib.h>
#include <stdio.h>
#include <iosLib.h>
#include <taskLib.h>
#include <semLib.h>
#include <memLib.h>
#include <lstLib.h>
#include <vme.h>
#include <vxLib.h>
#include <sysLib.h>
#include <stdio.h>

#include <dbDefs.h>
#include <dbCommon.h>
#include <dbAccess.h>
#include <epicsPrint.h>
#include <dbEvent.h>
#include <recGbl.h>
#include <devSup.h>
#include <dbDefs.h>
#include <link.h>
#include <ellLib.h>

#include "egRecord.h"
#include <vme64x_cr.h>
#include <egeventRecord.h>
#include "egDefs.h"


#ifdef EG_DEBUG
#define STATIC
#define epicsPrintf printf
#else
#define STATIC static
#endif

#define EG_MONITOR			/* Include the EG monitor program */
#define RAM_LOAD_SPIN_DELAY	1	/* taskDelay() for waiting on RAM */

/* Only look at these bits when ORing on new bits in the control register */
#define CTL_OR_MASK	(0x9660)
#define FRAME_CLOCK     125e6
#define EVG_CODE_END      0x007f


typedef struct MrfEVGStruct
{
  unsigned short	Control   ;
  unsigned short	EventMask ;
  unsigned short	VmeEvent  ;	/* and distributed bus data (byte wide) */
  unsigned short	obsolete_Seq2Addr  ;
  unsigned short	obsolete_Seq2Data  ;
  unsigned short	obsolete_Seq1Addr  ;
  unsigned short	obsolete_Seq1Data  ;

  unsigned short	Event0Map   ;	/**< Event Mapping Register for external (HW) input 0 */
  unsigned short	Event1Map   ;	/**< Event Mapping Register for external (HW) input 1 */
  unsigned short	Event2Map   ;	/**< Event Mapping Register for external (HW) input 2 */
  unsigned short	Event3Map   ;	/**< Event Mapping Register for external (HW) input 3 */
  unsigned short	Event4Map   ;	/**< Event Mapping Register for external (HW) input 4 */
  unsigned short	Event5Map   ;	/**< Event Mapping Register for external (HW) input 5 */
  unsigned short	Event6Map   ;	/**< Event Mapping Register for external (HW) input 6 */
  unsigned short	Event7Map   ;	/**< Event Mapping Register for external (HW) input 7 */
  /* Extended registers */
  unsigned short	MuxCountDbEvEn;   /**< Multiplexed counter distributed bus (byte wide) */
					    /**< enables and event trigger enables (byte wide) */
  unsigned short	obsolete_Seq1ExtAddr  ;      /**< Extended address SEQ RAM 1 */
  unsigned short	obsolete_Seq2ExtAddr  ;      /**< Extended address SEQ RAM 2 */
  unsigned short	Seq1ClockSel  ;     /**< Sequence RAM 1 clock selection */
  unsigned short	Seq2ClockSel  ;     /**< Sequence RAM 2 clock selection */
  unsigned short	ACinputControl;     /**< AC related controls register */
  unsigned short	MuxCountSelect;     /**< reset, Mux seq. trigger enable and Mux select */
  unsigned short	MuxPrescaler;       /**< Mux prescaler values (25MHz/value) */
  unsigned short	FPGAVersion ;      /**< FPGA firmware register number (only series 100) */
  UINT32 Reserved_0x30;
  UINT32 Reserved_0x34;
  UINT32 Reserved_0x38;
  UINT32 Reserved_0x3C;
  unsigned short	RfSelect; 	/**< RF Clock Selection register */
  UINT16 MxcPolarity;			/**< Mpx counter reset polarity */
  UINT16 Seq1Addr;    			/**< 11 bit address, 0-2047 (2 kB event RAM) */
  UINT16 Seq1Data;    			/**< Event code, 8 bits */
  UINT32 Seq1Time;    			/**< Time offset in event clock from trigger */
  UINT32 Seq1Pos;     			/**< Current sequence time position */
  UINT16 Seq2Addr;    			/**< 11 bit address, 0-2047 */
  UINT16 Seq2Data;    			/**< Event code, 8 bits */
  UINT32 Seq2Time;    			/**< Time offset in event clock from trigger */
  UINT32 Seq2Pos;     			/**< Current sequence time position */
  UINT16 EvanControl; 			/**< Event analyser control register */
  UINT16 EvanEvent;   			/**< Event analyser event code, 8 bits */
  UINT32 EvanTimeH;   			/**< Bits 63-32 of event analyser time counter */
  UINT32 EvanTimeL;   			/**< Bits 31-0 of event analyser time counter */
  UINT16 uSecDivider;               /* Reserved_0x68; */
  UINT16 DataBufSize;   /*2006 May 16 leige*/
  UINT32 Reserved_0x6C;
  UINT32 Reserved_0x70;
  UINT32 Reserved_0x74;
  UINT32 Reserved_0x78;
  UINT32 Reserved_0x7C;
  UINT32 FracDivControl;  	/**< Micrel SY87739L programming word */
  UINT32 DelayRf;		
  UINT32 DelayRx;		
  UINT32 DelayTx;
  UINT32 AdiControl;		
  UINT32 FbTxFrac;
}MrfEVGStruct;


typedef struct
{
  MrfEVGStruct 		*pEg;		/**< pointer to card described. */
  SEM_ID		EgLock;
  struct egRecord	*pEgRec;	/**< Record that represents the card */
  double		Ram1Speed;	/**< in Hz */
  long			Ram1Dirty;	/**< needs to be reloaded */
  double		Ram2Speed;	/**< in Hz */
  long			Ram2Dirty;	/**< needs to be reloaded */
  ELLLIST		EgEvent;	/**< RAM event list head. All events are kept in one list. */
  long                  MaxRamPos;       /**< Operation limit of RAM */
  unsigned int 		RFselect;	/**< RF source select word */
} EgLinkStruct;

typedef struct
{
  ELLNODE			node;
  struct	egeventRecord	*pRec;
} EgEventNode;

/* leige test evg200 and evr200. 2005 Oct 12 */
STATIC long EgRegGet(int regAddr);
STATIC long EgRegSet(int regAddr, int value);
STATIC MrfEVGStruct *lgEg;
/*int EgDebug = 6; */
/*end of leige test */

STATIC long EgInitDev(int pass);
STATIC long EgProcEgRec(struct egRecord *pRec);
STATIC long EgDisableFifo(EgLinkStruct *pParm);
STATIC long EgEnableFifo(EgLinkStruct *pParm);
STATIC long EgMasterEnable(EgLinkStruct *pParm);
STATIC long EgMasterDisable(EgLinkStruct *pParm);
STATIC long EgResetAll(EgLinkStruct *pParm);
STATIC long EgSetTrigger(EgLinkStruct *pParm, unsigned int Channel, unsigned short Event);
STATIC long EgEnableTrigger(EgLinkStruct *pParm, unsigned int Channel, int state);
STATIC int  EgSeqEnableCheck(EgLinkStruct *pParm, unsigned int Seq);
STATIC long EgEnableVme(EgLinkStruct *pParm, int state);
STATIC long EgGenerateVmeEvent(EgLinkStruct *pParm, int Event);
/* Sequencer-related routines */
STATIC long EgClearSeq(EgLinkStruct *pParm, int channel);
STATIC long EgSeqTrigger(EgLinkStruct *pParm, unsigned int Seq);
STATIC long EgSetSeqMode(EgLinkStruct *pParm, unsigned int Seq, int Mode);
STATIC int  EgReadSeqRam(EgLinkStruct *pParm, int channel, unsigned char *pBuf);
STATIC int  EgWriteSeqRam(EgLinkStruct *pParm, int channel, unsigned char *pBuf);
STATIC long EgGetAltStatus(EgLinkStruct *pParm, int Ram);
STATIC long EgEnableAltRam(EgLinkStruct *pParm, int Ram);
STATIC long EgRamClockSet(EgLinkStruct *pParm, long Ram, long Clock);
STATIC long EgRamClockGet(EgLinkStruct *pParm, long Ram);
STATIC long EgPlaceRamEvent(struct egeventRecord *pRec);
STATIC long EgScheduleRamProgram(int card);
STATIC long EgGetMode(EgLinkStruct *pParm, int ram);
STATIC long EgGetRamEvent(EgLinkStruct *pParm, long Ram, long Addr);

/*Debug routines */
STATIC int  EgDumpRegs(EgLinkStruct *pParm);
STATIC int  SetupSeqRam(EgLinkStruct *pParm, int channel);
STATIC void SeqConfigMenu(void);
STATIC void PrintSeq(EgLinkStruct *pParm, int channel);
STATIC int  ConfigSeq(EgLinkStruct *pParm, int channel);
STATIC void menu(void);
STATIC long EgCheckTaxi(EgLinkStruct *pParm);
STATIC long EgMasterEnableGet(EgLinkStruct *pParm);
STATIC long EgCheckFifo(EgLinkStruct *pParm);
STATIC long EgGetTrigger(EgLinkStruct *pParm, unsigned int Channel);
STATIC long EgGetEnableTrigger(EgLinkStruct *pParm, unsigned int Channel);
STATIC long EgGetBusyStatus(EgLinkStruct *pParm, int Ram);
STATIC long EgGetEnableMuxDistBus(EgLinkStruct *pParm, unsigned int Channel);
STATIC long EgGetEnableMuxEvent(EgLinkStruct *pParm, unsigned int Channel);
STATIC long EgEnableMuxDistBus(EgLinkStruct *pParm, unsigned int Channel, int state);
STATIC long EgEnableMuxEvent(EgLinkStruct *pParm, unsigned int Channel, int state);
STATIC long EgGetACinputControl(EgLinkStruct *pParm, unsigned int Channel);
STATIC long EgSetACinputControl(EgLinkStruct *pParm, unsigned int Channel, int state);
STATIC long EgGetACinputDivisor(EgLinkStruct *pParm, unsigned short DlySel);
STATIC long EgSetACinputDivisor(EgLinkStruct *pParm, unsigned short Divisor, unsigned short DlySel);
STATIC long EgResetMuxCounter(EgLinkStruct *pParm, unsigned int Channel);
STATIC long EgResetMPX(EgLinkStruct *pParm, unsigned int Mask);
STATIC long EgGetEnableMuxSeq(EgLinkStruct *pParm, unsigned int SeqNum);
STATIC long EgEnableMuxSeq(EgLinkStruct *pParm, unsigned int SeqNum, int state);
STATIC unsigned long EgGetMuxPrescaler(EgLinkStruct *pParm, unsigned short Channel);
STATIC unsigned long EgSetMuxPrescaler(EgLinkStruct *pParm, unsigned short Channel, unsigned long Divisor);
STATIC long EgGetFpgaVersion(EgLinkStruct *pParm);

#define NUM_EG_LINKS	21		/**< Total allowed number of EG boards */

/* Parms for the event generator task */
#define	EGRAM_NAME	"EgRam"
#define	EGRAM_PRI	100
#define EGRAM_OPT	VX_FP_TASK|VX_STDIO
#define	EGRAM_STACK	4000

/* static EgLinkStruct	EgLink[NUM_EG_LINKS]; */ 
STATIC EgLinkStruct	EgLink[NUM_EG_LINKS]; /*leige debug*/
static int		EgNumLinks = 0;
static SEM_ID		EgRamTaskEventSem;
static int		ConfigureLock = 0;

/**
 *
 * Routine used to verify that a given card number is valid.
 * Returns zero if valid, nonzero otherwise.
 *
 **/
STATIC long EgCheckCard(int Card)
{
  if ((Card < 0)||(Card >= EgNumLinks))
    return(-1);
  if (EgLink[Card].pEg == NULL)
    return(-1);
  return(0);
}

/**
 *
 * User configurable card addresses are saved in this array.
 * To configure them, use the EgConfigure command from the startup script
 * when booting Epics.
 *
 **/
int EgConfigure(int Card, unsigned long CardAddress, unsigned short RFsel)
{
  unsigned long Junk;

  if (ConfigureLock != 0)
  {
    epicsPrintf("devApsEg: Cannot change configuration after init.  Request ignored\n");
    return(-1);
  }
  if (Card >= NUM_EG_LINKS)
  {
    epicsPrintf("devApsEg: Card number invalid, must be 0-%d\n", NUM_EG_LINKS);
    return(-1);
  }
  if (CardAddress > 0xffff)
  {
    epicsPrintf("devApsEg: Card address invalid, must be 0x0000-0xffff\n");
    return(-1);
  }
  /* CSR handling here. The slot number is used as the card number */
  vmeCSRWriteADER(Card,0,(CardAddress&0xff00)|0xa4);

  if(sysBusToLocalAdrs(VME_AM_SUP_SHORT_IO, (char*)CardAddress, (char**)&EgLink[Card].pEg)!=OK)
  {
    epicsPrintf("devApsEg: Failure mapping requested A16 address\n");
    EgLink[Card].pEg = NULL;
    return(-1);
  }
  if (vxMemProbe((char*)&(EgLink[Card].pEg->Control), READ, sizeof(short), (char*)&Junk) != OK)
  {
    epicsPrintf("devApsEg: Failure probing for event generator... Card disabled\n");
    EgLink[Card].pEg = NULL;
    return(-1);
  }
  EgLink[Card].RFselect = RFsel;
  if (Card >= EgNumLinks)
    EgNumLinks = Card+1;
  /*leige test*/
  printf("EgConfigure: address: %d\n", (int)(&(EgLink[Card].pEg)));
  printf("%d\n", (int)Junk);
  lgEg=EgLink[Card].pEg;
  /*end of leige test */
  printf("get EVG RfSelect - %8X\n", EgLink[Card].pEg->RfSelect);
  printf("get EVG uSecDivider - %4X\n", EgLink[Card].pEg->uSecDivider);

  return(0);
}

int EgConfigure1(int Card, unsigned long CardAddress, unsigned short divider)
{
  unsigned long Junk;

  if (ConfigureLock != 0)
  {
    epicsPrintf("devApsEg: Cannot change configuration after init.  Request ignored\n");
    return(-1);
  }
  if (Card >= NUM_EG_LINKS)
  {
    epicsPrintf("devApsEg: Card number invalid, must be 0-%d\n", NUM_EG_LINKS);
    return(-1);
  }
  if (CardAddress > 0xffff)
  {
    epicsPrintf("devApsEg: Card address invalid, must be 0x0000-0xffff\n");
    return(-1);
  }
  /* CSR handling here. The slot number is used as the card number */
  vmeCSRWriteADER(Card,0,(CardAddress&0xff00)|0xa4);

  if(sysBusToLocalAdrs(VME_AM_SUP_SHORT_IO, (char*)CardAddress, (char**)&EgLink[Card].pEg)!=OK)
  {
    epicsPrintf("devApsEg: Failure mapping requested A16 address\n");
    EgLink[Card].pEg = NULL;
    return(-1);
  }
  if (vxMemProbe((char*)&(EgLink[Card].pEg->Control), READ, sizeof(short), (char*)&Junk) != OK)
  {
    epicsPrintf("devApsEg: Failure probing for event generator... Card disabled\n");
    EgLink[Card].pEg = NULL;
    return(-1);
  }

/*leige set */
  printf("get original RfSelect - %8X\n", EgLink[Card].pEg->RfSelect);
  printf("get original FracDivControl - %8X\n", EgLink[Card].pEg->FracDivControl);
  printf("get original uSecDivider - %4X\n", EgLink[Card].pEg->uSecDivider);
  
  switch (divider) {
     case 4: 
        EgLink[Card].pEg->RfSelect = 0x001c;
        EgLink[Card].pEg->FracDivControl = 0x00fe816d;
        EgLink[Card].pEg->uSecDivider = 0x7d;
        break;
     case 5:
        EgLink[Card].pEg->RfSelect = 0x001e;
        EgLink[Card].pEg->FracDivControl = 0x025b41ed; 
        EgLink[Card].pEg->uSecDivider = 100;
        break;
     case 10:
        EgLink[Card].pEg->RfSelect = 0x0012;
        EgLink[Card].pEg->FracDivControl = 0x025b43ad;
        EgLink[Card].pEg->uSecDivider = 50;
        break;
     default: 
        printf("EgConfigure1, use default\n");
    }
/*end of leige set */

  EgLink[Card].RFselect = EgLink[Card].pEg->RfSelect;
  if (Card >= EgNumLinks)
    EgNumLinks = Card+1;
  /*leige test*/
  printf("EgConfigure: address: %d\n", (int)(&(EgLink[Card].pEg)));
  printf("%d\n", (int)Junk);
  lgEg=EgLink[Card].pEg;

  sysUsDelay(1000000);

  printf("get EVG RfSelect - %8X\n", EgLink[Card].pEg->RfSelect);
  printf("get current FracDivControl - %8X\n", EgLink[Card].pEg->FracDivControl);
  printf("get EVG uSecDivider - %4X\n", EgLink[Card].pEg->uSecDivider);

  /*end of leige test */

  return(0);
}


/**EgConfigure230   ************
 * leige
*******************/

int EgConfigure230(int Card, unsigned long CardAddress, unsigned short divider)
{
  unsigned long Junk;

  if (ConfigureLock != 0)
  {
    epicsPrintf("devApsEg: Cannot change configuration after init.  Request ignored\n");
    return(-1);
  }
  if (Card >= NUM_EG_LINKS)
  {
    epicsPrintf("devApsEg: Card number invalid, must be 0-%d\n", NUM_EG_LINKS);
    return(-1);
  }
  if (CardAddress > 0xffff)
  {
    epicsPrintf("devApsEg: Card address invalid, must be 0x0000-0xffff\n");
    return(-1);
  }
  /* CSR handling here. The slot number is used as the card number */
  vmeCSRWriteADER(Card,0,(CardAddress&0xff00)|0xa4);

  if(sysBusToLocalAdrs(VME_AM_SUP_SHORT_IO, (char*)CardAddress, (char**)&EgLink[Card].pEg)!=OK)
  {
    epicsPrintf("devApsEg: Failure mapping requested A16 address\n");
    EgLink[Card].pEg = NULL;
    return(-1);
  }
  if (vxMemProbe((char*)&(EgLink[Card].pEg->Control), READ, sizeof(short), (char*)&Junk) != OK)
  {
    epicsPrintf("devApsEg: Failure probing for event generator... Card disabled\n");
    EgLink[Card].pEg = NULL;
    return(-1);
  }

/*leige set */
  printf("get original RfSelect - %8X\n", EgLink[Card].pEg->RfSelect);
  printf("get original FracDivControl - %8X\n", EgLink[Card].pEg->FracDivControl);
  printf("get original uSecDivider - %4X\n", EgLink[Card].pEg->uSecDivider);
 
  switch (divider) {
     case 4:
        EgLink[Card].pEg->RfSelect = 0x01c3;
        EgLink[Card].pEg->FracDivControl = 0x00fe816d;
        EgLink[Card].pEg->uSecDivider = 0x7d;
        break;
     case 5:
        EgLink[Card].pEg->RfSelect = 0x01c4;
        EgLink[Card].pEg->FracDivControl = 0x025b41ed;
        EgLink[Card].pEg->uSecDivider = 100;
        break;
     default:
        EgLink[Card].pEg->RfSelect = 0x01c3;
        EgLink[Card].pEg->FracDivControl = 0x00fe816d;
        EgLink[Card].pEg->uSecDivider = 0x7d;
        printf("EgConfigure230, default Rf divider 4\n");
    }
/*end of leige set */

  EgLink[Card].RFselect = EgLink[Card].pEg->RfSelect;
  if (Card >= EgNumLinks)
    EgNumLinks = Card+1;
  /*leige test*/
  printf("EgConfigure230: address: %d\n", (int)(&(EgLink[Card].pEg)));
  printf("%d\n", (int)Junk);
  lgEg=EgLink[Card].pEg;

  sysUsDelay(1000000);

  printf("get EVG RfSelect - %8X\n", EgLink[Card].pEg->RfSelect);
  printf("get current FracDivControl - %8X\n", EgLink[Card].pEg->FracDivControl);
  printf("get EVG uSecDivider - %4X\n", EgLink[Card].pEg->uSecDivider);

  /*end of leige test */

  return(0);
}  /* end of EgConfigure230 */

/** EgConfigureA24  by leige for HEPS home made evg, 2019-02-25 **/
/** Card number is the slot number,
    VME_AM_STD_USR_DATA 0x3d
**/
int EgConfigureA24(int Card, unsigned long CardAddress, unsigned short divider)
{
  unsigned long Junk;

    if (ConfigureLock != 0)
  {
    epicsPrintf("devApsEg: Cannot change configuration after init.  Request ignored\n");
    return(-1);
  }
  if (Card > 21 || Card < 1) 
     {
        printf("EgConfigureA24: Card number must be 1-21\n");
        return(-1);
     }
  if (CardAddress > 0xffff)
  {
    epicsPrintf("devApsEg: Card address invalid, must be 0x0000-0xffff\n");
    return(-1);
  }

 if(sysBusToLocalAdrs(VME_AM_STD_USR_DATA, (char*)CardAddress, (char**)&EgLink[Card].pEg)!=OK)
  {
    epicsPrintf("devApsEg: Failure mapping requested A24 address\n");
    EgLink[Card].pEg = NULL;
    return(-1);
  }
  if (vxMemProbe((char*)&(EgLink[Card].pEg->Control), READ, sizeof(short), (char*)&Junk) != OK)
  {
    epicsPrintf("devApsEg: Failure probing for event generator... Card disabled\n");
    EgLink[Card].pEg = NULL;
    return(-1);
  }

  printf("get original RfSelect - %8X\n", EgLink[Card].pEg->RfSelect);
  printf("get original FracDivControl - %8X\n", EgLink[Card].pEg->FracDivControl);
  printf("get original uSecDivider - %4X\n", EgLink[Card].pEg->uSecDivider);
  
  switch (divider) {
     case 4: 
        EgLink[Card].pEg->RfSelect = 0x001c;
        EgLink[Card].pEg->FracDivControl = 0x00fe816d;
        EgLink[Card].pEg->uSecDivider = 0x7d;
        break;
     case 5:
        EgLink[Card].pEg->RfSelect = 0x001e;
        EgLink[Card].pEg->FracDivControl = 0x025b41ed; 
        EgLink[Card].pEg->uSecDivider = 100;
        break;
     case 10:
        EgLink[Card].pEg->RfSelect = 0x0012;
        EgLink[Card].pEg->FracDivControl = 0x025b43ad;
        EgLink[Card].pEg->uSecDivider = 50;
        break;
     default: 
        printf("EgConfigure1, use default\n");
    }

  EgLink[Card].RFselect = EgLink[Card].pEg->RfSelect;
  if (Card >= EgNumLinks)
    EgNumLinks = Card+1;
  
  printf("EgConfigure: address: %d\n", (int)(&(EgLink[Card].pEg)));
  printf("%d\n", (int)Junk);
  lgEg=EgLink[Card].pEg;

  sysUsDelay(1000000);

  printf("get EVG RfSelect - %8X\n", EgLink[Card].pEg->RfSelect);
  printf("get current FracDivControl - %8X\n", EgLink[Card].pEg->FracDivControl);
  printf("get EVG uSecDivider - %4X\n", EgLink[Card].pEg->uSecDivider);

  return(0);

} /* end of EgConfigureA24 */

/**
 *
 * This is used to run thru the list of egevent records and dump their values 
 * into a sequence RAM.
 * The events are always sorted in delay and priority order and have
 * the APOS in the final value. The routine must only go through the
 * list and place the timestamp/event values into the RAM. All egevents
 * are kept in the same list; in separate (non alt) modes the events not
 * belonging to the RAM being programmed are just skipped.
 * There is one catch: the collisions are resolved even for separate RAMs when
 * this is not necessary.
 * This has to be considered again, although for normal operation it has no
 * consequences.
 *
 * The caller of this function MUST hold the link-lock for the board holding
 * the ram to be programmed.
 *
 **/
STATIC long EgLoadRamList(EgLinkStruct *pParm, long Ram)
{
  volatile MrfEVGStruct		*pEg = pParm->pEg;
  volatile unsigned short	*pAddr;
  volatile unsigned int		*pTime;
  volatile unsigned short	*pData;
  ELLNODE			*pNode;
  struct egeventRecord		*pEgevent;
  int				RamPos = 0;
  int				AltFlag = 0;
  int 				dummy; /*trick to flush bridge pipeline*/
  int 				maxtime=0;
  double			RamSpeed;
  volatile long origEvt;
  /*  int                           OldEvent = 0;*/

#ifdef EG_DEBUG
  if (EgDebug>5) 
  printf("EgLoadRamList %d\n", (int)Ram);
#endif

  RamPos = 0;
  AltFlag = 0;
  origEvt=-1; /* first time, set to a negative number to show that nothing programmed yet */

  if (Ram == 1)
    RamSpeed = pParm->Ram1Speed;
  else
    RamSpeed = pParm->Ram2Speed;

  /* When in ALT mode, all event records have to be downloaded */
  
  if (EgGetMode(pParm, 1) == egMOD1_Alternate)
  {
    RamSpeed = pParm->Ram1Speed;	/* RAM1 clock used when in ALT mode */
    AltFlag = 1;
  }
  if (Ram == 1)
  {
    pAddr = &pEg->Seq1Addr;
    pTime = &pEg->Seq1Time;
    pData = &pEg->Seq1Data;
  }
  else
  {
    pAddr = &pEg->Seq2Addr;
    pTime = &pEg->Seq2Time;
    pData = &pEg->Seq2Data;
  }
  /* Get first egevent record from list */
  pNode = ellFirst(&pParm->EgEvent);
  while (pNode != NULL)
  {
    pEgevent = ((EgEventNode *)pNode)->pRec;
   
    if (AltFlag!=1 && pEgevent->ram !=Ram) {} else
    {
	    if (pEgevent->apos < pParm->MaxRamPos)
	      {
		/* put the record's event code into the RAM */
		*pAddr=RamPos;
		dummy = *pAddr;
		*pTime=pEgevent->apos;
		*pData=pEgevent->enm;
	
		/* Remember where the last event went into the RAM */
		RamPos++;
	      }
    	else /* Overflow the event ram... toss it out. */
    	{
      		pEgevent->apos = pParm->MaxRamPos+1;;
    	}
	maxtime=pEgevent->apos; /* last event time */
            /* check for divide by zero problems */
    if (RamSpeed > 0)
    {
      /* Figure out the actual position, with conversion between units when necessary */
      switch (pEgevent->unit)
      {
      case REC_EGEVENT_UNIT_TICKS:
        pEgevent->adly = pEgevent->apos;
        break;
      case REC_EGEVENT_UNIT_FORTNIGHTS:
        pEgevent->adly = ((float)pEgevent->apos / (60.0 * 60.0 * 24.0 * 14.0)) / RamSpeed;
        break;
      case REC_EGEVENT_UNIT_WEEKS:
        pEgevent->adly = ((float)pEgevent->apos / (60.0 * 60.0 * 24.0 * 7.0)) / RamSpeed;
        break;
      case REC_EGEVENT_UNIT_DAYS:
        pEgevent->adly = ((float)pEgevent->apos / (60.0 * 60.0 * 24.0)) / RamSpeed;
        break;
      case REC_EGEVENT_UNIT_HOURS:
        pEgevent->adly = ((float)pEgevent->apos / (60.0 * 60.0)) / RamSpeed;
        break;
      case REC_EGEVENT_UNIT_MINUITES:
        pEgevent->adly = ((float)pEgevent->apos / 60.0) / RamSpeed;
        break;
      case REC_EGEVENT_UNIT_SECONDS:
        pEgevent->adly = (float)pEgevent->apos / RamSpeed;
        break;
      case REC_EGEVENT_UNIT_MILLISECONDS:
        pEgevent->adly = (float)pEgevent->apos * (1000.0 / RamSpeed);
        break;
      case REC_EGEVENT_UNIT_MICROSECONDS:
        pEgevent->adly = (float)pEgevent->apos * (1000000.0 / RamSpeed);
        break;
      case REC_EGEVENT_UNIT_NANOSECONDS:
        pEgevent->adly = (float)pEgevent->apos * (1000000000.0 / RamSpeed);
        break;
      }
    }
    else
      pEgevent->adly = 0;
    
    }
    /* get next record */
    pNode = ellNext(pNode);

    if (pEgevent->tpro)
      epicsPrintf("EgLoadRamList(%s) adly %g ramspeed %g\n", pEgevent->name, pEgevent->adly, RamSpeed);
    if (pEgevent->tpro)
      epicsPrintf("EgLoadRamList: LastRamPos %d\n",RamPos);

    /* Release database monitors for dpos, apos, and adly here */
    
    db_post_events(pEgevent, &pEgevent->adly, DBE_VALUE|DBE_LOG);
    db_post_events(pEgevent, &pEgevent->dpos, DBE_VALUE|DBE_LOG);
    db_post_events(pEgevent, &pEgevent->apos, DBE_VALUE|DBE_LOG);
    
  }
  /* put the end of sequence event code last */
  *pAddr=RamPos;
  dummy = *pAddr;
  *pData=EVG_CODE_END;
  *pTime=maxtime+1;

  *pAddr = 0;	/* set back to 0 so will be ready to run */

  return(0);
}
/**
 *
 * This task is used to do the actual manipulation of the sequence RAMs on
 * the event generator cards.  There is only one task on the crate for as many
 * EG-cards that are used.
 *
 * The way this works is we wait on a semaphore that is given when ever the
 * data values for the SEQ-rams is altered.  (This data is left in the database
 * record.)  When this task wakes up, it runs thru a linked list (created at
 * init time) of all the event-generator records for the RAM that was logically
 * updated.  While doing so, it sets the event codes in the RAM.
 *
 * The catch is that a given RAM can not be altered when it is enabled for 
 * event generation.  If we find that the RAM needs updating, but is busy
 * This task will poll the RAM until it is no longer busy, then download
 * it.
 *
 * This task views the RAMs as being in one of two modes.  Either alt-mode, or
 * "the rest".  
 *
 * When NOT in alt-mode, we simply wait for the required RAM to become disabled
 * and non-busy and then download it.  
 *
 * In alt-mode, we can download to either of the two RAMs and then tell the 
 * EG to start using that RAM.  The EG can only use one RAM at a time in 
 * alt-mode, but we still have a problem when one RAM is running, and the other
 * has ALREADY been downloaded and enabled... so they are both 'busy.'
 *
 **/
STATIC void EgRamTask(void)
{
  int	j;
  int	Spinning = 0;

  printf("EgRamTask\n");
  for (;;)
  {
    if (Spinning != 0)
      taskDelay(RAM_LOAD_SPIN_DELAY);
    else
      semTake(EgRamTaskEventSem, WAIT_FOREVER);

    Spinning = 0;
    for (j=0;j<EgNumLinks;j++)
    {
      if (EgCheckCard(j) == 0)
      {
      /* Lock the EG link */
      semTake (EgLink[j].EgLock, WAIT_FOREVER);	/******** LOCK ***************/

      if (EgGetMode(&EgLink[j], 1) != egMOD1_Alternate)
      { /* Not in ALT mode, each ram is autonomous */
        if(EgLink[j].Ram1Dirty != 0)
        {
	  /* Make sure it is disabled and not running */
	  if ((EgGetMode(&EgLink[j], 1) == egMOD1_Off) && (EgGetBusyStatus(&EgLink[j], 1) == 0))
	  { /* download the RAM */
	    EgClearSeq(&EgLink[j], 1);
	    /*EgLoadRamList(&EgLink[j], 1); */
	    EgLink[j].Ram1Dirty = 0;
	  }
	  else
	    Spinning = 1;
        }
        if(EgLink[j].Ram2Dirty != 0)
        {
	  /* Make sure it is disabled and not running */
	  if ((EgGetMode(&EgLink[j], 2) == egMOD1_Off) && (EgGetBusyStatus(&EgLink[j], 2) == 0))
	  { /* download the RAM */
	    EgClearSeq(&EgLink[j], 2);
	    /*EgLoadRamList(&EgLink[j], 2);*/
	    EgLink[j].Ram2Dirty = 0;
	  }
	  else
	    Spinning = 1;
        }
      }
      else /* We are in ALT mode, we have one logical RAM */
      {
	/* See if RAM 1 is available for downloading */
	if ((EgLink[j].Ram1Dirty != 0)||(EgLink[j].Ram2Dirty != 0))
	{
	  if (EgGetAltStatus(&EgLink[j], 1) == 0)
	  {
	    EgClearSeq(&EgLink[j], 1);
	    EgLoadRamList(&EgLink[j], 1);
	    EgLink[j].Ram1Dirty = 0;
	    EgLink[j].Ram2Dirty = 0;

	    /* Turn on RAM 1 */
	    EgEnableAltRam(&EgLink[j], 1);

	  }
	  else  if (EgGetAltStatus(&EgLink[j], 2) == 0)
	  {
	    EgClearSeq(&EgLink[j], 2);
	    EgLoadRamList(&EgLink[j], 2);
	    EgLink[j].Ram1Dirty = 0;
	    EgLink[j].Ram2Dirty = 0;

	    /* Turn on RAM 2 */
	    EgEnableAltRam(&EgLink[j], 2);
	  }
	  else
	    Spinning = 1;
	}
      }

      /* Unlock the EG link */
      semGive (EgLink[j].EgLock);		/******** UNLOCK *************/
      }
    }
  }
}

/**
 *
 * Used to init and start the EG-RAM task.
 *
 **/
STATIC long EgStartRamTask(void)
{
  printf("EgStartRamTask\n");
  if ((EgRamTaskEventSem = semBCreate(SEM_Q_PRIORITY, SEM_EMPTY)) == NULL)
  {
    epicsPrintf("EgStartRamTask(): cannot create semaphore for ram task\n");
    return(-1);
  }

  /* vxWorks's STUPID taskSpawn... they put in varargs, but didn't enable it. */
  if (taskSpawn(EGRAM_NAME, EGRAM_PRI, EGRAM_OPT, EGRAM_STACK, (FUNCPTR)EgRamTask, 0,0,0,0,0,0,0,0,0,0) == ERROR)
  {
    epicsPrintf("EgStartRamTask(): failed to start EG-ram task\n");
    return(-1);
  }
  return (0);
}

/**
 *
 * Called at init time to init the EG-device support code.
 *
 * NOTE: can be called more than once for each init pass.
 *
 **/
STATIC long EgInitDev(int pass)
{
  printf("EgInitDev\n");
  if (pass==0)
  { /* Do any hardware initialization (no records init'd yet) */
    int	j;
    static int firsttime = 1;
    
    printf("EgInitDev pass 0\n");
    if (firsttime)
    {
      firsttime = 0;
      EgStartRamTask();
      for (j=0;j<EgNumLinks;j++)
      {
        if (EgCheckCard(j) == 0)
	{
          if ((EgLink[j].EgLock = semBCreate(SEM_Q_PRIORITY, SEM_FULL)) == NULL)
            epicsPrintf("EgInitDev can not create semaphore\n");
  
          EgResetAll(&EgLink[j]);
          EgLink[j].pEgRec = NULL;
          EgLink[j].Ram1Speed = 0;
          EgLink[j].Ram1Dirty = 0;
          EgLink[j].Ram2Speed = 0;
          EgLink[j].Ram2Dirty = 0;
          ellInit(&(EgLink[j].EgEvent));
	}
      }
    }
  }
  else if (pass==1)
  { /* do any additional initialization */
    int	j;
    printf("EgInitDev pass 1\n");
    for (j=0;j<EgNumLinks;j++)
    {
      if (EgCheckCard(j) == 0)
        EgScheduleRamProgram(j);	/* OK to do more than once */
    }
  }
  return(0);
}

/**
 *
 * This is called by any function that determines that the sequence RAMs are
 * out of sync with the database records.  We simply wake up the EG RAM task.
 *
 **/
STATIC long EgScheduleRamProgram(int card)
{
  semGive(EgRamTaskEventSem);
  return(0);
}
/**
 *
 * Support for initialization of EG records.
 *
 **/

STATIC long EgInitEgRec(struct egRecord *pRec)
{
  EgLinkStruct	*pLink;

  printf("EgInitEgRec \n");
  if (EgCheckCard(pRec->out.value.vmeio.card) != 0)
  {
    recGblRecordError(S_db_badField, (void *)pRec, "devApsEg::EgInitEgRec() bad card number");
    return(S_db_badField);
  }

  if (EgLink[pRec->out.value.vmeio.card].pEgRec != NULL)
  {
    recGblRecordError(S_db_badField, (void *)pRec, "devApsEg::EgInitEgRec() only one record allowed per card");
    return(S_db_badField);
  }
  pLink = &EgLink[pRec->out.value.vmeio.card];

  pLink->pEgRec = pRec;
  pLink->Ram1Speed = pRec->r1sp;
  pLink->Ram2Speed = pRec->r2sp;
  if(pRec->rmax >  EG_SEQ_RAM_SIZE) {
     pLink->MaxRamPos = pRec->rmax;
     pRec->rmax = EG_SEQ_RAM_SIZE;
  } else {
    pLink->MaxRamPos = pRec->rmax;
  }


  /* Keep the thing off until we are dun setting it up */
  EgMasterDisable(pLink);

  pRec->lffo = pRec->fifo;
  if (pRec->fifo)
    EgEnableFifo(pLink);
  else
    EgDisableFifo(pLink);
    
  {
  volatile MrfEVGStruct  *pEg = pLink->pEg;
  if (EgLink[pRec->out.value.vmeio.card].RFselect != NULL)
    pEg->RfSelect =  EgLink[pRec->out.value.vmeio.card].RFselect;
    else {
    pEg->RfSelect = 0x0014;			/* Select internal RF syntethiser by default*/
    }
  printf("RFSelect - %x\n", pEg->RfSelect);    
  } 

  /* Disable these things so I can adjust them */
  EgEnableTrigger(pLink, 0, 0);
  EgEnableTrigger(pLink, 1, 0);
  EgEnableTrigger(pLink, 2, 0);
  EgEnableTrigger(pLink, 3, 0);
  EgEnableTrigger(pLink, 4, 0);
  EgEnableTrigger(pLink, 5, 0);
  EgEnableTrigger(pLink, 6, 0);
  EgEnableTrigger(pLink, 7, 0);

  /* Set the trigger event codes */
  pRec->let0 = pRec->et0;
  EgSetTrigger(pLink, 0, pRec->et0);
  pRec->let1 = pRec->et1;
  EgSetTrigger(pLink, 1, pRec->et1);
  pRec->let2 = pRec->et2;
  EgSetTrigger(pLink, 2, pRec->et2);
  pRec->let3 = pRec->et3;
  EgSetTrigger(pLink, 3, pRec->et3);
  pRec->let4 = pRec->et4;
  EgSetTrigger(pLink, 4, pRec->et4);
  pRec->let5 = pRec->et5;
  EgSetTrigger(pLink, 5, pRec->et5);
  pRec->let6 = pRec->et6;
  EgSetTrigger(pLink, 6, pRec->et6);
  pRec->let7 = pRec->et7;
  EgSetTrigger(pLink, 7, pRec->et7);

  /* Set enables for the triggers not that they have valid values */
  EgEnableTrigger(pLink, 0, pRec->ete0);
  EgEnableTrigger(pLink, 1, pRec->ete1);
  EgEnableTrigger(pLink, 2, pRec->ete2);
  EgEnableTrigger(pLink, 3, pRec->ete3);
  EgEnableTrigger(pLink, 4, pRec->ete4);
  EgEnableTrigger(pLink, 5, pRec->ete5);
  EgEnableTrigger(pLink, 6, pRec->ete6);
  EgEnableTrigger(pLink, 7, pRec->ete7);
 
 

  /* Disable multiplexed counters so I can adjust them */

EgEnableMuxDistBus(pLink, 0, 0);
EgEnableMuxDistBus(pLink, 1, 0);
EgEnableMuxDistBus(pLink, 2, 0);
EgEnableMuxDistBus(pLink, 3, 0);
EgEnableMuxDistBus(pLink, 4, 0);
EgEnableMuxDistBus(pLink, 5, 0);
EgEnableMuxDistBus(pLink, 6, 0);
EgEnableMuxDistBus(pLink, 7, 0);
EgEnableMuxEvent(pLink, 0, 0);
EgEnableMuxEvent(pLink, 1, 0);
EgEnableMuxEvent(pLink, 2, 0);
EgEnableMuxEvent(pLink, 3, 0);
EgEnableMuxEvent(pLink, 4, 0);
EgEnableMuxEvent(pLink, 5, 0);
EgEnableMuxEvent(pLink, 6, 0);
EgEnableMuxEvent(pLink, 7, 0);

  	/* Set the multiplexed counter prescalers */
EgSetMuxPrescaler(pLink, 0, pRec->mc0p);
EgSetMuxPrescaler(pLink, 1, pRec->mc1p);
EgSetMuxPrescaler(pLink, 2, pRec->mc2p);
EgSetMuxPrescaler(pLink, 3, pRec->mc3p);
EgSetMuxPrescaler(pLink, 4, pRec->mc4p);
EgSetMuxPrescaler(pLink, 5, pRec->mc5p);
EgSetMuxPrescaler(pLink, 6, pRec->mc6p);
EgSetMuxPrescaler(pLink, 7, pRec->mc7p);
  
	/* Enable back the multiplexed counters again */
EgEnableMuxDistBus(pLink, 0, pRec->md0e);
EgEnableMuxDistBus(pLink, 1, pRec->md1e);
EgEnableMuxDistBus(pLink, 2, pRec->md2e);
EgEnableMuxDistBus(pLink, 3, pRec->md3e);
EgEnableMuxDistBus(pLink, 4, pRec->md4e);
EgEnableMuxDistBus(pLink, 5, pRec->md5e);
EgEnableMuxDistBus(pLink, 6, pRec->md6e);
EgEnableMuxDistBus(pLink, 7, pRec->md7e);
EgEnableMuxEvent(pLink, 0, pRec->me0e);
EgEnableMuxEvent(pLink, 1, pRec->me1e);
EgEnableMuxEvent(pLink, 2, pRec->me2e);
EgEnableMuxEvent(pLink, 3, pRec->me3e);
EgEnableMuxEvent(pLink, 4, pRec->me4e);
EgEnableMuxEvent(pLink, 5, pRec->me5e);
EgEnableMuxEvent(pLink, 6, pRec->me6e);
EgEnableMuxEvent(pLink, 7, pRec->me7e);

 	/* Set AC related register */
EgSetACinputControl(pLink, 0, pRec->asyn);
EgSetACinputControl(pLink, 1, pRec->aev0);
EgSetACinputControl(pLink, 2, pRec->asq1);
EgSetACinputControl(pLink, 3, pRec->asq2);
EgSetACinputDivisor(pLink, pRec->adiv, 0);
EgSetACinputDivisor(pLink, pRec->aphs, 1);
EgEnableMuxSeq(pLink, 1, pRec->msq1);
EgEnableMuxSeq(pLink, 2, pRec->msq2);
  
pRec->fpgv = EgGetFpgaVersion(pLink);

 
 
  /* These are one-shots... init them for future detection */
  pRec->trg1 = 0;
  pRec->trg2 = 0;
  pRec->clr1 = 0;
  pRec->clr2 = 0;
  pRec->vme  = 0;
  pRec->mcrs = 0;

  /* Init the clock prescalers */
  EgRamClockSet(pLink,1,pRec->r1ck);
  EgRamClockSet(pLink,2,pRec->r2ck);
  pRec->lr1c = EgRamClockGet(pLink,1);
  pRec->lr2c = EgRamClockGet(pLink,2);

  /* set RAM speed if the internal clock divider is used */
  if (pRec->r1ck != 0) {
    pRec->r1sp = FRAME_CLOCK / (pRec->r1ck+1);
    pLink->Ram1Speed = pRec->r1sp;
    db_post_events(pRec, &pRec->r1sp, DBE_VALUE|DBE_LOG);
  }
  if (pRec->r2ck != 0) {
    pRec->r2sp = FRAME_CLOCK / (pRec->r2ck+1);
    pLink->Ram2Speed = pRec->r2sp;
    db_post_events(pRec, &pRec->r2sp, DBE_VALUE|DBE_LOG);
  }

  /* BUG -- have to deal with ALT mode... if either is ALT, both must be ALT */
  pRec->lmd1 = pRec->mod1;
  EgSetSeqMode(pLink, 1, pRec->mod1);
  pRec->lmd2 = pRec->mod2;
  EgSetSeqMode(pLink, 2, pRec->mod2);

  if (pRec->enab)
    EgMasterEnable(pLink);
  else
    EgMasterDisable(pLink);
  pRec->lena = pRec->enab;

  if (EgCheckTaxi(pLink) != 0)
    pRec->taxi = 1;
  else
    pRec->taxi = 0;
  printf("end of EgInitEgRec\n");
  return(0);
}

/**
 *
 * Process an EG record.
 *
 **/
STATIC long EgProcEgRec(struct egRecord *pRec)
{
  EgLinkStruct	*pLink = &EgLink[pRec->out.value.vmeio.card];

#ifdef EG_DEBUG
  if (EgDebug>5) printf("EgProcEgRec \n");
#endif
  if (pRec->tpro > 10)
    epicsPrintf("devApsEg::proc(%s) link%d at %p\n", pRec->name, 
	pRec->out.value.vmeio.card, pLink);

  /* Check if the card is present */
  if (EgCheckCard(pRec->out.value.vmeio.card) != 0)
    return(0);

  semTake (pLink->EgLock, WAIT_FOREVER);

  {
    /* Master enable */
    /*if (pRec->enab != pRec->lena)*/
    if (pRec->enab != EgMasterEnableGet(pLink))
    {
      if (pRec->enab == 0)
      {
        if (pRec->tpro > 10)
          epicsPrintf(", Master Disable");
        EgMasterDisable(pLink);
      }
      else
      {
        if (pRec->tpro > 10)
          epicsPrintf(", Master Enable");
        EgMasterEnable(pLink);
      }
      pRec->lena = pRec->enab;
    }
  
    /* Check for a mode change. */
    if (pRec->mod1 != pRec->lmd1)
    {
      if (pRec->tpro > 10)
        epicsPrintf(", Mode1=%d", pRec->mod1);
      if (pRec->mod1 == egMOD1_Alternate)
      {
        pRec->mod1 = egMOD1_Alternate;
        pRec->lmd1 = egMOD1_Alternate;
        pRec->mod2 = egMOD1_Alternate;
        pRec->lmd2 = egMOD1_Alternate;

        pLink->Ram1Dirty = 1;
        pLink->Ram2Dirty = 1;
      }
      else if (pRec->lmd1 == egMOD1_Alternate)
      {
	pRec->mod2 = egMOD1_Off;
	pRec->lmd2 = egMOD1_Off;
	EgSetSeqMode(pLink, 2, egMOD1_Off);
	pLink->Ram1Dirty = 1;
	pLink->Ram2Dirty = 1;
      }
      EgSetSeqMode(pLink, 1, pRec->mod1);
      pRec->lmd1 = pRec->mod1;
    }
    if (pRec->mod2 != pRec->lmd2)
    {
      if (pRec->tpro > 10)
        epicsPrintf(", Mode2=%d", pRec->mod2);
      if (pRec->mod2 == egMOD1_Alternate)
      {
        pRec->mod1 = egMOD1_Alternate;
        pRec->lmd1 = egMOD1_Alternate;
        pRec->mod2 = egMOD1_Alternate;
        pRec->lmd2 = egMOD1_Alternate;

        pLink->Ram1Dirty = 1;
        pLink->Ram2Dirty = 1;
      }
      else if (pRec->lmd2 == egMOD1_Alternate)
      {
	pRec->mod1 = egMOD1_Off;
	pRec->lmd2 = egMOD1_Off;
	EgSetSeqMode(pLink, 1, egMOD1_Off);
	pLink->Ram1Dirty = 1;
	pLink->Ram2Dirty = 1;
      }
      EgSetSeqMode(pLink, 2, pRec->mod2);
      pRec->lmd2 = pRec->mod2;
    }
    /* Deal with FIFO enable flag */
    if (pRec->fifo != pRec->lffo)
    {
      if (pRec->fifo == 0)
      {
        if (pRec->tpro > 10)
          epicsPrintf(", FIFO Disable");
        EgDisableFifo(pLink);
      }
      else
      {
        if (pRec->tpro > 10)
          epicsPrintf(", FIFO Enable");
        EgEnableFifo(pLink);
      }
      pRec->lffo = pRec->fifo;
    }
  
    /* We use the manual triggers as one-shots.  They get reset to zero */
    if (pRec->trg1 != 0)
    {
      if (pRec->tpro > 10)
        epicsPrintf(", Trigger-1");
      EgSeqTrigger(pLink, 1);
      pRec->trg1 = 0;
    }
    if (pRec->trg2 != 0)
    {
      if (pRec->tpro > 10)
        printf(", Trigger-2");
      EgSeqTrigger(pLink, 2);
      pRec->trg2 = 0;
    }
  
    /* We use the clears as as one-shots.  They get reset to zero */
    if (pRec->clr1 !=0)
    {
      if (pRec->tpro > 10)
        epicsPrintf(", clear-1");
      EgClearSeq(pLink, 1);
      pRec->clr1 = 0;
    }
    if (pRec->clr2 !=0)
    {
      if (pRec->tpro > 10)
        epicsPrintf(", clear-2");
      EgClearSeq(pLink, 2);
      pRec->clr2 = 0;
    }
  
    /* We use the VME event trigger as a one-shot. It is reset to zero */
    if (pRec->vme != 0)
    {
      if (pRec->tpro > 10)
        epicsPrintf(", VME-%ld", pRec->vme);
      EgGenerateVmeEvent(pLink, pRec->vme);
      pRec->vme = 0;
    }
    /* Field to reset the counters is also an one-shot. It is reset to zero */
    if (pRec->mcrs != 0)
    {
      if (pRec->tpro > 10)
        epicsPrintf(", Reset MPX counters");
      EgResetMPX(pLink, pRec->mcrs);
      pRec->mcrs = 0;
    }

        
    /* 
     * If any triggers are enabled... set their values.
     * Otherwise, disable the associated trigger map register.
     *
     * NOTE:  The EG board only allows changes to the trigger codes
     * when the board is disabled.  Users of these triggers must disable
     * them, change the trigger event code and then re-enable them to
     * get them to work.
     */
    if (pRec->ete0)
    {
      if (pRec->et0 != EgGetTrigger(pLink, 0))
      {
        EgEnableTrigger(pLink, 0, 0);
        EgSetTrigger(pLink, 0, pRec->et0);
        pRec->let0 = pRec->et0;
      }
      EgEnableTrigger(pLink, 0, 1);
    }
    else
      EgEnableTrigger(pLink, 0, 0);

    if (pRec->ete1)
    {
      if (pRec->et1 != EgGetTrigger(pLink, 1))
      {
        EgEnableTrigger(pLink, 1, 0);
        EgSetTrigger(pLink, 1, pRec->et1);
        pRec->let1 = pRec->et1;
      }
      EgEnableTrigger(pLink, 1, 1);
    }
    else
      EgEnableTrigger(pLink, 1, 0);

    if (pRec->ete2)
    {
      if (pRec->et2 != EgGetTrigger(pLink, 2))
      {
        EgEnableTrigger(pLink, 2, 0);
        EgSetTrigger(pLink, 2, pRec->et2);
        pRec->let2 = pRec->et2;
      }
      EgEnableTrigger(pLink, 2, 1);
    }
    else
      EgEnableTrigger(pLink, 2, 0);

    if (pRec->ete3)
    {
      if (pRec->et3 != EgGetTrigger(pLink, 3))
      {
        EgEnableTrigger(pLink, 3, 0);
        EgSetTrigger(pLink, 3, pRec->et3);
        pRec->let3 = pRec->et3;
      }
      EgEnableTrigger(pLink, 3, 1);
    }
    else
      EgEnableTrigger(pLink, 3, 0);

    if (pRec->ete4)
    {
      if (pRec->et4 != EgGetTrigger(pLink, 4))
      {
        EgEnableTrigger(pLink, 4, 0);
        EgSetTrigger(pLink, 4, pRec->et4);
        pRec->let4 = pRec->et4;
      }
      EgEnableTrigger(pLink, 4, 1);
    }
    else
      EgEnableTrigger(pLink, 4, 0);

    if (pRec->ete5)
    {
      if (pRec->et5 != EgGetTrigger(pLink, 5))
      {
        EgEnableTrigger(pLink, 5, 0);
        EgSetTrigger(pLink, 5, pRec->et5);
        pRec->let5 = pRec->et5;
      }
      EgEnableTrigger(pLink, 5, 1);
    }
    else
      EgEnableTrigger(pLink, 5, 0);

    if (pRec->ete6)
    {
      if (pRec->et6 != EgGetTrigger(pLink, 6))
      {
        EgEnableTrigger(pLink, 6, 0);
        EgSetTrigger(pLink, 6, pRec->et6);
        pRec->let6 = pRec->et6;
      }
      EgEnableTrigger(pLink, 6, 1);
    }
    else
      EgEnableTrigger(pLink, 6, 0);

    if (pRec->ete7)
    {
      if (pRec->et7 != EgGetTrigger(pLink, 7))
      {
        EgEnableTrigger(pLink, 7, 0);
        EgSetTrigger(pLink, 7, pRec->et7);
        pRec->let7 = pRec->et7;
      }
      EgEnableTrigger(pLink, 7, 1);
    }
    else
      EgEnableTrigger(pLink, 7, 0);

    
  if (pRec->md0e != EgGetEnableMuxDistBus(pLink, 0)) EgEnableMuxDistBus(pLink, 0, pRec->md0e);
  if (pRec->md1e != EgGetEnableMuxDistBus(pLink, 1)) EgEnableMuxDistBus(pLink, 1, pRec->md1e);
  if (pRec->md2e != EgGetEnableMuxDistBus(pLink, 2)) EgEnableMuxDistBus(pLink, 2, pRec->md2e);
  if (pRec->md3e != EgGetEnableMuxDistBus(pLink, 3)) EgEnableMuxDistBus(pLink, 3, pRec->md3e);
  if (pRec->md4e != EgGetEnableMuxDistBus(pLink, 4)) EgEnableMuxDistBus(pLink, 4, pRec->md4e);
  if (pRec->md5e != EgGetEnableMuxDistBus(pLink, 5)) EgEnableMuxDistBus(pLink, 5, pRec->md5e);
  if (pRec->md6e != EgGetEnableMuxDistBus(pLink, 6)) EgEnableMuxDistBus(pLink, 6, pRec->md6e);
  if (pRec->md7e != EgGetEnableMuxDistBus(pLink, 7)) EgEnableMuxDistBus(pLink, 7, pRec->md7e);

  if (pRec->me0e != EgGetEnableMuxEvent(pLink, 0)) EgEnableMuxEvent(pLink, 0, pRec->me0e);
  if (pRec->me1e != EgGetEnableMuxEvent(pLink, 1)) EgEnableMuxEvent(pLink, 1, pRec->me1e);
  if (pRec->me2e != EgGetEnableMuxEvent(pLink, 2)) EgEnableMuxEvent(pLink, 2, pRec->me2e);
  if (pRec->me3e != EgGetEnableMuxEvent(pLink, 3)) EgEnableMuxEvent(pLink, 3, pRec->me3e);
  if (pRec->me4e != EgGetEnableMuxEvent(pLink, 4)) EgEnableMuxEvent(pLink, 4, pRec->me4e);
  if (pRec->me5e != EgGetEnableMuxEvent(pLink, 5)) EgEnableMuxEvent(pLink, 5, pRec->me5e);
  if (pRec->me6e != EgGetEnableMuxEvent(pLink, 6)) EgEnableMuxEvent(pLink, 6, pRec->me6e);
  if (pRec->me7e != EgGetEnableMuxEvent(pLink, 7)) EgEnableMuxEvent(pLink, 7, pRec->me7e);

 
    if (pRec->md0e || pRec->me0e)
    {
      EgGetMuxPrescaler(pLink, 0);
      if (pRec->mc0p != EgGetMuxPrescaler(pLink, 0))
      {
	EgEnableMuxDistBus(pLink, 0, 0);
	EgEnableMuxEvent(pLink, 0, 0);
        EgSetMuxPrescaler(pLink, 0, pRec->mc0p);
      }
      if (pRec->md0e) EgEnableMuxDistBus(pLink, 0, 1);
      if (pRec->me0e) EgEnableMuxEvent(pLink, 0, 1);
   }
    else
      {
	EgEnableMuxDistBus(pLink, 0, 0);
	EgEnableMuxEvent(pLink, 0, 0);
      }
	
    
    if (pRec->md1e || pRec->me1e)
    {
      EgGetMuxPrescaler(pLink, 1);
      if (pRec->mc1p != EgGetMuxPrescaler(pLink, 1))
      {
	EgEnableMuxDistBus(pLink, 1, 0);
	EgEnableMuxEvent(pLink, 1, 0);
        EgSetMuxPrescaler(pLink, 1, pRec->mc1p);
      }
      if (pRec->md1e) EgEnableMuxDistBus(pLink, 1, 1);
      if (pRec->me1e) EgEnableMuxEvent(pLink, 1, 1);
   }
    else
      {
	EgEnableMuxDistBus(pLink, 1, 0);
	EgEnableMuxEvent(pLink, 1, 0);
      }
    
    
    if (pRec->md2e || pRec->me2e)
    {
      EgGetMuxPrescaler(pLink, 2);
      if (pRec->mc2p != EgGetMuxPrescaler(pLink, 2))
      {
	EgEnableMuxDistBus(pLink, 2, 0);
	EgEnableMuxEvent(pLink, 2, 0);
        EgSetMuxPrescaler(pLink, 2, pRec->mc2p);
      }
      if (pRec->md2e) EgEnableMuxDistBus(pLink, 2, 1);
      if (pRec->me2e) EgEnableMuxEvent(pLink, 2, 1);
   }
    else
      {
	EgEnableMuxDistBus(pLink, 2, 0);
	EgEnableMuxEvent(pLink, 2, 0);
      }
    
    
    if (pRec->md3e || pRec->me3e)
    {
      EgGetMuxPrescaler(pLink, 3);
      if (pRec->mc3p != EgGetMuxPrescaler(pLink, 3))
      {
	EgEnableMuxDistBus(pLink, 3, 0);
	EgEnableMuxEvent(pLink, 3, 0);
        EgSetMuxPrescaler(pLink, 3, pRec->mc3p);
      }
      if (pRec->md3e) EgEnableMuxDistBus(pLink, 3, 1);
      if (pRec->me3e) EgEnableMuxEvent(pLink, 3, 1);
   }
    else
      {
	EgEnableMuxDistBus(pLink, 3, 0);
	EgEnableMuxEvent(pLink, 3, 0);
      }
    
    
    if (pRec->md4e || pRec->me4e)
    {
      EgGetMuxPrescaler(pLink, 4);
      if (pRec->mc4p != EgGetMuxPrescaler(pLink, 4))
      {
	EgEnableMuxDistBus(pLink, 4, 0);
	EgEnableMuxEvent(pLink, 4, 0);
        EgSetMuxPrescaler(pLink, 4, pRec->mc4p);
      }
      if (pRec->md4e) EgEnableMuxDistBus(pLink, 4, 1);
      if (pRec->me4e) EgEnableMuxEvent(pLink, 4, 1);
   }
    else
      {
	EgEnableMuxDistBus(pLink, 4, 0);
	EgEnableMuxEvent(pLink, 4, 0);
      }
    
    
    if (pRec->md5e || pRec->me5e)
    {
      EgGetMuxPrescaler(pLink, 5);
      if (pRec->mc5p != EgGetMuxPrescaler(pLink, 5))
      {
	EgEnableMuxDistBus(pLink, 5, 0);
	EgEnableMuxEvent(pLink, 5, 0);
        EgSetMuxPrescaler(pLink, 5, pRec->mc5p);
      }
      if (pRec->md5e) EgEnableMuxDistBus(pLink, 5, 1);
      if (pRec->me5e) EgEnableMuxEvent(pLink, 5, 1);
   }
    else
      {
	EgEnableMuxDistBus(pLink, 5, 0);
	EgEnableMuxEvent(pLink, 5, 0);
      }
    
    if (pRec->md6e || pRec->me6e)
    {
      EgGetMuxPrescaler(pLink, 6);
      if (pRec->mc6p != EgGetMuxPrescaler(pLink, 6))
      {
	EgEnableMuxDistBus(pLink, 6, 0);
	EgEnableMuxEvent(pLink, 6, 0);
        EgSetMuxPrescaler(pLink, 6, pRec->mc6p);
      }
      if (pRec->md6e) EgEnableMuxDistBus(pLink, 6, 1);
      if (pRec->me6e) EgEnableMuxEvent(pLink, 6, 1);
   }
    else
      {
	EgEnableMuxDistBus(pLink, 6, 0);
	EgEnableMuxEvent(pLink, 6, 0);
      }
    
    if (pRec->md7e || pRec->me7e || pRec->msq1 || pRec->msq2)
    {
      EgGetMuxPrescaler(pLink, 7);
      if (pRec->mc7p != EgGetMuxPrescaler(pLink, 7))
      {
	EgEnableMuxDistBus(pLink, 7, 0);
	EgEnableMuxEvent(pLink, 7, 0);
        EgEnableMuxSeq(pLink, 1, 0);
        EgEnableMuxSeq(pLink, 2, 0);
        EgSetMuxPrescaler(pLink, 7, pRec->mc7p);
      }
      if (pRec->md7e) EgEnableMuxDistBus(pLink, 7, 1);
      if (pRec->me7e) EgEnableMuxEvent(pLink, 7, 1);
      if (pRec->msq1) EgEnableMuxSeq(pLink, 1, 1);
      if (pRec->msq2) EgEnableMuxSeq(pLink, 2, 1);
   }
    else
      {
	EgEnableMuxDistBus(pLink, 7, 0);
	EgEnableMuxEvent(pLink, 7, 0);
        EgEnableMuxSeq(pLink, 1, 0);
        EgEnableMuxSeq(pLink, 2, 0);
      }
    
  if (pRec->asyn != EgGetACinputControl(pLink, 0))
  	EgSetACinputControl(pLink, 0, pRec->asyn);

  if (pRec->aev0 != EgGetACinputControl(pLink, 1))
	EgSetACinputControl(pLink, 1, pRec->aev0);
  
  if ( pRec->asq1 != EgGetACinputControl(pLink, 2))
	EgSetACinputControl(pLink, 2, pRec->asq1);
	
  if ( pRec->asq2 != EgGetACinputControl(pLink, 3))
	EgSetACinputControl(pLink, 3, pRec->asq2);
	
  if ( pRec->adiv != EgGetACinputDivisor(pLink, 0))
	EgSetACinputDivisor(pLink, pRec->adiv, 0);

  if ( pRec->aphs != EgGetACinputDivisor(pLink, 1))
	EgSetACinputDivisor(pLink, pRec->aphs, 1);

  if ( pRec->msq1 != EgGetEnableMuxSeq(pLink, 1))
	EgEnableMuxSeq(pLink, 1, pRec->msq1);
	
  if ( pRec->msq2 != EgGetEnableMuxSeq(pLink, 2))
	EgEnableMuxSeq(pLink, 2, pRec->msq2);
    
    
    
    
    /* TAXI stuff? */
    if (EgCheckTaxi(pLink) != 0)
      pRec->taxi = 1;

    /* clock change? */
    if (pRec->r1ck != pRec->lr1c)
      {
	pRec->lr1c=EgRamClockGet(pLink,1);
	EgRamClockSet(pLink,1,pRec->r1ck);
      }
    if (pRec->r2ck != pRec->lr2c)
      {
	pRec->lr2c=EgRamClockGet(pLink,2);
	EgRamClockSet(pLink,2,pRec->r2ck);
      }
    /* set RAM speed if the internal clock divider is used */
    if (pRec->r1ck != 0) {
      pRec->r1sp = FRAME_CLOCK /  (pRec->r1ck+1);
      pLink->Ram1Speed = pRec->r1sp;
      db_post_events(pRec, &pRec->r1sp, DBE_VALUE|DBE_LOG);
    }
    if (pRec->r2ck != 0) {
      pRec->r2sp = FRAME_CLOCK / (pRec->r2ck+1);
      pLink->Ram2Speed = pRec->r2sp;
      db_post_events(pRec, &pRec->r2sp, DBE_VALUE|DBE_LOG);
    }

    if (pRec->tpro > 10)
      epicsPrintf("\n");
    if(pRec->rmax >  EG_SEQ_RAM_SIZE) {
      pLink->MaxRamPos = pRec->rmax;
      pRec->rmax = EG_SEQ_RAM_SIZE;
    } else {
      pLink->MaxRamPos = pRec->rmax;
    }

  }
  semGive (pLink->EgLock);
  EgScheduleRamProgram(pRec->out.value.vmeio.card);
  pRec->udf = 0;
if (EgDebug>5)  printf("end of EgProcEgRec \n");
  return(0);
}

/** 
   Device Support Entry Table (dset) for EG records
 **/
struct {
  long	number;
  long (*p1)();
  long (*p2)();
  long (*p3)();
  long (*p4)();
  long (*p5)();
} devEg={ 5, NULL, EgInitDev, EgInitEgRec, NULL, EgProcEgRec};

/**
 *
 * Support for initialization of egevent records.
 *
 **/
STATIC long EgInitEgEventRec(struct egeventRecord *pRec)
{
  double RamSpeed;

#ifdef EG_DEBUG
    printf("EgInitEgEventRec\n");
#endif
  if (EgCheckCard(pRec->out.value.vmeio.card) != 0)
  {
    recGblRecordError(S_db_badField, (void *)pRec, "devApsEg::EgInitEgEventRec() bad card number");
    return(S_db_badField);
  }
  
  pRec->self = pRec;
  pRec->lram = pRec->ram;
  pRec->levt = 0;		/* force program on first process */

  /* Scale delay to actual position */
  if (pRec->ram == egeventRAM_RAM_2)
    RamSpeed = EgLink[pRec->out.value.vmeio.card].Ram2Speed;
  else
    RamSpeed = EgLink[pRec->out.value.vmeio.card].Ram1Speed;
  
  switch (pRec->unit)
    {
    case REC_EGEVENT_UNIT_TICKS:
      pRec->dpos = pRec->dely;
      break;
    case REC_EGEVENT_UNIT_FORTNIGHTS:
      pRec->dpos = ((float)pRec->dely * 60.0 * 60.0 * 24.0 * 14.0) * RamSpeed;
      break;
    case REC_EGEVENT_UNIT_WEEKS:
      pRec->dpos = ((float)pRec->dely * 60.0 * 60.0 * 24.0 * 7.0) * RamSpeed;
      break;
    case REC_EGEVENT_UNIT_DAYS:
      pRec->dpos = ((float)pRec->dely * 60.0 * 60.0 * 24.0) * RamSpeed;
      break;
    case REC_EGEVENT_UNIT_HOURS:
      pRec->dpos = ((float)pRec->dely * 60.0 *60.0) * RamSpeed;
      break;
    case REC_EGEVENT_UNIT_MINUITES:
      pRec->dpos = ((float)pRec->dely * 60.0) * RamSpeed;
      break;
    case REC_EGEVENT_UNIT_SECONDS:
      pRec->dpos = pRec->dely * RamSpeed;
      break;
    case REC_EGEVENT_UNIT_MILLISECONDS:
      pRec->dpos = ((float)pRec->dely/1000.0) * RamSpeed;
      break;
    case REC_EGEVENT_UNIT_MICROSECONDS:
      pRec->dpos = ((float)pRec->dely/1000000.0) * RamSpeed;
      break;
    case REC_EGEVENT_UNIT_NANOSECONDS:
      pRec->dpos = ((float)pRec->dely/1000000000.0) * RamSpeed;
      break;
    }
  
  /* Put the event record in the proper list */
  semTake (EgLink[pRec->out.value.vmeio.card].EgLock, WAIT_FOREVER);
  if (pRec->ram == egeventRAM_RAM_2)
  {
    EgPlaceRamEvent(pRec);
    EgLink[pRec->out.value.vmeio.card].Ram2Dirty = 1;
  }
  else /* RAM_1 */
  {
    EgPlaceRamEvent(pRec);
    EgLink[pRec->out.value.vmeio.card].Ram1Dirty = 1;
  }
  semGive(EgLink[pRec->out.value.vmeio.card].EgLock);
  return(0);
}
/**
 *
 * Process an EGEVENT record.
 *
 **/
STATIC long EgProcEgEventRec(struct egeventRecord *pRec)
{
  MrfEVGStruct   *pLink = EgLink[pRec->out.value.vmeio.card].pEg;
  double	RamSpeed;
  
#ifdef EG_DEBUG
    printf("EgProcEgEventRec\n");
#endif
  if (pRec->tpro > 10)
    epicsPrintf("devApsEg::EgProcEgEventRec(%s) link%d at %p\n", pRec->name,
        pRec->out.value.vmeio.card, pLink);

  /* Check if the card is present */
  if (EgCheckCard(pRec->out.value.vmeio.card) != 0)
    return(0);

  /* Check for ram# change */
  if (pRec->ram != pRec->lram)
  {
    EgLink[pRec->out.value.vmeio.card].Ram1Dirty = 1;
    EgLink[pRec->out.value.vmeio.card].Ram2Dirty = 1;

    if (pRec->tpro > 10)
      epicsPrintf("devApsEg::EgProcEgEventRec(%s) ram-%d\n", pRec->name, pRec->ram);

    semTake (EgLink[pRec->out.value.vmeio.card].EgLock, WAIT_FOREVER);
    
    EgPlaceRamEvent(pRec);
    semGive(EgLink[pRec->out.value.vmeio.card].EgLock);
    pRec->lram = pRec->ram;
  }

  if (pRec->ram == egeventRAM_RAM_2)
    RamSpeed = EgLink[pRec->out.value.vmeio.card].Ram2Speed;
  else
    RamSpeed = EgLink[pRec->out.value.vmeio.card].Ram1Speed;

  if (pRec->enm != pRec->levt)
  {
    if (pRec->ram == egeventRAM_RAM_2)
      EgLink[pRec->out.value.vmeio.card].Ram2Dirty = 1;
    else
      EgLink[pRec->out.value.vmeio.card].Ram1Dirty = 1;
    pRec->levt = pRec->enm;
  }
  /* Check for time/position change */

  if (pRec->dely != pRec->ldly)
  {
    /* Scale delay to actual position */
    switch (pRec->unit)
    {
    case REC_EGEVENT_UNIT_TICKS:
      pRec->dpos = pRec->dely;
      break;
    case REC_EGEVENT_UNIT_FORTNIGHTS:
      pRec->dpos = ((float)pRec->dely * 60.0 * 60.0 * 24.0 * 14.0) * RamSpeed;
      break;
    case REC_EGEVENT_UNIT_WEEKS:
      pRec->dpos = ((float)pRec->dely * 60.0 * 60.0 * 24.0 * 7.0) * RamSpeed;
      break;
    case REC_EGEVENT_UNIT_DAYS:
      pRec->dpos = ((float)pRec->dely * 60.0 * 60.0 * 24.0) * RamSpeed;
      break;
    case REC_EGEVENT_UNIT_HOURS:
      pRec->dpos = ((float)pRec->dely * 60.0 *60.0) * RamSpeed;
      break;
    case REC_EGEVENT_UNIT_MINUITES:
      pRec->dpos = ((float)pRec->dely * 60.0) * RamSpeed;
      break;
    case REC_EGEVENT_UNIT_SECONDS:
      pRec->dpos = pRec->dely * RamSpeed;
      break;
    case REC_EGEVENT_UNIT_MILLISECONDS:
      pRec->dpos = ((float)pRec->dely/1000.0) * RamSpeed;
      break;
    case REC_EGEVENT_UNIT_MICROSECONDS:
      pRec->dpos = ((float)pRec->dely/1000000.0) * RamSpeed;
      break;
    case REC_EGEVENT_UNIT_NANOSECONDS:
      pRec->dpos = ((float)pRec->dely/1000000000.0) * RamSpeed;
      break;
    }
    if (pRec->tpro)
      epicsPrintf("EgProcEgEventRec(%s) dpos=%ld\n", pRec->name, pRec->dpos);
    
    semTake (EgLink[pRec->out.value.vmeio.card].EgLock, WAIT_FOREVER);

    /* this line causes reprogramming of RAM after delay change */
    EgPlaceRamEvent(pRec);
    semGive(EgLink[pRec->out.value.vmeio.card].EgLock);

    pRec->ldly = pRec->dely;
    if (pRec->ram == egeventRAM_RAM_2)
      EgLink[pRec->out.value.vmeio.card].Ram2Dirty = 1;
    else
      EgLink[pRec->out.value.vmeio.card].Ram1Dirty = 1;
    db_post_events(pRec, &pRec->adly, DBE_VALUE|DBE_LOG);
    db_post_events(pRec, &pRec->dpos, DBE_VALUE|DBE_LOG);
    db_post_events(pRec, &pRec->apos, DBE_VALUE|DBE_LOG);
      
      
  }
  EgScheduleRamProgram(pRec->out.value.vmeio.card);

  return(0);
}

/* Device Support Table (dset) for egevent records */
 
struct {
  long	number;
  long (*p1)();
  long (*p2)();
  long (*p3)();
  long (*p4)();
  long (*p5)();
} devEgEvent={ 5, NULL, EgInitDev, EgInitEgEventRec, NULL, EgProcEgEventRec};

/******************************************************************************
 *
 * Driver support functions.
 *
 * These are service routines to support the record init and processing
 * functions above.
 *
 ******************************************************************************/
/**
 *
 * Place an egevent record to the linked list, in delay time (dely) order.
 * The end result is a list in which all egevent records are stored in
 * the order of apos, smallest first.
 * The priority (VAL field) is checked so that a high priority event
 * gets its desired delay always and a lower priority event is delayed.
 * Algorithm description:
 * 	take ptr=first record in list
 *	while ptr!=NULL && not done
 *		if new->apos < ptr->apos
 *			insert 'new' before 'ptr'. Done
 *		else if new->apos == ptr->apos
 *			if ptr->prio < new->prio //list event has higher prio, leave it in place
 *			else
 *			//replace 'ptr' with 'new' and continue to find a place for the replaced one.
 *				insert 'new' after 'ptr'
 *				remove 'ptr'
 *				new = ptr
 *			new->apos++ 
 *		else proceed to next element in list
 *	//end while
 *	if ptr ==NULL 
 *		// nothing was done. Simply add the new event to the end of list
 *		
 *
 **/
STATIC long EgPlaceRamEvent(struct egeventRecord *pRec)
{
  
  ELLNODE			*pNode;
  ELLLIST                        *pList;
  struct egeventRecord		*pListEvent;
  struct egeventRecord		*pNew;
  int nth; /*nth element in list*/
  int inserted;

#ifdef EG_DEBUG
  printf("EgPlaceRamEvent\n");
#endif
  inserted = 0;
  pNew = pRec;
  pNew->apos=pNew->dpos;
  pList = (&(EgLink[pRec->out.value.vmeio.card].EgEvent));
  /* remove the record form the list if it already is there */
  nth = ellFind(pList,&(pRec->eln));
  if (nth!=-1) {
   pNode=ellNth(pList,nth);
   if(pNode!=NULL) 
   {
#ifdef EG_DEBUG     
     epicsPrintf("found record %s, remove from list before re-insert\n",(((EgEventNode *)pNode)->pRec)->name);
#endif     
     ellDelete(pList,pNode);
   }
   }
  /* now the real business */
  pNode = ellFirst(pList);
  while((pNode !=NULL) & (!inserted))
    {
      pListEvent = ((EgEventNode *)pNode)->pRec;
#ifdef EG_DEBUG
      epicsPrintf("considering record %s (%ld) vs %s (%ld)\n",pNew->name,pNew->dpos,
		  pListEvent->name, pListEvent->dpos);
#endif
      if (pNew->apos < pListEvent->apos )
      {
	  ellInsert(pList, pNode->previous, &(pNew->eln));
	  inserted = 1;
      } else if (pNew->apos == pListEvent->apos )
      {

      if (pListEvent->val <= pNew->val) 
	{
	  pNode=ellNext(pNode); /* new has lower or equal priority, do not move old */
	  pListEvent = ((EgEventNode *)pNode)->pRec;
	}
      else  
      {
		ellInsert(pList, pNode, &(pNew->eln));
		ellDelete(pList,pNode);
		pNode=ellNext(pNode); /* skip to the next node */
		pNew=pListEvent;
	}
	pNew->apos++;	
	} else
	      {
		pNode=ellNext(pNode);
	      }
      }
  if(!inserted)  ellAdd(pList, &(pNew->eln));
#ifdef EG_DEBUG
  epicsPrintf("record %s inserted \n",pNew->name);
#endif
  return(0);
}

/**
 *
 * Return the event code from the requested RAM and physical position.
 *
 **/
STATIC long EgGetRamEvent(EgLinkStruct *pParm, long Ram, long Addr)
{
  volatile MrfEVGStruct *pEg = pParm->pEg;
  volatile unsigned short temp, temp2;

#ifdef EG_DEBUG 
  printf("EgGetRamEvent \n");
#endif
  if (EgSeqEnableCheck(pParm, Ram))     /* Can't alter a running RAM */
    return(-2);

  if (Ram == 1)
  {
    pEg->Seq1Addr = Addr;
    for(temp2=0;temp2<10;temp2++); /* HACK - delay to let address settle */
    temp = pEg->Seq1Time;
    return(temp);
  }
  else
  {
    pEg->Seq2Addr = Addr;
    for(temp2=0;temp2<10;temp2++); /* HACK - delay to let address settle */
    temp = pEg->Seq2Time;
    return(temp);
  }
}

/**
 *
 * Return a zero if no TAXI violation status set, one otherwise.
 *
 **/
STATIC long EgCheckTaxi(EgLinkStruct *pParm)
{
  volatile MrfEVGStruct *pEg = pParm->pEg;
#ifdef EG_DEBUG
  printf("EgCheckTaxi\n");
#endif
  return(pEg->Control & 0x0001);
}

/**
 *
 * Shut down the in-bound FIFO.
 *
 **/
STATIC long EgDisableFifo(EgLinkStruct *pParm)
{
  volatile MrfEVGStruct *pEg = pParm->pEg;

#ifdef EG_DEBUG
  printf("EgDisableFifo \n");
#endif
  pEg->Control = (pEg->Control&CTL_OR_MASK)|0x1000;	/* Disable FIFO */
  pEg->Control = (pEg->Control&CTL_OR_MASK)|0x2001;	/* Reset FIFO (plus any taxi violations) */

  return(0);
}

/**
 *
 * Turn on the in-bound FIFO.
 *
 **/
STATIC long EgEnableFifo(EgLinkStruct *pParm)
{
  volatile MrfEVGStruct *pEg = pParm->pEg;

#ifdef EG_DEBUG
  printf("EgEnableFifo \n");
#endif
  if (pEg->Control & 0x1000)	/* If enabled already, forget it */
  {
    pEg->Control = (pEg->Control&CTL_OR_MASK)|0x2001;	/* Reset FIFO & taxi */
    pEg->Control = (pEg->Control&CTL_OR_MASK)&~0x1000;	/* Enable the FIFO */
  }
  return(0);
}

/**
 *
 * Return a 1 if the FIFO is enabled and a zero otherwise.
 *
 **/
STATIC long EgCheckFifo(EgLinkStruct *pParm)
{
  volatile MrfEVGStruct *pEg = pParm->pEg;

  if (pEg->Control & 0x1000)
    return(0);
  return(1);
}

/**
 *
 * Turn on the master enable.
 *
 **/
STATIC long EgMasterEnable(EgLinkStruct *pParm)
{
  volatile MrfEVGStruct *pEg = pParm->pEg;

#ifdef EG_DEBUG
  printf("EgMasterEnable \n");
#endif
  pEg->Control = (pEg->Control&CTL_OR_MASK)&~0x8000;	/* Clear the disable bit */
  return(0);
}

/**
 *
 * Turn off the master enable.
 *
 **/
STATIC long EgMasterDisable(EgLinkStruct *pParm)
{
  volatile MrfEVGStruct *pEg = pParm->pEg;

#ifdef EG_DEBUG
  printf("EgMasterDisable \n");
#endif
  pEg->Control = (pEg->Control&CTL_OR_MASK)|0x8000;	/* Set the disable bit */
  return(0);
}

/**
 *
 * Return the value of the RAM clock register.
 *
 **/
STATIC long EgRamClockGet(EgLinkStruct *pParm, long Ram)
{
  volatile MrfEVGStruct *pEg = pParm->pEg;
  long Clock=-1;

#ifdef EG_DEBUG
  printf("EgRamClockGet \n");
#endif
  if (Ram ==1) {
    Clock = pEg -> Seq1ClockSel;
  } else if (Ram ==2) {
    Clock = pEg -> Seq2ClockSel;
  } else
    return(-1);

  return(Clock);
}
/**
 *
 * Set the value of the RAM clock.
 *
 **/
STATIC long EgRamClockSet(EgLinkStruct *pParm, long Ram, long Clock)
{
  volatile MrfEVGStruct *pEg = pParm->pEg;

#ifdef EG_DEBUG
  printf("EgRamclockSet\n");
#endif
  if (Ram ==1 ) {
    pEg -> Seq1ClockSel = Clock;
  } else if (Ram ==2) {
    pEg -> Seq2ClockSel = Clock;
  } else
    return(-1);
  return(0);
}

/**
 *
 * Return a one of the master enable is on, and a zero otherwise.
 *
 **/
STATIC long EgMasterEnableGet(EgLinkStruct *pParm)
{
/* 
#ifdef EG_DEBUG
  printf("EgMasterEnableGet \n");
#endif
*/
  if (pParm->pEg->Control&0x8000)
    return(0);

  return(1);
}

/**
 *
 * Clear the requested sequence RAM.
 *
 **/
STATIC long EgClearSeq(EgLinkStruct *pParm, int channel)
{
  volatile MrfEVGStruct *pEg = pParm->pEg;

#ifdef EG_DEBUG
  printf("EgClearSeq \n");
#endif
  if (channel == 1)
  {
    while(pEg->Control & 0x0004)
      taskDelay(1);		/* Wait for running seq to finish */
  
    pEg->EventMask &= ~0x0004;	/* Turn off seq 1 */
    pEg->Control = (pEg->Control&CTL_OR_MASK)|0x0004;	/* Reset seq RAM address */
  }
  else if(channel == 2)
  {
    while(pEg->Control & 0x0002)
      taskDelay(1);
  
    pEg->EventMask &= ~0x0002;
    pEg->Control = (pEg->Control&CTL_OR_MASK)|0x0002;
    }
  return(0);
}

/**
 *
 * Set the trigger event code for a given channel number.
 *
 **/
STATIC long EgSetTrigger(EgLinkStruct *pParm, unsigned int Channel, unsigned short Event)
{
  volatile MrfEVGStruct	*pEg = pParm->pEg;
  volatile unsigned short *pShort;
  
#ifdef EG_DEBUG
  printf("EgSetTrigger \n");
#endif
  if(Channel > 7)
    return(-1);

  pShort = &(pEg->Event0Map);
  pShort[Channel] = Event;

  return(0);
}

/**
 *
 * Return the event code for the requested trigger channel.
 *
 **/
STATIC long EgGetTrigger(EgLinkStruct *pParm, unsigned int Channel)
{
  volatile MrfEVGStruct  *pEg = pParm->pEg;

#ifdef EG_DEBUG
  if (EgDebug>10) printf("EgGetTrigger \n");
#endif
  if(Channel > 7)
    return(0);
  return(((unsigned short *)(&(pEg->Event0Map)))[Channel]);
}

/**
 *
 * Enable or disable event triggering for a given Channel.
 *
 **/
STATIC long EgEnableTrigger(EgLinkStruct *pParm, unsigned int Channel, int state)
{
  volatile MrfEVGStruct  *pEg = pParm->pEg;
  unsigned short	j;

#ifdef EG_DEBUG
  if (EgDebug>10) printf("EgEnableTrigger \n");
#endif
  if (Channel > 7)
    return(-1);

  j = 0x08;
  j <<= Channel;

  if (state)
    pEg->EventMask |= j;
  else
    pEg->EventMask &= ~j;

  return(0);
}
/**
 Get Trigger Enable Status 
 **/

STATIC long EgGetEnableTrigger(EgLinkStruct *pParm, unsigned int Channel)
{
  volatile MrfEVGStruct  *pEg = pParm->pEg;
  unsigned short        j;

  if (Channel > 7)
    return(0);

  j = 0x08;
  j <<= Channel;

  return(pEg->EventMask & j ?1:0);
}
/**
 Get DB Mux Enable Status 
 **/
STATIC long EgGetEnableMuxDistBus(EgLinkStruct *pParm, unsigned int Channel)
{
  volatile MrfEVGStruct  *pEg = pParm->pEg;
  unsigned short        j;

#ifdef EG_DEBUG
  if (EgDebug>10) printf("EgGetEnableMuxDistBus \n");
#endif
  if (Channel > 7)
    return(0);

  j = 0x0100;
  j <<= Channel;

  return(pEg->MuxCountDbEvEn & j ?1:0);
}
/**
 *  Enable event generation from mux counter 
 **/

STATIC long EgGetEnableMuxEvent(EgLinkStruct *pParm, unsigned int Channel)
{
  volatile MrfEVGStruct  *pEg = pParm->pEg;
  unsigned short        j;

#ifdef EG_DEBUG
  if (EgDebug>10) printf("EgGetEnableMuxEvent \n");
#endif
  if (Channel > 7)
    return(0);

  j = 0x01;
  j <<= Channel;

  return(pEg->MuxCountDbEvEn & j ?1:0);
}

/**
 * Enable Mux counter output to distributed bus
 **/

STATIC long EgEnableMuxDistBus(EgLinkStruct *pParm, unsigned int Channel, int state)
{
  volatile MrfEVGStruct  *pEg = pParm->pEg;
  unsigned short	j;

#ifdef EG_DEBUG
  if (EgDebug>10) printf("EgEnableMuxDistBus\n");
#endif
  if (Channel > 7)
    return(-1);

  j = 0x0100;
  j <<= Channel;

  if (state)
    pEg->MuxCountDbEvEn |= j;
  else
    pEg->MuxCountDbEvEn &= ~j;

  return(0);
}
/**
 Enable event generation from Mux counter 
 **/
STATIC long EgEnableMuxEvent(EgLinkStruct *pParm, unsigned int Channel, int state)
{
  volatile MrfEVGStruct  *pEg = pParm->pEg;
  unsigned short	j;

  if (Channel > 7)
    return(-1);

#ifdef EG_DEBUG
  if (EgDebug>10) printf("EgEnableMuxEvent\n");
#endif
  j = 0x0001;
  j <<= Channel;

  if (state)
    pEg->MuxCountDbEvEn |= j;
  else
    pEg->MuxCountDbEvEn &= ~j;

  return(0);
}
/**
 Get AC input control status
 **/

STATIC long EgGetACinputControl(EgLinkStruct *pParm, unsigned int bit)
{
  volatile MrfEVGStruct  *pEg = pParm->pEg;
  unsigned short        j;

#ifdef EG_DEBUG
  if (EgDebug>10) printf("EgGetACinputControl\n");
#endif
  if (bit > 3)
    return(-1);

  j = 0x1000;
  j <<= bit;

  return(pEg->ACinputControl & j ?1:0);
}

/**
 Enable AC synchronization of Mux counter 
 **/
STATIC long EgSetACinputControl(EgLinkStruct *pParm, unsigned int bit, int state)
{
  volatile MrfEVGStruct  *pEg = pParm->pEg;
  unsigned short	j;

#ifdef EG_DEBUG
  if (EgDebug>10) printf("EgSetACinputControl\n");
#endif
  if (bit > 7)
    return(-1);

  j = 0x1000;
  j <<= bit;

  if (state)
    pEg->ACinputControl |= j;
  else
    pEg->ACinputControl &= ~j;

  return(0);
}

/**
 * read AC phase shift or divisor 
 */
STATIC long EgGetACinputDivisor(EgLinkStruct *pParm, unsigned short DlySel)
{
  volatile MrfEVGStruct *pEg = pParm->pEg;
  unsigned char Divisor=0;
  
#ifdef EG_DEBUG
  if (EgDebug>10) printf("EgGetACinputDivisor \n");
#endif
  DlySel <<= 8; 
  DlySel &= 0x0100; 
  if(DlySel) 
  	pEg -> ACinputControl |= DlySel;
	
  else 
  	pEg -> ACinputControl &= ~(0x0100);
	
  Divisor = pEg -> ACinputControl;
  return(Divisor);
}
/**
 * return AC phase shift or divisor 
 */

STATIC long EgSetACinputDivisor(EgLinkStruct *pParm, unsigned short Divisor, unsigned short DlySel)
{
  volatile MrfEVGStruct *pEg = pParm->pEg;
  unsigned char LocalDivisor=0;
  DlySel <<= 8; 
  DlySel &= 0x0100; 
  LocalDivisor = Divisor & 0x00ff;
 
#ifdef EG_DEBUG
  if (EgDebug>10) printf("EgSetACinputDivisor\n");
#endif 
  if(DlySel) 
	pEg -> ACinputControl |= DlySel;
  else 
  	pEg -> ACinputControl &= ~(0x0100);
	
     pEg -> ACinputControl = (pEg -> ACinputControl & 0x0ff00) | LocalDivisor;
     
  return(0);
}
/**
 * reset (resynchronize) Multiplexed counter 
 */

STATIC long EgResetMuxCounter(EgLinkStruct *pParm, unsigned int Channel)
{
  volatile MrfEVGStruct  *pEg = pParm->pEg;
  unsigned short	j;

#ifdef EG_DEBUG
  if (EgDebug>5) printf("EgResetMuxCounter\n");
#endif
  if (Channel > 7)
    return(-1);

  j = 0x0100;
  j <<= Channel;

    pEg->MuxCountSelect |= j;

  return(0);
}

/**
 * reset multiple MPX counters 
 */

STATIC long EgResetMPX(EgLinkStruct *pParm, unsigned int Mask)
{
  volatile MrfEVGStruct  *pEg = pParm->pEg;
  unsigned short	j;

#ifdef EG_DEBUG
  if (EgDebug>5) printf("EgResetMPX\n");
#endif
  j = Mask;
  j <<= 8;

    pEg->MuxCountSelect |= j;

  return(0);
}

STATIC long EgGetEnableMuxSeq(EgLinkStruct *pParm, unsigned int SeqNum)
{
  volatile MrfEVGStruct  *pEg = pParm->pEg;
  unsigned short        j;

#ifdef EG_DEBUG
    if (EgDebug>10) printf("EgGetEnableMuxSeq\n");
#endif
  if (SeqNum > 2 || SeqNum == 0)
    return(-1);

  j = 0x0020;
  j <<= SeqNum;

  return(pEg->MuxCountSelect & j ?1:0);
}

STATIC long EgEnableMuxSeq(EgLinkStruct *pParm, unsigned int SeqNum, int state)
{
  volatile MrfEVGStruct  *pEg = pParm->pEg;
  unsigned short	j;

#ifdef EG_DEBUG
    if (EgDebug>10) printf("EgEnableMuxSeq\n");
#endif
  if (SeqNum > 2 || SeqNum == 0)
    return(-1);

  j = 0x0020;
  j <<= SeqNum;

  if (state)
    pEg->MuxCountSelect |= j;
  else
    pEg->MuxCountSelect &= ~j;

  return(0);
}

STATIC unsigned long EgGetMuxPrescaler(EgLinkStruct *pParm, unsigned short Channel)
{
  volatile MrfEVGStruct *pEg = pParm->pEg;
  unsigned long PreScaleVal=0;
  unsigned short	Addr = 0x0000 , j = 0x0000;

#ifdef EG_DEBUG
    if (EgDebug>10) printf("EgGetMuxPrescaler\n");
#endif
  j += Channel;
  if (Channel > 7)
    return(-1);
	
   Addr = pEg -> MuxCountSelect;
   Addr &= 0x0fff8;	/* high word select */
   Addr |= j;
   
   if ((Addr & 0x0007) != Channel) Addr |= Channel;
   pEg -> MuxCountSelect = Addr;
   PreScaleVal = pEg -> MuxPrescaler;
   PreScaleVal <<= 16;
   PreScaleVal &= 0x0ffff0000;
   Addr &= 0x0fff7;	/* low word select */
   pEg -> MuxCountSelect = Addr;
   PreScaleVal |= pEg -> MuxPrescaler;

   return(PreScaleVal); 
}

STATIC unsigned long EgSetMuxPrescaler(EgLinkStruct *pParm, unsigned short Channel, unsigned long Divisor)
{
  volatile MrfEVGStruct *pEg = pParm->pEg;
  unsigned short	j=0x0000, Word=0x0000;

#ifdef EG_DEBUG
    if (EgDebug>10) printf("EgSetMuxPrescaler\n");
#endif
  j += Channel;
  if (Channel > 7)
  return(-1);

   pEg -> MuxCountSelect &= 0x0fff7;	/* high word clear */
   pEg -> MuxCountSelect |= j;		/* Mux select */
   pEg -> MuxCountSelect |= 0x0008;	/* high word select */
   Word = (Divisor >> 16) & 0x0000ffff;
   pEg -> MuxPrescaler = Word;
   
   pEg -> MuxCountSelect &= 0x0fff7;	/* low word select */
   Word = Divisor & 0x0000ffff;
   pEg -> MuxPrescaler = Word;
   
  return(0);
}

STATIC long EgGetFpgaVersion(EgLinkStruct *pParm)
{
  volatile MrfEVGStruct *pEg = pParm->pEg;

  return(pEg -> FPGAVersion & 0x0fff);
}
/**
 *
 * Return a one if the request RAM is enabled or a zero otherwise.
 * We use this function to make sure the RAM is disabled before we access it.
 *
 **/
STATIC int EgSeqEnableCheck(EgLinkStruct *pParm, unsigned int Seq)
{
  volatile MrfEVGStruct  *pEg = pParm->pEg;

  if (Seq == 1)
    return(pEg->EventMask & 0x0004);
  return(pEg->EventMask & 0x0002);
}

/**
 *
 * This routine does a manual trigger of the requested SEQ RAM.
 *
 **/
STATIC long EgSeqTrigger(EgLinkStruct *pParm, unsigned int Seq)
{
  volatile MrfEVGStruct  *pEg = pParm->pEg;

#ifdef EG_DEBUG
    if (EgDebug>10) printf("EgSeqTrigger\n");
#endif
  if (Seq == 1)
    pEg->Control = (pEg->Control&CTL_OR_MASK)|0x0100;
  else if (Seq == 2)
    pEg->Control = (pEg->Control&CTL_OR_MASK)|0x0080;
  else
    return(-1);

  return(0);
}
/**
 *
 * Select mode of Sequence RAM operation.
 *
 * OFF:	
 *	Stop the generation of all events from sequence RAMs.
 * NORMAL:
 *	Set to cycle on every trigger.
 * NORMAL_RECYCLE:
 *	Set to continuous run on first trigger.
 * SINGLE:
 *	Set to disable self on an END event.
 * ALTERNATE:
 *      Use one ram at a time, but use both to allow seamless modifications
 *
 **/
STATIC long EgSetSeqMode(EgLinkStruct *pParm, unsigned int Seq, int Mode)
{
  volatile MrfEVGStruct  *pEg = pParm->pEg;

#ifdef EG_DEBUG
    if (EgDebug>10) printf("EgSetSeqMode \n");
#endif
  /* Stop and disable the sequence RAMs */
  if (Mode == egMOD1_Alternate)
  {
    pEg->EventMask &= ~0x3006;		/* Disable *BOTH* RAMs */
    pEg->Control = (pEg->Control&CTL_OR_MASK)&~0x0066;	
    while(pEg->Control & 0x0006)
      taskDelay(1);			/* Wait until finish running */
  }
  else if (Seq == 1)
  {
    pEg->EventMask &= ~0x2004;	/* Disable SEQ 1 */
    pEg->Control = (pEg->Control&CTL_OR_MASK)&~0x0044;
    while(pEg->Control & 0x0004)
      taskDelay(1);		/* Wait until finish running */
    pEg->EventMask &= ~0x0800;	/* Kill ALT mode */
  }
  else if (Seq == 2)
  {
    pEg->EventMask &= ~0x1002;	/* Disable SEQ 2 */
    pEg->Control = (pEg->Control&CTL_OR_MASK)&~0x0022;
    while(pEg->Control & 0x0002)
      taskDelay(1);		/* Wait until finish running */
    pEg->EventMask &= ~0x0800;	/* Kill ALT mode */
  }
  else
    return(-1);

  switch (Mode)
  {
  case egMOD1_Off:
    break;

  case egMOD1_Normal:
    if (Seq == 1)
      pEg->EventMask |= 0x0004;	/* Enable Seq RAM 1 */
    else
      pEg->EventMask |= 0x0002; /* Enable Seq RAM 2 */
    break;

  case egMOD1_Normal_Recycle:
    if (Seq == 1)
    {
      pEg->EventMask |= 0x0004;	/* Enable Seq RAM 1 */
      pEg->Control = (pEg->Control&CTL_OR_MASK)|0x0040;	/* Set Seq 1 recycle */
    }
    else
    {
      pEg->EventMask |= 0x0002;
      pEg->Control = (pEg->Control&CTL_OR_MASK)|0x0020;
    }
    break;

  case egMOD1_Single:
    if (Seq == 1)
      pEg->EventMask |= 0x2004;	/* Enable Seq RAM 1 in single mode */
    else
      pEg->EventMask |= 0x1002;
    break;

  case egMOD1_Alternate:	/* The ram downloader does all the work */
    pEg->EventMask |= 0x0800;	/* turn on the ALT mode bit */
    break;

  default:
    return(-1);
  }
  return(-1);
}

/**
 *
 * Check to see if a sequence ram is currently running
 *
 **/
STATIC long EgGetBusyStatus(EgLinkStruct *pParm, int Ram)
{
  volatile MrfEVGStruct  *pEg = pParm->pEg;

#ifdef EG_DEBUG
    if (EgDebug>10) printf("EgGetBusyStatus\n");
#endif
  if (Ram == 1)
  {
    if (pEg->Control & 0x0004)
      return(1);
    return(0);
  }
  if (pEg->Control & 0x0002)
    return(1);
  return(0);
}
/**
 *
 * Ram is assumed to be in ALT mode, check to see if the requested RAM is
 * available for downloading (Not enabled and idle.)
 *
 * Returns zero if RAM is available or nonzero otherwise.
 *
 **/
STATIC long EgGetAltStatus(EgLinkStruct *pParm, int Ram)
{
  volatile MrfEVGStruct  *pEg = pParm->pEg;

#ifdef EG_DEBUG
    if (EgDebug>10) printf("EgGetAltStatus\n");
#endif
  if (EgGetBusyStatus(pParm, Ram) != 0)
    return(1);

  if (Ram == 1)
  {
    if (pEg->EventMask & 0x0004)
      return(1);

  }
  else
  {
    if (pEg->EventMask & 0x0002)
      return(1);
  }
  return(0);
}
/**
 *
 * Return the operating mode of the requested sequence RAM.
 *
 **/
STATIC long EgGetMode(EgLinkStruct *pParm, int Ram)
{
  volatile MrfEVGStruct  *pEg = pParm->pEg;
  unsigned short	Mask;
  unsigned short	Control;

#ifdef EG_DEBUG
    if (EgDebug>10) printf("EgGetMode\n");
#endif
  Mask = pEg->EventMask & 0x3806;
  Control = pEg->Control & 0x0060;

  if (Mask & 0x0800)
    return(egMOD1_Alternate);

  if (Ram == 1)
  {
    Mask &= 0x2004;
    Control &= 0x0040;

    if ((Mask == 0) && (Control == 0))
      return(egMOD1_Off);
    if ((Mask == 0x0004) && (Control == 0))
      return(egMOD1_Normal);
    if ((Mask == 0x0004) && (Control == 0x0040))
      return(egMOD1_Normal_Recycle);
    if (Mask & 0x2000)
      return(egMOD1_Single);
  }
  else
  {
    Mask &= 0x1002;
    Control &= 0x0020;

    if ((Mask == 0) && (Control == 0))
      return(egMOD1_Off);
    if ((Mask == 0x0002) && (Control == 0))
      return(egMOD1_Normal);
    if ((Mask == 0x0002) && (Control == 0x0020))
      return(egMOD1_Normal_Recycle);
    if (Mask & 0x1000)
      return(egMOD1_Single);
  }
  epicsPrintf("EgGetMode() seqence RAM in invalid state %4.4X %4.4X\n", pEg->Control, pEg->EventMask);
  return(-1);
}

STATIC long EgEnableAltRam(EgLinkStruct *pParm, int Ram)
{
  volatile MrfEVGStruct  *pEg = pParm->pEg;
  unsigned short	Mask = pEg->EventMask;

#ifdef EG_DEBUG
    if (EgDebug>10) printf("EgEnableAltRam\n");
#endif
  while(pEg->Control &0x06) taskDelay(1);
  if (Ram == 1)
    pEg->EventMask = (Mask & ~0x0002)|0x0004;
  else
    pEg->EventMask = (Mask & ~0x0004)|0x0002;
  return(0);
}
/**
 *
 * Enable or disable the generation of VME (software) events.
 *
 **/
STATIC long EgEnableVme(EgLinkStruct *pParm, int state)
{
  volatile MrfEVGStruct  *pEg = pParm->pEg;

#ifdef EG_DEBUG
    printf("EgEnableVme\n");
#endif
  if (state)
    pEg->EventMask |= 0x0001;
  else
    pEg->EventMask &= ~0x0001;

  return(0);
}

/**
 *
 * Send the given event code out now.
 *
 **/
STATIC long EgGenerateVmeEvent(EgLinkStruct *pParm, int Event)
{
  volatile MrfEVGStruct  *pEg = pParm->pEg;

  pEg->VmeEvent = Event;
  return(0);
}
/**
 *
 * Disable and clear everything on the Event Generator
 *
 **/
STATIC long EgResetAll(EgLinkStruct *pParm)
{
  volatile MrfEVGStruct *pEg = pParm->pEg;

#ifdef EG_DEBUG
    printf("EgResetAll\n");
#endif
  pEg->Control = 0x8000;	/* Disable all local event generation */
  pEg->EventMask = 0;		/* Disable all local event generation */

  EgDisableFifo(pParm);

  EgClearSeq(pParm, 1);
  EgClearSeq(pParm, 2);

  EgSetTrigger(pParm, 0, 0);	/* Clear all trigger event numbers */
  EgSetTrigger(pParm, 1, 0);
  EgSetTrigger(pParm, 2, 0);
  EgSetTrigger(pParm, 3, 0);
  EgSetTrigger(pParm, 4, 0);
  EgSetTrigger(pParm, 5, 0);
  EgSetTrigger(pParm, 6, 0);
  EgSetTrigger(pParm, 7, 0);

  EgEnableVme(pParm, 1);	/* There is no reason to disable VME events */

  return(0);
}

/**
 *
 * Read the requested sequence RAM into the buffer passed in.
 *
 **/
STATIC int EgReadSeqRam(EgLinkStruct *pParm, int channel, unsigned char *pBuf)
{
  volatile MrfEVGStruct		*pEg = pParm->pEg;
  volatile unsigned short	*pSeqData;
  int	j;
  volatile short t;

#ifdef EG_DEBUG
    printf("EgReadSeqRam\n");
#endif
  if (EgSeqEnableCheck(pParm, channel))
  {
    pEg->Control = (pEg->Control&CTL_OR_MASK)&~0x0600;
    return(-1);
  }
  if (channel == 1)
  {
    pSeqData = &pEg->Seq1Data;
    pEg->Control = (pEg->Control&CTL_OR_MASK)|0x0404;	/* Auto inc the address */
    pEg->Seq1Addr = 0;
  }
  else if (channel == 2)
  {
    pSeqData = &pEg->Seq2Data;
    pEg->Control = (pEg->Control&CTL_OR_MASK)|0x0202;     /* Auto inc the address */
    pEg->Seq2Addr = 0;
  }
  else
    return(-1);

  for(t=0;t<5;t++);
  for(j=0;j<EG_SEQ_RAM_SIZE; j++)
    pBuf[j] = *pSeqData;

  pEg->Control = (pEg->Control&CTL_OR_MASK)&~0x0600;	/* kill the auto-inc */

  if (channel == 1)
    pEg->Control = (pEg->Control&CTL_OR_MASK)|0x0004;
  else
    pEg->Control = (pEg->Control&CTL_OR_MASK)|0x0002;

  return(0);
}

/**
 *
 * Program the requested sequence RAM from the buffer passed in.
 *
 **/
STATIC int EgWriteSeqRam(EgLinkStruct *pParm, int channel, unsigned char *pBuf)
{
  volatile MrfEVGStruct		*pEg = pParm->pEg;
  volatile unsigned short	*pSeqData;
  int	j;

#ifdef EG_DEBUG
    printf("EgWriteSeqRam\n");
#endif
  if (EgSeqEnableCheck(pParm, channel))
  {
    pEg->Control = (pEg->Control&CTL_OR_MASK)&~0x0600;	/* kill the auto-inc */
    return(-1);
  }

  if (channel == 1)
  {
    pSeqData = &pEg->Seq1Data;
    pEg->Control = (pEg->Control&CTL_OR_MASK)|0x0404;	/* Auto inc the address */
    pEg->Seq1Addr = 0;
  }
  else if (channel == 2)
  {
    pSeqData = &pEg->Seq2Data;
    pEg->Control = (pEg->Control&CTL_OR_MASK)|0x0202;     /* Auto inc the address */
    pEg->Seq2Addr = 0;
  }
  else
    return(-1);

#ifdef EG_DEBUG
  epicsPrintf("ready to download ram\n");
#endif
  taskDelay(20);
  for(j=0;j<EG_SEQ_RAM_SIZE; j++)
    *pSeqData = pBuf[j];

  pEg->Control = (pEg->Control&CTL_OR_MASK)&~0x0600;	/* kill the auto-inc */

  if (channel == 1)		/* reset the config'd channel */
    pEg->Control = (pEg->Control&CTL_OR_MASK)|0x0004;
  else
    pEg->Control = (pEg->Control&CTL_OR_MASK)|0x0002;

#ifdef EG_DEBUG
  epicsPrintf("sequence ram downloaded\n");
#endif
  taskDelay(20);
  return(0);
}

/**
 *
 * The following section is a console-run test program for the event generator
 * card.  It is crude, but it works.
 *
 **/
#ifdef EG_MONITOR

STATIC unsigned char TestRamBuf[EG_SEQ_RAM_SIZE];

STATIC int EgDumpRegs(EgLinkStruct *pParm)
{
  volatile MrfEVGStruct          *pEg = pParm->pEg;

  printf("Control        = %4.4X\n", pEg->Control);

  printf("Event Enable   = %4.4X\n",pEg->EventMask);
  printf("SEQ1 prescaler = %4.4X\n",pEg->Seq1ClockSel);
  printf("SEQ2 prescaler = %4.4X\n",pEg->Seq2ClockSel);

  printf("Card status info: \n");
  if(pEg->Control&0x8000) printf("*Card disabled \n") ;
			    else printf("*Card enabled \n");
  if(pEg->Control&0x4000) printf("*Fifo full \n") ;
			    else printf("*Fifo OK \n");
  if(pEg->Control&0x1000) printf("*Fifo disabled \n") ;
			    else printf("*Fifo enabled \n");
  if(pEg->Control&0x0400) printf("*RAM 1 autoincrement on \n") ;
			    else printf("*RAM 1 autoincrement off \n");
  if(pEg->Control&0x0200) printf("*RAM 2 autoincrement on \n") ;
			    else printf("*RAM 2 autoincrement off \n");
  if(pEg->Control&0x0040) printf("*RAM 1 recycle mode \n") ;
  if(pEg->Control&0x0020) printf("*RAM 2 recycle mode \n") ;
  if(pEg->Control&0x0010) printf("*RAM 1 null filling \n") ;
  if(pEg->Control&0x0080) printf("*RAM 2 null filling \n") ;
  if(pEg->Control&0x0040) printf("*RAM 1 running \n") ;
  if(pEg->Control&0x0020) printf("*RAM 2 running \n") ;
  if(pEg->Control&0x0001) printf("*RXVIO detected \n") ;
			    else printf("*RXVIO clear \n");
  if(pEg->EventMask&0x2000) printf("*RAM 1 single sequence \n") ;
  if(pEg->EventMask&0x1000) printf("*RAM 2 single sequence \n") ;
  if(pEg->EventMask&0x0800) printf("*Common (alternate) mode set \n") ;
  if(pEg->EventMask&0x0004) printf("*RAM 1 enabled \n") ;
  if(pEg->EventMask&0x0002) printf("*RAM 2 enabled \n") ;
  if(pEg->EventMask&0x0001) printf("*VME event generation enabled \n") ;

  if(pEg->EventMask&0x0400) printf("*External event 7 enabled \n") ;
  if(pEg->EventMask&0x0200) printf("*External event 6 enabled \n") ;
  if(pEg->EventMask&0x0100) printf("*External event 5 enabled \n") ;
  if(pEg->EventMask&0x0080) printf("*External event 4 enabled \n") ;
  if(pEg->EventMask&0x0040) printf("*External event 3 enabled \n") ;
  if(pEg->EventMask&0x0020) printf("*External event 2 enabled \n") ;
  if(pEg->EventMask&0x0010) printf("*External event 1 enabled \n") ;
  if(pEg->EventMask&0x0008) printf("*External event 0 enabled \n") ;
  printf("--\n");

  return(0);
}
/**
 *
 * Function to aid in debugging. Writes some data to the event RAM
 *
 **/

STATIC int SetupSeqRam(EgLinkStruct *pParm, int channel)
{
  int		j;

#ifdef EG_DEBUG
    printf("SetupSeqRam\n");
#endif
  if (EgSeqEnableCheck(pParm, channel))
  {
    printf("Sequence ram %d is enabled, can not reconfigure\n", channel);
    return(-1);
  }

  for(j=0; j<=20; j++)
    TestRamBuf[j] = 100-j;

  TestRamBuf[j] = EG_SEQ_RAM_EVENT_END;

  j++;

  for(;j<EG_SEQ_RAM_SIZE; j++)
    TestRamBuf[j] = EG_SEQ_RAM_EVENT_NULL;

  if (EgWriteSeqRam(pParm, channel, TestRamBuf) < 0)
  {
    printf("setup failed\n");
    return(-1);
  }

  return(0);
}

STATIC void SeqConfigMenu(void)
{
  printf("c - clear sequence ram\n");
  printf("p - print sequence ram\n");
  printf("s - setup sequence ram\n");
  printf("m0 - set mode to off\n");
  printf("m1 - set mode to normal\n");
  printf("m2 - set mode to normal-recycle\n");
  printf("m3 - set mode to single\n");
  printf("m4 - set mode to alternate\n");
  printf("t - manual triger seq ram\n");
  printf("q - quit and return to main menu\n");

  return;
}
STATIC void PrintSeq(EgLinkStruct *pParm, int channel)
{
  int		j;

  if (EgReadSeqRam(pParm, channel, TestRamBuf) < 0)
  {
    printf("Sequence ram is currently enabled, can not read\n");
    return;
  }

  printf("Sequence ram %d contents:\n", channel);
  for(j=0; j< EG_SEQ_RAM_SIZE; j++)
  {
    if(TestRamBuf[j] != EG_SEQ_RAM_EVENT_NULL)
      printf("%5d: %3d\n", j, TestRamBuf[j]);
  }
  printf("End of sequence ram dump\n");
  return;
}

STATIC int ConfigSeq(EgLinkStruct *pParm, int channel)
{
  char buf[100];

#ifdef EG_DEBUG
  printf("ConfigSeq\n");
#endif
  while(1)
  {
    SeqConfigMenu();
    if (fgets(buf, sizeof(buf), stdin) == NULL)
      return(0);

    switch (buf[0])
    {
    case 'c': 
	if (EgSeqEnableCheck(pParm, channel))
	{
	  printf("can not config, ram is enabled\n");
	  break;
	}
        EgClearSeq(pParm, channel); 
	break;

    case 'p': 
      PrintSeq(pParm, channel); 
      break;
    case 's': 
      SetupSeqRam(pParm, channel); 
      break;
    case 'm': 
      switch (buf[1])
      {
      case '0': 
        EgSetSeqMode(pParm, channel, egMOD1_Off); 
	break;
      case '1': 
	if (EgSeqEnableCheck(pParm, channel))
	{
	  printf("can not config, ram is enabled\n");
	  break;
	}
        EgSetSeqMode(pParm, channel, 1); 
	break;
      case '2': 
	if (EgSeqEnableCheck(pParm, channel))
	{
	  printf("can not config, ram is enabled\n");
	  break;
	}
        EgSetSeqMode(pParm, channel, 2); 
        break;
      case '3': 
	if (EgSeqEnableCheck(pParm, channel))
	{
	  printf("can not config, ram is enabled\n");
	  break;
	}
        EgSetSeqMode(pParm, channel, 3); 
	break;
      case '4': 
	if (EgSeqEnableCheck(pParm, channel))
	{
	  printf("can not config, ram is enabled\n");
	  break;
	}
        EgSetSeqMode(pParm, channel, 4); 
	break;
      }
    break;
    case 't': 
        EgSeqTrigger(pParm, channel); 
	break;
    case 'q': return(0);
    }
  }
  return(0);
}

STATIC void menu(void)
{
  printf("r - Reset everything on the event generator\n");
  printf("z - dump control and event mask regs\n");
  printf("m - turn on the master enable\n");
  printf("1 - configure sequence RAM 1\n");
  printf("2 - configure sequence RAM 2\n");
  printf("q - quit and return to vxWorks shell\n");
  printf("gnnn - generate a one-shot event number nnn\n");
  printf("annn - read RAM 1 position nnn\n");
  printf("bnnn - read RAM 2 position nnn\n");
  return;
}
int EG(void)
{
  static int	Card = 0;
  char 		buf[100];
  
  printf("Event generator testerizer\n");
  printf("Enter Card number [%d]:", Card);
  fgets(buf, sizeof(buf), stdin);
  sscanf(buf, "%d", &Card);
 
  if (EgCheckCard(Card) != 0)
  {
    printf("Invalid card number specified\n");
    return(-1);
  }
  printf("Card address: %p\n", EgLink[Card].pEg);

  
  while (1)
  {
    menu();

    if (fgets(buf, sizeof(buf), stdin) == NULL)
      return(0);
  
    switch (buf[0])
    {
    case 'r': EgResetAll(&EgLink[Card]); break;
    case '1': ConfigSeq(&EgLink[Card], 1); break;
    case '2': ConfigSeq(&EgLink[Card], 2); break;
    case 'm': EgMasterEnable(&EgLink[Card]); break;
    case 'g': EgGenerateVmeEvent(&EgLink[Card], atoi(&buf[1])); break;
    case 'a': printf("\n %d: 0x%lx\n",atoi(&buf[1]),EgGetRamEvent(&EgLink[Card],1, atoi(&buf[1]))); break;
    case 'b': printf("\n0x%lx\n",EgGetRamEvent(&EgLink[Card],2, atoi(&buf[1]))); break;
    case 'z': EgDumpRegs(&EgLink[Card]); break;
    case 'q': return(0);
    }
  }
  return(0);
}
#endif /* EG_MONITOR */

int EvgSeqRamRead(MrfEVGStruct *pEvg, int ram, int address,
		     int len)
{
  volatile MrfEVGStruct *pEg = pEvg;
  int dummy;
  int i;

#ifdef EG_DEBUG
  printf("EvgSeqRamRead\n");
#endif
  if (ram != 1 && ram != 2)
    return ERROR;

  if (address < 0 || address > 2048)
    return ERROR;

  if (len < 1 || address + len - 1> 2048)
    return ERROR;

  if (ram == 1)
    {
      for (i = 0; i < len; i++)
	{
	  pEg->Seq1Addr = address + i;
	  /* Read back to flush pipelines */
	  dummy = pEg->Seq1Addr;
	  printf("Address %d evno %d timestamp %d\n",dummy, pEg->Seq1Data, pEg->Seq1Time);
	}
    }
  else
    {
      for (i = 0; i < len; i++)
	{
	  pEg->Seq2Addr = address + i;
	  /* Read back to flush pipelines */
	  dummy = pEg->Seq2Addr;
	  printf("Address %d evno %d timestamp %d\n",dummy, pEg->Seq2Data, pEg->Seq2Time);
	}
    }

  return OK;
}
int EgDumpSEQRam(int card,int ram,int num)
{
  volatile MrfEVGStruct          *pEg = EgLink[card].pEg;
  EvgSeqRamRead(pEg,ram,0,num);
  return OK;
}

int DumpEGEV(int card, int ram)
{
  ELLNODE			*pNode;
  struct egeventRecord		*pEgevent;
  int position=0;

  pNode = ellFirst(&(EgLink[card].EgEvent));
  while(pNode !=NULL)
    {
      pEgevent = ((EgEventNode *)pNode)->pRec;
      printf("Position %d record %s (evno %ld dpos %ld apos %ld prio %d)\n",position,
	     pEgevent->name, pEgevent->enm, pEgevent->dpos,pEgevent->apos,pEgevent->val);
      
      pNode=ellNext(pNode);
      position++;
    }
  return(0);
  
}


STATIC long EgRegGet(int regAddr)
{
  return(0);
}

STATIC long EgRegSet(int regAddr, int value)
{

  return(0);

}

