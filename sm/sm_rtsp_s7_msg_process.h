#ifndef SM_RTSP_S7_MSG_PROCESS_H_INCLUDED
#define SM_RTSP_S7_MSG_PROCESS_H_INCLUDED
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <sys/socket.h>
#include <linux/if.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include "public_def.h"
//asm发给sm的as的信息
typedef struct _AS_INFO
{
char ip[40];
int downPort;
}AS_INFO;
//sm发给asm的setup中ss的相关信息
typedef struct _S7_SS
{
char client[128];
char destination[40];
int client_port;
INT64 bandwidth;
}S7_SS;
//sm发给asm的setup消息
typedef struct _S7_SETUP_MSG
{
char asm_ip[40];
int asm_port;
int cseq;
char require[256];
char session_group[256];
char ondemandsessionid[128];
char policy[128];
char app_id[256];
int app_type;
S7_SS ss;
}S7_SETUP_MSG;
//asm发给sm的setup response的消息
typedef struct _S7_SETUP_RES
{
int err_code;
int cseq;
char protocol[10];
INT64 session;
char ondemandsessionid[128];
S7_SS ss;
AS_INFO as;
INT64 stream_handle;
}S7_SETUP_RES;
//sm发给asm的teardown消息
typedef struct _S7_TEARDOWN_MSG
{
char asm_ip[40];
int asm_port;
int cseq;
char require[256];
int reason;
INT64 session;
char ondemandsessionid[128];
}S7_TEARDOWN_MSG;
//asm发给sm的teardown response
typedef struct _S7_TEARDOWN_RES
{
int err_code;
int cseq;
INT64 session;
char ondemandsessionid[128];
}S7_TEARDOWN_RES;
//asm发给sm的announce消息
typedef struct _S7_ANNOUNCE_MSG
{
char sm_ip[40];
int sm_port;
int cseq;
char require[128];
INT64 session;
int notice;
char event_date[30];
char ondemandsessionid[128];
}S7_ANNOUNCE_MSG;
//sm发给asm的announce response
typedef struct _S7_ANNOUNCE_RES
{
int err_code;
int cseq;
INT64 session;
char ondemandsessionid[128];
}S7_ANNOUNCE_RES;
//创建sm发送给asm的setup的消息
int rtsp_s7_setup_msg_encode(S7_SETUP_MSG msg,char *setup_msg);
//解析asm向sm发送的setup response消息
int rtsp_s7_setup_res_parse(char *setup_res_mag,S7_SETUP_RES *res);
//创建sm发送给asm的teardown消息
int rtsp_s7_teardown_msg_encode(S7_TEARDOWN_MSG msg, char *teardown_msg);
//解析asm发给sm的teardown response消息
int rtsp_s7_teardown_res_parse(char *teardown_res_msg, S7_TEARDOWN_RES *res);
//解析asm发给sm的announce消息
int rtsp_s7_announce_msg_parse(char *announce_msg, S7_ANNOUNCE_MSG *res);
//创建sm发给asm的announce response消息
int rtsp_s7_announce_res_encode(S7_ANNOUNCE_RES res, char *announce_res);
#endif // SM_RTSP_S7_MSG_PROCESS_H_INCLUDED