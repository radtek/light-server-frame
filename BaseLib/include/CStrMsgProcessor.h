/****************************************************
 *  @file    $URL: file:///D:/SourceCode/MyPrj/SourceLib/TopViewXP/trunk/lib/BaseLib/include/CStrMsgProcessor.h $
 *
 *  @brief 服务器端字符消息处理类。用于处理服务器接收下来的字符串形式的交互消息
 *
 *  $Id: CStrMsgProcessor.h 17 2008-11-18 11:38:38Z gang.chen $
 *
 *  @author gang chen <eyesfour@gmail.com>
 ***************************************************/
#ifndef __CSTRMSGPROCESSOR_H__
#define __CSTRMSGPROCESSOR_H__

#include <iostream>
#include <map>
#include <queue>
#include <string>

#include "../include/common.h"
#include "../include/common_socket.h"
#include "../include/common_thread.h"

using namespace std;
/**
* @brief 服务器收到的文本消息结构体
*/
typedef struct _TStrMsg
{
	SOCKET socket;			/**<与客户端建立连接的套接字*/
	char clientip[16];		/**<客户端IP地址*/
	string msg;				/**<消息*/
}TStrMsg;

class CStrMsgProcessor
{
public:
	/*
	* @brief 字符消息处理类构造函数
	* @param nTimeOut 未收到消息后主动断开的超时时间
	*/
	CStrMsgProcessor(int nTimeOut, char *szTail = "$");

	/*
	* @brief 字符消息处理类析构函数
	*/
	~CStrMsgProcessor();

	/*
	* @brief 客户端连入事件
	* @param socket 客户端连接socket
	* @param szClientIP 客户端IP地址
	*/
	void OnAccept(SOCKET socket, const char *szClientIP);

	/*
	* @breif 收到客户端消息事件
	* @param socket 客户端连接socket
	* @param szClientIP 客户端IP地址
	* @param buf 消息缓冲区指针
	* @param len 消息长度
	*/
	void OnRead(SOCKET socket, const char *szClientIP, char *buf, int len);

	/*
	* @brief 客户端断开连接事件
	* @param socket 客户端连接socket
	* @param szClientIP 客户端IP地址
	*/
	void OnClose(SOCKET socket, const char *szClientIP);

	/*
	* @brief 当前消息缓冲队列中完整的消息条数
	* @return 消息条数
	*/
	int MsgCount(void);

	/*
	* @brief 从消息缓冲队列中提取消息
	* @return 返回的消息，void *类型，由调用者转换成具体的类型，如TStrMsg *
	*/
	void *GetMsg(void);

	/*
	* @brief 将最老的消息从消息缓冲队列中弹出，通常在GetMsg后调用
	*/
	void PopMsg(void);

	/*
	* @brief 检查超时无消息的客户端，此函数由CServer定时调用。
	*/
	void CheckTimeout(void);
private:
	int m_nTailSize;
	char m_szTail[32];
	queue<TStrMsg *> m_MsgQueue;			/**<消息缓冲队列*/
	map<SOCKET, string> m_RecvStrMap;		/**<用于存放接收到的不完整的消息，与socket对应*/
	map<SOCKET, int> m_TimeOutTick;			/**<与socket对应的超时计时器*/
	int m_nTimeOut;							/**<默认的超时时间*/
	ThreadMutex m_Mutex;				/**<线程互斥锁*/
};

#endif

