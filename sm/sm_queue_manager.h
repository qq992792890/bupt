#ifndef _SM_QUEUEMANAGER_H_
#define  _SM_QUEUEMANAGER_H_

//#define SM_MAXNUM 100

//#pragma pack(1)
/*�����ڴ��������ݽṹ*/
typedef struct
{
	int data_flag;/*��������*/
	int process_flag;/*���̱�־*/
	int data_len;
	char data[1024];
}SOCKET_DATA;

int PushToQueue(SOCKET_DATA* pItem,SOCKET_DATA*);

int PopFromQueue(int ,SOCKET_DATA* pItem,SOCKET_DATA*);

#endif  //_erm_QUEUEMANAGER_H_

