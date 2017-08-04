//#include "http_types.h"


#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define BUFSIZE 200

#define LINE_NUM 10
/*http contains*/
typedef struct __head__{
            char name[BUFSIZE];
            char value[BUFSIZE];
              
}http_head_contain;

/*http header*/
typedef struct __http_header{
    char method[BUFSIZE];
    char url[BUFSIZE];
    char version[BUFSIZE];
    http_head_contain contain[LINE_NUM];/*http contain head*/
}http_request;




int readline_buf(char **buf,char *usr,size_t maxlen){
           int i=0,len1,len2;

           if(*buf==NULL||(usr)==NULL){
               return (-1);
            }
            len1=strlen(*buf);
            len2=maxlen;
            (*buf)+=strspn(*buf," ");
           
            maxlen=len2>len1?len2:len1;
           
             usr[0]=*(*buf);  
            for(i=1;i<maxlen;i++){
               if(*(*buf+i)=='\n'&&( *(*buf+i-1)=='\r')){
                   
                    usr[i-1]=0;
                    break;
               }else if(*(*buf+i)=='\n'&&( *(*buf+i+1)=='\r')){
                    
                       usr[i]=0;
                       break;
               }
               else if(*(*buf+i)=='\0'){
                   
                   break;
               }
               usr[i]=*(*buf+i);  
               
                    
            }
            usr[i]=0;
            *buf+=(i+1);
            return (i);
            
}

void * analasy(const char *buffer){
             
             char buf[BUFSIZE];
             bzero(buf,BUFSIZE);
             
             int i=0;
             
             if(strstr(buffer,"\r\n\r\n")==NULL){/*If null buf is nut full*/
                 return NULL;
             }
            
             char *cpstr=strdup(buffer);
            
            // http_request *req=(http_request *)malloc(sizeof(http_request));
             
            //  if((readline_buf(&cpstr,buf,BUFSIZE))!=0){
            //         strcpy(req->method,strtok(buf," "));
            //         printf("%s ",req->method);
            //         strcpy(req->url,strtok(NULL," "));
            //          printf("%s ",req->url);
            //         strcpy(req->version,strtok(NULL," "));
            //          printf("%s ",req->version);
            //  }
             bzero(buf,BUFSIZE);
            //   while((readline_buf(&cpstr,buf,BUFSIZE))!=0){
            //          strcpy(req->contain[i].name,strtok(buf,":"));
            //          strcpy(req->contain[i].value,strtok(NULL,":"));
            //          i++;
            //  }
             free(cpstr);
             return NULL ;
}



int main(){

    char *m="GET / HTTP/1.1\r\nHost: www.baidu.com\r\nUser-Agent: Mozilla/5.0 (Windows; U; Windows NT 5.1; en-US; rv:1.7.6)\r\n\r\n";
    char buf[BUFSIZE];
    bzero(buf,BUFSIZE);
    
   // analasy(m);

    return 0;
}