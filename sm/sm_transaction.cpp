#include "sm_rtsp_s1_msg_process.h"
#include "sm_rtsp_s3_msg_process.h"
#include "sm_rtsp_s6_msg_process.h"
#include "sm_rtsp_public_function.h"
#include "sm_rtsp_s7_msg_process.h"
#include "sm_communication_module.h"
#include "sm_transaction.h"
#include "sm_log.h"
#include <iostream>
#include <hiredis/hiredis.h>
//#include <occi.h>
using namespace std;
//using namespace oracle::occi;
extern int LVLDEBUG;

int pthread_TP(char *S1_msg,int msg_len,int OC_sd,pthread_args *p_args)
{
    int ret = -1;
    ret = rtsp_get_msg_type(S1_msg);

    if (ret == RTSP_ID_S1_SETUP) {
	fprintf(stderr, "Tp_setup begin...\n");
        Tp_Setup(S1_msg,msg_len,OC_sd,p_args);
    } else if (ret == RTSP_ID_S1_TEARDOWN) {
        Tp_Teardown(S1_msg,msg_len,OC_sd,p_args);
    } else {
        write(OC_sd,"TYPE ERROR",20);
    }
    return 0;
}

int Tp_Setup(char * S1_msg,int msg_len,int OC_sd,pthread_args *p_args)
{
    redisContext *c;
    redisReply *reply;
    const char *hostname = "127.0.0.1";
    int port = 6379;
    struct timeval timeout = {1, 500000};
    c = redisConnectWithTimeout(hostname, port, timeout);

    S1_SETUP_MSG s1_setup_msg;
    S1_SETUP_RES s1_setup_res;
    //S3_SETUP_MSG s3_setup_msg;
    //S3_SETUP_RES s3_setup_res;
    //S6_SETUP_MSG s6_setup_msg;
    //S6_SETUP_RES s6_setup_res;
    S7_SETUP_RES s7_setup_res;
    S7_SETUP_MSG s7_setup_msg;

    int erm_sd = -1;
    int odrm_sd = -1;
    int asm_sd = -1;
    INT64 oc2sm_session = 0;//stb和sm之间的session id
    int sm2erm_cseq = 0;//sm和erm之间的cseq
    int sm2odrm_cseq = 0;//sm和odrm之间的cseq
    int sm2asm_cseq = 0;
    struct timeval now_tmp;
    int i = 0;
    int ret = -1;
    char ondemandsession[37] = "";
    char sendbuf[1024] = "";//存储发送消息的缓冲
    char recvbuf[1024] = "";//存储收到消息的缓冲
    //char sql_cmd[512] = "";//存储sql命令
    //string cmd;
//连接数据库
    //Connection *conn = p_args->connPool->getAnyTaggedConnection();
	//Statement *stmt = conn->createStatement();
	
	ondemandsessionid_generate(ondemandsession);//产生ondemandsessionid
	
	gettimeofday(&now_tmp, 0);
	srand((now_tmp.tv_sec * 1000) + (now_tmp.tv_usec / 1000));
	oc2sm_session = 1 + (int) (10.0 * rand() / (100000 + 1.0));//生成S1接口session id
	//sm2erm_cseq = 1 + (int) (1.0 * rand() / (100000 + 1.0));//生成sm和erm之间的cseq
	//sm2odrm_cseq = 1 + (int) (1.0 * rand() / (100000 + 1.0));//生成sm和odrm之间的cseq
    sm2asm_cseq = 1 + (int) (1.0 * rand() / (100000 + 1.0));
//cout << ondemandsession	<< endl;
    
//解析收到的s1_setup_msg
	memset(&s1_setup_msg,0x00,sizeof(S1_SETUP_MSG));
	fprintf(stderr, "parse s1 message..\n");
    rtsp_s1_setup_msg_parse(S1_msg,&s1_setup_msg);//解析收到的s1_setup_msg
	/*
		对stb发来的setup msg进行错误处理
	*/    
/*
//与ERM建立会话
    memset(&s6_setup_msg,0x00,sizeof(S6_SETUP_MSG));
    strcpy(s6_setup_msg.ip,ERM_IP);
    s6_setup_msg.cseq = sm2erm_cseq;
    strcpy(s6_setup_msg.require,RTSP_S6_REQUIRE);
    strcpy(s6_setup_msg.session_group,SESSION_GRUOP);//自己设定、主要用来getpramater时获取session组的信息，每个SM一个组
    strcpy(s6_setup_msg.encryption_type,"Session");//S2
    strcpy(s6_setup_msg.cas_id,"0x1234");//S2
    strcpy(s6_setup_msg.encrypt_control,"block_stream_until_encrypted=\"false\";encryption_scheme=\"AES\";key_length=128");//S2
    strcpy(s6_setup_msg.ondemandsessionid,ondemandsession);
    strcpy(s6_setup_msg.policy,"priority=1");//S2
    strcpy(s6_setup_msg.inband_marker,"type=4;pidType=A;pidValue=01EE;dataType=T;insertDuration=10000;data=4002003030…");//待定，部分从S2获得，data从哪来？
    s6_setup_msg.qam_num = s1_setup_msg.qam_num;
    for (i=0;i<s6_setup_msg.qam_num;i++) {
        s6_setup_msg.qam_info[i].bandwidth = 2700000;
        strcpy(s6_setup_msg.qam_info[i].qam_name,s1_setup_msg.qam[i].qam_name);
        strcpy(s6_setup_msg.qam_info[i].client,s1_setup_msg.qam[i].client);
        strcpy(s6_setup_msg.qam_info[i].modulation,"qam64");
    }
	fprintf(stderr, "connect to erm...\n");
    ret=ConnectSock(&erm_sd,ERM_PORT,ERM_IP);//创建与ERM的连接
    if (ret==-1) {
	fprintf(stderr, "connect to erm error...\n");
        sm_log(LVLDEBUG,SYS_INFO,"Connect erm error\n");
      	//错误处理
        memset(sendbuf,0x00,1024);
        rtsp_err_res_encode(RTSP_ResponseCode_ERMSetupFailed_NoResponse,s1_setup_msg.cseq,sendbuf);//向STB报错：连接ERM失败
        ret = rtsp_write(OC_sd,sendbuf,strlen(sendbuf)+1);//向STB发送ERROR消息
    	sm_log(LVLDEBUG,SYS_INFO,"setup send2oc msg:%s, len:%d\n",sendbuf,ret);        
        return -1;
    }

	fprintf(stderr, "connect to erm success...\n");
    memset(sendbuf,0x00,1024);		
	fprintf(stderr, "msg to S6 encode\n");
    rtsp_s6_setup_msg_encode(s6_setup_msg,sendbuf);//积极模式下或悲观模式下创建SM发给ERM的SETUP信息
	fprintf(stderr, "send msg to S6\n");
    ret = rtsp_write(erm_sd,sendbuf,strlen(sendbuf)+1);//向ERM发送SETUP消息
    sm_log(LVLDEBUG,SYS_INFO,"setup send2erm msg:%s, len:%d\n",sendbuf,ret);
//接收ERM发回的SETUP RESPONSE
	memset(recvbuf,0x00,1024);    
    ret = rtsp_read(erm_sd,recvbuf,1024) ;//接收ERM发回的SETUP RESPONSE
	fprintf(stderr, "read msg from S6 done\n");
    sm_log(LVLDEBUG,SYS_INFO,"setup recv from erm res:%s, len:%d\n",recvbuf,ret);
    //积极模式下或悲观模式下解析ERM向SM发送的Response消息
	memset(&s6_setup_res,0x00,sizeof(S6_SETUP_RES));
	rtsp_s6_setup_res_parse(recvbuf,&s6_setup_res);
	fprintf(stderr, "parse msg from S6 done\n");
    //对erm发来的setup response进行错误处理
	//将与ERM的会话信息入库
	//snprintf(sql_cmd,512,"INSERT INTO SM_S6 VALUES('%s',%llu,'%s','%s','%s','%s','%s','%s','%s','%s',%llu,'%s','%s',%d,'%s','%s','%s','%s','%s','%s')", \
	//					 ondemandsession,s6_setup_res.session,s6_setup_msg.ip,s6_setup_msg.session_group,s6_setup_msg.encryption_type,\
	//					 s6_setup_msg.cas_id,s6_setup_msg.encrypt_control,s6_setup_msg.policy,s6_setup_msg.inband_marker,s6_setup_res.embedded_encryptor,\
	//					 s6_setup_msg.qam_info[0].bandwidth,s6_setup_res.client,s6_setup_res.destination,s6_setup_res.client_port,\
	//					 s6_setup_res.qam_destination,s6_setup_res.qam_name,s6_setup_res.qam_group,s6_setup_res.modulation,s6_setup_res.edge_input_group,"SETUP");    
	//cmd = sql_cmd;
	//stmt->executeUpdate(cmd.c_str());
	//stmt->executeUpdate("commit");
*/	
/*
//与ODRM建立会话
	memset(&s3_setup_msg,0x00,sizeof(S3_SETUP_MSG));
    strcpy(s3_setup_msg.odrm_ip,"ODRM_IP");
    s3_setup_msg.odrm_port = ODRM_PORT;
    s3_setup_msg.cseq = sm2odrm_cseq;
    strcpy(s3_setup_msg.require,RTSP_S3_REQUIRE);
    strcpy(s3_setup_msg.ondemand_session_id,ondemandsession);

    strcpy(s3_setup_msg.sop_group[0],"boston.spg1");//待定
    strcpy(s3_setup_msg.sop_group[1],"boston.spg2");//待定

    s3_setup_msg.qam_num = 1;//积极模式下ERM返回唯一QAM
    strcpy(s3_setup_msg.qam[0].client,s6_setup_res.client);
    strcpy(s3_setup_msg.qam[0].destination,s6_setup_res.destination);
    s3_setup_msg.qam[0].client_port = s6_setup_res.client_port;
    s3_setup_msg.qam[0].bandwidth = 2700000; //由S2消息中Resource Descriptors中的TSDownstreamBandwidth获得
    	
    strcpy(s3_setup_msg.session_group,SESSION_GRUOP);
    s3_setup_msg.start_point_slot = 1;//S2获得
    strcpy(s3_setup_msg.start_point_npt,"3000");//S2获得
    strcpy(s3_setup_msg.policy,"priority=1");//S2获得
    strcpy(s3_setup_msg.inband_marker,"type=4;pidType=A;pidValue=01EE;dataType=T;insertDuration=10000;data=4002003030");//待定，部分从S2获得，data从哪来？

    s3_setup_msg.sdp_version = 0;
    strcpy(s3_setup_msg.email_add,"-");
    
    sprintf(s3_setup_msg.ntp,"%lu",NTP_time(time(NULL)));//生成会话NTP时间
    strcpy(s3_setup_msg.add_type,"IN");
    strcpy(s3_setup_msg.ip_version,"IP4");
    strcpy(s3_setup_msg.sm_ip,SM_IP);
    strcpy(s3_setup_msg.s," ");
    s3_setup_msg.time[0] = 0;//会话开始时间
    s3_setup_msg.time[1] = 0;//会话结束时间 0 0表示永远可用
    strcpy(s3_setup_msg.provider_id,"comcast.com");//S2
    strcpy(s3_setup_msg.asset_id,"abcd1234567890123456");//S2
    s3_setup_msg.start_npt = 1000;//S2
    s3_setup_msg.stop_npt = 6000;//S2

    strcpy(s3_setup_msg.c,"IN IP4 0.0.0.0");
    strcpy(s3_setup_msg.m,"video 0 udp MP2T");

    memset(sendbuf,0x00,1024);
    rtsp_s3_setup_msg_encode(s3_setup_msg,sendbuf);
    // printf("%s",sendbuf);

    ret=ConnectSock(&odrm_sd,ODRM_PORT,ODRM_IP);
    if (ret==-1) {
        sm_log(LVLDEBUG,SYS_INFO,"Connect odrm error\n");
      	//错误处理
        memset(sendbuf,0x00,1024);
        rtsp_err_res_encode(RTSP_ResponseCode_ODRMSetupFailed_NoResponse,s1_setup_msg.cseq,sendbuf);//向STB报错：连接ODRM失败
        ret = rtsp_write(OC_sd,sendbuf,strlen(sendbuf)+1);//向STB发送ERROR消息
    	sm_log(LVLDEBUG,SYS_INFO,"setup send2oc msg:%s, len:%d\n",sendbuf,ret);   
        return -1;
    }
    ret = rtsp_write(odrm_sd,sendbuf,strlen(sendbuf)+1);//向ODRM发送SETUP消息
    sm_log(LVLDEBUG,SYS_INFO,"setup send2odrm msg:%s, len:%d\n",sendbuf,ret);
//接收ODRM发回的SETUP RESPONSE
    memset(recvbuf,0x00,1024);
    ret = rtsp_read(odrm_sd,recvbuf,1024);//接收ODRM发回的SETUP RESPONSE  
    sm_log(LVLDEBUG,SYS_INFO,"setup recv from odrm res:%s, len:%d\n",recvbuf,ret);
    memset(&s3_setup_res,0x00,sizeof(S3_SETUP_RES));
    rtsp_s3_setup_res_parse(recvbuf,&s3_setup_res);//解析ODRM发给SM的SETUP RESPONSE消息 
	//对odrm发来的setup response进行错误处理
	//将与ODRM的会话信息入库
	//snprintf(sql_cmd,512,"INSERT INTO SM_S6 VALUES('%s',%llu,'%s','%s','%s','%s','%s','%s','%s','%s',%llu,'%s','%s',%d,'%s','%s','%s','%s','%s','%s')", \
	//					 ondemandsession,s6_setup_res.session,s6_setup_msg.ip,s6_setup_msg.session_group,s6_setup_msg.encryption_type,\
	//					 s6_setup_msg.cas_id,s6_setup_msg.encrypt_control,s6_setup_msg.policy,s6_setup_msg.inband_marker,s6_setup_res.embedded_encryptor,\
	//					 s6_setup_msg.qam_info[0].bandwidth,s6_setup_res.client,s6_setup_res.destination,s6_setup_res.client_port,\
	//					 s6_setup_res.qam_destination,s6_setup_res.qam_name,s6_setup_res.qam_group,s6_setup_res.modulation,s6_setup_res.edge_input_group,"SETUP");    
	//memset(sql_cmd,0x00,sizeof(sql_cmd));
	//snprintf(sql_cmd,512,"INSERT INTO SM_S3 VALUES('%s',%llu,'%s',%d,'%s','%s','%s',%d,'%s','%s','%s','%s','%s',%d,'%s',%d,%llu,'%s',%d,%d,'%s','%s',%d,%d,%llu,'%s','%s','%s','%s',%d,%llu,'%s')",\
	//					 ondemandsession,s3_setup_res.session,s3_setup_msg.odrm_ip,s3_setup_msg.odrm_port,s3_setup_res.sop_group,s3_setup_res.sop,\
	//					 s3_setup_msg.session_group,s3_setup_msg.start_point_slot,s3_setup_msg.start_point_npt,s3_setup_msg.policy,s3_setup_msg.inband_marker,\
	//					 s3_setup_res.client,s3_setup_res.destination,s3_setup_res.client_port,s3_setup_res.source,s3_setup_res.server_port,s3_setup_res.bandwidth,\
	//					 s3_setup_msg.sm_ip,s3_setup_msg.time[0],s3_setup_msg.time[1],s3_setup_msg.provider_id,s3_setup_msg.asset_id,s3_setup_msg.start_npt,s3_setup_msg.stop_npt,\
	//					 s3_setup_res.ss_session,s3_setup_res.ntp,s3_setup_res.ss_ip,s3_setup_res.protocol,s3_setup_res.host,s3_setup_res.port,s3_setup_res.stream_handle,"SETUP");    
	//cmd = sql_cmd;
	//stmt->executeUpdate(cmd.c_str());
	//stmt->executeUpdate("commit");
*/
//与ASM建立会话
    if(s1_setup_msg.app_id != NULL){
        memset(&s7_setup_msg,0x00,sizeof(S7_SETUP_MSG));
        strcpy(s7_setup_msg.asm_ip,ASM_IP);
        s7_setup_msg.asm_port = ASM_PORT;
        s7_setup_msg.cseq = sm2asm_cseq;
        strcpy(s7_setup_msg.require,RTSP_S7_REQUIRE);
        strcpy(s7_setup_msg.session_group,SESSION_GRUOP);//自己设定、主要用来getpramater时获取session组的信息，每个SM一个组
        strcpy(s7_setup_msg.ondemandsessionid,ondemandsession);
        strcpy(s7_setup_msg.policy,"priority=1");//S2
        strcpy(s7_setup_msg.app_id, s1_setup_msg.app_id);
        s7_setup_msg.app_type = s1_setup_msg.app_type;
        //strcpy(s7_setup_msg.ss.client, s3_setup_res.client);
        strcpy(s7_setup_msg.ss.client, s1_setup_msg.qam[0].client);
        //strcpy(s7_setup_msg.ss.destination, s3_setup_res.destination);
        strcpy(s7_setup_msg.ss.destination, "127.0.0.1");
        //s7_setup_msg.ss.client_port = s3_setup_res.client_port;
        s7_setup_msg.ss.client_port = SS_PORT;
        //s7_setup_msg.ss.bandwidth = s3_setup_res.bandwidth;      
        s7_setup_msg.ss.bandwidth = 2700000;

        fprintf(stderr, "connect to asm...\n");
        ret=ConnectSock(&asm_sd,ASM_PORT,ASM_IP);//创建与ASM的连接
        if (ret==-1) {
	    fprintf(stderr, "connect to asm error...\n");
            sm_log(LVLDEBUG,SYS_INFO,"Connect asm error\n");
            
            //错误处理
            memset(sendbuf,0x00,1024);
            rtsp_err_res_encode(RTSP_ResponseCode_ASMSetupFailed_NoResponse,s1_setup_msg.cseq,sendbuf);//向STB报错：连接ASM失败
            ret = rtsp_write(OC_sd,sendbuf,strlen(sendbuf)+1);//向STB发送ERROR消息
        	sm_log(LVLDEBUG,SYS_INFO,"setup send2oc msg:%s, len:%d\n",sendbuf,ret);        
            return -1;
        }

        fprintf(stderr, "connect to asm success...\n");
        memset(sendbuf,0x00,1024);		
        fprintf(stderr, "msg to S7 encode\n");
        rtsp_s7_setup_msg_encode(s7_setup_msg,sendbuf);
        fprintf(stderr, "send msg to S7\n");
        ret = rtsp_write(asm_sd,sendbuf,strlen(sendbuf)+1);
        sm_log(LVLDEBUG,SYS_INFO,"setup send2asm msg:%s, len:%d\n",sendbuf,ret);
//接    收ASM发回的SETUP RESPONSE
        memset(recvbuf,0x00,1024);    
        ret = rtsp_read(asm_sd,recvbuf,1024) ;
        fprintf(stderr, "read msg from S7 done\n");
        sm_log(LVLDEBUG,SYS_INFO,"setup recv from asm res:%s, len:%d\n",recvbuf,ret);
   
        memset(&s7_setup_res,0x00,sizeof(S7_SETUP_RES));
        rtsp_s7_setup_res_parse(recvbuf,&s7_setup_res);
        fprintf(stderr, "parse msg from S7 done\n");

        char *numstr;
        sprintf(numstr, "%llu", s7_setup_res.session);
        reply = (redisReply*)redisCommand(c,"SET %s %s", ondemandsession, numstr);
        freeReplyObject(reply);
        redisFree(c);
	    /*

	    	对asm发来的setup response进行错误处理
	    */
	    //将与ASM的会话信息入库
	    //snprintf(sql_cmd,512,"INSERT INTO SM_S7 VALUES('%s',%llu,'%s','%s','%s','%s','%s','%s','%s','%s',%llu,'%s','%s',%d,'%s','%s','%s','%s','%s','%s')", \
	    //					 ondemandsession,s7_setup_res.session,s7_setup_msg.ip,s7_setup_msg.session_group,s7_setup_msg.encryption_type,\

	    //					 s7_setup_msg.cas_id,s7_setup_msg.encrypt_control,s7_setup_msg.policy,s7_setup_msg.inband_marker,s7_setup_res.embedded_encryptor,\
	    //					 s7_setup_msg.qam_info[0].bandwidth,s7_setup_res.client,s7_setup_res.destination,s7_setup_res.client_port,\
	    //					 s7_setup_res.qam_destination,s7_setup_res.qam_name,s7_setup_res.qam_group,s7_setup_res.modulation,s7_setup_res.edge_input_group,"SETUP");    
	//cmd = sql_cmd;
	//stmt->executeUpdate(cmd.c_str());
	//stmt->executeUpdate("commit");
    }
	
//向STB发送setup response消息
    //生成随机session号
    memset(&s1_setup_res,0x00,sizeof(S1_SETUP_RES));	
    s1_setup_res.cseq = s1_setup_msg.cseq;
    s1_setup_res.session = oc2sm_session;
    
    strcpy(s1_setup_res.destination, "127.0.0.1");
    strcpy(s1_setup_res.ondemand_session_id,ondemandsession);
    strcpy(s1_setup_res.client_session_id,s1_setup_msg.client_session_id);
    strcpy(s1_setup_res.emm_data,"40203F21A5");//待定
    s1_setup_res.sdp_version = 0;
    strcpy(s1_setup_res.email_add,"-");
    
    strcpy(s1_setup_res.add_type,"IN");
    strcpy(s1_setup_res.ip_version,"IP4");
   
    strcpy(s1_setup_res.s," ");
   
    strcpy(s1_setup_res.c,"IN IP4 0.0.0.0");
    strcpy(s1_setup_res.m,"video 0 udp MP2T");
    if(s1_setup_msg.app_id == NULL) {
        //strcpy(s1_setup_res.destination,s6_setup_res.qam_destination);
        //s1_setup_res.ss_session = s3_setup_res.ss_session;
        //strcpy(s1_setup_res.ntp,s3_setup_res.ntp);
        //strcpy(s1_setup_res.ss_ip,s3_setup_res.ss_ip);
        //s1_setup_res.time[0] = s3_setup_res.time[0];
        //s1_setup_res.time[1] = s3_setup_res.time[1];
        //strcpy(s1_setup_res.protocol,s3_setup_res.protocol);
        //strcpy(s1_setup_res.host,s3_setup_res.host);
        //s1_setup_res.port = s3_setup_res.port;
        //s1_setup_res.stream_handle = s3_setup_res.stream_handle;
    }
    else{
        //strcpy(s1_setup_res.destination,s6_setup_res.qam_destination);
        strcpy(s1_setup_res.destination,"");
        //s1_setup_res.ss_session = s3_setup_res.ss_session;
        s1_setup_res.ss_session = -1;
        strcpy(s1_setup_res.ntp,"");
        strcpy(s1_setup_res.ss_ip,"");
        s1_setup_res.time[0] = 0;
        s1_setup_res.time[1] = 0;
        //strcpy(s1_setup_res.protocol,s3_setup_res.protocol);
        strcpy(s1_setup_res.protocol, "lscp");
        strcpy(s1_setup_res.host,s7_setup_res.as.ip);
        s1_setup_res.port = s7_setup_res.as.downPort;
        s1_setup_res.stream_handle = s7_setup_res.stream_handle;
    }

    memset(sendbuf,0x00,1024);
    rtsp_s1_setup_res_encode(s1_setup_res,sendbuf);//创建SM发给STB的setup response消息
    ret = rtsp_write(OC_sd,sendbuf,strlen(sendbuf)+1);//向STB发送SETUP消息
    sm_log(LVLDEBUG,SYS_INFO,"setup send2oc msg:%s, len:%d\n",sendbuf,ret);
    	
    //将与STB的会话信息入库
    //snprintf(sql_cmd,512,"INSERT INTO SM_S1 VALUES('%s','%s',%llu,'%s',%d,'%s','%s','%s','%s',%llu,'%s','%s',%d,%d,'%s','%s',%d,%llu,'%s')",\
    //					 ondemandsession,s1_setup_res.client_session_id,s1_setup_res.session,s1_setup_msg.sm_ip,s1_setup_msg.sm_port,\
    //					 s1_setup_msg.purchase_token,s1_setup_msg.server_id,s1_setup_res.emm_data,s1_setup_res.destination,\
    //					 s1_setup_res.ss_session,s1_setup_res.ntp,s1_setup_res.ss_ip,s1_setup_res.time[0],s1_setup_res.time[1],\
    //					 s1_setup_res.protocol,s1_setup_res.host,s1_setup_res.port,s1_setup_res.stream_handle,"SETUP");    
	//cmd = sql_cmd;
	//stmt->executeUpdate(cmd.c_str());
    //stmt->executeUpdate("commit");

//关//闭数据库连接
	//conn->terminateStatement(stmt);
    //p_args->connPool->releaseConnection(conn);
    return 0;
}

int Tp_Teardown(char * S1_msg,int msg_len,int OC_sd,pthread_args *p_args)
{
    redisContext *c;
    redisReply *reply;
    const char *hostname = "127.0.0.1";
    int port = 6379;
    struct timeval timeout = {1, 500000};
    c = redisConnectWithTimeout(hostname, port, timeout);

    S1_TEARDOWN_MSG s1_tear_msg;
    S1_TEARDOWN_RES s1_tear_res;
    //S3_TEARDOWN_MSG s3_tear_msg;
    //S3_TEARDOWN_RES s3_tear_res;
    //S6_TEARDOWN_MSG1 s6_tear_msg1;
    //S6_TEARDOWN_RES2 s6_tear_res2;
    S7_TEARDOWN_MSG s7_tear_msg;
    S7_TEARDOWN_RES s7_tear_res;

    int erm_sd = -1;
    int odrm_sd = -1;
    int asm_sd = -1;
    int ret = -1;
    char sendbuf[1024] = "";
    char recvbuf[1024] = "";
    //char sql_cmd[256] = "";//存储sql命令
    //string cmd;
    int oc2sm_session = 0;
	//int odrm2sm_session = 0;//odrm和sm之间的session id
	//int erm2sm_session = 0;//erm和sm之间的session id
    int asm2sm_session = 0;
	//int sm2erm_cseq = 0;
	//int sm2odrm_cseq = 0;
    int sm2asm_cseq = 0;
	struct timeval now_tmp;
	
//连接数据库   
    //Connection *conn = p_args->connPool->getAnyTaggedConnection();
	//Statement *stmt = conn->createStatement();

//生成session和cseq	
	gettimeofday(&now_tmp, 0);
	srand((now_tmp.tv_sec * 1000) + (now_tmp.tv_usec / 1000));
	oc2sm_session = 1 + (int) (10.0 * rand() / (100000 + 1.0));//生成S1接口session id
	//sm2erm_cseq = 1 + (int) (1.0 * rand() / (100000 + 1.0));//生成sm和erm之间的cseq
	//sm2odrm_cseq = 1 + (int) (1.0 * rand() / (100000 + 1.0));//生成sm和odrm之间的cseq
    sm2asm_cseq = 1 + (int) (1.0 * rand() / (100000 + 1.0));

//解析STB发给SM的TEARDOWN消息
    rtsp_s1_teardown_msg_parse(S1_msg,&s1_tear_msg);//解析STB发给SM的TEARDOWN消息


	
//撤销与ODRM的会话
	//数据库查询指定业务ondemandsessionid的S3接口的会话号    
    //snprintf(sql_cmd,256,"SELECT SESSION_ID FROM SM_S3 WHERE ONDEMAND_SESSION_ID='%s'",s1_tear_msg.ondemand_session_id);
    //cmd = sql_cmd;
    //ResultSet *rs = stmt->executeQuery(cmd.c_str());
    //rs->next();
    //odrm2sm_session = (int)rs->getNumber(1);
    //cout << odrm2sm_session << endl;
    /*
    strcpy(s3_tear_msg.odrm_ip,ODRM_IP);
    s3_tear_msg.odrm_port = ODRM_PORT;
    s3_tear_msg.cseq = sm2odrm_cseq;
    s3_tear_msg.reason = s1_tear_msg.reason;
    s3_tear_msg.session = odrm2sm_session;
    strcpy(s3_tear_msg.ondemand_session_id,s1_tear_msg.ondemand_session_id);

    rtsp_s3_teardown_msg_encode(s3_tear_msg,sendbuf);

    ret=ConnectSock(&odrm_sd,ODRM_PORT,ODRM_IP);
    if (ret==-1) {
        sm_log(LVLDEBUG,SYS_INFO,"Connect odrm error");
       	//错误处理
        return -1;
    }
    ret = rtsp_write(odrm_sd,sendbuf,strlen(sendbuf)+1);//向ODRM发送teardown消息
    sm_log(LVLDEBUG,SYS_INFO,"teardown send2odrm msg:%s, len:%d\n",sendbuf,ret);
    ret = rtsp_read(odrm_sd,recvbuf,1024);//接收ODRM发回的teardown response
    sm_log(LVLDEBUG,SYS_INFO,"teardown recv from odrm res:%s, len:%d\n",recvbuf,ret);
    //解析odrm发来 的teardown response
    rtsp_s3_teardown_res_parse(recvbuf,&s3_tear_res);
	//修改数据库状态
	//snprintf(sql_cmd,256,"UPDATE SM_S3 SET STATUS='TEARDOWN' WHERE ONDEMAND_SESSION_ID='%s'",s1_tear_msg.ondemand_session_id);    
	//cmd = sql_cmd;
	//stmt->executeUpdate(cmd.c_str());
    //stmt->executeUpdate("commit");    
*/

    
//撤销与ERM的会话（积极模式）
	//数据库查询指定业务ondemandsessionid的S3接口的会话号    
    //snprintf(sql_cmd,256,"SELECT SESSION_ID FROM SM_S6 WHERE ONDEMAND_SESSION_ID='%s'",s1_tear_msg.ondemand_session_id);
    //cmd = sql_cmd;
    //rs = stmt->executeQuery(cmd.c_str());
    //rs->next();
    //erm2sm_session = (int)rs->getNumber(1);
/*    
    strcpy(s6_tear_msg1.rtsp_ip,ERM_IP);
    s6_tear_msg1.cseq = sm2erm_cseq;
    s6_tear_msg1.reason = s1_tear_msg.reason;
    s6_tear_msg1.session = erm2sm_session;
    strcpy(s6_tear_msg1.ondemandsessionid,s1_tear_msg.ondemand_session_id);
    
    memset(sendbuf,0x00,1024);
    rtsp_s6_teardown_msg_encode(s6_tear_msg1,sendbuf);//创建向STB发送的teardown response
    
    ret=ConnectSock(&erm_sd,ERM_PORT,ERM_IP);
    if (ret==-1) {
        sm_log(LVLDEBUG,SYS_INFO,"Connect erm error");
       	//错误处理
        return -1;
    }
    ret = rtsp_write(erm_sd,sendbuf,strlen(sendbuf)+1);//向ERM发送SETUP消息
    sm_log(LVLDEBUG,SYS_INFO,"teardown send2erm msg:%s, len:%d\n",sendbuf,ret);
    memset(recvbuf,0x00,1024);
    ret = rtsp_read(erm_sd,recvbuf,1024);//接收ERM发回的SETUP RESPONSE
    sm_log(LVLDEBUG,SYS_INFO,"teardown recv from erm res:%s, len:%d\n",recvbuf,ret);
    //解析ERM发给SM的Teardown Response
	rtsp_s6_teardown_res_parse(recvbuf,&s6_tear_res2);
*/	   
    //修改数据库状态
	//snprintf(sql_cmd,256,"UPDATE SM_S6 SET STATUS='TEARDOWN' WHERE ONDEMAND_SESSION_ID='%s'",s1_tear_msg.ondemand_session_id);    
	//cmd = sql_cmd;
	//stmt->executeUpdate(cmd.c_str());
    //stmt->executeUpdate("commit"); 

    reply = (redisReply*)redisCommand(c,"GET %s", (char*)s1_tear_msg.ondemand_session_id);
    asm2sm_session = atoi(reply->str);
    freeReplyObject(reply);
    strcpy(s7_tear_msg.asm_ip, ASM_IP);
    s7_tear_msg.asm_port = ASM_PORT;
    s7_tear_msg.cseq = sm2asm_cseq;
    strcpy(s7_tear_msg.require, RTSP_S7_REQUIRE);
    s7_tear_msg.reason = s1_tear_msg.reason;
    s7_tear_msg.session = asm2sm_session;
    strcpy(s7_tear_msg.ondemandsessionid, s1_tear_msg.ondemand_session_id);

    memset(sendbuf, 0x00, 1024);
    rtsp_s7_teardown_msg_encode(s7_tear_msg, sendbuf);

    ret = ConnectSock(&asm_sd, ASM_PORT, ASM_IP);
    if(ret == -1) {
        sm_log(LVLDEBUG,SYS_INFO,"Connect asm error");
        return -1;
    }
    ret = rtsp_write(asm_sd,sendbuf,strlen(sendbuf)+1);
    sm_log(LVLDEBUG,SYS_INFO,"teardown send2asm msg:%s, len:%d\n",sendbuf,ret);
    memset(recvbuf,0x00,1024);
    ret = rtsp_read(asm_sd,recvbuf,1024);
    sm_log(LVLDEBUG,SYS_INFO,"teardown recv from asm res:%s, len:%d\n",recvbuf,ret);
    rtsp_s7_teardown_res_parse(recvbuf, &s7_tear_res);

//向STB发送teardown response
    s1_tear_res.err_code = 200;//自己设置，注意出错处理
    s1_tear_res.cseq = s1_tear_msg.cseq;
    s1_tear_res.session = oc2sm_session;
    strcpy(s1_tear_res.ondemand_session_id,s1_tear_msg.ondemand_session_id);
    strcpy(s1_tear_res.client_session_id,s1_tear_msg.client_session_id);
	memset(sendbuf,0x00,1024);
    rtsp_s1_teardown_res_encode(s1_tear_res,sendbuf);
    ret = rtsp_write(OC_sd,sendbuf,strlen(sendbuf)+1);//向STB发送teardown response消息
    sm_log(LVLDEBUG,SYS_INFO,"teardown send2oc res:%s, len:%d\n",sendbuf,ret);
    //修改数据库状态
	//snprintf(sql_cmd,256,"UPDATE SM_S1 SET STATUS='TEARDOWN' WHERE ONDEMAND_SESSION_ID='%s'",s1_tear_msg.ondemand_session_id);    
	//cmd = sql_cmd;
	//stmt->executeUpdate(cmd.c_str());
    //stmt->executeUpdate("commit"); 

//关闭数据库连接
	//stmt->closeResultSet(rs);
	//conn->terminateStatement(stmt);
    //p_args->connPool->releaseConnection(conn);
    return 0;
}
