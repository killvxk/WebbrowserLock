//******************************************    
//纯C++的Socket访问Http封装类，Neeao修改    
//http://neeao.com    
//2009-08-25    
//******************************************    

#include <stdio.h>    
#include <stdlib.h>    
#include <string>    
#include <winsock2.h>    
#pragma comment(lib, "WS2_32")   

#include "HTTPRequest.h"     

#pragma comment(lib,"WS2_32.LIB")
#ifdef _DEBUG    
#undef THIS_FILE    
static char THIS_FILE[]=__FILE__;    
#define new DEBUG_NEW    
#endif    
   
   
//////////////////////////////////////////////////////////////////////    
// Construction/Destruction    
//////////////////////////////////////////////////////////////////////    
   
Request::Request()    
{    
   
}    
   
Request::~Request()    
{    
   
}    
   
   
//*******************************************************************************************************    
//MemBufferCreate:     
//                  Passed a MemBuffer structure, will allocate a memory buffer     
//                   of MEM_BUFFER_SIZE.  This buffer can then grow as needed.    
//*******************************************************************************************************    
void Request::MemBufferCreate(MemBuffer *b)    
{    
    b->size = MEM_BUFFER_SIZE;    
    b->buffer =(unsigned    char *) malloc( b->size );    
    b->position = b->buffer;    
}    
   
//*******************************************************************************************************    
// MemBufferGrow:      
//                  Double the size of the buffer that was passed to this function.     
//*******************************************************************************************************    
void Request::MemBufferGrow(MemBuffer *b)    
{    
    size_t sz;    
    sz = b->position - b->buffer;    
    b->size = b->size *2;    
    b->buffer =(unsigned    char *) realloc(b->buffer,b->size);    
    b->position = b->buffer + sz;   // readjust current position    
}    
   
//*******************************************************************************************************    
// MemBufferAddByte:     
//                  Add a single byte to the memory buffer, grow if needed.    
//*******************************************************************************************************    
void Request::MemBufferAddByte(MemBuffer *b,unsigned char byt)    
{    
    if( (size_t)(b->position-b->buffer) >= b->size )    
        MemBufferGrow(b);    
   
    *(b->position++) = byt;    
}    
   
//*******************************************************************************************************    
// MemBufferAddBuffer:    
//                  Add a range of bytes to the memory buffer, grow if needed.    
//*******************************************************************************************************    
void Request::MemBufferAddBuffer(MemBuffer *b,    
                                 unsigned char *buffer, size_t size)    
{    
    while( ((size_t)(b->position-b->buffer)+size) >= b->size )    
        MemBufferGrow(b);    
   
    memcpy(b->position,buffer,size);    
    b->position+=size;    
}    
   
//*******************************************************************************************************    
// GetHostAddress:     
//                  Resolve using DNS or similar(WINS,etc) the IP     
//                   address for a domain name such as www.wdj.com.     
//*******************************************************************************************************    
DWORD Request::GetHostAddress(LPCSTR host)    
{    
    struct hostent *phe;    
    char *p;    
   
    phe = gethostbyname( host );    
   
    if(phe==NULL)    
        return 0;    
   
    p = *phe->h_addr_list;    
    return *((DWORD*)p);    
}    
   
//*******************************************************************************************************    
// SendString:     
//                  Send a string(null terminated) over the specified socket.    
//*******************************************************************************************************    
void Request::SendString(SOCKET sock,LPCSTR str)    
{    
    send(sock,str,strlen(str),0);    
}    
   
//*******************************************************************************************************    
// ValidHostChar:     
//                  Return TRUE if the specified character is valid    
//                      for a host name, i.e. A-Z or 0-9 or -.:     
//*******************************************************************************************************    
BOOL Request::ValidHostChar(char ch)    
{    
    return( isalpha(ch) || isdigit(ch)    
        || ch=='-' || ch=='.' || ch==':' );    
}    
   
   
//*******************************************************************************************************    
// ParseURL:     
//                  Used to break apart a URL such as     
//                      http://www.localhost.com:80/TestPost.htm into protocol, port, host and request.    
//*******************************************************************************************************    
void Request::ParseURL(string url,LPSTR protocol,int lprotocol,LPSTR host,int lhost,LPSTR request,int lrequest,int *port)    
{    
    char *work,*ptr,*ptr2;    
   
    *protocol = *host = *request = 0;    
    *port=80;    
   
    work = strdup(url.c_str());    
    strupr(work);    
   
    ptr = strchr(work,':');                         // find protocol if any    
    if(ptr!=NULL)    
    {    
        *(ptr++) = 0;    
        lstrcpynA(protocol,work,lprotocol);    
    }    
    else   
    {    
        lstrcpynA(protocol,"HTTP",lprotocol);    
        ptr = work;    
    }    
   
    if( (*ptr=='/') && (*(ptr+1)=='/') )            // skip past opening /'s     
        ptr+=2;    
   
    ptr2 = ptr;                                     // find host    
    while( ValidHostChar(*ptr2) && *ptr2 )    
        ptr2++;    
   
    *ptr2=0;    
    lstrcpynA(host,ptr,lhost);    
   
    lstrcpynA(request,url.c_str() + (ptr2-work),lrequest);   // find the request    
   
    ptr = strchr(host,':');                         // find the port number, if any    
    if(ptr!=NULL)    
    {    
        *ptr=0;    
        *port = atoi(ptr+1);    
    }    
   
    free(work);    
}    
   
//*******************************************************************************************************    
// SendHTTP:     
//                  Main entry point for this code.      
//                    url           - The URL to GET/POST to/from.    
//                    headerSend        - Headers to be sent to the server.    
//                    post          - Data to be posted to the server, NULL if GET.    
//                    postLength    - Length of data to post.    
//                    req           - Contains the message and headerSend sent by the server.    
//    
//                    returns 1 on failure, 0 on success.    
//*******************************************************************************************************    
int Request::SendHTTP(string url,LPCSTR headerReceive,BYTE *post,    
                      DWORD postLength,HTTPRequest *req)    
{    
    WSADATA         WsaData;    
    SOCKADDR_IN     sin;    
    SOCKET          sock;    
    char            buffer[512];    
    char            protocol[20],host[256],request[1024];    
    int             l,port,chars,err;    
    MemBuffer       headersBuffer,messageBuffer;    
    char            headerSend[1024];    
    BOOL            done;    
   
   
   
   
    ParseURL(url,protocol,sizeof(protocol),host,sizeof(host),       // Parse the URL    
        request,sizeof(request),&port);    
    if(strcmp(protocol,"HTTP"))    
        return 1;    
   
    err = WSAStartup (0x0101, &WsaData);                            // Init Winsock    
    if(err!=0)    
        return 1;    
   
    sock = socket (AF_INET, SOCK_STREAM, 0);    
    //if (socket == INVALID_SOCKET)    
    if (sock == INVALID_SOCKET)    
    {    
        return 1;    
    }    
   
    sin.sin_family = AF_INET;                                       //Connect to web sever    
    sin.sin_port = htons( (unsigned short)port );    
    sin.sin_addr.s_addr = GetHostAddress(host);    
   
    if( connect (sock,(LPSOCKADDR)&sin, sizeof(SOCKADDR_IN) ) )    
    {    
   
        return 1;    
    }    
   
   
    if( !*request )    
        lstrcpynA(request,"/",sizeof(request));    
   
    if( post == NULL )    
    {    
        SendString(sock,"GET ");    
        strcpy(headerSend, "GET ");    
    }    
    else     
    {    
        SendString(sock,"POST ");    
        strcpy(headerSend, "POST ");    
    }    
    SendString(sock,request);    
    strcat(headerSend, request);    
   
    SendString(sock," HTTP/1.0/r/n");    
    strcat(headerSend, " HTTP/1.0/r/n");    
   
    SendString(sock,"Accept: image/gif, image/x-xbitmap,"   
        " image/jpeg, image/pjpeg, application/vnd.ms-excel,"   
        " application/msword, application/vnd.ms-powerpoint,"   
        " */*/r/n");    
    strcat(headerSend, "Accept: image/gif, image/x-xbitmap,"   
        " image/jpeg, image/pjpeg, application/vnd.ms-excel,"   
        " application/msword, application/vnd.ms-powerpoint,"   
        " */*/r/n");    
   
    SendString(sock,"Accept-Language: en-us/r/n");    
    strcat(headerSend, "Accept-Language: en-us/r/n");    
   
    SendString(sock,"Accept-Encoding: gzip, default/r/n");    
    strcat(headerSend, "Accept-Encoding: gzip, default/r/n");    
   
    SendString(sock,"User-Agent: Neeao/4.0/r/n");    
    strcat(headerSend, "User-Agent: Neeao/4.0/r/n");    
   
    if(postLength)    
    {    
        sprintf(buffer,"Content-Length: %ld/r/n",postLength);    
        SendString(sock,buffer);    
        strcat(headerSend, buffer);    
    }    
    //SendString(sock,"Cookie: mycookie=blablabla/r/n");    
    //  printf("Cookie: mycookie=blablabla/r/n");    
    SendString(sock,"Host: ");    
    strcat(headerSend, "Host: ");    
   
    SendString(sock,host);    
    strcat(headerSend, host);    
   
    SendString(sock,"/r/n");    
    strcat(headerSend, "/r/n");    
   
    if( (headerReceive!=NULL) && *headerReceive )    
    {    
        SendString(sock,headerReceive);    
        strcat(headerSend, headerReceive);    
    }    
   
    SendString(sock,"/r/n");                                // Send a blank line to signal end of HTTP headerReceive    
    strcat(headerSend, "/r/n");    
   
    if( (post!=NULL) && postLength )    
    {    
        send(sock,(const char*)post,postLength,0);    
        post[postLength]    = '/0';    
   
        strcat(headerSend, (const char*)post);    
    }    
   
    //strcpy(req->headerSend, headerSend);    
    req->headerSend     = (char*) malloc( sizeof(char*) * strlen(headerSend));    
    strcpy(req->headerSend, (char*) headerSend );    
   
   
    MemBufferCreate(&headersBuffer );    
    chars = 0;    
    done = FALSE;    
   
    while(!done)    
    {    
        l = recv(sock,buffer,1,0);    
        if(l<0)    
            done=TRUE;    
   
        switch(*buffer)    
        {    
        case '/r':    
            break;    
        case '/n':    
            if(chars==0)    
                done = TRUE;    
            chars=0;    
            break;    
        default:    
            chars++;    
            break;    
        }    
   
        MemBufferAddByte(&headersBuffer,*buffer);    
    }    
   
    req->headerReceive  = (char*) headersBuffer.buffer;    
    *(headersBuffer.position) = 0;    
   
   
   
    MemBufferCreate(&messageBuffer);                            // Now read the HTTP body    
   
    do   
    {    
        l = recv(sock,buffer,sizeof(buffer)-1,0);    
        if(l<0)    
            break;    
        *(buffer+l)=0;    
        MemBufferAddBuffer(&messageBuffer, (unsigned char*)&buffer, l);    
    } while(l>0);    
    *messageBuffer.position = 0;    
    req->message = (char*) messageBuffer.buffer;    
    req->messageLength = (messageBuffer.position - messageBuffer.buffer);    
   
   
    closesocket(sock);                                          // Cleanup    
   
    return 0;    
}    
   
   
//*******************************************************************************************************    
// SendRequest    
//    
//*******************************************************************************************************    
int Request::SendRequest(bool IsPost, string url, string& psHeaderSend, string& psHeaderReceive, string& psMessage)    
{    
    HTTPRequest         req;    
    int                 i,rtn;    
    LPSTR               buffer;    
   
    req.headerSend                          = NULL;    
    req.headerReceive                       = NULL;    
    req.message                             = NULL;    
   
    //Read in arguments    
   
   
    if(IsPost)    
    {                                                   /* POST */    
        i       = psHeaderSend.length();    
        buffer  = (char*) malloc(i+1);    
        strcpy(buffer, psHeaderSend.c_str());    
   
        rtn             = SendHTTP( url,    
            "Content-Type: application/x-www-form-urlencoded/r/n",    
            (unsigned char*)buffer,    
            i,    
            &req);    
   
        free(buffer);    
    }    
    else/* GET */   
    {    
        rtn = SendHTTP(url,NULL,NULL,0,&req);    
    }    
   
   
   
    if(!rtn)                                            //Output message and/or headerSend     
    {    
        psHeaderSend        = req.headerSend;    
        psHeaderReceive     = req.headerReceive;    
        psMessage           = req.message;    
   
   
        free(req.headerSend);    
        free(req.headerReceive);    
        free(req.message);    
        return 1;    
    }    
    else   
    {    
        return 0;    
    }    
}   