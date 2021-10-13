/*
 *
 *  File:     devMrfGUNTX.h
 *            $RCSfile: devMrfGUNTX.h,v $
 *            $Date: 2006/10/17 00:58:05 $
 *            $Revision: 1.1.1.1 $
 *
 *  Title:    Register definitions for GUNTX
 *
 *  Author:   Jukka Pietarinen
 *            Micro-Research Finland Oy
 *            <jukka.pietarinen@mrf.fi>
 *
 *            $Log: devMrfGUNTX.h,v $
 *            Revision 1.1.1.1  2006/10/17 00:58:05  leige
 *            BEPCII timing system
 *
 *            Revision 1.1.1.1  2004/12/30 07:39:34  jpietari
 *            Moved project to CVS
 *
 *
 */

/* Config Register Bit Definitions */

#define MRF_GUNTX_CONF_FP_TRIG1EN  0x00000001 /* CH1 Front panel trigger enable */
#define MRF_GUNTX_CONF_FP_TRIG2EN  0x00000002 /* CH2 Front panel trigger enable */
#define MRF_GUNTX_CONF_MEAS1_IN    0x00001000 /* CH1 measure input */
#define MRF_GUNTX_CONF_MEAS2_IN    0x00002000 /* CH2 measure input */
#define MRF_GUNTX_CONF_SW_TRIG1EN  0x00010000 /* CH1 software trigger enable */
#define MRF_GUNTX_CONF_SW_TRIG2EN  0x00020000 /* CH2 software trigger enable */
#define MRF_GUNTX_CONF_SW_MEAS1EN  0x00040000 /* CH1 measure enable */
#define MRF_GUNTX_CONF_SW_MEAS2EN  0x00080000 /* CH2 measure enable */
#define MRF_GUNTX_CONF_SW_TRIG1    0x00400000 /* CH1 software trigger */
#define MRF_GUNTX_CONF_SW_TRIG2    0x00800000 /* CH2 software trigger */
#define MRF_GUNTX_CONF_SW_MEAS1    0x01000000 /* CH1 measure trigger */
#define MRF_GUNTX_CONF_SW_MEAS2    0x02000000 /* CH2 measure trigger */
#define MRF_GUNTX_CONF_MEAS1_OFF   0x00000000 /* CH1 measure trigger mode: off */
#define MRF_GUNTX_CONF_MEAS1_SW    0x10000000 /* CH1 measure trigger mode: software */
#define MRF_GUNTX_CONF_MEAS1_CH1   0x20000000 /* CH1 measure trigger mode: CH1 */
#define MRF_GUNTX_CONF_MEAS2_OFF   0x00000000 /* CH2 measure trigger mode: off */
#define MRF_GUNTX_CONF_MEAS2_SW    0x40000000 /* CH2 measure trigger mode: software */
#define MRF_GUNTX_CONF_MEAS2_CH2   0x80000000 /* CH2 measure trigger mode: CH2 */

/* Control Register Bit Definitions */

#define MRF_GUNTX_MAX_DELAY        1023
#define MRF_GUNTX_CTRL_CLKSEL0     0x00000000 /* RF Clock multiplexer select clock 0 */
#define MRF_GUNTX_CTRL_CLKSEL1     0x00010000 /* RF Clock multiplexer select clock 1 */
#define MRF_GUNTX_CTRL_CLKSEL2     0x00020000 /* RF Clock multiplexer select clock 2 */
#define MRF_GUNTX_CTRL_CLKSEL3     0x00030000 /* RF Clock multiplexer select clock 3 */
#define MRF_GUNTX_CTRL_MEAS1SELCH1 0x00000000 /* Measure CH1 output */
#define MRF_GUNTX_CTRL_MEAS1SELREF 0x00080000 /* Measure Reference clock */
#define MRF_GUNTX_CTRL_MEAS1SELRX1 0x000C0000 /* Measure RX signal */
#define MRF_GUNTX_CTRL_MEAS2SELCH2 0x00000000 /* Measure CH2 output */
#define MRF_GUNTX_CTRL_MEAS2SELREF 0x00200000 /* Measure Reference clock */
#define MRF_GUNTX_CTRL_MEAS2SELRX2 0x00300000 /* Measure RX signal */

typedef struct MrfGUNTXStruct
{
  UINT32 DelayCh1;    /* offset 0x000 */
  UINT32 WidthCh1;    /* offset 0x004 */
  UINT32 DelayCh2;    /* offset 0x008 */
  UINT32 WidthCh2;    /* offset 0x00C */
  UINT32 DelayMeas1;  /* offset 0x010 */
  UINT32 Reserved1;   /* offset 0x014 */
  UINT32 DelayMeas2;  /* offset 0x018 */
  UINT32 Reserved2;   /* offset 0x01C */
  UINT32 Reserved3;   /* offset 0x020 */
  UINT32 Reserved4;   /* offset 0x024 */
  UINT32 Reserved5;   /* offset 0x028 */
  UINT32 Reserved6;   /* offset 0x02C */
  UINT32 Reserved7;   /* offset 0x030 */
  UINT32 Reserved8;   /* offset 0x034 */
  UINT32 Control;     /* offset 0x038 */
  UINT32 Config;      /* offset 0x03C */
  UINT16 FTDelay2;    /* offset 0x040 */
  UINT16 FTMeas2;     /* offset 0x042 */
  UINT16 DAResv1;     /* offset 0x044 */
  UINT16 FREQT2;      /* offset 0x046 */
  UINT16 FREQT1;      /* offset 0x048 */
  UINT16 FTDelay1;    /* offset 0x04a */
  UINT16 FTMeas1;     /* offset 0x04c */
  UINT16 DAResv2;     /* offset 0x04e */
  UINT16 DAResv3;     /* offset 0x050 */
  UINT16 HeatMeas2;   /* offset 0x052 */
  UINT16 DAResv4;     /* offset 0x054 */
  UINT16 DAResv5;     /* offset 0x056 */
  UINT16 DAResv6;     /* offset 0x058 */
  UINT16 Heat1;       /* offset 0x05a */
  UINT16 HeatMeas1;   /* offset 0x05c */
  UINT16 Heat2;       /* offset 0x05e */
  UINT16 TempB;       /* offset 0x060 */
  UINT16 TempMeas1;   /* offset 0x062 */
  UINT16 Temp1;       /* offset 0x064 */
  UINT16 ADResv1;     /* offset 0x066 */
  UINT16 Temp2;       /* offset 0x068 */
  UINT16 ADResv2;     /* offset 0x06a */
  UINT16 TempMeas2;   /* offset 0x06c */
  UINT16 TempA;       /* offset 0x06e */
  UINT16 ADResv3;     /* offset 0x070 */
  UINT16 ADResv4;     /* offset 0x072 */
  UINT16 FineDelay1;  /* offset 0x074 */
  UINT16 FineMeas1;   /* offset 0x076 */
  UINT16 FineResv1;   /* offset 0x078 */
  UINT16 FineDelay2;  /* offset 0x07a */
  UINT16 FineMeas2;   /* offset 0x07c */
  UINT16 FineResv2;   /* offset 0x07e */
} MrfGUNTXStruct;

STATUS GunTxSetDelay(MrfGUNTXStruct *pGunTx, int channel, UINT32 delay);
STATUS GunTxGetDelay(MrfGUNTXStruct *pGunTx, int channel, UINT32 *delay);
STATUS GunTxSetWidth(MrfGUNTXStruct *pGunTx, int channel, UINT32 width);
STATUS GunTxGetWidth(MrfGUNTXStruct *pGunTx, int channel, UINT32 *width);
STATUS GunTxFPEnable(MrfGUNTXStruct *pGunTx, int channel, int state);
int GunTxFPGetState(MrfGUNTXStruct *pGunTx, int channel);
STATUS GunTxSwEnable(MrfGUNTXStruct *pGunTx, int channel, int state);
int GunTxSwGetState(MrfGUNTXStruct *pGunTx, int channel);
STATUS GunTxSwTrigger(MrfGUNTXStruct *pGunTx, int channel);
STATUS GunTxSetFTDelay(MrfGUNTXStruct *pGunTx, int channel, UINT32 ftdelay);
STATUS GunTxShowTemp(MrfGUNTXStruct *pGunTx);
