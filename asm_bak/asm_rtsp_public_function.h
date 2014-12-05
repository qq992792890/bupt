#ifndef SM_RTSP_PUBLIC_H
#define SM_RTSP_PUBLIC_H

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
#include <arpa/inet.h>
#include <ctype.h>
#include <uuid/uuid.h>
#include <time.h>
#include "public_def.h"



//����ondemandsessionid
int ondemandsessionid_generate(char *id);
//����NTP TIME
unsigned long NTP_time(time_t t);
//����URL��ַ
int parse_url(char *url, char *ip,int *port,char *dir);
//��ȡ��������IP��ַ
int getlocalip(char* eth,char* localip);
//ɾ���ַ�����β�ո�
char *trim(char *str);
//��ȡ��Ϣ����
int rtsp_get_msg_type(char *msg);
//����SM����ERM�Ĵ���response��Ϣ
int rtsp_err_res_encode(int err_code,int cseq,char *err_msg);
//��ѯTeardown��Ϣ��Reasonͷ�б���Ŷ�Ӧ������
int rtsp_reason_description(int reason_code,char *description);
//��ѯAnnounce��Ϣ��Noticeͷ�б���Ŷ�Ӧ������
int rtsp_notice_description(int notice_code,char *description);


#endif


