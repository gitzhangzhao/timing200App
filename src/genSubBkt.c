#include	<vxWorks.h>
#include	<types.h>
#include	<stdioLib.h>
#include        <stdlib.h>

#include	<dbDefs.h>
#include	<genSubRecord.h>
#include	<dbCommon.h>
#include	<recSup.h>

#include <drvTS.h>
#include <tsDefs.h>

#define CALCSUM 10000
int i, n, tmp, tmp_i, pCount;
FILE *fpBkt;
int calc[CALCSUM];
struct timespec ts[CALCSUM];
#define TOTAL_BUCKET_SYNC 402
#define TOTAL_BUCKET_CLI 396
#define TOTAL_BUCKET TOTAL_BUCKET_CLI

  int bktRam[TOTAL_BUCKET];
  int pattern[TOTAL_BUCKET];
  FILE *fp, *fp_out;
  char *syncBktRamFile="/home21/pubrun/consys/timing/bktFiles/bktRamDiv5.txt";
  char *collBktRamFile="/home21/pubrun/consys/timing/bktFiles/collBktRam5.txt";
  char *bsrPattFile="/home21/operator/bucket/bktPattern";
  char *berPattFile="/home21/operator/bucket/bktPatternBER";
  char bepc_tm_buf[256];
/*  int enmGunT=0x24, enmRam0=0x20, enmBkt0=0x21, enmKck=0x22, enmKly=0x7F; */
/* enmKly=0x28, 2006Nov15 use enmKly to set 0x7f */

long TMsubBktInit(struct genSubRecord*  pgsub)
{
    printf("TMsubBktInit was called\n");
    i = 0; n = 0;

       fp = fopen(collBktRamFile, "rb");
         for (tmp_i=0; tmp_i<TOTAL_BUCKET && (fgets(bepc_tm_buf,256,fp)!=NULL); 
tmp_i++) {
           if (bepc_tm_buf[0]=='#') {printf("%s\n", bepc_tm_buf); tmp_i--;}
             else {
               sscanf(bepc_tm_buf, "%d%d", &tmp, &(bktRam[tmp_i]));
               if (bktRam[tmp_i]<0) {
                  printf("illegal ram position\n");
                  bktRam[tmp_i] = 1;
                }
               else if (bktRam[tmp_i]>2000) { /* seqRam length is 2048 */
                  printf("illegal ram position\n");
                  bktRam[tmp_i] = 2000;
                }
             }
           }
         fclose(fp);

        for (tmp_i=0; tmp_i<TOTAL_BUCKET; tmp_i++) {
               printf("bkt, ramUnit %d, %d\n", tmp_i, bktRam[tmp_i]); 
              }

      tmp_i=0;
      pCount = 0;
      fp = fopen(berPattFile, "rb");
      if (fp==NULL) {
           printf("pattern file open error\n");
           }
      else {
          for (tmp_i=0; (fgets(bepc_tm_buf,256,fp)!=NULL)&&(tmp_i<TOTAL_BUCKET); tmp_i++ ) {
           sscanf(bepc_tm_buf, "%d", &pattern[tmp_i]);
           if ((pattern[tmp_i]>401)||(pattern[tmp_i]<0)) {
             printf("illegal bucket number. Use bucker 0\n");
             pattern[tmp_i]=0;
             }
           pCount ++;
           }
          fclose(fp);
          printf("total buckets in pattern file %d\n", pCount);
          for (tmp_i=0; tmp_i<pCount; tmp_i++) {
            printf("%d , ", pattern[tmp_i]);
            }
          printf("\n");
        }

    pgsub->vala=malloc(sizeof(int));
    if (pgsub->vala==NULL) {
      printf("malloc sizeof int for vala failed\n");
      }

    tmp_i=0;   
    return(0);
}

long TMsubBktProcess(pgsub)
    struct genSubRecord  *pgsub;
{
    printf("tmp_i %d, pattern tmp_i %d, bktRam %d\n", tmp_i, pattern[tmp_i], bktRam[pattern[tmp_i]]);
    pgsub->lei=bktRam[pattern[tmp_i]];
    printf("pgsub vallei -- %d\n", pgsub->lei);
    pgsub->val=bktRam[pattern[tmp_i]];
    printf("pgsub vallei -- %d\n", pgsub->val);

    tmp_i++;
    if (tmp_i>=pCount) tmp_i=0;
   
    return(0);
}
