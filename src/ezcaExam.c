#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <cadef.h>
#include <caerr.h>

#include <dbDefs.h> /* needed for PVNAME_SZ and FLDNAME_SZ */
#include <db_access.h>

#define epicsExportSharedSymbols
#include <shareLib.h>

#include "ezca.h" 
#include "EzcaScan.h"

main()
{
char str[40];

printf("get timestamp\n");
Ezca_timeStamp("leige:aiEzample", str);
printf("%s", str);

}
