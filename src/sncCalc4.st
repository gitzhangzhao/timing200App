program timingTest ("unit=R:TM:main")

%%#include <time.h>
%%#include "tmStatusDef.h"
%%#include <drvTS.h>
%%#include <tsDefs.h>
%%#include <stdio.h>
%%#include <stdlib.h>

%%   FILE *fp, *fp_out;
%%   char *filename="/home21/home/leige/epicsApp/lgTimingApp/src/calc4";
%%   char buf[256];

int calc4;
assign calc4 to "L:TM:test:calc4";
monitor calc4

%int calc[10000];

%%int i=0;
%% struct timespec ts[10000];


ss ssTiming1
{
	state tmStateWaiting
	{
	  when (1)
	  { printf("prepare to record time\n");
	    fp = fopen(filename, "rb");
            
	  } state normalWork
	}

	state normalWork
	{
	  when(i<10000) 
	  {
		calc[i]=calc4;
%%		clock_gettime(CLOCK_REALTIME, &ts);
		ts.tv_sec -= TS_VXWORKS_TO_EPICS_EPOCH;
%%		(void)printf("clock_gettime, %s\n", tsStampToText(&ts,TS_TEXT_MONDDYYYY, nowText));
		printf("timestamp %d start\n", i);
/*		TSprintTimeStamp(bucket[i][1]);*/
                TSgetTimeStamp(bucket[i][1],&ts);
                printf("event %d occurred: %lu.%lu\n",bucket[i][1],ts.tv_sec,ts.tv_nsec);

		timeStart[i]=1;
		pvPut(timeStart[i]);

		i=i % 3;
		eventNo=0;
		pvPut(eventNo);
		ramUnit=bucket[i][0];
		pvPut(ramUnit);
		eventNo=bucket[i][1];
		pvPut(eventNo);
/*		TSprintTimeStamp(bucket[i][1]);*/
		i++;
		
		timeEnd[i]=1;
		pvPut(timeEnd[i]);
		/*TSprintTimeStamp(bucket[i][1]);*/
		delay(.02);

	  } state normalWork
	  when (tmStatus == TM_INJ_STOP)
		{
			printf("\n stop\n");
		} state stateStop
	}

	state stateStop
	{
		when (0)
		{
		} state tmStateWaiting
	}
}
