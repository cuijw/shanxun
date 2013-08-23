/**
 ******************************************************************************
 * @file      routerdial.c
 * @brief     C Source file of routerdial.c.
 * @details   This file including all API functions's 
 *            implement of routerdial.c.	
 *
 * @copyright .
 ******************************************************************************
 */
 
/*-----------------------------------------------------------------------------
 Section: Includes
 ----------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "log.h"
#include "ini.h"
#include "socket.h"
#include "urldecode.h"

/*-----------------------------------------------------------------------------
 Section: Type Definitions
 ----------------------------------------------------------------------------*/
/* NONE */

/*-----------------------------------------------------------------------------
 Section: Constant Definitions
 ----------------------------------------------------------------------------*/
#define DLL1_API extern"C" __declspec(dllexport)

/*-----------------------------------------------------------------------------
 Section: Global Variables
 ----------------------------------------------------------------------------*/
/* NONE */

/*-----------------------------------------------------------------------------
 Section: Local Variables
 ----------------------------------------------------------------------------*/
/**
 * һ���ַ�������������
 * 1���û���
 * 2������
 * 3��·������ַ
 */
#if 0
/* ˮ��·���� */
static const char *prouterchar = "GET /userRpm/PPPoECfgRpm.htm?wantype=2&VnetPap=0&acc=%s&psw=%s&linktype=4&waittime2=0&Connect=%%C1%%AC+%%BD%%D3 HTTP/1.1\r\n"
"Accept: image/gif, image/x-xbitmap, image/jpeg, image/pjpeg, application/x-shockwave-flash, application/xaml+xml, application/vnd.ms-xpsdocument, application/x-ms-xbap, application/x-ms-application, application/QVOD, application/QVOD, */*\r\n"
"Referer: http://192.168.1.1/userRpm/PPPoECfgRpm.htm\r\n"
"Accept-Language: zh-cn\r\n"
"Accept-Encoding: gzip, deflate\r\n"
"User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1) ; .NET CLR 3.0.04506.30; .NET CLR 3.0.04506.648; .NET CLR 3.5.21022; .NET CLR 2.0.50727)\r\n"
"Host: 192.168.1.1\r\n"
"Connection: Keep-Alive\r\n"
"Authorization: Basic YWRtaW46YWRtaW4=\r\n\r\n";
#endif
#if 1
/* TP-LINK·���� */
static const char *prouterchar = "GET /userRpm/PPPoECfgRpm.htm?wantype=2&VnetPap=0&acc=%s&psw=%s&linktype=4&waittime2=0&Connect=%%C1%%AC+%%BD%%D3 HTTP/1.1\r\n"
        "Accept: */*\r\n"
        "Accept-Language: zh-cn\r\n"
        "UA-CPU: x86\r\n"
        "Accept-Encoding: gzip, deflate\r\n"
        "User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1) ; .NET CLR 3.0.04506.30; .NET CLR 3.0.04506.648; .NET CLR 3.5.21022; .NET CLR 2.0.50727)\r\n"
        "Host: %s\r\n"
        "Connection: Keep-Alive\r\n"
        "Authorization: Basic YWRtaW46YWRtaW4=\r\n\r\n";
;
#endif
/*-----------------------------------------------------------------------------
 Section: Local Function Prototypes
 ----------------------------------------------------------------------------*/
static char serverip[16] = {0};
static unsigned short port = 0u;
static char logflag = 0;

/*-----------------------------------------------------------------------------
 Section: Global Function Prototypes
 ----------------------------------------------------------------------------*/
/* NONE */

/*-----------------------------------------------------------------------------
 Section: Function Definitions
 ----------------------------------------------------------------------------*/
static unsigned int
connect_start(void)
{
    (void)ini_get_server_ip(serverip);
    (void)ini_get_server_port(&port);
    (void)ini_get_log_flag(&logflag);

    (void)log_init();
    log_on(logflag);
    return socket_init(serverip, port);
}

static void
connect_close(unsigned int socketfd)
{
    socket_close(socketfd);
    log_exit();
}

//
extern"C" __declspec(dllexport) int _stdcall _RouterDial(char *pusr, char *passwd)
{
    unsigned int socket = 0;
    char sendbuf[2048] = {0};
    char *pusrname = NULL;
    int sendlen = 0;

    if ((socket = connect_start()) == 0)
    {
        log_exit();
        return 0;
    }
#if 0
    log_print("in %s() arg: usr[0x%08x]:%s passwd[0x%08x]%s\n",
            __FUNCTION__, pusr, pusr, passwd, passwd);
#endif
    /* ���û���ת��Ϊurlcode */
    pusrname = url_encode(pusr);
    sprintf(sendbuf, prouterchar, pusrname, passwd, serverip);
    free(pusrname); /* ������ͷ� */
    sendlen = strlen(sendbuf) + 1;
    log_print("send:\n%s\n", sendbuf);
    do
    {
        if (sendlen != socket_send(socket, sendbuf, sendlen))
        {
            log_print("%s�����������ݳ���\n", __FUNCTION__);
            break;
        }
        else
        {
            log_print("%s�����������ݳɹ�\n", __FUNCTION__);
        }
#if 0
        log_print("recv:\n");
        while (0 < socket_recv(socket, sendbuf, sizeof(sendbuf)))
        {
            log_print("%s", sendbuf);
        }
#endif
    } while(0);
    connect_close(socket);

    return 0;
}

/*----------------------------routerdial.c--------------------------------*/
