#ifndef SM_RTSP_S1_H
#define SM_RTSP_S1_H

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

//STB����SM��SETUP��Ϣ��QAM�������Ϣ
typedef struct _QAM
{
    INT64 bandwidth;
    char qam_name[128];
    char client[128];
    char modulation[128];
}QAM;
//����STB����SM��SETUP��Ϣ��ȡ�Ĳ���
typedef struct _S1_SETUP_MSG
{
	char sm_ip[40];
	int sm_port;
	char rtsp_version[10];
	char require[40];
	char purchase_token[37];
	char server_id[40];
	int cseq;
	char client_session_id[50];
	QAM qam[QAM_NUM_MAX];
	int qam_num;		
}S1_SETUP_MSG;
//SM����STB��SETUP RESPONSE��Ϣ���õ��Ĳ���
typedef struct _S1_SETUP_RES
{
	int cseq;
	INT64 session;
	char destination[20];
	char ondemand_session_id[128];
	char client_session_id[50];
	char emm_data[30];
	char content_type[30];
	//sdp��Ϣ����
	int sdp_version;		//NGOD��Ĭ��Ϊ"0"
	char email_add[5];		//NGOD��Ĭ��Ϊ"-"
	INT64 ss_session;		//RTSP session ID of the Streaming Server,S1�ӿ���ʹ�ô˲��������Ϳ�����ϢC1
	char ntp[50];			//the time that the session setup message was created
	char add_type[10];		//"IN"
	char ip_version[10];	//"IP4"
	char ss_ip[40];			//IP address of the RTSP server on the Streaming Server
	char s[2];				//NGOD��Ĭ��Ϊ" "
	int time[2];			//Describes the validity start/end times of the session. 0 indicates media is always available
	char protocol[10];		//����rtsp, lscp, lscpu
	char host[40];			//the IP address or fully qualified DNS name of the stream control port
	int port;				//the TCP or UDP port number of the stream control port.
	INT64 stream_handle;	//the RTSP session ID or LSCP stream handle to control this stream, depending on the <protocol> value.
	char c[15];				//NGOD��Ĭ��Ϊ"IN IP4 0.0.0.0 "
	char m[17];				//NGOD��Ĭ��Ϊ"video 0 udp MP2T"			
}S1_SETUP_RES;
//STB����SM��teardown��Ϣ�еĲ���
typedef struct _S1_TEARDOWN_MSG
{
	char sm_ip[40];
	int sm_port;
	char rtsp_version[10];
	int cseq;
	char require[40];
	int reason;
	INT64 session;
	char ondemand_session_id[128];
	char client_session_id[50];
}S1_TEARDOWN_MSG;
//SM����STB��teardown response��Ϣ�еĲ���
typedef struct _S1_TEARDOWN_RES
{
	int err_code;
	int cseq;
	INT64 session;
	char ondemand_session_id[128];
	char client_session_id[50];
}S1_TEARDOWN_RES;
//����SM����STB��ANNOUNCE��Ϣ�еĲ���
typedef struct _S1_ANNOUNCE_MSG
{
	char sm_ip[40];
	int sm_port;
	int cseq;
	char require[40];
	INT64 session;
	int notice;
	char event_date[30];
	char npt[30];
	char ondemand_session_id[128];	
}S1_ANNOUNCE_MSG;
//����STB����SM��Announce Response��Ϣ�еĲ���
typedef struct _S1_ANNOUNCE_RES
{
	int err_code;
	int cseq;
	INT64 session;
	char ondemand_session_id[128];	
}S1_ANNOUNCE_RES;
//����STB����SM��PING��Ϣ�еĲ���
typedef struct _S1_PING_MSG
{
	char sm_ip[40];
	int sm_port;	
	int cseq;
	char require[40];
	INT64 session;
	char ondemand_session_id[128];	
}S1_PING_MSG;
//����SM����STB��Ping Response��Ϣ�еĲ���
typedef struct _S1_PING_RES
{
	int err_code;
	int cseq;
	INT64 session;
	char ondemand_session_id[128];	
}S1_PING_RES;



//����STB����SM��SETUP��Ϣ
int rtsp_s1_setup_msg_parse(char *setup_msg,S1_SETUP_MSG *msg);
//����SM����STB��SETUP RESPONSE��Ϣ
int rtsp_s1_setup_res_encode(S1_SETUP_RES res,char *setup_res);
//����STB����SM��TEARDOWN��Ϣ
int rtsp_s1_teardown_msg_parse(char *tear_msg,S1_TEARDOWN_MSG *tear);
//����SM����STB��TEARDOWN RESPONSE��Ϣ
int rtsp_s1_teardown_res_encode(S1_TEARDOWN_RES res,char *tear_res);
//����SM����STB��ANNOUNCE��Ϣ
int rtsp_s1_announce_msg_encode(S1_ANNOUNCE_MSG ann,char *ann_msg);
//����STB����SM��Announce Response��Ϣ
int rtsp_s1_announce_res_parse(char *ann_res,S1_ANNOUNCE_RES *ann);
//����STB����SM��PING��Ϣ
int rtsp_s1_ping_msg_parse(char *ping_msg,S1_PING_MSG *ping);
//����SM����STB��Ping Response��Ϣ
int rtsp_s1_ping_res_encode(S1_PING_RES ping,char *ping_res);


#endif


