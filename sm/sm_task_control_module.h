#ifndef SM_TASK_CONTROL_MODULE_H
#define SM_TASK_CONTROL_MODULE_H

#include "sm_queue_manager.h"
#include "sm_log.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#include <sys/mman.h>
#include <string.h>
#include <stropts.h>
#include <iostream>
#include <occi.h>
using namespace std;
using namespace oracle::occi;

//#pragma pack(1)
#define LOCKFILE "./lockfile"
#define LOCKDB "./lockdb"
#define LOCKMODE (S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)
#define MMAPDIR "./mmapdir"
#define write_lock(fd) lockfile((fd),F_WRLCK)
#define write_unlock(fd) lockfile((fd),F_UNLCK)
typedef void Sigfunc(int);
typedef struct {
	int flag;/*��ʶ�ö��ڴ������Ƿ�Ϊ��*/
	char *mmap_queue;/*sizeof(int)+sizeof(SOCKET_DATA)*/
}mmap_queue_info;
typedef struct {
	char *Queue_addr;/*��̬�����ڴ�νڵ�ָ��*/
}Queue_info;

typedef struct{
	int data_id;/*�����ڴ��������ݱ��*/
	int data_flag;
	int pthread_flag;
	int sd;
	int data_len;
	char data[1024];
}pthread_queue;
/*��¼����pid�Ͷ�Ӧ�Ľڵ�ָ�룬�ڽ����쳣�˳�ʱ������*/
typedef struct{
	int pid;/*����pid*/
	char *pid_queue_addr;/*�����ڴ�ڵ�ָ��*/
}process_info;

typedef struct{
	int i;
	StatelessConnectionPool *connPool;
}pthread_args;

int lockfile(int fd,int type);
int interproxy(int lockfd);
int process_manager(int fr,int sr,int lockfd);
void *pthread_SM(void *arg);

#endif

