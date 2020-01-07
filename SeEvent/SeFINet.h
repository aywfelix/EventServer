#pragma once

#include <memory>
#include <string>
#include "SePlatForm.h"
#include "Session.h"
#include "SeFNodeNet.pb.h"
#include "SeFNet.h"

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

	UINT64 Htonll(UINT64 nData)
	{
#ifdef _WIN32
		return htonll(nData);
#else
		return htobe64(nData);
#endif
	}

	UINT64 Ntohll(UINT64 nData)
	{
#ifdef _WIN32
		return ntohll(nData);
#else
		return be64toh(nData);
#endif
	}

	UINT32 Htonl(UINT32 nData)
	{
#ifdef _WIN32
		return htonl(nData);
#else
		return htobe32(nData);
#endif
	}

	UINT32 Ntohl(UINT32 nData)
	{
#ifdef _WIN32
		return ntohl(nData);
#else
		return be32toh(nData);
#endif
	}

	UINT16 Htons(UINT16 nData)
	{
#ifdef _WIN32
		return htons(nData);
#else
		return htobe16(nData);
#endif
	}

	UINT16 Ntohs (UINT16 nData)
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
	NetMsgHead(UINT16 MsgID, UINT32 Size):mMsgID(MsgID), mSize(Size){}

	// Message Head[ MsgID(2) | MsgSize(4) ]
	virtual void EnCode(char* strData)
	{
		UINT16 nMsgID = Htons(mMsgID);
		memcpy(strData, &nMsgID, sizeof(nMsgID));

		UINT32 nPackSize = mSize + MSG_HEAD_LEN;
		UINT32 nSize = Htonl(nPackSize);
		memcpy(strData + 2, &nSize, sizeof(nSize));
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

private:
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
	std::string name;
	EServerType ServerType;
	ConnectState ConnState;
	socket_t SockFd;
	std::shared_ptr<SeFNet> pNet;
};

// 服务器保存连接的客户端信息 客户端上报的信息
struct ServerData
{
	std::shared_ptr<SeFNetProto::ServerReport> ServerInfo;
	socket_t fd;
};

using ConnectDataPtr = std::shared_ptr<ConnectData>;
using ServerDataPtr = std::shared_ptr<ServerData>;

