#include <stdio.h>
#include <registryFunction.h>
#include <subRecord.h>
#include <aSubRecord.h>
#include <epicsExport.h>
#include "commonSocket.h"
#include "commonXml.h"
#include "commonDoc.h"
#include <libxml/parser.h>


int mySubDebug = 0;
int process_order = 0;
int socketFD = -1;
char host[256];
xmlDoc* xmldoc = NULL;

static long subPollInit(subRecord *precord) {
  process_order++;
  if (mySubDebug>-1) {
    printf("[ subPollInit ] %d Record %s called subPollInit(%p)\n", process_order, precord->name, (void*) precord);
  }
  
  return 0;
}


static long subPollProcess(subRecord *precord) {
  process_order++;
  if (mySubDebug>-1)
    printf("[ subPollProcess ]: %d Record %s called subPollProcess(%p)\n",process_order, precord->name, (void*) precord);
  
  // find dpm nr
  int idpm;
  char str0[256];
  char str1[256];  
  getStringFromEpicsName(precord->name,str0,0);
  getStringFromEpicsName(precord->name,str1,1);
  if(strcmp(str0,"SVT")==0 && (strcmp(str1,"dpm")==0 || strcmp(str1,"dtm")==0)) {
    idpm = getIntFromEpicsName(precord->name,2);  
  } else {
    printf("[ subPollProcess ]: Wrong precord name to call this function?!  (%s)\n", precord->name);    
    exit(1);
  }
  
  if(strcmp(str1,"dpm")==0) {
    sprintf(host,"dpm%d",idpm);
  }
  else {
    sprintf(host,"dtm%d",idpm);    
  }

  socketFD = open_socket(host,8090);
  
  
  if(socketFD>0) {
    printf("[ subPollProcess ]: successfully opened socket at %d\n", socketFD);

    if(xmldoc!=NULL) {
      printf("[ subPollProcess ]: dpm doc is not null(%p). Clean up.\n", xmldoc);
      xmlFreeDoc(xmldoc);
      xmlCleanupParser();      
      xmldoc = NULL;
    }

    if (mySubDebug>-1)
      printf("[ subPollProcess ]: get the xml doc\n");
    
    getDpmXmlDoc(socketFD, 0, &xmldoc);
    
    
    if (mySubDebug>-1)
      printf("[ subPollProcess ]: found xml doc at %p\n", xmldoc);
    
    
    
/*     if(xmldoc!=NULL) {       */
/*       if (mySubDebug>-1) */
/* 	printf("[ subPollProcess ]: clean up dpm doc (%p)\n", xmldoc); */
/*       xmlFreeDoc(xmldoc); */
/*       xmlCleanupParser();       */
/*       xmldoc = NULL; */
/*    } */
    
  } else {
    printf("[ subPollProcess ]: [ WARNING ]: failed to open socket\n");
  }


  


  if(socketFD>0) {
    printf("[ subPollProcess ]: close socket %d\n", socketFD);
    socketFD = close_socket(socketFD);
  }

  return 0;
}


static long subDpmStateInit(aSubRecord *precord) {
  process_order++;
  if (mySubDebug) {
    printf("[ subDpmStateInit ]: %d Record %s called subDpmStateInit(%p)\n", process_order, precord->name, (void*) precord);
  }

  strcpy(precord->vala,"init...");

  return 0;
}

static long subDpmStateProcess(aSubRecord *precord) {
  process_order++;
  if (mySubDebug) {
    printf("[ subDpmStateProcess ]: %d Record %s called subDpmStateProcess(%p)\n",process_order, precord->name, (void*) precord);
  }

  char state[256];
  strcpy(precord->vala, "default");

  getRunStateProcess(precord->name, xmldoc, state);

  strcpy(precord->vala, state);

  
  return 0;
}



/* Register these symbols for use by IOC code: */

epicsExportAddress(int, mySubDebug);
epicsRegisterFunction(subPollInit);
epicsRegisterFunction(subPollProcess);
epicsRegisterFunction(subDpmStateInit);
epicsRegisterFunction(subDpmStateProcess);
