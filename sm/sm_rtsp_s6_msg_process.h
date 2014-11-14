#ifndef SM_RTSP_S6_H
#define SM_RTSP_S6_H

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


//SM����ERM��SETUP��Ϣ��QAM�������Ϣ
typedef struct _QAM_INFO
{
    INT64 bandwidth;
    char qam_name[128];
    char client[128];
    char modulation[128];
}QAM_INFO;
//SM����ERM��SETUP��Ϣ
typedef struct _S6_SETUP_MSG
{
    char ip[40];
    int cseq;
    char require[256];
    char session_group[256];
    char encryption_type[128];
    char cas_id[128];
    char encrypt_control[256];
    char ondemandsessionid[256];
    char policy[128];
    char inband_marker[256];
    QAM_INFO qam_info[QAM_NUM_MAX];
    int qam_num;
}S6_SETUP_MSG;

//ERM����SM��SETUP��Ϣ��QAM�������Ϣ(������ģʽ��ʹ��)
typedef struct _QAM_Info1
{
    char destination[40];
    int client_port;
    char client[128];
}QAM_Info1;
//ERM����SM��setup��Ϣ��������ģʽ��ʹ�ã�
typedef struct _S6_SETUP_MSG1
{
	//char rtsp_ip[16];
	int cseq;
	char require[256];
	char session_group[256];
	char ondemandsessionid[256];
	char policy[128];
	QAM_Info1 qam_info1[QAM_NUM_MAX];
	int qam_num;
}S6_SETUP_MSG1;
//SM����ERM��setup response��Ϣ(������ģʽ��ʹ��)
typedef struct _S6_SETUP_RES1
{
	int err_code;
	int cseq;
	INT64 session;
	char destination[40];
	int client_port;
	char source[40];
	int server_port;
	char client[20];
	char embedded_encryptor[10];
	char ondemandsessionid[256];	
}S6_SETUP_RES1;
//ERM����SM��setup response��Ϣ
typedef struct _S6_SETUP_RES
{
	int err_code;
	int cseq;
	INT64 session;
	char client[20];
    char destination[40];
    int client_port;
	char qam_destination[30];
    char qam_name[50];
	char qam_group[50];
	char modulation[20];
	char edge_input_group[50];
	char embedded_encryptor[10];
	char ondemandsessionid[256];	
}S6_SETUP_RES;
//SM��ERM����teardown��Ϣ
typedef struct _S6_TEARDOWN_MSG1
{
	char rtsp_ip[40];
	int cseq;
	char require[128];
	int reason;
	INT64 session;
	char ondemandsessionid[128];
}S6_TEARDOWN_MSG1;
//ERM��SM����teardown��Ϣ
typedef struct _S6_TEARDOWN_MSG2
{
	char rtsp_ip[40];
	int cseq;
	char require[128];
	int reason;
	INT64 session;
	char ondemandsessionid[128];
}S6_TEARDOWN_MSG2;
//SM��ERM����teardown response
typedef struct _S6_TEARDOWN_RES1
{
	int err_code;
	int cseq;
	INT64 session;
	char ondemandsessionid[128];
}S6_TEARDOWN_RES1;
//ERM��SM����teardown response
typedef struct _S6_TEARDOWN_RES2
{
	int err_code;
	int cseq;
	INT64 session;
	char ondemandsessionid[128];
}S6_TEARDOWN_RES2;
//ERM��SM����announce��Ϣ
typedef struct _S6_ANNOUNCE_MSG
{
	char rtsp_ip[40];
	int cseq;
	char require[128];
	INT64 session;
	int notice;
	char event_date[30];
	char npt[30];
	char ondemandsessionid[128];
}S6_ANNOUNCE_MSG;
//SM��ERM����ANNOUNCE RESPONSE
typedef struct _S6_ANNOUNCE_RES
{
	int err_code;
	int cseq;
	INT64 session;
	char ondemandsessionid[128];
}S6_ANNOUNCE_RES;


//����ģʽ�»򱯹�ģʽ�´���SM����ERM��SETUP��Ϣ
int rtsp_s6_setup_msg_encode(S6_SETUP_MSG msg,char *setup_msg);
//����ģʽ�½���ERM��ΪRTSP������SM���͵�SETUP��Ϣ
int rtsp_s6_pes_setup_msg_parse(char *setup_msg,S6_SETUP_MSG1 *msg1);
//����ģʽ��SM��ERM���͵�Response��Ϣ
int rtsp_s6_pes_setup_res_encode(S6_SETUP_RES1 res1,char *res_msg);
//����ģʽ�»򱯹�ģʽ�½���ERM��SM���͵�Response��Ϣ
int rtsp_s6_setup_res_parse(char *msg,S6_SETUP_RES *res);
//����SM��ERM���͵�TEARDOWN��Ϣ
int rtsp_s6_teardown_msg_encode(S6_TEARDOWN_MSG1 msg1,char *tear_msg);
//����ERM��Ϊ������SM���͵�TEARDOWN��Ϣ
int rtsp_s6_teardown_msg_parse(char *tear_msg,S6_TEARDOWN_MSG2 *msg2);
//����SM����ERM��Teardown Response
int rtsp_s6_teardown_res_encode(S6_TEARDOWN_RES1 res1,char *tear_res);
//����ERM����SM��Teardown Response
int rtsp_s6_teardown_res_parse(char *tear_res,S6_TEARDOWN_RES2 *res2);
//����ERM��SM���͵�ANNOUNCE��Ϣ
int rtsp_s6_announce_msg_parse(char * announce,S6_ANNOUNCE_MSG *ann);
//����SM��ERM���͵�ANNOUNCE RESPONSE��Ϣ
int rtsp_s6_announce_res_encode(S6_ANNOUNCE_RES res,char *ann_res);

#endif
