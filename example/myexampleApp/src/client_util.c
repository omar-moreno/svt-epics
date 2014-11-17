#include "client_util.h"
#include <time.h>
#include <ctype.h>
#include "sys/ioctl.h"

int client_util_debug = 3;
char *xml_string = NULL;
xmlDoc* doc = NULL;
xmlNode* xml_root = NULL;
int xml_poll_status = 0;

void free_xml_string() {
  if(client_util_debug>2) printf("free_xml_string(): at %p\n",xml_string);
  if(xml_string!=NULL) {
    free(xml_string);
    xml_string = NULL;
    if(client_util_debug>2) printf("free_xml_string(): done\n");
  } else {
    if(client_util_debug>2) printf("free_xml_string(): already nil\n");
  }
  if(client_util_debug>2) printf("free_xml_string(): now at %p\n",xml_string);
}

char* strToUpper( char* s )
{
  char* p = s;
  while (*p) {
    if(islower(*p)) {
      *p = toupper(*p);
    }
    p++;
  }
  return s;
}


int getFebIdFromDaqMap(int hyb_id, char half[]) {  
  int feb_id;
  // use default value until the DAQ map can be added
  if(strcmp(half,"bot")==0) {
    feb_id = 0;
  }
  else if(strcmp(half,"top")==0) {
    feb_id = 1;
  } else {
    printf("ERROR getting feb id from hybrid %d and half %s\n",hyb_id,half);
    feb_id=-1;
  }
  return feb_id;
}


double extractTempValFromString(char value[]) {
  double t = 0.0;
  char* p_start = strstr(value,"C (");
  if(p_start != NULL) {
    char* t_str = (char*) malloc(p_start-value); //strlen(value));
    memcpy(t_str,value,p_start-value);//,p_start-value);
    t = atof(t_str);
    //if(mySubDebug) {
    //  printf("convert %s to float %f\n",t_str,t);
    //}
    free(t_str);
  } 
  return t;
}

void getSubStrFromName(char name[],const int i, char board_type[], const int MAX) {
  char buf[MAX];
  strcpy(buf,name);
  int idx;
  char* pch;
  memset(board_type,'\0',MAX);
  pch = strtok(buf,":");
  idx=0;
  while(pch!=NULL) {
    if(idx==i) {
      if(strlen(pch)>MAX) {
	printf("ERROR pch string is too long!\n");	
      } else {
	strcpy(board_type,pch);
	break;
      }
    }
    idx++;    
    pch = strtok(NULL,":");
  }  
  return;
}

void getType(char name[], char board_type[], const int MAX) {
  return getSubStrFromName(name,1,board_type,MAX);
}
void getBoardType(char name[], char board_type[], const int MAX) {
  return getSubStrFromName(name,2,board_type,MAX);
}
void getHalf(char name[], char board_type[], const int MAX) {
  return getSubStrFromName(name,3,board_type,MAX);
}
void getId(char name[], char board_type[], const int MAX) {
  return getSubStrFromName(name,4,board_type,MAX);
}
void getChName(char name[], char board_type[], const int MAX) {
  return getSubStrFromName(name,5,board_type,MAX);
}
void getAction(char name[], char board_type[], const int MAX) {
  return getSubStrFromName(name,6,board_type,MAX);
}
void getFebId(char name[], char board_type[], const int MAX) {
  return getSubStrFromName(name,3,board_type,MAX);
}
void getFebChName(char name[], char board_type[], const int MAX) {
  return getSubStrFromName(name,4,board_type,MAX);
}
void getFebAction(char name[], char board_type[], const int MAX) {
  return getSubStrFromName(name,5,board_type,MAX);
}



void socket_error(const char *msg)
{
    perror(msg);
    //exit(0);
}

int open_socket(char* hostname, int portno) {
  struct sockaddr_in serv_addr;
  struct hostent *server;
  int socketfd;
  if(client_util_debug>0) printf("open_socket : open for %s:%d \n",hostname,portno);
  socketfd = socket(AF_INET, SOCK_STREAM, 0);
  if (socketfd < 0) {
    socket_error("ERROR opening socket");
    return socketfd;
  }
  if(client_util_debug>1) printf("open_socket : opened socket %d \n",socketfd);
  if(client_util_debug>1) printf("open_socket : set server\n");
  server = gethostbyname(hostname);
  if (server == NULL) {
    socket_error("ERROR, no such host");
    return -1;
  }
  if(client_util_debug>1) printf("open_socket : got server at %p\n",server);
  bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  bcopy((char *)server->h_addr, 
        (char *)&serv_addr.sin_addr.s_addr,
        server->h_length);
  serv_addr.sin_port = htons(portno);

  if(client_util_debug>1) printf("open_socket : connect\n");
  
  if (connect(socketfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) {
    socket_error("ERROR connecting");
    return -1;
  }

  if(client_util_debug>1) printf("open_socket : return socket %d\n",socketfd);

  return socketfd;

}

int close_socket(int socketfd) {
  return close(socketfd);
}



void getXmlDoc(int sockfd, int read_status, int read_config) {

  if(client_util_debug>0) printf("getXmlDoc: from socket %d (read_status %d read_config %d)\n",sockfd,read_status,read_config);
  
  // check that I'm not already polling it
  if(xml_poll_status==1) {
    if(client_util_debug>0) printf("getXmlDoc: already polling so don't continue for this process (%d,%p)\n",xml_poll_status,xml_string);    
    return;
  }
  
  // occupy the poll flag
  xml_poll_status = 1;
  
  //clear old documents
  if(client_util_debug>1) printf("getXmlDoc: free xml string %p\n",xml_string);
  free_xml_string();
  //if(xml_string!=NULL) {
  //  free(xml_string);
  //}
  if(client_util_debug>1) printf("getXmlDoc: free xml string done %p\n",xml_string);
  if(client_util_debug>1) printf("getXmlDoc: free xml doc\n");
  if(doc!=NULL) {
    xmlFreeDoc(doc);
    xmlCleanupParser();
    doc=NULL;
    xml_root=NULL;
  }
  if(client_util_debug>1) printf("getXmlDoc: free xml doc done\n");

  // check that the socket is open
  if(sockfd<=0) {
    if(client_util_debug>0) printf("getXmlDoc: socket is not open.\n");
    //clear the flag
    xml_poll_status = 0;
    return;
  }


  // Do a status and config read to be sure to catch all info
  // Need to see exactly how the falgs are set in the return string 
  // from the server to see what to do here.

  if(read_status>0) {

    if(client_util_debug>0) printf("getXmlDoc: ReadStatus\n");
    
    writeReadStatus(sockfd);

  }
 
  if(read_config>0) {

    if(client_util_debug>0) printf("getXmlDoc: ReadConfig\n");
    
    writeReadConfig(sockfd);

  }

  if(client_util_debug>1) printf("getXmlDoc: Before reading xml string (%p)\n",xml_string);

  pollXmlString(sockfd);

  if(client_util_debug>1) printf("getXmlDoc: After reading xml string (%p)\n",xml_string);

  if(xml_string!=NULL) {
    if(strlen(xml_string)>0) {
      if(client_util_debug>1) printf("getXmlDoc: create xml document from xml string(strlen %ld)\n",strlen(xml_string));
      if(client_util_debug>0) printf("getXmlDoc: xml string:\n%s\n",xml_string);
      doc = xmlReadMemory(xml_string,strlen(xml_string),"noname.xml",NULL,0);
      if(client_util_debug>1) printf("getXmlDoc: xml doc done %p\n",doc);
      if(doc!=NULL) {
	xml_root = xmlDocGetRootElement(doc);
	if(xml_root!=NULL) {
	  if(client_util_debug>1) printf("getXmlDoc: found xml_root name %s\n",(xml_root)->name);
	}
      } else {
	printf("getXmlDoc: problem building xml doc at %p from \n%s\n",doc,xml_string);
	exit(1);
      }
    } else {
      printf("ERROR: getXmlDoc xml_string is there but has zero string length!\n");	
      exit(1);
    }
  } else {
    printf("WARNING: getXmlDoc: xml_string is NULL after reading from socket!\n");	
  }
  
  //clear the flag
  xml_poll_status = 0;
  if(client_util_debug>0) printf("getXmlDoc: cleared the flag and return (%d)\n",xml_poll_status);

}


int getXMLPollStatus() {
  int status = 0;
  if(xml_string!=NULL) {
    if(strlen(xml_string)>0) {
      if(doc!=NULL) {
	if(xml_root!=NULL) {
	  status = 1;
	}
      }
    }
  }
  return status;
}


void writeHybridVSwitch(int sockid, int value, int hyb_id) {  
  char buffer[256];
  char hyb_tag[256];
  char toggle[256];
  int n;
  memset(buffer,0,256);  
  memset(hyb_tag,0,256);  
  memset(toggle,0,256);  
  if(value==1) sprintf(toggle,"%s","True");
  else sprintf(toggle,"%s","False");
  sprintf(hyb_tag,"Hybrid%dPwrEn",hyb_id);
  sprintf(buffer,"<system><config><FrontEndTestFpga><FebCore><%s>%s</%s></FebCore></FrontEndTestFpga></config></system>\f",hyb_tag,toggle,hyb_tag);
  if(client_util_debug > 0) printf("writeHybridVSwitch: Executing:\"%s\"\n",buffer);
  n = write(sockid,buffer,strlen(buffer));
  if(n<0) 
    socket_error("writeHybridVSwitch: ERROR writing to socket");
  return;  
}




void writeHybridVTrim(int sockid, int value, int hyb_id, char ch_name[]) {  
  char buffer[256];
  char hyb_tag[256];
  char ch_name_corr[256];
  int n;
  memset(buffer,0,256);  
  memset(hyb_tag,0,256);  
  memset(ch_name_corr,0,256);  
  // correct name
  if(strcmp(ch_name,"dvdd")==0) strcpy(ch_name_corr,"Dvdd");
  if(strcmp(ch_name,"avdd")==0) strcpy(ch_name_corr,"Avdd");
  if(strcmp(ch_name,"v125")==0) strcpy(ch_name_corr,"V1_25");
  sprintf(hyb_tag,"Hybrid%d%sTrim",hyb_id,ch_name_corr);
  //  sprintf(buffer,"<system><config><FrontEndTestFpga><FebCore><%s>0x%x</%s></FebCore></FrontEndTestFpga></config></system>\f",hyb_tag,value,hyb_tag);
  sprintf(buffer,"<system><config><FrontEndTestFpga><FebCore><%s>%d</%s></FebCore></FrontEndTestFpga></config></system>\f",hyb_tag,value,hyb_tag);
  if(client_util_debug > 0) printf("writeHybridVTrim: Executing:\"%s\"\n",buffer);
  n = write(sockid,buffer,strlen(buffer));
  if(n<0) 
    socket_error("writeHybridVTrim: ERROR writing to socket");
  return;  
}



void writeReadConfig(int sockfd) {
  int n;
  //char buffer[] = "<system><command>ReadConfig</command></system>\f";
  char buffer[] = "<system><command><ReadConfig/></command></system>\f";
  if(client_util_debug) printf("writeReadConfig: Executing:\"%s\"\n",buffer);
  n = write(sockfd,buffer,strlen(buffer));
  if(n<0) 
    socket_error("writeReadConfig: ERROR writing to socket");
  return;
}

void writeReadStatus(int sockfd) {
  int n;
  char buffer[] = "<system><command><ReadStatus/></command></system>\f";
  if(client_util_debug) printf("writeReadStatus: Executing:\"%s\"\n",buffer);
  n = write(sockfd,buffer,strlen(buffer));
  if(n<0) 
    socket_error("writeReadStatus: ERROR writing to socket");
  return;
}





void getXMLValue(char tag[], char value[],const unsigned int MAX) {
  if(client_util_debug>0) printf("getXMLValue: tag \"%s\"\n",tag);
  if(doc==NULL) {
    if(client_util_debug>0) printf("WARNING: getXMLValue: couldn't get xml document %p\n",doc);
    return;
  }
  if(xml_root==NULL) {
    if(client_util_debug>0) printf("WARNING: getXMLValue: couldn't get root from document\n");
    return;
  }  
  if(client_util_debug>1) printf("getXMLValue: retrieve value from xml document\n");
  retrieveValue(doc,xml_root,tag,value,MAX);
  if(client_util_debug>1) printf("getXMLValue: retrieved value \"%s\" from xml\n",value);
}


void readFebT(int feb_id, char value[],char ch_name[],const unsigned int MAX) {
  char tag[256];// = "system:status:FrontEndTestFpga:FebCore:AxiXadc:Temperature";
  memset(tag,0,256);
  if(strcmp(ch_name,"axixadc")==0) {
    strcpy(tag,"system:status:FrontEndTestFpga:FebCore:AxiXadc:Temperature");
  } else if(strcmp(ch_name,"FebTemp0")==0 || strcmp(ch_name,"FebTemp1")==0) {
    sprintf(tag,"system:status:FrontEndTestFpga:FebCore:PowerMonitor:%s",ch_name);
  } else {
    printf("Error: wrong ch_name \"%s\" for readFebT\n",ch_name);
    return;
  }
  getXMLValue(tag,value,MAX);
}

void readHybridI(int feb_id, int hyb_id,char ch_name[], char value[],const unsigned int MAX) {
  char tag[256];
  sprintf(tag,"system:status:FrontEndTestFpga:FebCore:PowerMonitor:Hybrid%d_%s_Current",hyb_id,strToUpper(ch_name));
  getXMLValue(tag,value,MAX);
}

void readHybridT(int feb_id, int hyb_id, int tId, char value[],const unsigned int MAX) {
  char tag[256];
  sprintf(tag,"system:status:FrontEndTestFpga:FebCore:PowerMonitor:Hybrid%d_ZTemp%d",hyb_id,tId);
  getXMLValue(tag,value,MAX);
}

void readHybridV(int feb_id, int hyb_id,char ch_name[], char ch_pos[], char value[],const unsigned int MAX) {  
  char tag[256];
  char* ch_name_upper;
  ch_name_upper = strToUpper(ch_name);
  if(client_util_debug>1) printf("ch_name %s\n",ch_name_upper);
  sprintf(tag,"system:status:FrontEndTestFpga:FebCore:PowerMonitor:Hybrid%d_%s_%s",hyb_id,ch_name_upper,ch_pos);
  getXMLValue(tag,value,MAX);
}


void readHybridVTrim(int feb_id, int hyb_id, char ch_name[], char value[],const unsigned int MAX) {  
  char tag[256];
  char ch_name_corr[256];
  memset(tag,0,256);  
  memset(ch_name_corr,0,256);  
  // correct name
  if(strcmp(ch_name,"dvdd")==0) strcpy(ch_name_corr,"Dvdd");
  if(strcmp(ch_name,"avdd")==0) strcpy(ch_name_corr,"Avdd");
  if(strcmp(ch_name,"v125")==0) strcpy(ch_name_corr,"V1_25");
  sprintf(tag,"system:config:FrontEndTestFpga:FebCore:Hybrid%d%sTrim",hyb_id,ch_name_corr);
  getXMLValue(tag,value,MAX);
  return;  
}



xmlNode* retrieveElement(xmlDoc* doc, xmlNode* node, char* tag) {
  xmlNode* cur_node = NULL;
  xmlNode* found_it = NULL;
  //int debug = 1;
  for(cur_node = node; cur_node; cur_node = cur_node->next) {
    if(found_it!=NULL) {
      if(client_util_debug>3) printf("stop at cur_node %s prev %s  \n",cur_node->name,cur_node->prev->name);
      break;
    }
    if(client_util_debug>3) printf("Looking for %s and comparing to %s\n",tag,cur_node->name);
    if( (!xmlStrcmp(cur_node->name,(const xmlChar*)tag)) ) {
      if(client_util_debug>3) printf("found an element of type %d\n",cur_node->type);
      if (cur_node->type == XML_ELEMENT_NODE) {
        if(client_util_debug>3) printf("found it\n");
        found_it = cur_node;
        break;
      }
    }

    if(found_it!=NULL) {
      if(client_util_debug>3) printf("found it at name %s \n",cur_node->name);
      return found_it;
    }

    found_it = retrieveElement(doc,cur_node->children,tag);

  }

  return found_it;
}









int findSystemStr(char* buf, const int MAX, char** start) {
  if(client_util_debug>1) printf("finding system string from %p and %d chars len with start at %p\n",buf,MAX,*start);
  char* b;
  char* e;
  char* s;
  char* p_ending;
  char* status_tag_s;
  char* status_tag_e;

  b = buf;
  while(1!=0) {    
    s = strstr(b,"<system>");  
    p_ending = strchr(b,'\f');  

    if(s!=NULL) {
      if(p_ending!=NULL) {      
	//check that status exists
	if(client_util_debug>1) printf("found system at len %ld and ending and len %ld\n",s-b,p_ending-b);
	status_tag_s = strstr(b,"<status>");
	status_tag_e = strstr(b,"</status>");
	// look at this system string  if status tags are found inside the ending
	if(status_tag_s!=NULL && status_tag_e!=NULL) {
	  if(client_util_debug>1) printf("found status tags at len %ld and %ld\n",status_tag_s-b, status_tag_e-b);
	  if((status_tag_s-b)<(p_ending-b) && (status_tag_e-b)<(p_ending-b)) {
	    if(client_util_debug>1) printf("found status tags inside ending\n");
	    // return this
	    *start = s;
	    e = p_ending-1;
	    if(client_util_debug>1) {
	      printf("found s at %p and e at %p and *start at %p with len %ld \n",s,e,*start,e-s);
	      printf("last characters are:\n");
	      int ii;
	      for(ii=-50;ii<=0;++ii) {
		char ee = *(e+ii);
		printf("%d: '%c'\n",ii,ee);
	      }
	    }
	    return (int)(e-s);
	  }
	} 
	else {
	  // go to next, if there is one
	  b = p_ending+1;
	  if((b-buf)>MAX) return -1;
	}
      } else {
	if(client_util_debug>1) printf("p_ending couldn't be found\n"); 
	// nothing in this string to work with
	break;
      }
    } else {
      if(client_util_debug>1) printf("<system> couldn't be found\n"); 
      // nothing in this string to work with
      break;      
    }
  }
  
 
  return -1;
}




void pollXmlString(int socketfd) {
  char* buf = NULL;
  char* buf_loop = NULL;
  int buf_len;
  int read_i;
  int read_n;
  int nsleep;
  int counter;
  int n_endings;
  time_t timer;
  time_t cur_time;
  struct tm *lt;
  int dt;
  char *pch;

  
  if(client_util_debug>0) printf("pollXmlString:  from socket %d \n", socketfd);

  free_xml_string();
  
  time(&timer);
  
  if(client_util_debug>0) {
    lt = localtime(&timer);
    printf("pollXmlString: start_time at %s\n",asctime(lt));
  }
  
  nsleep=0;
  counter=0;
  read_i=0;
  buf_len=0;
  n_endings=0;
  dt=0;
  
  while(nsleep<6) { 
    
    time(&cur_time);
    dt = difftime(cur_time,timer);
    
    if(client_util_debug>0) printf("pollXmlString: Try to read from socket (nsleep %d read_i %d time %ds)\n",nsleep,read_i,dt);
    
    read_n = 0;
    ioctl(socketfd, FIONREAD, &read_n);
    
    if(client_util_debug>0) {
      printf("pollXmlString: %d chars available on socket\n",read_n);
    }
    
    if(read_n>0) {      
      
      // allocate memory needed
      if(client_util_debug>1) printf("pollXmlString: Allocate %d array\n",read_n);      
      
      // check that the buffer used is not weird
      if(buf_loop!=NULL) {
	printf("pollXmlString: ERROR: pollXmlString: buf_loop is not null!\n");
	exit(1);
      }
      
      // allocate space to hold the input
      buf_loop = (char*) calloc(read_n+1,sizeof(char));
      
      if(client_util_debug>1) printf("pollXmlString: Allocated buf_loop array at %p strlen %ld with %d length \n",buf_loop,strlen(buf_loop),(int)sizeof(char)*(read_n+1));      
      
      // Read from socket
      read_n = read(socketfd,buf_loop,read_n);
      
      if(client_util_debug>0) printf("pollXmlString: Read %d chars from socket\n",read_n);
      
      if(client_util_debug>2) printf("pollXmlString: buf_loop strlen is %ld\n",strlen(buf_loop));
      
      if (read_n < 0) {
	printf("pollXmlString: ERROR: pollXmlString: read %d from socket\n",read_n);
	exit(1);
      }
      
      //fix c-string terminations, cases where there is a termination in the middle..
      int k;
      for(k=0;k<read_n;++k) {
      	//printf("pollXmlString: '%c'\n",buf_loop[k]);
      	if(buf_loop[k]=='\0') {
      	  if(client_util_debug>2) printf("pollXmlString: fix termination at idx %d in this buf_loop\n",k);
	  buf_loop[k]=' ';
      	}
      }
      
      if(client_util_debug>2) printf("pollXmlString: Fixed buf_loop strlen %ld:\n%s\n",strlen(buf_loop),buf_loop);
      

      // search for xml endings in this buffer
      pch = strchr(buf_loop,'\f'); 
      while(pch!=NULL) { 
	if(client_util_debug>1) printf("pollXmlString: found ending at %p (array index %ld) in this buf!\n",pch,pch-buf_loop); 
	n_endings++; 
	pch = strchr(pch+1,'\f'); 
      } 
      
      
      // copy to other buffer while looping            
      if(client_util_debug>2) printf("pollXmlString: Copy %d to other buffer (at %p before realloc) \n",read_n,buf);      

      // reallocate more memory
      buf = (char*) realloc(buf,sizeof(char)*(buf_len+read_n));
      if(buf==NULL) {
	printf("pollXmlString: ERROR: pollXmlString: failed to allocated buf\n");
	if(buf_loop==NULL) {
	  free(buf_loop);
	}
	exit(1);
      }
      
      if(client_util_debug>2) printf("pollXmlString: Allocated longer buf at %p and copy to pointer %p (offset= %d) \n",buf,buf+buf_len,buf_len);      
      

      // do the copy
      memcpy(buf+buf_len,buf_loop,sizeof(char)*read_n);
      
      if(client_util_debug>1) printf("pollXmlString: memcpy done\n");
      
      //update the buffer length counter
      buf_len += read_n;      
      
      if(client_util_debug>1) printf("pollXmlString: free buf_loop\n");
      
      // free loop buffer for next loop
      if(buf_loop!=NULL) {
	free(buf_loop);
	buf_loop=NULL;
      }
      
      if(client_util_debug>2) printf("pollXmlString: end of read_i %d with buf strlen %ld\n",read_i,strlen(buf));
      
      read_i++;
      
    } // read_n>0
    else {
      if(client_util_debug>0) printf("pollXmlString: Nothing to read from socket. Sleep 1s..\n");      
      sleep(1);
      nsleep++;
    } 
    
    
    
    if(n_endings>1) {
      if(client_util_debug>0) printf("pollXmlString: \nfound %d endings at read_i %d with at len %d and strlen %ld. Stop reading from buffer\n",n_endings,read_i,buf_len,strlen(buf));      
      break;
    }
    
    
    counter++;
    
    // dummy check for time-out?
    if(counter>50) {
      printf("pollXmlString: Many (%d) iterations to get XML string. Something is strange\n",counter);
      break;
    }
    
  }
  
  
  
  if(client_util_debug>0) {
    printf("pollXmlString: Done reading from socket. Found %d endings and a buf_len of %d\n",n_endings, buf_len);
    if(buf!=NULL) printf("pollXmlString: strlen %ld\n", strlen(buf));
  }
  
  
  if(n_endings>=1) {
    if(client_util_debug>0) printf("pollXmlString: \nPick out config and status string between <system> and %d endings in string with strlen %ld and buf_len %d\n",n_endings,strlen(buf),buf_len);
    if(client_util_debug>1) printf("pollXmlString: \nbuf: \n%s\n",buf);
    
    //search for the <status> tag in each <system>->'\f' substring
    
    char* start = NULL;
    int len = findSystemStr(buf, buf_len,&start);    
    if(len>0) {      
      char* stop = start+len;
      if(client_util_debug>1) printf("pollXmlString: len %d start at %p stop at %p\n",len,start, stop);
      if(client_util_debug>1) printf("pollXmlString: calloc xml string len %d\n",len);
      xml_string = (char*) calloc(len+1,sizeof(char));
      //xml_string = (char*) malloc(sizeof(char)*len);
      if(client_util_debug>1) printf("pollXmlString: copy to xml string at %p\n",xml_string);	
      memcpy(xml_string,start,len);
      // terminate
      xml_string[len] = '\0'; 
      if(client_util_debug>1) printf("pollXmlString: \ncopied %d chars to %p with strlen %ld\n%s\n",len+1,xml_string,strlen(xml_string),xml_string);
      
/*       char* config = strstr(start,"<config>"); */
/*       if(config!=NULL && ((config-start)<len)) { */
/* 	char* status = strstr(start,"<status>"); */
/* 	if(status!=NULL && ((status-start)<len)) { */
/* 	  // seems we found all of them.  */
/* 	  if(client_util_debug>1) printf("pollXmlString: calloc xml string len %d\n",len); */
/* 	  xml_string = (char*) calloc(len,sizeof(char)); */
/* 	  //xml_string = (char*) malloc(sizeof(char)*len); */
/* 	  if(client_util_debug>1) printf("pollXmlString: copy to xml string at %p\n",xml_string);	 */
/* 	  memcpy(xml_string,start,len); */
/* 	  if(client_util_debug>1) printf("pollXmlString: \ncopied %d chars to %p with strlen %ld\n%s\n",len,xml_string,strlen(xml_string),xml_string); */
      
/* 	} else { */
/* 	   if(client_util_debug>0) printf("pollXmlString: \n no status found\n"); */
/* 	} */
/*       } else { */
/* 	if(client_util_debug>0) printf("pollXmlString: \n no config found\n"); */
/*       } */
    }
    else {
      if(client_util_debug>0) printf("pollXmlString: Couldn't find system and/or status string in xml buffer\n");
    }
  } 
  
  if(xml_string==NULL) {
    if(client_util_debug>0) printf("pollXmlString: No valid xml string extracted from this poll (%d endings)\n",n_endings);
  }
  
  return;

}




void retrieveValue(xmlDoc* doc, xmlNode* node, char* tags, char value[], const unsigned int MAX) {
  xmlChar* value_str = NULL;
  char* pch;
  char* pch_prev=NULL;
  xmlNode* cur_node = NULL;
  xmlNode* prev_node = node;
  //use a copy since it modifies the original string
  //char* tags = (char*)malloc(strlen(tags)*sizeof(char));
  //strcpy(tags,tags);
  if(client_util_debug>0) printf("retrieveValue: for tag \"%s\"\n",tags);
  if(strlen(tags)>0) {
    pch = strtok(tags," :,.-");
    while(pch!=NULL) {
      if(client_util_debug>2) printf("retrieveValue: Find element %s \n",pch);
      if(pch_prev!=NULL) {
        if(client_util_debug>2) printf("retrieveValue: Find element %s from children of prev element at %p\n",pch,prev_node->name);
        cur_node = retrieveElement(doc,prev_node->children,pch);
      }
      else {
        if(client_util_debug>2) printf("retrieveValue: Find element %s from element %s\n",pch,prev_node->name);
        cur_node = retrieveElement(doc,prev_node,pch);
      }

      // check that we found it
      if(cur_node != NULL) {
        if(client_util_debug>2) printf("retrieveValue: found cur_node name %s\n",cur_node->name);
      } else {
        if(client_util_debug>2) printf("retrieveValue: couldn't find cur_node\n");
        break;
      }

      pch_prev = pch;
      prev_node = cur_node;
      pch = strtok(NULL," :,.");
    }

    //if there is a node at the end it means we should get it's list of strings
    // if it's not the lowest level it might returns some garbage -> FIX THIS!
    if(cur_node !=NULL) {
      value_str  = xmlNodeListGetString(doc,cur_node->children,0);
      if(value_str!=NULL) {
        if(client_util_debug>2) printf("retrieveValue: Found value %s\n",value_str);
        if(strlen((char*) value_str)>=MAX) {
  	  if(client_util_debug>2) printf("retrieveValue: the value for tags=%s is %ld i.e. larger than MAX=%d, return no value!\n",tags,strlen((char*)value_str),MAX);
          value_str = NULL;
        } else {
	  if(client_util_debug>2) printf("retrieveValue: copy the value_str=\"%s\" (%ld) to %p\n",value_str,strlen((char*)value_str),value);
          strcpy((char*)value,(char*)value_str);
        }
      } else {
        value_str = NULL;
        if(client_util_debug>2) printf("retrieveValue: Found no value for tags %s\n",tags);
      }
    } else {
	value_str = NULL;
	if(client_util_debug>2) printf("retrieveValue: cur_node is null so no value found for tags=%s\n",tags);
    }
  }
  if(value_str!=NULL) {
    if(client_util_debug>1) printf("retrieveValue: free value_str\n");
    xmlFree(value_str);
  }
}


void getXmlDocStrFormat(char** xml_str, int * xml_str_len) {
  if(doc!=NULL) {
    xmlDocDumpFormatMemoryEnc	(doc,xml_str,xml_str_len, "UTF-8",1);    
  }
}
