program sncBkt2

%%#include <stdio.h>
%%#include <stdlib.h>

%%#define TOTAL_BUCKET_SYNC 402
%%#define TOTAL_BUCKET_CLI 396
%%#define TOTAL_BUCKET TOTAL_BUCKET_SYNC

%%  int bktRam[TOTAL_BUCKET];
%%  int pattern[TOTAL_BUCKET];
%%  FILE *fp, *fp_out;
%%  char *filename="/home21/pubrun/consys/timing/bktFiles/bktRamDiv5.txt";
%%  char *patternFile="/home21/operator/bktPattern";
%%  char bepc_tm_buf[256];
%%  int enmGunT=0x24, enmRam0=0x20, enmBkt0=0x21, enmKck=0x22, enmKly=0x7F;
/* enmKly=0x28, 2006Nov15 use enmKly to set 0x7f */

int delay3u;
int evgMod1;
assign evgMod1 to "R:TM:M1Evg:eg.MOD1";

int gunTDely;
assign gunTDely to "R:TM:M1Evg:egEvt:gunT.DELY";
int gunTEnm;
assign gunTEnm to "R:TM:M1Evg:egEvt:gunT.ENM";

int ram0Dely;
assign ram0Dely to "R:TM:M1Evg:egEvt:ram0.DELY";
int ram0Enm;
assign ram0Enm to "R:TM:M1Evg:egEvt:ram0.ENM";

int bkt0Dely;
assign bkt0Dely to "R:TM:M1Evg:egEvt:bkt0.DELY";
int bkt0Enm;
assign bkt0Enm to "R:TM:M1Evg:egEvt:bkt0.ENM";

int klyDely;
assign klyDely to "R:TM:M1Evg:egEvt:Kly.DELY";
int KlyEnm;
assign KlyEnm to "R:TM:M1Evg:egEvt:Kly.ENM";
 
int kckDely;
assign kckDely to "R:TM:M1Evg:egEvt:kck.DELY";
int KckEnm;
assign KckEnm to "R:TM:M1Evg:egEvt:kck.ENM";
 
int clr1;
assign clr1 to "R:TM:M1Evg:eg.CLR1";

int calcM1;
assign calcM1 to "R:TM:M1Evr:calc:gunT";
monitor calcM1;
int start;
assign start to "R:TM:M1Evr:bo:start";
monitor start;
int every;
assign every to "R:TM:M1Evr:ai:every";
monitor every;

int keStart;
assign keStart to "R:TM:KeEvr:bo:start";
int kpStart;
assign kpStart to "R:TM:KpEvr:bo:start";
int m2Start;
assign m2Start to "R:TM:M1Evr:bo:start";

int patternInj;
assign patternInj to "R:TM:M1Evr:bo:pInj";
monitor patternInj;
int singleInj;
assign singleInj to "R:TM:M1Evr:bo:sInj";
monitor singleInj;

int bktSelect;
assign bktSelect to "R:TM:M1Evr:ai:bktSelect";
monitor bktSelect;
/*int injTimes;
assign injTimes to "R:TM:M1Evr:ai:injTimes";
monitor injTimes; */
int injEle;
assign injEle to "R:TM:M1Evr:ao:ele";
monitor injEle; 
/*int injEle;
assign injEle to "R:TM:M1Evr:ai:injTimes";
monitor injEle;*/

int injTimesCount;
int lastCalcNo;
int tmp;
int tmp_i;
int pCount;
int everyC;

int current_bktRam;

ss tm_ss1
{
   state timingMainInit
   {
       when (1) {
         printf("\n timingMainInit\n\n");
         lastCalcNo = -1;
         pCount = 0;
         injTimesCount = 0;
         everyC = 0;
         delay3u = 2;

%{       fp = fopen(filename, "rb");
         for (tmp_i=0; tmp_i<TOTAL_BUCKET && (fgets(bepc_tm_buf,256,fp)!=NULL); tmp_i++) {
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
}%
         for (tmp_i=0; tmp_i<TOTAL_BUCKET; tmp_i++) {
               printf("bkt, ramUnit %d, %d\n", tmp_i, bktRam[tmp_i]); 
              }
         delay(.1);
        } state timingMainWait
   }
   state timingSetBasicTrig
   {
      when (1) {
        printf("set basic trigs for linac\n");
         evgMod1=0;
         pvPut(evgMod1);
         clr1 = 1;
         pvPut(clr1);

         ram0Dely=0;
         pvPut(ram0Dely);
         bkt0Dely=1;
         pvPut(bkt0Dely); /* should add 3 miliseconds' delay */
         gunTDely=2;
         pvPut(gunTDely);
         kckDely=3;
         pvPut(kckDely);
         klyDely=12; /* 2006Nov15, ENM=0x7F */
         pvPut(klyDely);
         evgMod1=4;
         pvPut(evgMod1);

         delay(.5);
        } state timingMainWait
   }

   state timingMainWait
   {
       when(patternInj==1)
       {
        tmp_i=0;
        pCount = 0;
%{      fp = fopen(patternFile, "rb");
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
          printf("patternCount %d\n", pCount);
          for (tmp_i=0; tmp_i<pCount; tmp_i++) {
            printf("%d , ", pattern[tmp_i]);
            }
          printf("\n");
          } /*end of else */
}%
       if (injEle==-1) {
         keStart = 1;
         pvPut(keStart);
         m2Start = 1;
         pvPut(m2Start);
         printf("Enable e- injection\n");
         } 
       else if (injEle==1) {
         kpStart = 1;
         pvPut(kpStart);
         m2Start = 1;
         pvPut(m2Start);
         printf("Enable e+ injection\n"); 
       }
	printf("Go to Continuous Injection, calcM1 %d\n", calcM1);
       } state timingContInj
/*       when(singleInj==1) 
       {
         injTimesCount = 0;
         printf("bktSelect %d\n", bktSelect);
         if ((bktSelect>=0) && (bktSelect<TOTAL_BUCKET)) {
           evgMod1=0;
           pvPut(evgMod1);
           ramUnit=bktRam[bktSelect];
           pvPut(ramUnit);
           evgMod1=1;
           pvPut(evgMod1);
           printf("go to singel injection\n");
           lastCalcNo=-1;
         }
         else {
           singleInj=0;
           pvPut(singleInj);
           }
       } state timingSingleInj
*/
       when (delay(0.5))
       {
       } state timingMainWait
   }
   state timingContInj
   {
       when (pCount<=0) {
         delay(.5);
         printf("go back to timingMainWait\n");
         } state timingMainWait
       when (patternInj==0) {
         keStart = 0;
         pvPut(keStart);
         kpStart = 0;
         pvPut(kpStart);
         m2Start = 0;
         pvPut(m2Start);
         printf("pInj=0, disable injection and return to timingMainWait\n");
         delay(.5);
         } state timingMainWait
       when(calcM1 != lastCalcNo)
       {
         printf("calc %d, %d\n", lastCalcNo, calcM1);
         lastCalcNo=calcM1;
         if (tmp_i>=pCount) {tmp_i=0;};
         printf("bucket %d, , ", pattern[tmp_i]);
         evgMod1=0;
         pvPut(evgMod1);
         clr1 = 1;
         pvPut(clr1);

         ram0Dely=0;
         pvPut(ram0Dely);
         bkt0Dely=1;
         pvPut(bkt0Dely); /* before, it once adds 3 miliseconds' delay */

         gunTDely= delay3u + bktRam[pattern[tmp_i]];
         printf("ramUnit %d\n", gunTDely);
         pvPut(gunTDely);
         kckDely=gunTDely + 1;
         pvPut(kckDely);
         klyDely=gunTDely + 10;
         pvPut(klyDely);


         evgMod1=4;
         pvPut(evgMod1);
         tmp_i++;
         printf("-----------calcM1, %d\n", calcM1);
       } state timingContInj
       when (delay(0.05)) {
         printf("time delay 0.05 second in timingContInj\n");
       } state timingContInj
       when (delay(1) && (calcM1==lastCalcNo))
       {
         printf("Continuous injection finished\n");
         lastCalcNo = -1;
         patternInj=0;
         pvPut(patternInj);
       } state timingMainWait
   }
/*   state timingSingleInj
   {
      when (singleInj==0) {
        printf("singleInj==0, goto state timingMainWait\n");
      } state timingMainWait
      when (injTimes<injTimesCount)
      {
        printf("injTimes %d\n", injTimes);
        singleInj=0;
        pvPut(singleInj);
        printf("Have injected enough times, goto timingMainWait\n");
      } state timingMainWait
      when (calcM1 != lastCalcNo) {
        printf("calcM1 %d\n", calcM1);
        if (injTimes>=injTimesCount) {
          printf("injTimesCount %d\n", injTimesCount);
          lastCalcNo=calcM1;
          injTimesCount ++;
          }
       } state timingSingleInj
      when (delay(.1)) {
        printf("just delay in timingSingleInj\n");
        } state timingSingleInj
   }
*/
}
