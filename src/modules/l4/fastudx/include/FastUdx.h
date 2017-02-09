
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the FASTUDX_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// FASTUDX_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.

#ifndef FASTUDX_H
#define FASTUDX_H

#define UDXMAXVER	2
#define UDXSLAVER	001


// //// //// //// //// //// //// //// //
// 1.84 ��Ҫ���ں˸ĳ��˶��̣߳����պͷ�����IO,�߼��϶����ֿ��ˣ�����֧�ָ��ã���Ӧ�ٶ����
// 1.86	�����ں˵�һ�θ������Ż����޸�BUG,���ȶ��汾
// 1.87 �޸��˶�ʱ����bug,����ÿ�����������ˣ�UdxTrackData,����20���û�����������������Ϊsizeof(INT64)*20
// 1.88 �޸���64λ�����bug,���Ӿ�̬64λlib,.a�ļ�
// 1.90 ��չ����󴰿�Ϊ16k���ڸ��ӳ٣�GM���١����ӳ�������������������ACK��Ƭ���ܡ�ȱ�㣨��֧������1.90��ǰ�汾��
// 1.91 ��ӵ�����ƽ������Ż�������ack����Ƶ�ʵ�������ֹ����Ҫ���ش�
// 1.92 �Ż��ڴ�ػ��գ�����СBUG���������绺����Ʋ���
// 1.93 ȥ�����ļ�����ӿڵ�Accept�������Ѿ���ʱ�������ˣ�����ʵ����cancel��������ǰ�汾û��ʵ��,�������������ӳ�ʱ�¼����ܴ�����BUG
// 1.94 ����IMulcardUdx֧�ֶ�����磨����,3G��)��һ��UDX���󣬽��е��������շ�����Ҫ�����ƶ��豸������ϵͳ��
// 1.95 �޸�1.93��������ӳ�ʱ���ܴ����¼���bug. ȥ�������ظ����õı���
// 1.96 �޸����ڴ�COPY��Ч�����⣬������һ��COPY���������5% CPUִ��Ч�ʣ����̵߳��ȣ����������ͷŹ��̣��������ȶ���
// 1.97 ��ǿ���ļ�������У�����ͬ���ļ�ʱ���������ط��ͳɹ��������޸�������̽�����
// 1.98 IFastUdx�ӿ��������Ƿ��Ƭѡ�����ѻص��¼�������������ȫ��Ӱ�죬���ݰ����������
// 1.991 �����˶��̲߳��м�����ԣ��޸�����SetFloatSize����16MBʱ��ACK����1500�ֽڵ�BUG������IMediaPush�ӿڷ�������Ƶ����ֹ����,���ٶȼ����㷨
// 1.992 �޸��Ǻϲ������BUG,�������ļ�У�����
// 1.993 �޸�linux������fork�ӽ���ʱ��ʱ�ӱ���ǰ�ͷ�����
// 1.994 �޸�OnStreamNeedMoreData(this)->OnStreamNeedMoreData(this,needdata),����IMediaPush�ص�OnMediaPushFrameEvent�������ͻ�����ʱ����ʾ����Ƶ�ж����컺�壬��ͨ���������壬�����ӳ�
//		 �����¼�OnStreamFinalRelease�������ӱ��ͷ�ǰ���ص���������һЩ�����û��Զ�������
// 1.995 �Է��Ͷ��й�����һЩ�����Ż������¼����������е������޸�IMediaPush�İ�ͷ�ṹ��SID��short�ĳ�int,��Ϸֲ�ʽ����
//		 �������������ӹ��򣬶��ڴ���������ƣ�����log��ֹ���ܣ��������������
// 1.996 ����1.995�У�ΪCBuffmaplist���Ż����Ͷ�����������ͻ�����bug��
// 1.997 ����ACK�з�Ƭ����
// 1.998 �޸�IMulcardUdx��ʵ�֣�ȫ��֧�ֶ࿨
// 1.999 �Ż��˰�ͷ��ȥ����ԴID,����תID��������4�ֽڣ���ԭ���İ�ͷ14�ֽڣ���Ϊ10�ֽ�,�����Ż��˶�̬����Ϊocx��activex�˳����⡣
//       �Ż���ack�����Ĵ�С��200KB����������ACK��������2K, 10MB������ACK����5KB
// 2.0   �Ż���ͳ�����ڣ�Ԥ���������������Զ��������ͻ���
// // 

// MS VC++ 10.0 _MSC_VER = 1600
// MS VC++ 9.0 _MSC_VER = 1500
// MS VC++ 8.0 _MSC_VER = 1400
// MS VC++ 7.1 _MSC_VER = 1310
// MS VC++ 7.0 _MSC_VER = 1300
// MS VC++ 6.0 _MSC_VER = 1200
// MS VC++ 5.0 _MSC_VER = 1100


#ifdef WIN32
	#if _MSC_VER >= 1600 // for vc8, or vc9 vs 2008 ~ vs2013

#include <WinSock2.h>
#include <windows.h>

#include <MMSystem.h>
#include <assert.h>

#ifndef _cplusplus
#include <atlbase.h>
#endif

#include <fcntl.h>

#include <process.h>
#include <io.h>

typedef unsigned int UDP_LONG ;
typedef unsigned short UDP_SHORT ;
typedef BYTE UDP_BYTE ; 

#pragma comment(lib,"ws2_32.lib")
#include <map>
#include <vector>
#include <string>
#include <list>
using namespace std;


	#else
		#include "udxos.h"
	#endif
#else
	#include "udxos.h"
#endif


enum ERROCODE
{
	//errocode : 0,�ɹ���1���µ����ӵ�����2��Զ�ܾ̾�����,3��ʱ
	UDX_CON_SUCCEED,
		UDX_CON_NEWCON,
		UDX_CON_EJECT,
		UDX_CON_TIMEOUT,
		UDX_CON_SELF
};

#ifdef WIN32
#pragma pack( push, 1 )
#define UDXPACKED 
#else
#define UDXPACKED	__attribute__((packed, aligned(1)))
#endif

typedef void (CALLBACK UDXPRC)(int eventtype ,int erro,long s, BYTE* pData, int len);
typedef void (CALLBACK UDXP2PPRC)(void* addrRemote,int errocode,char* user1,char* user2,INT64 dwuser);

typedef UDXPRC FAR *LPUDXPRC;
typedef UDXP2PPRC FAR *LPUDXP2PPRC;


	//�Ե�ַ��һЩת����һ�㲻��ʹ��
class IUdxTools
{
public:
	virtual INT64 Addr2Int64(SOCKADDR* pAddr,UDP_LONG streamId) = 0;//����ַ��һ��WORDת��Ϊһ��64λ����
	virtual void TraceAddr(SOCKADDR* pAddr) = 0;//��ӡ
	virtual void GetSpeedStr(char * buff,INT64 speed  ) = 0;//�õ����ͻ���յ��ٶ��ַ���
	virtual void Trace(const char* strLog) = 0;
	virtual int GetMD5( BYTE* pData,int len,char * pStrMD5Out ) = 0;
	virtual UDP_LONG GetHashLong(char* strKey) = 0;
	virtual void LimitDate(int y,int m,int d) = 0;
};

#define UDXCHANNELCOUNT	2	//ͨ������
#define MSGID	0			//��Ϣͨ��
#define DATAID	1			//����ͨ��

class IUdxInfo									//udx��ǰ��Ϣ���󲿷���ϢΪ��ͨ����Ϣ
{
public:
	INT64 m_dwRead;								//��ǰ���յ������ݳ���,�Ѿ�ȷ�ϵ�
	INT64 m_dwWrite;							//��ǰ�ӷ��͵����ݳ���,�Ѿ�ȷ�ϵ�
	INT64 m_ReadCount;							//��ǰ���յ��İ���,�Ѿ�ȷ�ϵ�
	INT64 m_WriteCount;							//��ǰ�ӷ��͵İ���,�Ѿ�ȷ�ϵ�
	INT64 m_SendTotalCount;						//�ܹ����͵��ܰ���,�����ش�����
	INT64 m_ReSendCount;						//�����ش��İ���

	INT64 m_dwDumpCount;						//�յ����ذ���
	INT64 m_dwOutRange;							//�յ���,���ڽ��մ����еİ���
	DWORD m_start;						//��ǰ��ʼʱ��
	DWORD s1,e1,s2,e2,m_sendindex;				//��ǰ�շ�����ʼ���,����ǰ�������еĴ�����������
	DWORD m_ackcount;							//�յ���ACK����
	INT64 m_currentspeedread;					//��ǰ�����ٶ�,�ֽ�/��,�Ѿ�ȷ�ϵ�
	INT64 m_currentspeedsend;					//��ǰ�����ٶ�,�ֽ�/��,�Ѿ�ȷ�ϵ�
	DWORD m_lastUpdateTime;						//�ϴθ���udxinfoʱ�� 
	
	int m_ttl;									//��ǰ������ʱ��
	int m_minttl;								//�����ֵ���СRTT
	int m_SecSendSize;							//ÿ��ͳ�����ڷ��͵ĳ���
	int m_SecReSendSize;						//ÿ��ͳ�������ط������ݳ���
	
	INT64 m_uncheckcount;						//�����˵�û��ȷ�ϵİ���
	INT64 m_checked;							//�Ѿ�ȷ�ϵİ���
	INT64 m_expect;								//�� �����͵İ���
	INT64 m_buffsize;							//��ǰ�����д��ڵİ���
	INT64 m_SendBewControl;						//ÿ��ͳ�����ڷ��͵��ٶ�
	INT64 m_WillBeSendSize;						//��ʹ��Ҫ���͵��ٶȣ�����

	int m_sendsyncount;							//���͵�ͬ��������
	int m_readsyncount;							//���յ���ͬ��������

	INT64 m_SendBuffCount[UDXCHANNELCOUNT];		//�ɹ�����sendbuff�Ĵ���
	INT64 m_WriteBuffCount[UDXCHANNELCOUNT];	//���ͳɹ��Ĵ���
	INT64 m_ReadBuffCount[UDXCHANNELCOUNT];		//���յ���sendbuff�����Ľ��հ�����
	
	virtual void GetSpeedStr(char * buff,BOOL bSend = TRUE,BOOL bCurrent = FALSE)=0;			//�õ�ʵʱ/ƽ���ٶ�,�ַ���
	virtual DWORD GetCurrentSpeed(BOOL bSend = TRUE)=0;											//�õ���ǰ�ٶ�
	virtual DWORD GetSpeed(BOOL bSend = TRUE)=0;												//�õ�ƽ���ٶ�
	virtual char* GetInfo()=0;																	//�õ��ַ���,��ǰ������Ϣ
	virtual void Reset()=0;																		//���¼�ʱ
	virtual void UpDateCurrentSpeed()=0;														//ˢ�¸�����Ϣ
	virtual float GetLostRate() = 0;															//��ǰ������
	
	//��ע:
	//�����ӽ����Ժ�,���ֲ��������Զ���UDX�ڲ�ȥ���ϸ���,����,Ӧ�ò�Ҳ���Ե���reset�������¼���,���ǵ���RESET������
	//��UDX�ڲ�����,ֻ����ΪӦ�ò�Ĳο�����.
}UDXPACKED;


struct IUdxCfg									//����UDX��һЩ����
{
	int mode;//����Fastudx.cfg.mode=1;��ʾ���������ɵ�UDXTCP����CPY ȫ�����ã�����Ĭ��������
	int maxdatabuffwnd[2];// 
	int submss;									//��ǰ���ӵ�MSS,ĿǰĬ��Ϊ1024�����Ϊ1400��һ������²���Ҫ�������ֵ��
	int maxlostrate;							//��󶪰��ʣ�default(350/1000==35% ),���뷶Χ(1~1000)
	int expectbew;								//Ԥ��������B/�룩��
												//��ʵ����������ʱ������ӵ���㷨������ӵ���㷨Ӱ���С,������ʵʱ��Ӧ���У�
												//��Ϊ���Թ����������������������Ϊ��ʹ���ڹ�С����ɶ�δ��䣬Ӱ�쵽�ӳ�
	int maxsendbew;					//���������������(B/S)
	int minsendbew;
	int flagfriend;					//�Ѻñ�־����fastmode��ʱ��1.��ʾ����������������������������ʱ�����������������ٶ�
	int mergeframe;					//������� 1.���ж��С��MSS�����ݰ���������������ʱ�����ܻᱻUDX�ϲ���һ��MSS���ͣ��Խ�Լ��ͷ����߷���Ч��. 0.���ϲ�
	int brokenframe;				//�ְ����� 1.������ʱ�����ܣ����MSS�ϲ�ʱ�����һ���������ܻᱻ�ָ��������������磺��800 + 600 > mss(1024)),ͬ��������(800,600)�ᱻ�ֳ�(1024,1400<800+600> - 1024<mss> = 376) 0. ��֮
	int fastmode;					//ֻ׷������ٶȣ�����ᶪ����0.Ĭ�����ã���Ԥ�������ʿ��ơ�1.���Զ����ʿ��ƣ�ֻ׷�������Ч���ݡ�
	int fixbew;						//�̶�����
	int filetransmode;				//�ļ�����ģʽ
	int delaytosend;				//�ӳٷ��ͣ������͵�����<MSSʱ�����ӳ�50MS���ͣ��ϲ����С��һ���ͣ�Ĭ�ϻ�û�д�
	int segmentspace;				//ʧ������Ĭ����3����1~5)���������޸�
	int maxackcheckcount;			//Ĭ��UDX50MSӦ��һ��������������󣬿��Թ涨���ٰ����������л�ӦACK
	int maxlocalackchecktime;			//Ĭ��UDX50MSӦ��һ��������������󣬿��Թ涨����MS�������л�ӦACK
	int maxremoteackchecktime;			//Ĭ��UDX50MSӦ��һ��������������󣬿��Թ涨����MS�������л�ӦACK
	int enablemsgchannel;			//����Ϣͨ��
	int enablewriteevent;			//��д֪ͨ
	int enabletimer;				//�򿪶�ʱ��֪ͨ
}UDXPACKED;

class IUdxLogSink
{
public: 
	virtual void Log(char* str) = 0;//�ڲ�UDXд��־����Ϣ��ͨ������ӿڵ�����Ӧ�ò���Դ����������дLOG
};

struct IUdxGlobalCfg
{
	IUdxGlobalCfg(){memset(this,0,sizeof(IUdxGlobalCfg));};
	int bInit;
	int mastver;
	int slaverver;
	int udxclock;				//udx�ڲ�ʱ��,Ĭ��25ms
	int udxcheckacktimer;		//udx�ڲ�ack�ذ�Ƶ��,Ĭ��35����
	int udxmintimeout;			//udx�ڲ���С��ʱ300ms
	int udxmaxtimeout;			//udx�ڲ����ʱ10000ms
	int udxackcount;			//���ٸ�����Ӧһ��ACK
	int udxdebug;
	DWORD maxsendbew;			//ȫ���ϴ��������������İ�
	char ext[10];
	IUdxLogSink* pLog;
	int bDisableLog;
}UDXPACKED;

struct UdxConnectInfo
{
	UDP_SHORT linktype;
	UDP_SHORT linkindex;
}UDXPACKED;
class IUdxUnkownPackSink		//���ڷ�UDX��UDP��������������ص��󣬻�ص�����Щ������Ӧ�ô���
{
public:
	virtual void OnUnkownData(SOCKADDR * pAddr,BYTE* pData,long len){};
	virtual void OnThreadExit(){};
};

class IWaitEvent				//UDXͨ���Ƿ�ɷ������¼�֪ͨ
{
public:
	virtual int Wait(DWORD ms) = 0;
	virtual void SetEvent() = 0;
};


struct FileInfo
{
	INT64 len;
	char name[256];
	UDP_BYTE context1[10];
	UDP_BYTE context2[10];
	UDP_BYTE context3[10];
}UDXPACKED;


class IUdxFSink					//�ļ��ص��ӿ�
{
public:
	//��Զ�����µ��ļ����������¼�
	virtual int OnNewFile(FileInfo * pInfo,BOOL bContinued){return bContinued + 1;};//1 save new file ,2 continue,else cancel
	//�����շ�ͬ�⣬���俪ʼ����Դ��䷽
	virtual void OnTransFileBegin(char* filename,BOOL bSend){} ;
	//Զ��ȡ��Ϣ����
	virtual int OnRemoteCancel(BOOL bSend){return 0;} ;
	//�ļ��������
	virtual void FileTransmitDone(char* filename,BOOL bSend){};//bSend = falseʱΪ���գ����յ����¼�ʱ����ʾ�ļ�������ɣ���������ɾ����UDX����
	//���������ʱ���ᷢ��һ��֪ͨ�����ͷ����������ͷ���FileTransmitDone��Ϣ������Ĺص�����UDX,�ᵼ�·��ͷ��ղ�����������¼���
	//���ԣ����ļ�����ʱ��Ӧ�õ����ͷ��յ�FileTransmitDone�¼�ʱ�ٹص�UDX�����Ա�֤���߶������յ����¼�������֤�ļ���������
	
	//�ļ����ݷ���������
	virtual void OnFileContinue(INT64 startpoint,BOOL bSend){};
	//�ļ����ݵ�������
	virtual void OnFileReadByts(DWORD dwSize){};
	//�ļ����ݷ��Ͷ���
	virtual void OnFileWriteByts(DWORD dwSize){};
	
	//�ṩ���ܽӿ�
	virtual void OnEncoderByts(BYTE*pData,DWORD dwSize){};
	
	//�ṩ���ܽӿ�
	virtual void OnDecoderByts(BYTE*pData,DWORD dwSize){};

	//���շ���һ��ͬ���ļ�������1����ʾǿ�����´� ��������ʾ��������, ����������̾ͻ�ȡ��ǰ10����10���м�10���ֽڣ�����������ȶԣ�������ͬ����Ϊ��ͬһ���ļ����������سɹ�
	virtual int OnCheckSameNameFile(FileInfo * pInfo){return 0;} ;

	//������Ϣ
	virtual void OnTransFileErro(int errocode){};

	virtual void OnCancelAck(BOOL bSend){};
};

class IUdxFileTransmitor 
{
public:
	//�õ���ǰ�շ��˰ٷֱ�
	virtual float GetPercent(BOOL bSend) = 0;
	//�����ļ�
	virtual void SendFile(char* filename) = 0;
	//�����Ƿ����,�����շ�������ʱ����������ɾ��UDX���󣬱�֤�ڷ��ͷ�����ɾ��UDX����
	virtual BOOL IsDone(BOOL bSend) = 0;
	//���ñ����ļ�·��Ŀ¼
	virtual void SetSaveFileDir(char* savedir) = 0;
	//���ñ����ļ���������·��
	virtual void SetSaveFileName(char* savename) = 0;
	//�����ļ��¼��ص��ӿ�
	virtual void SetFSink(IUdxFSink * pSink) = 0;
	//ȡ�����ͻ����ȡ�����ͻ����
	virtual void Cancel(BOOL bSend) = 0;
	//�õ��򿪵��ļ����ܳ���
	virtual INT64 GetTotalSendfileLen() = 0;
	virtual INT64 GetTotalReadfileLen() = 0;	
	//���öϵ������ȣ����lBpС��1024�ֽڣ�����൱��lBp = 1024.Ĭ����2M��ÿ2M����дһ���ļ�
	virtual void SetBPLength(long lBp) = 0;
	//�õ�BP����
	virtual long GetBPLength() = 0;

	virtual char * GetSendFileName() = 0;
	virtual char * GetReadFileName() = 0;
	//�����ļ�
	virtual void SendFileW(wchar_t* filename) = 0;
};

class IUdxP2pSink//p2p�¼��ص��ӿ�
{
public:
	virtual void OnP2pConnect(SOCKADDR addrRemote,int errocode,char* user1,char* user2,INT64 dwuser){};//���ӻص�
};
class IUdxP2pClient//P2P����ӿ�
{
public:
	virtual void SetSink(IUdxP2pSink * pSink) = 0;
	virtual void SetNatServer(char*mip,long mport) = 0;
	virtual void SetTimeOut(DWORD ms = 30000) = 0;
	virtual BOOL ConnectServer(char* user1,char* user2,INT64 dwUser) = 0;
	virtual SOCKADDR GetLocalAddr() = 0;
	virtual void Start() = 0;
	virtual void Stop() = 0;
	virtual void Clear() = 0;//�������֮ǰ���ӵ��û���Ҳ���Բ��ܣ��ڲ��ᶨʱ����
	virtual void SetCB(LPUDXP2PPRC pcb) = 0;//����P2P �ص�ָ��
};
#define UdxTrackData_Len	30

struct UdxTrackData
{
	INT64 data[UdxTrackData_Len];
};

#ifndef _streammedialib_h

#define AUDIOFRAME_A	0
#define VIDEOFRAME_I	1
#define VIDEOFRAME_P	2
#define DATAFRAME_I		3

struct UdxFrameType
{
	unsigned char type0:1;
	unsigned char type1:2;
	unsigned char type2:5;
	unsigned int  sid;
}UDXPACKED;

#endif

#ifdef WIN32
#pragma pack( pop)
#endif

class IMediaPush
{
public:
	//��UDX������������Ƶ��,�ڲ���call SendFrames,Ҳ�����ⲿ����SendFrames
	//�����շ��յ�����ʱ�����ݵ�һ�ֽ�Ϊһ��BYTE��ʾ�������type,�����ڽ����ں��棬Ҳ����OnStreamRead(pData + 1<ʵ������ָ��>,len - 1 <ʵ�ʳ��ڶ�> );
	virtual void SendFrame(UDP_LONG sid,BYTE* pData,int len,int type1/* AUDIOFRAME_A or VIDEOFRAME_I or VIDEOFRAME_P */,int type2) = 0;

	//������󻺳����Ƶ������������ӵ��ʱ���ܻ�������������û����Ը��ݣ��ɼ������� * maxcount = ��������ӳ�ʱ��
	virtual void SetAudioFrameMaxCount(int maxcount) = 0;

	virtual void SendFrames() = 0;
	//������������Ƶ���ݣ��ָ�����ʹ״̬����ʱ����ǿ�Ʊ���������I�죬���л�������Ƶ
	virtual void Reset() = 0;
};

class IFastUdx;
class IUdxTcp : public IUdxFileTransmitor //==============    UDX �������ӽӿ�       ====================
{
public:
	virtual long AddLife() = 0;							//�������������ü�����������com��������������ڵڶ����߳��в���������ӣ������������
	virtual void Destroy() = 0;							//�Ͽ��������
	virtual BOOL IsConnected() = 0;						//�����Ƿ�����
	virtual BOOL IsFullBuffs(int ich) = 0;				//�����Ƿ��ܷ�������
	virtual void SetBuffWindow(int ich,DWORD size) = 0;	//���û����С�����ֽ�Ϊ��λ,���峤�����û�����SetFloatDataSize������
	virtual void SetMaxDataWindowSize(int ich,DWORD size) = 0;//���ô��ڴ�С���԰�����Ϊ��λ������Ϊ��8��16��32��64��...��8092~ ���8092*2����
	virtual BOOL SendBuff(BYTE* pData,int len) = 0;		//�����棬��ʾ��ָ�������Ѿ��ɹ�������UDX�����У����峤�����û�����SetFloatDataSize������
	virtual BOOL SendMsg(BYTE* pData,int len) = 0;		//������Ϣ
	virtual IUdxInfo* GetUdxInfo() = 0;					//�õ��ڲ�һЩ��Ϣ
	virtual IUdxCfg * GetUdxCfg() = 0;					//�������ӵģ����������
	virtual UDP_SHORT GetStreamID() = 0;				//���ص���ID
	virtual UDP_SHORT GetDesStreamID() = 0;				//���ӳɹ���Զ�˵ı�����ID
	virtual INT64 GetUserData() = 0;					//�����ӹ������û����ݣ�������ָ�룬�Զ�������
	virtual void SetUserData(INT64 dwUser) = 0; 
	virtual IWaitEvent* GetWaitEvent() = 0;				//���ӣ��������ݿɷ�ʱ���¼�����ָ��
	virtual SOCKADDR *GetRemoteAddr() = 0;				//���ӳɹ���Զ�˵�IP��Ϣ
	virtual int __DSendUdxBuff(SOCKADDR * pAddr ,BYTE* pData,int len) = 0;		//�����������ͷǿɿ�����
	virtual int __DSendUdxBuff2(SOCKADDR * pAddr ,BYTE* pData,int len) = 0;		//�����������ͷǿɿ����ݣ����ǻᴥ��
	virtual DWORD GetAppBuffAndUdxWndBuffSize() = 0;	//�õ���ǰ������udx�ڲ������ݳ��ȣ���Щ���ݻ�û�еõ�ȷ��
	virtual DWORD GetBuffWindow(int ich) = 0;			//�õ�Ӧ�ò����õĻ��峤�ȣ�����������ڲ������к��еĳ���
	virtual void SetTimeOut(int con,int hardbeat,int contimeout) = 0;		//���ó�ʱ���ֱ�Ϊ�����ӳ�ʱ����������������ӱ���ʱ�䣬��Ϊ��λ
	virtual UdxTrackData* GetUdxTrackData() = 0;		//�õ�����������ӹ�����ڴ����
	virtual void SetFloatDataSize(int floatdatasize) = 0;		//����Ӧ�û��峤��,���û������ʱΪ0��������Ĭ�ϻ��峤��8M��ֻ�������ͨ����Ч����Ϣͨ���������Ӱ��
	virtual int GetFloatDataSize() = 0;					//�õ�֮ǰ���õĻ����С��
	virtual void Close() = 0;							//�������������������£���Ͽ����ӣ�����ʲôҲ������
	virtual long ReleaseLife() = 0;						//������addlife����ļ���
	virtual void DetectReadedBuffSize(long &readsize,long &writesize) = 0;	//�û���⣬��û�д���OnStreamwrite/OnStreamRead�¼��е�δ��������ݳ���
	virtual IMediaPush* GetMediaPush() = 0;				//�õ�push�ӿ�,�ô˽ӿ�����������Ƶ�죬���Է�ֹ����
	virtual IFastUdx * GetFastUdx() = 0;				//����IFastUdx����
	virtual BOOL IsTransLink() = 0;//�����Ƿ�Ϊ��ת����
	virtual SOCKADDR *GetTransServerAddr() = 0;//���ص�ǰ�ߵ���תIP
	virtual UDP_SHORT GetLinkType() = 0;//����ʱ��ͨ��UdxConnectInfoָ������������1
	virtual UDP_SHORT GetLinkIndex() = 0;//����ʱ��ͨ��UdxConnectInfoָ������������2
	virtual void EnableAutoAjustBuffs(BOOL bEnable) = 0;//��UDX�Զ��������ͻ��峤��
	virtual int GetLinkDirect() = 0;//1����ʾ���ӷ��𷽣�0��ʾ���ӽ��ܷ�

};

//���ɣ�
// Q1 ���߳��У�����Ҵ�����TCP���󣬵�����AddLife
// �ر�ʱ �ҿ���
// 1.1 �ȵ���ReleaseLifeȻ�����Close��
// 1.2 �ȵ���Close Ȼ�����ReleaseLife
// 1.3 ֱ�ӵ���Destory��
// 
// Q2 ���߳������ �Ҵ�����TCP����û�е���AddLife
// �ر�ʱ ����destroy,�����ϸ����ؼ�������Ϊû�еڶ����̷߳���ͬһ������
// 
// Q3 ���߳��У�����븴��TCP����ô�м����̣߳���Ҫ����
// ����AddLife��������Ҫ����ͬ���߳��˳�ǰ���Ե���Destory ��
// ����ReleaseLife�����Ǳ�����һ���߳�����Ҫ���ùرռ���
//    Destory ���� Close + ReleaseLife


class IMultCardTcp;
class IUdxTcpSink
{
public:
	virtual void OnStreamPreConnect(SOCKADDR *pAddr,IUdxTcp * pTcp,int erro){};	//�ڴ˽ӿ��У������ø������
	virtual void OnStreamConnect(IUdxTcp * pTcp,int erro){};
	virtual void OnFileStreamConnect(IUdxTcp * pTcp,int erro){};				//0,��ʾ�µ�һ���ļ�����Ự��1���Ự����
	virtual void OnStreamBroken(IUdxTcp * pTcp){};								//�Ͽ�
	virtual void OnStreamRead(IUdxTcp * pTcp,BYTE* pData,int len){};			//��
	virtual void OnStreamWrite(IUdxTcp * pTcp,BYTE* pData,int len){};			//д
	virtual void OnStreamMsgRead(IUdxTcp * pTcp,BYTE* pData,int len){};			//��Ϣ�Ķ�
	virtual void OnStreamMsgWrite(IUdxTcp * pTcp,BYTE* pData,int len){};		//��Ϣ��д
	virtual void OnStreamNeedMoreData(IUdxTcp *pTcp,int needdata){};			//���Է����µ�д,��needdata(<=0��ʾ���Է�����)�ֽڿռ乩���ͣ����һ���������ݱ�ȷ�Ϻ󴥷�
																				//����ص��е����������֪ͨ�����ֻ���а���ȷ�ϵ�֪ͨ�����ԣ�Ӧ�ò�������������ϵĽ���Ͷ��
																				//������Ҫ����������߳�����ɷ��͡���һ��sendbuff�����ͨ������¼����ϵ��ӷ��͡�
	virtual void OnUnSafeStreamRead(IUdxTcp * pTcp,BYTE*pData,int len){};		//�������ӵĲ��ɿ����ݣ�һ������������Ƶ

	virtual void OnInteranlThreadExit(){};										//�ڲ��߳��ͷ�ʱ��������һ�����java,com,jni��ʹ�ã������ͷ���Դ
	virtual void OnStreamLinkIdle(IUdxTcp *pTcp){};								//����·���֣�����Ӧ�ò�û�����ݿɷ�ʱ��
																				//���յ����������������Ĵ����ڣ��û����Դ�ӡ������Ϣ��֤������������һ�㲻����
	virtual void OnStreamChancetoFillBuff(IUdxTcp *pTcp){};						//UDX�ڲ������߳��ṩ���ᣬ���з�������,ÿ50MS�̶�Ƶ�ʵ���
	virtual void OnMediaPushFrameEvent(int sid,int type,int framecount){};		//sid,������ʱ������SendFrameʧ��ʱ���ص�������ʵ�ʻ����˶���������,0������1��Ƶ��
	virtual void OnStreamFinalRelease(IUdxTcp * pTcp){};						//ÿ����������ͷ��¼������Դ���һЩ�����Ķ����ͷ�,��ͬ��OnStreamBrokenָ�յ��Ͽ���Ϣ�ͻ���ã�broken�¼����ǿ���(�������������releaselife)��ʹ�����OnStreamFinalRelease
																				//�����ڲ����ܻ��п������������˶�Ӧ�����ӵ���Ϣ�������������ͷŶ���������ͬ��������
																				//����ص���ÿһ�����ӵ�����ͷ���ڣ�������Ӧ�ò���������û�������ñ���(addlife)
	virtual void OnMultCardConnect(IMultCardTcp * pTcp,int erro,int linkcount){};
	virtual void OnMultCardBroken(IMultCardTcp * pTcp){};
	virtual void OnMultCardRead(IMultCardTcp * pTcp,BYTE*pData,int len){};


};

class IUdxTrans
{
public:
	virtual void OpenChannel(char*ip,UDP_SHORT port,char* strCName) = 0;		//��һ����תͨ��,ͨ�������õ��ڻ򳬹�25���ַ�
	virtual void CloseChannel(char*ip,UDP_SHORT port,char* strCName) = 0;		//�ص�һ����תͨ��
	virtual void CloseAllChannels() = 0;										//�ص����е���תͨ��
};

struct IFastPreCfg : public IUdxCfg//������һ��IUdxTcp�ӿ�ʱ��Ԥ�����õ�һЩ�������� 
{
};

class IFastUdx//UDXʵ������ ==============    UDX ���Ķ���       ====================
{
public:
	virtual BOOL Create(char* ip = NULL,UDP_SHORT port = 0) = 0;
	virtual BOOL AddBinding(char* ip,UDP_SHORT port) = 0;
	virtual IUdxTcp* Connect(char* ip,UDP_SHORT port,BOOL bSync = FALSE,INT64 dwUser = 0,INT64 expectbew = 0,int lostrat = 50,
		char*strCName = NULL,IMultCardTcp* pMultCard = NULL,UdxConnectInfo*pInfo = NULL) = 0;//ͨ�������õ��ڻ򳬹�25���ַ�

	virtual	BOOL Attach(SOCKET s) = 0;											//���Ѿ����ڵ�UDP�׽���
	virtual int Dettach() = 0;
	virtual void Destroy() = 0;													//����UDX����
	virtual void SetUnkownPackSink(IUdxUnkownPackSink *pUnkownPackSink) = 0;	//���÷�UDX��UDP��ʱ�ص���
	virtual void SetSink(IUdxTcpSink * pSink) = 0;								//���û����¼��Ļص���
	virtual void SetServerBlockSize(int size) = 0;								//�����ڲ��������̴����߳��������磬�߼����������¼��ص��߳�����IO�߳�����
	virtual SOCKADDR *GetLocalAddr() = 0;										//�õ�Ĭ��UDX�˿�ռ����Ϣ
	virtual IUdxP2pClient* GetP2pClient() = 0;									//�õ�P2P�ӿ�
	virtual int __DSendUdxBuff(SOCKADDR * pAddr ,BYTE* pData,int len) = 0;		//ֱ����ָ���ĵ�ַ���ͷ�UDX ԭʼ���ݰ�
	virtual IUdxTrans* GetUdxTrans() = 0;										//�õ���ת����ӿ�
	virtual void SetUdpSendThreadCount(int count) = 0;							//�����ڲ�����UDP���߳���
	virtual void SetConnectTimeOut(int secs) = 0;								//���������ӳ�ʱ
	virtual void Enable_DONTFRAGMENT(BOOL bEanble) = 0;							//���õײ�UDP�Ƿ������Ƭ��Ĭ�ϲ������Ƭ
	virtual int GetClientsCount() = 0;											//�����ܵ������������������������ʵʱ��
	virtual IUdxCfg* GetPreCreateUdxTcpCfg() = 0;								//����һ��Ԥ�����õ�����
	virtual void SetCB(LPUDXPRC pcb) = 0;
	virtual IMultCardTcp* CreateMultCard() = 0;
	virtual void SetUserData(INT64 dwUser) = 0;
	virtual INT64 GetUserData() = 0;
	virtual void LowCost() = 0;
};

class IMultCardTcp																//������� �󶨺�ͬʱΪһ�����������ݣ����������
{
public:
	virtual BOOL AddBinding(int index,char* ip,UDP_SHORT port) = 0;				//���Ӱ�IP
	virtual BOOL Connect(char*ip,UDP_SHORT port) = 0;							//ͨ��ָ����IP���ӶԷ�IP + port
	virtual void Close() = 0;													//�Ͽ�����
	virtual void Destroy() = 0;
	virtual BOOL SendBuff(BYTE* pData,int len) = 0;
	virtual void AddLife() = 0;
	virtual BOOL IsConnected() = 0;
	virtual UDP_SHORT GetMultCardID() = 0;										//ȡ�ñ����ӵ�ID��
	virtual UDP_SHORT GetMultCardDesID() = 0;									//ȡ�ñ����ӵĶԶ�ID��
	virtual void SetFloatSize(int floatsize ) = 0;								//���û�����
	virtual BOOL GetRemoteIPList(char* pList,int &bufflen) = 0;					//��IP:PORT�ո� ip-port,����false��ʾ���岻����bufflen������Ҫ�ĳ���,�ո�ָ�
	virtual BOOL GetLocalIPList(char* pList,int &bufflen) = 0;					// ip-port,����false��ʾ���岻����bufflen������Ҫ�ĳ���,�ո�ָ�
	virtual IWaitEvent* GetWaitEvent() = 0;
};


class IUdxLock//Ϊ���ڱ�д��ƽ̨����ʱ���ṩ����
{
public:
	virtual void Lock() = 0;
	virtual void Unlock() = 0;
	virtual void Destroy() = 0;
};

class IUdxEvent //�¼�����
{
public:
	virtual void SetEvent() = 0;
	virtual void ResetEvent() = 0;
	virtual BOOL Wait(int ms) = 0;
	virtual void Destroy() = 0;
};

class IUdxBuff
{
public:
	virtual BYTE* ChangePoint(int iLen) = 0;									//��̬��չ�����С
	virtual BYTE* GetBuff(void) = 0;											//ȡ�û���ͷָ��
	virtual int GetLen(void) = 0;												//���ػ���ʵ��ʹ�ó���
	virtual int GetMaxLen(void) = 0;											//������󻺳峤��
	virtual void Zero() = 0;													//��0
	virtual void FreeMem() = 0;													//�ͷ��ڴ�
	virtual void Reset() = 0;													//��0������ʹ�ó�����0
	virtual void Bind(BYTE* pData,int len) = 0;									//��ָ���ڴ�copy����������
	virtual BOOL Pop(BYTE* pData,int len) = 0;									//��ָ��ͷ������ָ������len�����ݣ�����cpy��pData�У�ԭ���峤�Ȼ����len
	virtual void AppendBuff(BYTE* pData,int iLen) = 0;							//׷��һ���ֻ���
	virtual void Destroy() = 0;													//�ͷ��Լ�
	virtual long AddLife() = 0;													//�������ü���
};

class IUdxFifoList //�Ƚ��ȳ�����
{
public:
	virtual void AddBuff(IUdxBuff * pBuff) = 0;									//����һ��buff����
	virtual IUdxBuff* GetBuff() = 0;											//ȡ��һ��������ibuff����
	virtual void AddBuff(BYTE * pData,int len) = 0;								//ѹ��ָ���ĳ�������
	virtual void GetBuff(BYTE** ppData,int &len) = 0;							//ȡ��ָ����������
	virtual void FreeBuff(BYTE* pData) = 0;										//�ͷ���getbuff(BYTE**)ָ����ָ�ڴ�
	virtual int GetBuff(BYTE* pData,int len) = 0;								//����ָ�ȳ��ȵĻ������pData,ԭ�����������len
	virtual int GetBuffSize() = 0;												//����list�е������ܳ���
	virtual int GetBuffCount() = 0;												//����list��IBuff������
	virtual void EnableEvent(BOOL bEnable) = 0;									//�Ƿ����¼�����
	virtual BOOL Wait(int ms) = 0;												//�����¼������󣬵��������ݼ���ʱ���ᴥ���¼�
	virtual void Clear() = 0;													//�����������
	virtual void Destroy() = 0;													//�ͷ��Լ�
	virtual void SetWaitEvent(IUdxEvent* pEvent) = 0;							//�ؽӽ����¼�
};

class IUdxTime //ʱ��tick
{
public:
	virtual DWORD GetTickCount() = 0;
	virtual void Destroy() = 0;
};

class IUdxThreadSink //�̻߳ص�
{
public:
	virtual void UdxRun(){};
};
class IUdxThread  //�߳̽ӿ�
{
public:
	virtual void Start() = 0;
	virtual void Stop() = 0;
	virtual void SetCB(IUdxThreadSink* pSink) = 0;								//�����̻߳ص��࣬�������ʵ��UdxRun()
	virtual void Destroy() = 0;
};

class IUdxFile  //�ļ������ӿ�
{
public:
	virtual BOOL OpenFile(char* strFile,BOOL app = FALSE) = 0;					//���Ѿ����ڵ��ļ�
	virtual BOOL OpenFileW(wchar_t* strFile,BOOL app = FALSE) = 0;
	virtual BOOL CreateFile(char* strFile) = 0;									//�����ļ�
	virtual void Close(void) = 0;												//�ر�
	virtual BOOL IsOpen(void) = 0;												//�ļ��Ƿ��Ѿ�����
	virtual INT64 GetFileLength() = 0;											//�����ļ�����
	virtual INT64 Write(BYTE* pData, int len) = 0;								//д���ļ�
	virtual INT64 Read(BYTE* pData, int len) = 0;								//���ļ���������������ĳ���
	virtual void Seek(int type = 1) = 0;										//seek��ͷ0����β1
	virtual void SeekTo(int type,INT64 len) = 0;								//type 0��ͷ��ʼ��1β��ʼ len����Ϊ��ֵ���Ƿ�����
	virtual void Destroy() = 0;													//�ͷ��Լ������ص��ļ�
};

class IUdxRef
{
public:
	virtual long AddRef() = 0;
	virtual long Release() = 0;
};

IUdxGlobalCfg* GetUdxGlobalCfg();		//UDX��ȫ�ֱ����ӿڣ���������һЩȫ�ֲ���
IFastUdx* CreateFastUdx();				//����һ��UDX����
IUdxTools* GetUdxTools();				//UDX�ṩ�ĸ������ߣ�һ���ò���
IUdxLock* CreateUdxLock();				//����һ����
IUdxEvent* CreateUdxEvent();			//�����¼�
IUdxFifoList* CreateUdxList();			//����һ���Ƚ��ȳ�����
IUdxTime* CreateUdxTime();				//����tick����
IUdxThread* CreateUdxThread();			//�����̶߳���
IUdxFile* CreateUdxFile();				//����udx�ļ�����
IUdxBuff * CreateUdxBuff(int type);		//����һ��buff����
IUdxRef * CreateUdxRef();				//����һ�����ü�������


#endif
