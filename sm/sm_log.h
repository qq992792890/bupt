#ifndef _SM_LOG_H
#define _SM_LOG_H
#include <stdio.h>
#include<time.h>
#include <stdarg.h>
#include<pthread.h>
/*--------------------------------------------------------*/
/* �汾:v1.0

 * ����:������

 * ����ʱ��:2008-08-21

 *�޸ļ�¼:

 *����˵��:��־ģ�飬�����¼��־
 * 		��־�ļ�������
 *		1.ϵͳ��Ϣ��Systeminfo-p0.log
 *		(����ļ�������p0Ϊp1~pn)
 *		2.ͨ����Ϣ��1turnk-2008-08-11-p0.log
 *		(1turnkΪ1ͨ����2turnkΪ2ͨ�����������ƣ�

 ----------------------------------------------------------*/

#define LVLSYS 0
#define LVLERR 1
#define LVLDEBUGOFF 3 /*������־�رտ���*/
#define LVLDEBUGON 2  /*������־��������*/
#define MAXFILELEN (2150000000UL) /*һ����־��󳤶�~=2GB����λbyte*/
#define SYS_INFO -1 /*ϵͳ��־�Ĵ�ӡ��־*/
#define sm_log(x,y,...) thelog(x,y,__FILE__,__LINE__,__VA_ARGS__)
long getfilelen(FILE *fd); 
/* lvl ��־���� �֣�LVLSYS ,LVLDEBUG,LVLERR
 * idnum ͨ����,0 ������ͨ����ӡ��־.������ΪLVLSYSʱ ͨ��ΪidnumΪ0
 */
int thelog(int lvl,int idnum,const char *,int line,char *fmt, ...); 
void ChangeAscii2String(char *pStr, char *pAscii, int len);
#endif
