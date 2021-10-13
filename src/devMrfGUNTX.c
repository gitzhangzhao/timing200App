/*
 *
 *  File:     devMrfGUNTX.c
 *            $RCSfile: devMrfGUNTX.c,v $
 *            $Date: 2006/10/17 00:58:05 $
 *            $Revision: 1.1.1.1 $
 *
 *  Title:    GUNTX functions
 *
 *  Author:   Jukka Pietarinen
 *            Micro-Research Finland Oy
 *            <jukka.pietarinen@mrf.fi>
 *
 *            $Log: devMrfGUNTX.c,v $
 *            Revision 1.1.1.1  2006/10/17 00:58:05  leige
 *            BEPCII timing system
 *
 *            Revision 1.2  2005/01/07 13:18:09  jpietari
 *            Added 'volatile' keyword to regsiter pointers.
 *
 *            Revision 1.1.1.1  2004/12/30 07:39:34  jpietari
 *            Moved project to CVS
 *
 *
 */

#include <vxWorks.h>
#include <sysLib.h>
#include <vxLib.h>
#include <intLib.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <taskLib.h>

#include "devMrfGUNTX.h"

STATUS GunTxSetDelay(MrfGUNTXStruct *pGunTx, int channel, UINT32 delay)
{
  volatile UINT32 *pDelay;

  switch (channel)
    {
    case 1:
      pDelay = &pGunTx->DelayCh1;
      break;
    case 2:
      pDelay = &pGunTx->DelayCh2;
      break;
    default:
      return ERROR;
    }

  *pDelay = delay;
  return OK;
}

STATUS GunTxGetDelay(MrfGUNTXStruct *pGunTx, int channel, UINT32 *delay)
{
  volatile UINT32 *pDelay;

  switch (channel)
    {
    case 1:
      pDelay = &pGunTx->DelayCh1;
      break;
    case 2:
      pDelay = &pGunTx->DelayCh2;
      break;
    default:
      return ERROR;
    }

  *delay = *pDelay;
  return OK;
}

STATUS GunTxSetWidth(MrfGUNTXStruct *pGunTx, int channel, UINT32 width)
{
  volatile UINT32 *pWidth;

  switch (channel)
    {
    case 1:
      pWidth = &pGunTx->WidthCh1;
      break;
    case 2:
      pWidth = &pGunTx->WidthCh2;
      break;
    default:
      return ERROR;
    }

  *pWidth = width;
  return OK;
}

STATUS GunTxGetWidth(MrfGUNTXStruct *pGunTx, int channel, UINT32 *width)
{
  volatile UINT32 *pWidth;

  switch (channel)
    {
    case 1:
      pWidth = &pGunTx->WidthCh1;
      break;
    case 2:
      pWidth = &pGunTx->WidthCh2;
      break;
    default:
      return ERROR;
    }

  *width = *pWidth;
  return OK;
}

STATUS GunTxFPEnable(MrfGUNTXStruct *pGunTx, int channel, int state)
{
  MrfGUNTXStruct *pGun = pGunTx;
  UINT32 mask_or;

  switch (channel)
    {
    case 1:
      mask_or = MRF_GUNTX_CONF_FP_TRIG1EN;
      break;
    case 2:
      mask_or = MRF_GUNTX_CONF_FP_TRIG2EN;
      break;
    default:
      return ERROR;
    }

  pGun->Config |= mask_or;
  return OK;
}

int GunTxFPGetState(MrfGUNTXStruct *pGunTx, int channel)
{
  MrfGUNTXStruct *pGun = pGunTx;
  UINT32 mask_and;

  switch (channel)
    {
    case 1:
      mask_and = MRF_GUNTX_CONF_FP_TRIG1EN;
      break;
    case 2:
      mask_and = MRF_GUNTX_CONF_FP_TRIG2EN;
      break;
    default:
      return ERROR;
    }
 
  return (pGun->Config & mask_and ? 1 : 0);
}

STATUS GunTxSwEnable(MrfGUNTXStruct *pGunTx, int channel, int state)
{
  MrfGUNTXStruct *pGun = pGunTx;
  UINT32 mask_or;

  switch (channel)
    {
    case 1:
      mask_or = MRF_GUNTX_CONF_SW_TRIG1EN;
      break;
    case 2:
      mask_or = MRF_GUNTX_CONF_SW_TRIG2EN;
      break;
    default:
      return ERROR;
    }

  pGun->Config |= mask_or;
  return OK;
}

int GunTxSwGetState(MrfGUNTXStruct *pGunTx, int channel)
{
  MrfGUNTXStruct *pGun = pGunTx;
  UINT32 mask_and;

  switch (channel)
    {
    case 1:
      mask_and = MRF_GUNTX_CONF_SW_TRIG1EN;
      break;
    case 2:
      mask_and = MRF_GUNTX_CONF_SW_TRIG2EN;
      break;
    default:
      return ERROR;
    }
 
  return (pGun->Config & mask_and ? 1 : 0);
}

STATUS GunTxSwTrigger(MrfGUNTXStruct *pGunTx, int channel)
{
  MrfGUNTXStruct *pGun = pGunTx;
  UINT32 mask_or;

  switch (channel)
    {
    case 1:
      mask_or = MRF_GUNTX_CONF_SW_TRIG1;
      break;
    case 2:
      mask_or = MRF_GUNTX_CONF_SW_TRIG2;
      break;
    default:
      return ERROR;
    }

  pGun->Config |= mask_or;
  return OK;
}

STATUS GunTxSetFTDelay(MrfGUNTXStruct *pGunTx, int channel, UINT32 ftdelay)
{
  MrfGUNTXStruct *pGun = pGunTx;

  if (ftdelay > MRF_GUNTX_MAX_DELAY)
    return ERROR;

  switch (channel)
    {
    case 1:
      pGun->FineDelay1 = ftdelay;
      break;
    case 2:
      pGun->FineDelay2 = ftdelay;
      break;
    case 3:
      pGun->FineMeas1 = ftdelay;
      break;
    case 4:
      pGun->FineMeas2 = ftdelay;
      break;
    default:
      return ERROR;
    }
  
  return OK;
}

STATUS GunTxSweep(MrfGUNTXStruct *pGunTx, int channel, int *mem)
{
  MrfGUNTXStruct *pGun = pGunTx;
  int ft_delay;
  UINT32 measure;

  if (channel < 1 || channel > 2)
    return ERROR;

  for (ft_delay = 0; ft_delay < 1024; ft_delay++)
    {
      /* Set Measure delay line */
      GunTxSetFTDelay(pGunTx, channel + 2, ft_delay);
      /* Wait for delay to settle */
      taskDelay(1);
      /* Trigger */
      GunTxSwTrigger(pGunTx, channel);
      /* Read measure output */
      measure = pGun->Config;
      switch (channel)
	{
	case 1: 
	  if (measure & MRF_GUNTX_CONF_MEAS1_IN)
	    mem[ft_delay]++;
	  break;
	case 2:
	  if (measure & MRF_GUNTX_CONF_MEAS2_IN)
	    mem[ft_delay]++;
	  break;
	}
    }

  return OK;
} 

STATUS GunTxMeasure(MrfGUNTXStruct *pGunTx, int channel, int delay, int width)
{
  MrfGUNTXStruct *pGun = pGunTx;
  int datamem[1024];
  int i,j, data;

  /* Clear data memory */
  for (i = 0; i < 1024; i++)
    datamem[i] = 0;

  GunTxSetDelay(pGunTx, channel, delay);
  GunTxSetWidth(pGunTx, channel, width);
  GunTxSetFTDelay(pGunTx, channel, 0);
  switch (channel)
    {
    case 1:
      pGun->Config = MRF_GUNTX_CONF_SW_TRIG1EN |
	MRF_GUNTX_CONF_MEAS1_CH1;
      pGun->Control = MRF_GUNTX_CTRL_MEAS1SELCH1;
      break;
    case 2:
      pGun->Config = MRF_GUNTX_CONF_SW_TRIG2EN |
	MRF_GUNTX_CONF_MEAS2_CH2;
      pGun->Control = MRF_GUNTX_CTRL_MEAS2SELCH2;
      break;
    default:
      return ERROR;
    }

  for (i = 0; i < 1; i++)
    {
      printf("\rSweeping channel %d, loop %d. ", channel, i + 1);
      GunTxSweep(pGunTx, channel, datamem);
    }

  printf("\n");
  for (i = 0; i < 1024; i+=8)
    {
      data = 0;
      for (j = 0; j < 8; j++)
	data += datamem[i+j];
    printf("%d", data);
    }
  printf("\n");

  return OK;
}

STATUS GunTxShowTemp(MrfGUNTXStruct *pGunTx)
{
  int i;
  char *txtTempSensor[8] = {"TEMP B", "CH1 Measure", "CH1", NULL,
			    "CH2", NULL, "CH2 Measure", "TEMP A"};
  volatile UINT16 *pTemp;

  pTemp = &pGunTx->TempB;

  for (i = 0; i < 8; i++)
    if (txtTempSensor[i] != NULL)
      printf("%3.2f degree C at %s\n", pTemp[i] * 250.0 / 4096.0,
	     txtTempSensor[i]);

  return OK;
}

STATUS TempDump(MrfGUNTXStruct *pGunTx)
{
  int i;
  char *txtTempSensor[8] = {"TEMP B", "CH1 Measure", "CH1", NULL,
			    "CH2", NULL, "CH2 Measure", "TEMP A"};
  volatile UINT16 *pTemp;


  pTemp = &pGunTx->TempB;

  while(1)
    {
      for (i = 0; i < 8; i++)
	if (txtTempSensor[i] != NULL)
	  printf("%3.2f\t", pTemp[i] * 250.0 / 4096.0);
      printf("\n");
      sleep(10);
    }

  return OK;
}
