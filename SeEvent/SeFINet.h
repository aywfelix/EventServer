#pragma once

#include <memory>
#include <string>
#include "SePlatForm.h"
#include "Session.h"
#include "SeFNodeNet.pb.h"

// Message Head[ MsgID(2) | MsgSize(4) ]
#define MSG_HEAD_LEN 6

struct IMsgHead
{

	virtual void EnCode(char* strData) = 0;
	virtual void DeCode(const char* strData) = 0;
	virtual UINT16 GetMsgID() const = 0;
	virtual void SetMsgID(UINT16 nMsgID) = 0;
	virtual UINT32 GetBodyLength() const = 0;
	virtual void SetBodyLength(UINT32 nLength) = 0;

	INT64 Htonll(INT64 nData)
	{
#ifdef _WIN32
		return htonll(nData);
#else
		return htobe64(nData);
#endif
	}

	INT64 Ntohll(INT64 nData)
	{
#ifdef _WIN32
		return ntohll(nData);
#else
		return be64toh(nData);
#endif
	}

	INT32 Htonl(INT32 nData)
	{
#ifdef _WIN32
		return htonl(nData);
#else
		return htobe32(nData);
#endif
	}

	INT32 Ntohl(INT32 nData)
	{
#ifdef _WIN32
		return ntohl(nData);
#else
		return be32toh(nData);
#endif
	}

	INT16 Htons(INT16 nData)
	{
#ifdef _WIN32
		return htons(nData);
#else
		return htobe16(nData);
#endif
	}

	INT16 Ntohs (INT16 nData)
	{
#ifdef _WIN32
		return ntohs(nData);
#else
		return be16toh(nData);
#endif
	}

};

class NetMsgHead : public IMsgHead
{
public:
	NetMsgHead()
	{
		mSize = 0;
		mMsgID = 0;
	}

	// Message Head[ MsgID(2) | MsgSize(4) ]
	virtual void EnCode(char* strData)
	{
		UINT16 nMsgID = Htons(mMsgID);
		memcpy(strData, (void*)(&nMsgID), sizeof(mMsgID));

		UINT32 nPackSize = mSize + MSG_HEAD_LEN;
		UINT32 nSize = Htonl(nPackSize);
		memcpy(strData + 2, (void*)(&nSize), sizeof(mSize));
	}

	// Message Head[ MsgID(2) | MsgSize(4) ]
	virtual void DeCode(const char* strData)
	{
		UINT16 nMsgID = 0;
		memcpy(&nMsgID, strData, sizeof(mMsgID));
		mMsgID = Ntohs(nMsgID);

		UINT32 nPackSize = 0;
		memcpy(&nPackSize, strData + 2, sizeof(mSize));
		mSize = Ntohl(nPackSize) - MSG_HEAD_LEN;
	}

	virtual UINT16 GetMsgID() const
	{
		return mMsgID;
	}

	virtual void SetMsgID(UINT16 nMsgID)
	{
		mMsgID = nMsgID;
	}

	virtual UINT32 GetBodyLength() const
	{
		return mSize;
	}

	virtual void SetBodyLength(UINT32 nLength)
	{
		mSize = nLength;
	}

protected:
	UINT32 mSize;
	UINT16 mMsgID;
};

enum EServerType
{
	SERVER_TYPE_NONE = 0,
	SERVER_TYPE_MASTER = 1,
	SERVER_TYPE_GAME = 2,
	SERVER_TYPE_LOGIN = 3,
	SERVER_TYPE_WORLD = 4,
	SERVER_TYPE_GATE = 5,
	SERVER_TYPE_CHAT = 6,
	SERVER_TYPE_DB = 7,
	SERVER_TYPE_MYSQL = 8,
	SERVER_TYPE_REDIS = 9,
	SERVER_TYPE_PLAYER = 10,
	SERVER_TYPE_MAX = 11,
};

enum ConnectState
{
	DISCONNECT,
	CONNECTING,
	NORMAL,
	RECONNECT,
};

// 客户端需要连接服务器信息
struct ConnectData
{
	int ServerId;
	int Port;
	std::string Ip;
	EServerType ServerType;
	ConnectState ConnState;

	std::shared_ptr<seEventLoop> pNet;
};

// 服务器保存连接的客户端信息 客户端上报的信息
struct ServerData
{
	std::shared_ptr<SeFNetProto::ServerReport> ServerInfo;
	socket_t fd;
};