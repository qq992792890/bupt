#include <iostream>
#define btos(b,p) (short)b[p+1]|(short)b[p]<<8;        //˫�ֽ�תshort
#define btoi(b,p) (int)b[p+1]|(int)b[p]<<8;            //˫�ֽ�תint
#define MAX_STRING 256

using namespace std;
/*
typedef enum MessageType
{
	OPEN=1,
	UPDATE,
	NOTIFICATION,
	KEEPALIVE
}Type;
*/
typedef unsigned char byte;

typedef struct _Parameters                    //OPEN��Ϣ�����ṹ
{
	short ParameterType;                           //��������
	short SubParametersLen;                        //��������
	byte ParameterValue[256];                      //����ֵ(TypeΪ1ʱ��ֵΪ�ò�����Capability�����е�λ��)
}Parameters;

typedef struct _Capability                    //OPEN��Ϣ�еĲ���Capability Information
{
	short M_capabilityCode;                        //���ܴ���
	short M_capabilityLen;                         //�ù���ֵ�ĳ���
	byte M_capability[4];                            //����ֵ(CodeΪ1ʱ��ֵΪ��Ӧ·�����������Ԫ�ظ���)
}Capability;

typedef struct _RouteTypesSupported           //OPEN��Ϣ�в���Capability Information�е�RouteTypesSupported
{
	int AddressFamily;                           //��֧��·�����͵ĵ�ַ��
	int ApplicationProtocol;                     //Ӧ��Э��
}RouteTypesSupported;

typedef struct _OPEN                          //OPEN��Ϣ
{
	short Len;                                     //��Ϣ����(�����Լ�)
	byte type;                                     //��Ϣ���
	short version;                                 //Э��汾
	short holdtime;                                //ED�����HoldTimeֵ
	byte ID[4];                                         //VREP��ʶ(���N�ӿ�û��������Ϊ�豸��IPv4��ַ)
	short parametersLen;                                //��Ϣ�����������ܳ���
	Parameters parameters[256];                        //��������
	int ParameterNum;                                   //��������
	Capability capability[256];                        //����(������һ��)����
	RouteTypesSupported routetypessupported[256];      //֧�ֵ�·������(���ܵ�һ��)����
}OPEN;


typedef struct _PortID
{
        short slotnumber;
        short portnumber;
        short subinterface;
}PortID;
typedef struct _Attributes                    //UPDATE��Ϣ���Խṹ
{
	byte AttrFlag;                                 //���Ա�־(��λ����Ϊ0��ʾ��֪���ԣ�1��ʾδ֪���ԣ�����λ��Ч)
	byte AttrType;                                 //���Դ���
	short AttrLen;                                 //����ֵ�ĳ���
	byte* AttrValue;                               //����ֵ(ֵΪ��λ�õ��ַ�����������ʵ������)
}Attributes;
typedef struct _Routes                       //UPDATE��Ϣ·�ɽṹ
{
	int AddFamily;                               //·�ɵ�ַ��
	int AppProtocol;                             //Ӧ��Э��
	short AddLen;                                  //��ַ����
	byte Address[256];                                 //��ֵַ
}Routes;
typedef struct _Server                       //UPDATE��Ϣ�����ṹ(������һ����ַ��Ϣ)
{
	short AddLen;                                  //��ַ����
	byte Address[256];                                 //��ֵַhost[":"port]
	short ZoneNameLen;                             //Streaming Zone���ĳ���
	byte ZoneName[256];                                //Streaming Zone��
}Server;
typedef struct _Input                        //UPDATE��Ϣ��Ե�������Խṹ(��������������Ŀ�ĵ�ַ)
{
	byte SubnetMask[256];                              //��������
	byte Host[256];                                    //������ַ(��������RFC1123�涨��IPv4��ʽ��ַ��RFC2373�涨��IPv6��ʽ��ַ��
																		//��IPv6��ַҪ��"[]"������)
	PortID portID;                                    //����˿ں�
	int MaxGroupBW;                                //��Ե����ɳ��ص���������(Kbps)
	byte GroupName[256];                               //�������Ӧ��Edge Input Group��
}Input;
typedef struct _QAMParameters               //UPDATE��ϢQAM����
{
	int Frequency;                                 //����Ƶ��
	byte Modmode[256];                             //����ģʽ
	byte Interleaver;                              //��N6����������FEC interleaver�ĳ���
	short TSID;                                    //��QAM��PAT�����õ�TSID
	byte Annex;                                    //����QAM ITU-T annex����N6�����ĳ���
	short Channelwidth;                            //QAM��Channel Width
}QAMParameters;
typedef struct _StaticPorts                 //UPDATE��ϢUDP MAP���еľ�̬�˿�
{
	short UDPPort;                                 //UDP�˿�
	short ProgramID;                               //�˿ڶ�Ӧ�Ľ�Ŀ��
}StaticPorts;
typedef struct _DynamicPorts                //UPDATE��ϢUDP MAP���еĶ�̬�˿�
{
	short StartingPort;                            //��̬�˿ڷ�Χ����ʼ�˿�
	short StartingPID;                             //��ʼ�˿ڶ�Ӧ�Ľ�Ŀ��
	int Count;                                     //��̬�˿ڵķ�Χ(�˿ںͽ�Ŀ�ŵ�����һ)
}DynamicPorts;
typedef struct _NHSAlternates               //UPDATE��ϢNextHopServerAlternates����
{
	short NumAlternates;                           //alternate servers�ĸ���
	byte server[256][256];                         //һ��alternate servers
}NHSAlternates;
typedef struct _UPDATE                      //UPDATE��Ϣ
{
	short Len;                                     //��Ϣ����
	byte type;                                     //��Ϣ����
	Attributes attributes[256];                    //����ָ��(ָ�����в������ַ�������ʵ������)
	int AttributeNum;                              //��������
	Routes	WithdrawnRoutes[256];                  //���ɴ�·�ɴ�
	int WRoutesNum;                                //���ɴ�·�ɸ���
	Routes ReachableRoutes[256];                   //�ɴ�·�ɴ�
	int RRoutesNum;                                //�ɴ�·�ɸ���
	Server  NextHopServer;                         //��һ��������
	byte  QAMName[256][256];                       //QAM����
	int QAMNameNum;                                //QAM������
	int totalbw;                                   //�ܴ���
	int availablebw;                               //���ô���
	short cost;                                    //cost
	Input EdgeInput[256];                          //��Ե���봮
	int EdgeInputNum;
	QAMParameters QAMP;                            //QAM����
	StaticPorts SPorts[256];                       //UDP MAP�еľ�̬�˿ڴ�
	int SPortsNum;                                 //��̬�˿ڸ���
	DynamicPorts DPorts[256];                      //UDP MAP�еĶ�̬�˿ڴ�
	int DPortsNum;                                 //��̬�˿ڸ���
	int ServiceStatus;                             //����״̬
	int MaxMpegFlows;                              //֧�ֵ������������
	NHSAlternates Alternates;                      //alternate servers
	PortID OutputPort;                             //QAM������Ķ˿�
}UPDATE;
static int bytes2int(byte* b);                //�ֽ�����(4λ)ת��Ϊint��
bool ParseOPEN(byte* VREP,OPEN &M_OPEN);                      //OPEN��Ϣ��������
void InspectOPEN(OPEN &M_OPEN);                            //OPEN���
void HandlUnkownAttr(Attributes &attr);                     //δ֪��������
void ParseRoutes(byte* Attr,short Len,Routes* routes,int &Num);           //��������WithdrawnRoutes��ReachableRoutes
void ParseNHS(byte* Attr,short Len,Server &M_NextHopServer);        //��������NextHopServer
void ParseQAMN(byte* Attr,short Len,byte QAMNames[256][256],int &Num);       //��������QAMNames
void ParseEdgeInput(byte* Attr,short Len,Input* M_EdgeInput,int &Num);    //��������EdgeInput
void ParseQAMP(byte* Attr,short Len,QAMParameters &M_QAMP);         //��������QAMParameters
void ParseServiceStatus(byte* Attr,int &ServiceStatus);          //��������ServiceStatus
void ParseUDPMap(byte* Attr,short Len,StaticPorts* SPorts,DynamicPorts* DPorts,int &SNum,int &DNum);     //��������UDPMap
void ParseNHSA(byte* Attr,short Len,NHSAlternates &M_Alternates);        //��������NextHopServerAlternates
void ParsePortID(byte *Port,PortID &M_Port);                        //����PortID
bool ParseAttr(Attributes* &M_attr,int ANum,UPDATE &M_UPDATE);        //������������
bool ParseUPDATE(byte VREP[],UPDATE &M_UPDATE);                    //UPDATE��Ϣ��������
void InspectUPDATE(UPDATE &M_UPDATE);                        //UPDATE���
bool ParseNOTIFICATION(byte VREP[]);                          //NOTIFICATION��Ϣ��������
bool Parse(byte VREP[]);                       //�����������
bool PackageOpen(OPEN M_OPEN,char* OpenMessage);                //���OPEN��Ϣ
bool PackageKeepalive(char* KeepMessage);                       //���KEEPALIVE��Ϣ
bool PackageNotification(short ECode,short ESubCode,char* Date,char* NotificationMessage);        //���NOTIFICATION��Ϣ
void OPENOut(OPEN M_OPEN);                                    //���OPEN��Ϣ
void UPDATEOut(UPDATE M_UPDATE);                              //���UPDATE��Ϣ
