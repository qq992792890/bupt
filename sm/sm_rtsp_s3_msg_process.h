#ifndef SM_RTSP_S3_H
#define SM_RTSP_S3_H

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

//SM����ODRM��SETUP��Ϣ��QAM�������Ϣ
typedef struct _S3_QAM
{
    char client[128];
    char destination[40];
    int client_port;
    INT64 bandwidth;
}S3_QAM;
//����SM����ODRM��SETUP��Ϣ
typedef struct _S3_SETUP_MSG
{
	char odrm_ip[40];
	int odrm_port;
	int cseq;
	char require[40];
	char ondemand_session_id[256];
	char sop_group[5][20];
	S3_QAM qam[QAM_NUM_MAX];
	int qam_num;
	char session_group[20];
	int start_point_slot;
	char start_point_npt[50];
	char policy[128];
	char inband_marker[256];
	char content_type[30];
	//sdp��Ϣ����
	int sdp_version;		//NGOD��Ĭ��Ϊ"0"
	char email_add[5];		//NGOD��Ĭ��Ϊ"-"
	char ntp[50];			//the time that the session setup message was created
	char add_type[10];		//"IN"
	char ip_version[10];	//"IP4"
	char sm_ip[40];			//�����Ự�ķ�������ַ��SM��ODRM��IP���˴�ӦΪSM��IP
	char s[2];				//NGOD��Ĭ��Ϊ" "
	int time[2];			//Describes the validity start/end times of the session. 0 indicates media is always available
	//a=X-playlist-item
	char provider_id[128];	//the CableLabs provider_id for the content asset	
	char asset_id[40];		//the CableLabs asset_id for the content asset	
	int start_npt;			// an 8-digit hex ASCII value, no ��0x�� prefix and no leading zero
	int stop_npt;			// an 8-digit hex ASCII value, no ��0x�� prefix and no leading zero			
	
	char c[15];				//NGOD��Ĭ��Ϊ"IN IP4 0.0.0.0 "
	char m[17];				//NGOD��Ĭ��Ϊ"video 0 udp MP2T"					
}S3_SETUP_MSG;

//ODRM����SM��SETUP RESPONSE��Ϣ���õ��Ĳ���
typedef struct _S3_SETUP_RES
{
	int err_code;
	int cseq;
	INT64 session;
	char ondemand_session_id[128];
	char sop_group[50];
	char sop[50];
	
    char client[128];
    char destination[40];
    int client_port;
    char source[40];
    int server_port;    
    INT64 bandwidth;
	
	char content_type[50];
	int content_length;
	
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
}S3_SETUP_RES;

//SM��ODRM����teardown��Ϣ
typedef struct _S3_TEARDOWN_MSG
{
	char odrm_ip[40];
	int odrm_port;
	int cseq;
	char require[128];
	int reason;
	INT64 session;
	char ondemand_session_id[128];
}S3_TEARDOWN_MSG;

//ODRM����SM��teardown response
typedef struct _S3_TEARDOWN_RES
{
	int err_code;
	int cseq;
	INT64 session;
	char ondemand_session_id[128];
	int stop_point_slot;
	char stop_point_npt[50];
}S3_TEARDOWN_RES;

//ODRM��SM����announce��Ϣ
typedef struct _S3_ANNOUNCE_MSG
{
	char sm_ip[40];
	int sm_port;
	int cseq;
	char require[128];
	INT64 session;
	int notice;
	char event_date[30];
	char npt[30];
	char ondemand_session_id[128];
}S3_ANNOUNCE_MSG;

//SM��ODRM����ANNOUNCE RESPONSE
typedef struct _S3_ANNOUNCE_RES
{
	int err_code;
	int cseq;
	INT64 session;
	char ondemand_session_id[128];
}S3_ANNOUNCE_RES;

//SM��ODRM����GET_PARAMETER��Ϣ
typedef struct _S3_GET_PARAMETER_MSG
{
	char odrm_ip[40];
	int odrm_port;
	int cseq;
	INT64 session;
	char require[128];
	char content_type[50];
	int content_length;
	char parameter[50];
}S3_GET_PARAMETER_MSG;
//ODRM����SM��GET_PARAMETER RESPONSE
typedef struct _S3_GET_PARAMETER_RES
{
	int err_code;
	int cseq;
	INT64 session;
	char content_type[50];
	char content_length;
	char parameter_val[300];
}S3_GET_PARAMETER_RES;

//SM��ODRM����SET_PARAMETER��Ϣ
typedef struct _S3_SET_PARAMETER_MSG
{
	char odrm_ip[40];
	int odrm_port;
	int cseq;
	char require[128];
	char content_type[50];
	int content_length;
	char content[50];
}S3_SET_PARAMETER_MSG;

//ODRM����SM��SET_PARAMETER RESPONSE
typedef struct _S3_SET_PARAMETER_RES
{
	int err_code;
	int cseq;
}S3_SET_PARAMETER_RES;



//����SM����ODRM��SETUP��Ϣ
int rtsp_s3_setup_msg_encode(S3_SETUP_MSG msg,char *setup_msg);
//����ODRM����SM��SETUP RESPONSE��Ϣ
int rtsp_s3_setup_res_parse(char *setup_res,S3_SETUP_RES *res);
//����SM����ODRM��TEARDOWN��Ϣ
int rtsp_s3_teardown_msg_encode(S3_TEARDOWN_MSG tear,char *tear_msg);
//����ODRM����SM��TEARDDOWN RESPONSE��Ϣ
int rtsp_s3_teardown_res_parse(char *tear_res,S3_TEARDOWN_RES *res);
//����ERM��SM���͵�ANNOUNCE��Ϣ
int rtsp_s3_announce_msg_parse(char * announce,S3_ANNOUNCE_MSG *ann);
//����SM��ERM���͵�ANNOUNCE RESPONSE��Ϣ
int rtsp_s3_announce_res_encode(S3_ANNOUNCE_RES res,char *ann_res);
//����SM��ODRM���͵�GET PARAMETER��Ϣ
int rtsp_s3_get_parameter_msg_encode(S3_GET_PARAMETER_MSG msg,char *get_parameter);
//����ODRM����SM��GET_PARAMETER RESPONSE��Ϣ
int rtsp_s3_get_parameter_res_parse(char *get_parameter_res,S3_GET_PARAMETER_RES *res);
//����SM��ODRM���͵�SET PARAMETER��Ϣ
int rtsp_s3_set_parameter_msg_encode(S3_SET_PARAMETER_MSG msg,char *set_parameter);
//����ODRM����SM��SET_PARAMETER RESPONSE��Ϣ
int rtsp_s3_set_parameter_res_parse(char *set_parameter_res,S3_SET_PARAMETER_RES *res);


#endif



