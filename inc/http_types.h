
#ifndef HTTP_TYPE_H

#define HTTP_TYPE_H

#undef BUFSIZE
#define BUFSIZE 1024
#define READ_COUNT  512
#define LINE_NUM    10

#include <errno.h>
#include <stdlib.h>
#include<string.h>


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


typedef struct __http_context{
           char *buf;
           http_request * (*parse_req)(char *);
}http_context;


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

/*real request */
static http_request * analasy(char *buffer){
             
             char buf[BUFSIZE];
             bzero(buf,BUFSIZE);
             int i=0;
             if(strstr(buffer,"\r\n\r\n")==NULL){/*If null buf is nut full*/
                 return NULL;
             }
             char *cpstr=strdup(buf);
             char buf[BUFSIZE];
             bzero(buf,BUFSIZE);
             http_request *req=(http_request *)malloc(sizeof(http_request));

             if(readline_buf(&cpstr,buf,BUFSIZE))!=0){
                    strcpy(req->method,strtok(buf," "));
                    strcpy(req->url,strtok(NULL," "));
                    strcpy(req->version,strtok(NULL," "));
             }
             bzero(buf,BUFSIZE);
              while((readline_buf(&cpstr,buf,BUFSIZE))!=0){
                     strcpy(req->contain[i].name,strtok(buf,":"));
                     strcpy(req->contain[i].value,strtok(buf,":"));
                     i++;
             }
             free(cpstr);
             
}


/*create http_context info*/
http_context *alloc_http_context(){
        
        http_context *item=NULL;
        if(item=(http_context *)malloc(sizeof(http_context))==NULL){
             //perror("alloc_http_context error");
             goto err;
        }
        item->buf=(char *)malloc(sizeof(char)*BUFSIZE);
        if(item->buf==NULL){
               goto err;
        }
        item->parse_req=analasy;//append http parse
        return item;
        err:
          errno=(-1);
          return NULL; 
}

/*
GET /demLog.png HTTP/1.1
Host: localhost:8888
Connection: keep-alive
Upgrade-Insecure-Requests: 1
User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_12_6) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/59.0.3071.115 Safari/537.36
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng
q=0.8
Accept-Encoding: gzip, deflate, br
Accept-Language: zh-CN,zh;q=0.8
*/

typedef    void (*func_t)(int ,char *,size_t);


/*core calllback*/
http_request * callback(http_context *http_ctx,func_t f_recv,int fd,char *buf,size_t len){
             
                f_recv(fd,buf,len);//callback

                strncat(http_ctx->buf,buf,strlen(buf));//append new data
                
                return http_ctx->parse_req(http_ctx->buf);
}                          

/*recv new data*/
void onRecv(int fd,char *buf,size_t len){
           
        size_t nleft=len;
        char *cur_ptr=buf;
        size_t nread=0;
        
        while(nleft>0){
                    if((nread=read(fd,cur_ptr,nleft))<0){
                            if(errno==EAGAIN||errno==EWOULDBLOCK){
                                break;
                            }else if(errno==EINTR){
                                nread=0;
                                continue;
                            }
                            goto err;
                    }else if(nread==0){
                        break;//eof
                    }
                    nleft-=nread;
                    strncat(buf,cur_ptr,strlen(cur_ptr));
                    cur_ptr+=nread;
                    
        }
         return len-nleft;
}

typedef struct http__tcp__level{
        int fd;/*connected fd*/
        http_context *http_contxt;/*http_context object*/
        http_request * (*callback)(http_context *, );/*call back*/
        char  *buf;/*void buf*/
}http_tcp;


http_tcp * http_tcp_alloc(int fd){
        http_tcp *item=NULL;
        http_context *http_ctx=NULL;
        item=(http_tcp *)malloc(sizeof(http_tcp));
        if(item==NULL){
            goto err;
        }
        item->fd=fd;
        item->http_contxt=alloc_http_context();
        item->buf=malloc(sizeof(char)*BUFSIZE);
        item->callback=callback;
        if(http_ctx==NULL){
            goto err;
        }
        return item;
        err:
           errno=(-1);
           return NULL;
}



/*****http parse arg****************/





/**************************************/



#endif 