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
int i, n;
FILE *fp;
int calc[CALCSUM];
struct timespec ts[CALCSUM];

long TMsubCalcInit(struct genSubRecord*  pgsub)
{
    printf("TMsubCalcInit was called\n");
    i = 0; n = 0;
    return(0);
}

long TMsubCalcProcess(pgsub)
    struct genSubRecord  *pgsub;
{
   if (i==CALCSUM+2) return(0);
   if (i<CALCSUM) {
     calc[i]=*((int *)(pgsub->a));
     clock_gettime(CLOCK_REALTIME, &ts[i]);
     i++;
     } 
   else {
     printf("have got %d data\n", i);
     fp = fopen("/home21/home/leige/calc.data", "wb");
     if (fp==NULL) {
       printf("NULL fp\n");
       return(-1);
       }
     for (n=0; n<CALCSUM; n++) {
  /*     printf("%d,%d,%d\n", calc[n], ts[n].tv_sec, ts[n].tv_nsec);*/
       fprintf(fp, "%d,%d,%d\n", calc[n], ts[n].tv_sec, ts[n].tv_nsec);
       }
     fclose(fp);
     i=CALCSUM +2;
     }
    return(0);
}
