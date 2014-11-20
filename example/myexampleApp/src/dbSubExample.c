#include <stdio.h>

#include <dbDefs.h>
#include <registryFunction.h>
#include <subRecord.h>
#include <aSubRecord.h>
#include <epicsExport.h>
#include <tcpipExample.h>
#include <stringinRecord.h>
#include <longinRecord.h>
#include <dbAccess.h>
#include <client_util.h>

#define N_HYBRIDS 18
#define N_HALVES 2
int mySubDebug = 0;
int process_order = 0;
const int BUF_SIZE = 256;
char* hostNameDef = "134.79.229.141";
char hostName[256];
int portDef = 8091;
int port;
static int hybToFeb[N_HALVES][N_HYBRIDS];
int sockfd = 0;
int counter = 0;
int status_poll_flag=0;
int status_flag=0;
const double def_hyb_v = -999.9;
const double def_hyb_i = -999.9;
const double def_hyb_t = -999.9;
const double def_AxiXadcTemp = -999.9;

static long subLVInit(subRecord *precord) {
  process_order++;
  if (mySubDebug) {
    printf("[ subLVInit ]: %d Record %s called subLVInit(%p)\n", process_order, precord->name, (void*) precord);
  }

  return 0;
}

static long subTempInit(subRecord *precord) {
   process_order++;
  if (mySubDebug) {
    printf("[ subTempInit ]: %d Record %s called subTempInit(%p)\n", process_order, precord->name, (void*) precord);
  }

  return 0;
}

static void getIpFromRecord(subRecord* precord, char value[], const int MAX) {
  char inpa_val[40]; // 40 is maximum size of stringin/out records
  memset(inpa_val,0,40);
  snprintf( inpa_val, 40, "%s", (char*) ((precord->inpa).value.constantStr) ) ;
  if(strlen(inpa_val)>0) {
    dbAddr paddr;
    if(dbNameToAddr(inpa_val,&paddr)!=0) {
      printf("[ getIpFromRecord ]: [ ERROR ]: dbNameToAddr for %s failed (paddr=%p)\n",inpa_val,&paddr);
    }
    struct stringinRecord* recA = (stringinRecord*)paddr.precord;
    if (mySubDebug>1) printf("[ getIpFromRecord ]: recA at %p\n",recA);
    if(recA!=NULL) {
      if (mySubDebug>1) printf("[ getIpFromRecord ]: recA name %s val %p\n",recA->name,recA->val);
      char inpa_val2[40];
      memset(inpa_val2,0,40);
      snprintf( inpa_val2, 40, "%s", recA->val ) ;
      if (mySubDebug>1) printf("[ getIpFromRecord ]: got IP %s\n",inpa_val2);
      if(strlen(inpa_val2)<MAX) {
	strcpy(value,inpa_val2);
      } else {
	printf("[ getIpFromRecord ]: [ WARNING ]: IP from DB is too long? %s \n",inpa_val2);
      }
    } else {
      printf("[ getIpFromRecord ]: [ WARNING ]: cannot get IP record from inpa_val %s \n",inpa_val);
    }
  } else {
      printf("[ getIpFromRecord ]: [ WARNING ]: INPA string has zero length \n");    
  }
}

static int getPortFromRecord(subRecord* precord) {
  char inpb_val[40]; // 40 is maximum size of stringin/out records
  memset(inpb_val,0,40);
  snprintf( inpb_val, 40, "%s", (char*) ((precord->inpb).value.constantStr) ) ;
  if(strlen(inpb_val)>0) {
    dbAddr paddr;
    if(dbNameToAddr(inpb_val,&paddr)!=0) {
      printf("[ getPortFromRecord ]: [ ERROR ]: dbNameToAddr for %s failed (paddr=%p)\n",inpb_val,&paddr);
    }
    struct longinRecord* recA = (longinRecord*)paddr.precord;
    if (mySubDebug>1) printf("[ getPortFromRecord ]: recA at %p\n",recA);
    if(recA!=NULL) {
      if (mySubDebug>1) printf("[ getPortFromRecord ]: recA name %s val %d\n",recA->name,recA->val);
      return recA->val;
    } else {
      printf("[ getPortFromRecord ]: [ WARNING ]: getPortFromRecord: cannot get port record from inpb_val %s \n",inpb_val);
    }
  } else {
      printf("[ getPortFromRecord ]: [ WARNING ]: getIpFromRecord: INPB string has zero length \n");    
  }
  return -1;
}

static int getLonginRecord(char* inpb_val) {
  if(strlen(inpb_val)>0) {
    dbAddr paddr;
    if(dbNameToAddr(inpb_val,&paddr)!=0) {
      printf("[ getLonginRecord ]: [ ERROR ]: dbNameToAddr for %s failed (paddr=%p)\n",inpb_val,&paddr);
    }
    struct longinRecord* recA = (longinRecord*)paddr.precord;
    if (mySubDebug>1) printf("[ getLonginRecord ]: recA at %p\n",recA);
    if(recA!=NULL) {
      if (mySubDebug>1) printf("[ getLonginRecord ]: recA name %s val %d\n",recA->name,recA->val);
      return recA->val;
    } else {
      printf("[ getLonginRecord ]: [ WARNING ]: cannot get port record from inpb_val %s \n",inpb_val);
    }
  } else {
      printf("[ getLonginRecord ]: [ WARNING ]: INPB string has zero length \n");    
  }
  return -1;

}

static void setFebMap(char* inpb_val, int value) {
  char halfstr[BUF_SIZE];
  char id[BUF_SIZE];
  int half;
  int hybid;
  char* name = NULL;
  if(strlen(inpb_val)>0) {
    dbAddr paddr;
    if(dbNameToAddr(inpb_val,&paddr)!=0) {
      printf("[ setFebMap ]: [ ERROR ]: dbNameToAddr for %s failed (paddr=%p)\n",inpb_val,&paddr);      
    }
    else {
      struct longinRecord* recA = (longinRecord*)paddr.precord;
      if (mySubDebug>2) if (mySubDebug>2) printf("[ setFebMap ]: recA at %p\n",recA);
      if(recA!=NULL) {
	if (mySubDebug>2) if (mySubDebug>2) printf("[ setFebMap ]: recA name %s val %d\n",recA->name,recA->val);
	name = recA->name;
      } else {
	printf("[ setFebMap ]: [ ERROR ]: recA is NULL!?\n");   
      }
    }
  }

  if(name==NULL) {
    printf("[ setFebMap ]: [ ERROR ]: failed to find name of the record.\n");   
    return;
  }
  
  if (mySubDebug>2) printf("[ setFebMap ]: get the half name from \"%s\"\n",name);
  
  getHalfFromDaqMapRecordName(name,halfstr,BUF_SIZE);

  if (mySubDebug>2) printf("[ setFebMap ]: found halfstr \"%s\"\n",halfstr);

  if(strcmp(halfstr,"top")==0) half=0;
  else if(strcmp(halfstr,"bot")==0) half=1;
  else {
    printf("[ setFebMap ]: [ ERROR ]: invalid half name \"%s\"\n", halfstr);
    return;
  }

  if (mySubDebug>2) printf("[ setFebMap ]: found half int %d\n",half);

  if (mySubDebug>2) printf("[ setFebMap ]: get the hybrid id from \"%s\"\n",name);
  
  getHybridFromDaqMapRecordName(name,id,BUF_SIZE);  

  if (mySubDebug>2) printf("[ setFebMap ]: found idstr \"%s\"\n",id);

  hybid = (int) strtol(id, (char**)NULL, 10);

  if (mySubDebug>2) printf("[ setFebMap ]: found id int %d\n",hybid);

  hybToFeb[half][hybid] = value;

  if (mySubDebug>2) printf("[ setFebMap ]: hybToFeb value %d\n",hybToFeb[half][hybid]);

  //if(name!=NULL) free(name);

  if (mySubDebug>2) printf("[ setFebMap ]: done\n");

}

static void setFebFromRecord(subRecord* precord) {

  if (mySubDebug>2) printf("[ setFebFromRecord ]: precord name \"%s\"\n",precord->name);
  
  char inpb_val[40]; // 40 is maximum size of stringin/out records
  int val;
  if (mySubDebug>2) printf("[ setFebFromRecord ]: get LNKA\n");
  
  memset(inpb_val,0,40);
  snprintf( inpb_val, 40, "%s", (char*) ((precord->inpa).value.constantStr) ) ;
  val = getLonginRecord(inpb_val);  
  setFebMap(inpb_val, val);
 

 
  if (mySubDebug>2) printf("[ setFebFromRecord ]: get LNKB\n");

  memset(inpb_val,0,40);
  snprintf( inpb_val, 40, "%s", (char*) ((precord->inpb).value.constantStr) ) ;
  val = getLonginRecord(inpb_val);
  setFebMap(inpb_val, val);


  if (mySubDebug>2) printf("[ setFebFromRecord ]: get LNKC\n");

  memset(inpb_val,0,40);
  snprintf( inpb_val, 40, "%s", (char*) ((precord->inpc).value.constantStr) ) ;
  val = getLonginRecord(inpb_val);
  setFebMap(inpb_val, val);

  if (mySubDebug>2) printf("[ setFebFromRecord ]: get LNKD\n");

  memset(inpb_val,0,40);
  snprintf( inpb_val, 40, "%s", (char*) ((precord->inpd).value.constantStr) ) ;
  val = getLonginRecord(inpb_val);
  setFebMap(inpb_val, val);

  if (mySubDebug>2) printf("[ setFebFromRecord ]: get LNKE\n");

  memset(inpb_val,0,40);
  snprintf( inpb_val, 40, "%s", (char*) ((precord->inpe).value.constantStr) ) ;
  val = getLonginRecord(inpb_val);
  setFebMap(inpb_val, val);

  if (mySubDebug>2) printf("[ setFebFromRecord ]: get LNKF\n");

  memset(inpb_val,0,40);
  snprintf( inpb_val, 40, "%s", (char*) ((precord->inpf).value.constantStr) ) ;
  val = getLonginRecord(inpb_val);
  setFebMap(inpb_val, val);

  if (mySubDebug>2) printf("[ setFebFromRecord ]: get LNKG\n");

  memset(inpb_val,0,40);
  snprintf( inpb_val, 40, "%s", (char*) ((precord->inpg).value.constantStr) ) ;
  val = getLonginRecord(inpb_val);
  setFebMap(inpb_val, val);

  if (mySubDebug>2) printf("[ setFebFromRecord ]: get LNKH\n");

  memset(inpb_val,0,40);
  snprintf( inpb_val, 40, "%s", (char*) ((precord->inph).value.constantStr) ) ;
  val = getLonginRecord(inpb_val);
  setFebMap(inpb_val, val);

  if (mySubDebug>2) printf("[ setFebFromRecord ]: get LNKI\n");

  memset(inpb_val,0,40);
  snprintf( inpb_val, 40, "%s", (char*) ((precord->inpi).value.constantStr) ) ;
  val = getLonginRecord(inpb_val);
  setFebMap(inpb_val, val);




  if (mySubDebug>2) printf("[ setFebFromRecord ]: done\n");

}

 
static void setupSocket(subRecord *precord) {
  process_order++;
  if (mySubDebug>1) {
    printf("[ setupSocket ]: %d Record %s called setupSocket(%p)\n", process_order, precord->name, (void*) precord);
  }

  // only setup socket from DB if it's the poll record?
  if(strcmp(precord->name,"SVT:poll_xml")==0) {    
    // look for host name and port in DB
    if (mySubDebug>1) printf("[ setupSocket ]: look for host name and port in DB\n");
    char host[40];
    int p;
    getIpFromRecord(precord,host,40);
    p = getPortFromRecord(precord);
    if(strlen(host)==0 || p<=0) {
      printf("[ setupSocket ]: [ ERROR ]: Couldn't get hostname or port from in DB.\n");
    } else {
      strcpy(hostName,host);
      port = p;
    }      
  }

  if(strcmp(hostName,"")==0) {
    printf("[ setupSocket ]: [ ERROR]: no valid hostname found.\n");
    return;
  } 

  if (mySubDebug>1) printf("[ setupSocket ]: Got host \"%s\" port %d\n",hostName,port);
  
  
}

static void printDaqMap() {
  printf("\n%10s %10s %10s\n","half","hybrid","feb");
  int i,j;
  for(i=0;i<N_HALVES;++i) {
    for(j=0;j<N_HYBRIDS;++j) {
      printf("%10d %10d %10d\n",i,j, hybToFeb[i][j]);
    }
  }
}

static void resetDaqMap() {
 
  int i,j;
  for(i=0;i<N_HALVES;++i) {
    for(j=0;j<N_HYBRIDS;++j) {
      hybToFeb[i][j] = -1;
    }
  }
  
}


static void setupDaqMap(subRecord *precord) {
  process_order++;
  if (mySubDebug>1) {
    printf("[ setupDaqMap ]: %d Record %s called setupDaqMap(%p)\n", process_order, precord->name, (void*) precord);
  }

  if (mySubDebug>1) printf("[ setupDaqMap ]: initialize daq map\n");
 
  
  // only setup socket from DB if it's the correct record...
  if(strstr(precord->name,"SVT:daqmap_sub:")!=NULL) {    
    if (mySubDebug>1) printf("[ setupDaqMap ]: look for feb in DB\n");
    setFebFromRecord(precord);
  } else {
    if (mySubDebug>1) printf("[ setupDaqMap ]: do not setup daqmap for this record\n");
  }
  if (mySubDebug>1) {
    printf("[ setupDaqMap ]: Resulting daq map in mem:\n");
    printDaqMap();
    printf("[ setupDaqMap ]: done\n");
  }
  
}


static long subPollDaqMapInit(subRecord *precord) {
  process_order++;
  if (mySubDebug) {
    printf("[ subPollDaqMapInit] %d Record %s called subPollDaqMapInit(%p)\n", process_order, precord->name, (void*) precord);
  }

  
  if (mySubDebug) {
    printf("[ subPollDaqMapInit] reset daq map\n");
  }

  resetDaqMap();
  //setupDaqMap(precord);

  if (mySubDebug) {
    printf("[ subPollDaqMapInit] done\n");
  }
  

  return 0;
}


static long subPollInit(subRecord *precord) {
  process_order++;
  if (mySubDebug) {
    printf("[ subPollInit ] %d Record %s called subPollInit(%p)\n", process_order, precord->name, (void*) precord);
  }

  return 0;
}


static long subPollStatInit(subRecord *precord) {
  process_order++;
  if (mySubDebug) {
    printf("[ subPollStatInit ] %d Record %s called subPollStatInit(%p)\n", process_order, precord->name, (void*) precord);
  }
  
  return 0;
}


  static void writeHybrid(subRecord* precord,char action[], int id, int feb_id, char ch_name[])
{
  if(mySubDebug) printf("[ writeHybrid ]: Record %s called writeHybrid %s with val %f for feb_id= %d  id=%d ch_name=%s\n", precord->name,action,precord->val,feb_id,id,ch_name);
  
  time_t cur_time;
  time_t timer;
  time(&timer);
  int dt;
  // get a valid socket
  dt = 0;
  if(sockfd>0) {
    if (mySubDebug) printf("[ writeHybrid ]: socket %d is already open, wait for it to close\n", sockfd);    
    while(sockfd>0 && dt<6) {
      time(&cur_time);
      dt = difftime(cur_time, timer);
      if (mySubDebug) printf("[ writeHybrid ]: socket %d is still open after %ds, sleep 1s\n", sockfd, dt);    
      sleep(1);
    }    
  }
  
  if(sockfd>0) {    
    printf("[ writeHybrid ]: [ WARNING ]: socket %d was still open after %ds, don't write anything\n", sockfd, dt); 
    return;
  } 
  else {    
    if (mySubDebug) printf("[ writeHybrid ]: Opening socket: host: %s:%d\n",hostName, port);    
    setupSocket(precord);
    sockfd = open_socket(hostName,port);
    if (mySubDebug) printf("[ writeHybrid ]: Opened socket : %d\n",sockfd);            
  }
  
  if(sockfd<=0) {
    printf("[ writeHybrid ]: [ ERROR ]: Failed to open socket in writeHybrid (host %s:%d) \n",hostName,sockfd);        
    return;
  }
  
  if(strcmp(action,"v_set_sub")==0) {    
    if(precord->val<255 && precord->val>0) {
      
      writeHybridVTrim(sockfd,(int)precord->val, id, ch_name);    
      
      if (mySubDebug) printf("[ writeHybrid ]: Poll xml string after write\n");
      
      getXmlDoc(sockfd,0,0);
      
      if (mySubDebug) printf("[ writeHybrid ]:  Poll XML done after write.\n");
      
    } else {
      printf("[ writeHybrid ]: [ ERROR]: voltage trim %f is not allowed!\n",precord->val);
      exit(1);
    }
    
  } 
  else if(strcmp(action,"switch_sub")==0) {    

    if(strcmp(ch_name,"all")==0) {
      
      int val = (int)precord->val;
      if(val==0 || val==1) {
	
	writeHybridVSwitch(sockfd, val, id);    
	
	if (mySubDebug) printf("[ writeHybrid ]: Poll xml string after write\n");

	getXmlDoc(sockfd,0,0);

	if (mySubDebug) printf("[ writeHybrid ]: Poll XML done after write.\n");
	
      } else {
	printf("[ writeHybrid ]: [ ERROR ]: voltage switch %d is not allowed!\n",val);
	exit(1);
      }

    } else {
      printf("[ writeHybrid ]: [ ERROR ]: this ch_name %s for action %s is not defined yet\n",ch_name,action);
    }    
  }
  else {
    printf("[ writeHybrid ]: [ ERROR ]: this action \"%s\" for writeHybrid is not defined!\n",action);
    exit(1);
  }
  
  if (mySubDebug) {
    printf("[ writeHybrid ]: Closing socket\n");
  }
  
  sockfd = close_socket(sockfd);
  
  if (mySubDebug) {
    printf("[ writeHybrid ]:  after closing socket is %d\n",sockfd);
  }

}




  static void readHybrid(subRecord* precord,char action[], int id, int feb_id, char ch_name[])
{
  double constant;
  if(mySubDebug) {
    printf("[ readHybrid ]: Record %s called readHybrid %s for feb_id= %d  id=%d ch_name=%s\n", precord->name,action,feb_id,id,ch_name);
  }

  //set to default
  //precord->val = def_hyb_t;
  
  char value[BUF_SIZE];
  memset(value,0,BUF_SIZE);
  constant = 1.;
  if(strcmp(action,"i_rd_sub")==0) {
    readHybridI(feb_id, id, ch_name, value, BUF_SIZE);
  } 
  else if(strcmp(action,"t_rd_sub")==0) {
    int tId;
    if(strcmp(ch_name,"temp0")==0) {
      tId = 0;
    }
    else if(strcmp(ch_name,"temp1")==0) {
      tId = 1;
    }
    else {
      printf("[ readHybrid ]: [ ERROR ]: the ch_name %s for action %s is not defined!\n",ch_name,action);
      exit(1);
    }
    readHybridT(feb_id, id, tId, value, BUF_SIZE);
  } 
  else if(strcmp(action,"vn_sub")==0) {
    readHybridV(feb_id, id, ch_name, "Near", value, BUF_SIZE);
  } 
  else if(strcmp(action,"vf_sub")==0) {
    readHybridV(feb_id, id, ch_name, "Far", value, BUF_SIZE);
  } 
  else if(strcmp(action,"v_set_rd_sub")==0) {
    readHybridVTrim(feb_id, id, ch_name, value, BUF_SIZE);
  } 
  else {
    printf("[ readHybrid ]: [ ERROR]: wrong action for readHybrid \"%s\"\n",action);
    return;
  }
  if (mySubDebug) {
    printf("[ readHybrid ]: Got value=\"%s\"\n",value);
  }
  if(strlen(value)>0) {
    precord->val = atof(value)*constant;
    if (mySubDebug) {
      printf("[ readHybrid ]: precord-val is now %f \n",precord->val);
    }
  }
}


  static void readFeb(subRecord* precord,char action[], int feb_id, char ch_name[])
{
  if(mySubDebug) printf("[ readFeb ]: Record %s called readFeb %s feb_id=%d ch_name=%s\n",precord->name,action,feb_id,ch_name);
  
  char value[BUF_SIZE];
  memset(value,0,BUF_SIZE);
  if(strcmp(action,"t_rd_sub")==0) {
    //set to default
    //precord->val=def_AxiXadcTemp;
    readFebT(feb_id, value, ch_name, BUF_SIZE);
    
  } 
  else {
    printf("[ readFeb ]: [ ERROR ]: No such action %s implemented for readFeb!\n",action);
    return;
  } 
  if (mySubDebug) {
    printf("[ readFeb ]: Got value=\"%s\"\n",value);
  }
  if(strlen(value)>0) {
    precord->val = atof(value);
    if (mySubDebug) {
      printf("[ readFeb ]: precord-val is now %f \n",precord->val);
    }
  }
}






static long subLVProcess(subRecord *precord) {
  process_order++;
  if (mySubDebug) {
    printf("[ subLVProcess ]: %d Record %s called subLVProcess(%p)\n",process_order, precord->name, (void*) precord);
  }
  //SVT:lv:hyb:bot:0:dvdd:vn_sub
  //SVT:lv:hyb:bot:0:dvdd:v_set_sub
  int feb_id;
  char type[BUF_SIZE];
  char board_type[BUF_SIZE];
  char half[BUF_SIZE];
  char id[BUF_SIZE];
  char ch_name[BUF_SIZE];
  char action[BUF_SIZE];
  getType(precord->name,type,BUF_SIZE);
  getBoardType(precord->name,board_type,BUF_SIZE);
  getHalf(precord->name,half,BUF_SIZE);
  getId(precord->name,id,BUF_SIZE);
  getChName(precord->name,ch_name,BUF_SIZE);
  getAction(precord->name,action,BUF_SIZE);

  if (mySubDebug) {
    printf("[ subLVProcess ]: Record %s has type %s board type \"%s\"\n", precord->name, type,board_type);
  }

  if(strcmp(type,"lv")!=0) {
    printf("[ subLVProcess ]: [ ERROR ]: this type is not valid \"%s\"\n",type);
    return 0;
  }
  
  char* p_end = id;
  long int li_id = strtol(id,&p_end,0);
  if(p_end!=id) {
    if(li_id<0 && li_id>17) {     
      printf("[ subLVProcess ]: [ ERROR ]: this hybrid id %ld is not valid\n",li_id);
      return 0;
    }
  } else {
    printf("[ subLVProcess ]: [ ERROR ]: converting this hybrid id %s is not valid\n",id);
    return 0;      
  }
  feb_id = getFebIdFromDaqMap((int)li_id,half);
  if(feb_id<0) {
    printf("[ subLVProcess ]: [ ERROR ]: getting feb id\n");
    return 0;
  } 
  
  if(strcmp(board_type,"hyb")==0) {
   
    if(strcmp(action,"vn_sub")==0 || strcmp(action,"vf_sub")==0 || strcmp(action,"i_rd_sub")==0 || strcmp(action,"v_set_rd_sub")==0) {

      if(strcmp(ch_name,"dvdd")!=0 && strcmp(ch_name,"avdd")!=0 && strcmp(ch_name,"v125")!=0) {
	printf("[ subLVProcess ]: [ ERROR ]: wrong option for hybrid ch: %s\n",ch_name);
	return 0;
      }
      readHybrid(precord,action,(int)li_id,feb_id,ch_name);  
      
    } else if(strcmp(action,"v_set_sub")==0 || strcmp(action,"switch_sub")==0) { 

      if(strcmp(ch_name,"dvdd")!=0 && strcmp(ch_name,"avdd")!=0 && strcmp(ch_name,"v125")!=0 && strcmp(ch_name,"all")!=0) {
	printf("[ subLVProcess ]: [ ERROR ]: wrong option for hybrid ch: %s\n",ch_name);
	return 0;
      }

      writeHybrid(precord,action,(int)li_id,feb_id,ch_name);  

    } else {
      printf("[ subLVProcess ]: [ ERROR ]: this hybrid action type is not valid \"%s\"\n",action);
      return 0;
    }    
  }
  else {
    printf("[ subLVProcess ]: [ ERROR ]: this board type is not valid \"%s\" for LV\n",board_type);
    return 0;
  }
  
  
  
  
  return 0;
}



static long subTempProcess(subRecord *precord) {
  process_order++;
  if (mySubDebug) {
    printf("[ subTempProcess ]:%d Record %s called subTempProcess(%p)\n",process_order, precord->name, (void*) precord);
  }
  //SVT:temp:hyb:bot:0:temp1:t_rd_sub
  //SVT:temp:hyb:bot:0:temp2:t_rd_sub
  //SVT:temp:fe:0:axixadc:t_rd_sub)
  int feb_id;
  char type[BUF_SIZE];
  memset(type,0,BUF_SIZE);
  char board_type[BUF_SIZE];
  char half[BUF_SIZE];
  char id[BUF_SIZE];
  char ch_name[BUF_SIZE];
  char action[BUF_SIZE];

  getType(precord->name,type,BUF_SIZE);
  getBoardType(precord->name,board_type,BUF_SIZE);


  if (mySubDebug) {
    printf("[ subTempProcess ]:Record %s has type %s board type \"%s\"\n", precord->name, type,board_type);
  }

  //  char tmp[BUF_SIZE];
  int type_cmp;
  if((type_cmp=strcmp(type,"temp"))!=0) {
    printf("[ subTempProcess ]: [ ERROR ]: this type is not valid \"%s\" cmp %d\n",type,type_cmp);
    return 0;
  }

  if(strcmp(board_type,"hyb")==0) {  
    getHalf(precord->name,half,BUF_SIZE);
    getId(precord->name,id,BUF_SIZE);
    getChName(precord->name,ch_name,BUF_SIZE);
    getAction(precord->name,action,BUF_SIZE);

    if(strcmp(ch_name,"temp0")!=0 && strcmp(ch_name,"temp1")!=0) {
      printf("[ subTempProcess ]: [ ERROR ]: wrong option for hybrid ch: %s\n",ch_name);
      return 0;
    }

    char* p_end = id;
    long int li_id = strtol(id,&p_end,0);
    if(p_end!=id) {
      if(li_id<0 && li_id>17) {     
	printf("[ subTempProcess ]: [ ERROR ]: this hybrid id %ld is not valid\n",li_id);
	return 0;
      }
    } else {
      printf("[ subTempProcess ]: [ ERROR ]: converting this hybrid id %s is not valid\n",id);
      return 0;      
    }
    
    feb_id = getFebIdFromDaqMap((int)li_id,half);
    if(feb_id<0) {
      printf("[ subTempProcess ]: [ ERROR ]: getting feb id\n");
      return 0;
    } 

   
    readHybrid(precord,action,(int)li_id,feb_id,ch_name);  


  } 
  else if(strcmp(board_type,"fe")==0) {  
    sprintf(half,"%s","bot");
    getFebId(precord->name,id,BUF_SIZE);
    getFebChName(precord->name,ch_name,BUF_SIZE);
    getFebAction(precord->name,action,BUF_SIZE);  
    feb_id = atoi(id);
    if(strcmp(action,"t_rd_sub")!=0) {
      printf("[ subTempProcess ]: [ ERROR ]: this feb action type is not valid \"%s\"\n",action);
      return 0;
    }  
    if(strcmp(ch_name,"axixadc") !=0 && strcmp(ch_name,"FebTemp0") !=0 && strcmp(ch_name,"FebTemp1") !=0 ) {
      printf("[ subTempProcess ]: [ ERROR ]: This ch name is not implemented for for readFeb: \"%s\" !\n",ch_name);
      return 0;      
    }
    
    readFeb(precord,action,feb_id,ch_name);  
    

  } 
  else {
    printf("[ subTempProcess ]: [ ERROR ]: this board type is not valid \"%s\"\n",board_type);
    return 0;
  }
  

  
  
  return 0;
}



static void updatePollStatusFlag() {
  // get the status from the client utils
  int status = getXMLPollStatus();
  // flip the status flag if xml poll was ok.
  if (mySubDebug) {
    printf("updatePollStatusFlag: start status_poll_flag = %d\n",status_poll_flag);
    printf("updatePollStatusFlag: getXMLPollStatus=%d\n",status);
  }
  if(status==1) {
    if(status_poll_flag==0) {
      status_poll_flag = 1;
    } else {
      status_poll_flag = 0;
    }
  }
  if (mySubDebug) printf("updatePollStatusFlag: end status_poll_flag = %d\n",status_poll_flag);
}



static long subPollProcess(subRecord *precord) {
  time_t cur_time;
  int dt;
  time_t timer;

  process_order++;
  if (mySubDebug>0) {
    printf("[ subPollProcess ]: %d Record %s called subPollProcess(%p)\n",process_order, precord->name, (void*) precord);
  }
  
  // check that the socket is available
  dt=0;
  time(&timer);
  if(sockfd>0) {
    if (mySubDebug>0) printf("[ subPollProcess ]: socket %d is already open, wait for it to close\n", sockfd);    
    while(sockfd>0 && dt<6) {
      time(&cur_time);
      dt = difftime(cur_time, timer);
      if (mySubDebug>0) printf("[ subPollProcess ]: socket %d is still open (%ds)\n", sockfd, dt);    
      sleep(1);
    }    
  }
  
  if(sockfd>0) {    
    printf("[ subPollProcess ]: [ WARNING ]: socket %d was still open after %ds, don't write anything\n", sockfd, dt); 
    return 0;
  } 
  else {

    if (mySubDebug>0) printf("[ subPollProcess ]: socket is available now (socket %d after %ds)\n", sockfd, dt); 
    // get a valid socket
    while(sockfd<=0 && dt<6) {
      time(&cur_time);
      dt = difftime(cur_time, timer);
      if (mySubDebug>0) printf("[ subPollProcess ]: try to setup socket (%ds)\n",dt);

      setupSocket(precord);

      sockfd = open_socket(hostName,port);
      
      if(sockfd<=0) {
	if (mySubDebug>0) printf("[ subPollProcess ]: couln't get socket for host \"%s\" and port %d after %ds, sleep before retrying\n",hostName,port,dt);	
	sleep(1);
      }
    }

  }

  if(sockfd<=0) {
    printf("[ subPollProcess ]: [ WARNING ]: couldn't open a socket (tried over %ds period). Check host and port?\n",dt);
    return 0;
  }
  
  // poll the xml string
  
  if (mySubDebug) printf("[ subPollProcess] : Poll xml string\n");
  
  getXmlDoc(sockfd,0,0);
  
  if (mySubDebug) printf("[ subPollProcess ]: Poll XML done. Close socket if needed\n");
  if(sockfd>0) {
    sockfd = close_socket(sockfd);
  } 

  if(mySubDebug>1) {
    char * s = NULL;
    int len;
    getXmlDocStrFormat(&s, &len);
    printf("[ subPollProcess ]: got XML with len %d\n", len);
    if(len>0) printf("\n%s\n",s);
    if(s!=NULL) {
      printf("[ subPollProcess ]: free string at %p\n",s);      
      free(s);
      printf("[ subPollProcess ]: done free string at %p\n",s);      
    }
  }

  if (mySubDebug) printf("[ subPollProcess ]: before update status_poll_flag = %d\n", status_poll_flag);
  
  updatePollStatusFlag();
  
  if (mySubDebug) printf("[ subPollProcess ]: after update status_poll_flag = %d\n", status_poll_flag);


  return 0;
}



static long subPollDaqMapProcess(subRecord *precord) {
  process_order++;
  if (mySubDebug) {
    printf("[ subPollDaqMapProcess] %d Record %s called subPollDaqMapProcess(%p)\n", process_order, precord->name, (void*) precord);
  }

  if (mySubDebug) {
    printf("[ subPollDaqMapProcess] setup daq map\n");
  }

  setupDaqMap(precord);

  if (mySubDebug) {
    printf("[ subPollDaqMapProcess] done setup daq map\n");
  }


  return 0;
}




static long subPollStatProcess(subRecord *precord) {
  process_order++;
  if (mySubDebug>0) {
    printf("[ subPollStatProcess ]: %d Record %s called subPollStatProcess(%p)\n",process_order, precord->name, (void*) precord);
    printf("[ subPollStatProcess ]: status_flag: %d\n",status_flag);
    printf("[ subPollStatProcess ]: status_poll_flag: %d\n",status_poll_flag);
  }    
  
  if(status_flag==status_poll_flag) {
    if (mySubDebug>0) {
      printf("[ subPollStatProcess ]: same flag: no update was done\n");
    }    
    precord->val = 0;
  } else {
    if (mySubDebug>0) {
      printf("[ subPollStatProcess ]: diff flag: update was done, flip the status_flag\n");
    }    
    precord->val = 1;
    // update status_flag
    if(status_flag==1) {
      status_flag = 0;
    } else {
      status_flag = 1;
    }
  }
  
  
  return 0;
}






/* Register these symbols for use by IOC code: */

epicsExportAddress(int, mySubDebug);
epicsRegisterFunction(subLVInit);
epicsRegisterFunction(subLVProcess);
epicsRegisterFunction(subTempInit);
epicsRegisterFunction(subTempProcess);
epicsRegisterFunction(subPollInit);
epicsRegisterFunction(subPollProcess);
epicsRegisterFunction(subPollStatInit);
epicsRegisterFunction(subPollStatProcess);
epicsRegisterFunction(subPollDaqMapInit);
epicsRegisterFunction(subPollDaqMapProcess);
