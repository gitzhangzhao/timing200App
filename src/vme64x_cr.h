/*
 *
 *  File:     vme64x_cr.h
 *            $RCSfile: vme64x_cr.h,v $
 *            $Date: 2006/10/17 00:58:05 $
 *            $Revision: 1.1.1.1 $
 *
 *  Title:    VME64x Configuration ROM (CR) structure definition
 *            and CR/CSR function prototypes
 *
 *  Author:   Jukka Pietarinen
 *            Micro-Research Finland Oy
 *            <jukka.pietarinen@mrf.fi>
 *
 *            $Log: vme64x_cr.h,v $
 *            Revision 1.1.1.1  2006/10/17 00:58:05  leige
 *            BEPCII timing system
 *
 *            Revision 1.1.1.1  2004/12/30 07:39:34  jpietari
 *            Moved project to CVS
 *
 *
 */

#define MRF_IEEE_OUI     0x000EB2
#define MRF_EVG200_BID   0x454700C8
#define MRF_EVR200RF_BID 0x455246C8
#define MRF_EVR200_BID   0x455200C8
#define MRF_GUNTX200_BID 0x475458C8
#define MRF_4CHTIM_BID   0x344354C8

#define CSR_BAR          0x7ffff
#define CSR_BIT_SET      0x7fffb
#define CSR_BIT_CLEAR    0x7fff7
#define CSR_CRAM_OWNER   0x7fff3
#define CSR_UD_BIT_SET   0x7ffef
#define CSR_UD_BIT_CLEAR 0x7ffeb
#define CSR_FN7_ADER     0x7ffd3
#define CSR_FN6_ADER     0x7ffc3
#define CSR_FN5_ADER     0x7ffb3
#define CSR_FN4_ADER     0x7ffa3
#define CSR_FN3_ADER     0x7ff93
#define CSR_FN2_ADER     0x7ff83
#define CSR_FN1_ADER     0x7ff73
#define CSR_FN0_ADER     0x7ff63
#define CSR_IRQL	 0x7fb03

typedef struct VME64x_CR
{
  UINT32 rom_checksum;
  UINT32 rom_length[3];
  UINT32 cr_data_access_width;
  UINT32 csr_data_access_width;
  UINT32 space_id;
  UINT32 ascii_C;
  UINT32 ascii_R;
  UINT32 ieee_oui[3];
  UINT32 board_id[4];
  UINT32 revision_id[4];
  UINT32 p_ascii_id[3];
  UINT32 reserved_a[8];
  UINT32 program_id;
  UINT32 beg_user_cr[3];
  UINT32 end_user_cr[3];
  UINT32 beg_cram[3];
  UINT32 end_cram[3];
  UINT32 beg_user_csr[3];
  UINT32 end_user_csr[3];
  UINT32 beg_SN[3];
  UINT32 end_SN[3];
  UINT32 slave_char;
  UINT32 UD_slave_char;
  UINT32 master_char;
  UINT32 UD_master_char;
  UINT32 irq_handler_cap;
  UINT32 irq_cap;
  UINT32 reserved_b;
  UINT32 CRAM_width;
  UINT32 fn_DAWPR[8];
  UINT32 fn_AMCAP[8][8];
  UINT32 fn_XAMCAP[8][32];
  UINT32 fn_ADEM[8][4];
  UINT32 reserved_c[3];
  UINT32 master_DAWPR;
  UINT32 master_AMCAP[8];
  UINT32 master_XAMCAP[32];
  UINT32 reserved_d[556];
} VME64x_CR;

STATUS vmeCSRMemProbe(char *adrs, int mode, int length, char *pVal);
void vmeCRShow(int slot);
void vmeCSRShow(int slot);
STATUS convCRtoLong(UINT32 *p, int bytes, UINT32 *pVal);
STATUS vmeCRSlotProbe(int slot, VME64x_CR *p_cr);
STATUS vmeCRFindBoard(int slot, UINT32 ieee_oui, UINT32 board_id,
		      int *p_slot);
STATUS vmeCSRWriteADER(int slot, int func, UINT32 ader);
