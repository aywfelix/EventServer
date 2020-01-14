#pragma once

#include <memory>
#include <string>
#include <functional>
#include "SocketDefine.h"
#include "SeFNodeNet.pb.h"


class SeNet;

#define SE_OK 0
#define SE_ERR -1

#define EV_NONE 0
#define EV_TIMEOUT 1
#define EV_READ	2
#define EV_WRITE 4
#define EV_CLOSED 8

enum LOOP_RUN_TYPE
{
	LOOP_RUN_BLOCK = 1,
	LOOP_RUN_NONBLOCK = 2,
};

enum SE_NET_EVENT
{
	SE_NET_EVENT_EOF = 0x10,	/**< eof file reached */
	SE_NET_EVENT_ERROR = 0x20,	/**< unrecoverable error encountered */
	SE_NET_EVENT_TIMEOUT = 0x40,	/**< user-specified timeout reached */
	SE_NET_EVENT_CONNECTED = 0x80,	/**< connect operation finished. */
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

using NET_RECEIVE_FUNCTOR = std::function<void(const socket_t nFd, const int nMsgId, const char* pMsg, const uint32_t nLen)>;
using NET_RECEIVE_FUNCTOR_PTR = std::shared_ptr<NET_RECEIVE_FUNCTOR>;

using NET_EVENT_FUNCTOR = std::function<void(const socket_t nFd, const SE_NET_EVENT nEvent, SeNet* pNet)>;
using NET_EVENT_FUNCTOR_PTR = std::shared_ptr<NET_EVENT_FUNCTOR>;


// Message Head[ MsgID(2) | MsgSize(4) ]
#define MSG_HEAD_LEN 6

struct IMsgHead
{
	virtual void EnCode(char* strData) = 0;
	virtual void DeCode(const char* strData) = 0;
	virtual uint16_t GetMsgID() const = 0;
	virtual void SetMsgID(uint16_t nMsgID) = 0;
	virtual uint32_t GetBodyLength() const = 0;
	virtual void SetBodyLength(uint32_t nLength) = 0;

	uint64_t Htonll(uint64_t nData)
	{
#ifdef SF_PLATFORM_WIN
		return htonll(nData);
#else
		return htobe64(nData);
#endif
	}

	uint64_t Ntohll(uint64_t nData)
	{
#ifdef SF_PLATFORM_WIN
		return ntohll(nData);
#else
		return be64toh(nData);
#endif
	}

	uint32_t Htonl(uint32_t nData)
	{
#ifdef SF_PLATFORM_WIN
		return htonl(nData);
#else
		return htobe32(nData);
#endif
	}

	uint32_t Ntohl(uint32_t nData)
	{
#ifdef SF_PLATFORM_WIN
		return ntohl(nData);
#else
		return be32toh(nData);
#endif
	}

	uint16_t Htons(uint16_t nData)
	{
#ifdef SF_PLATFORM_WIN
		return htons(nData);
#else
		return htobe16(nData);
#endif
	}

	uint16_t Ntohs (uint16_t nData)
	{
#ifdef SF_PLATFORM_WIN
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
	NetMsgHead(uint16_t MsgID, uint32_t Size):mMsgID(MsgID), mSize(Size){}

	// Message Head[ MsgID(2) | MsgSize(4) ]
	virtual void EnCode(char* strData)
	{
		uint16_t nMsgID = Htons(mMsgID);
		memcpy(strData, &nMsgID, sizeof(nMsgID));

		uint32_t nPackSize = mSize + MSG_HEAD_LEN;
		uint32_t nSize = Htonl(nPackSize);
		memcpy(strData + 2, &nSize, sizeof(nSize));
	}

	// Message Head[ MsgID(2) | MsgSize(4) ]
	virtual void DeCode(const char* strData)
	{
		uint16_t nMsgID = 0;
		memcpy(&nMsgID, strData, sizeof(mMsgID));
		mMsgID = Ntohs(nMsgID);

		uint32_t nPackSize = 0;
		memcpy(&nPackSize, strData + 2, sizeof(mSize));
		mSize = Ntohl(nPackSize) - MSG_HEAD_LEN;
	}

	virtual uint16_t GetMsgID() const
	{
		return mMsgID;
	}

	virtual void SetMsgID(uint16_t nMsgID)
	{
		mMsgID = nMsgID;
	}

	virtual uint32_t GetBodyLength() const
	{
		return mSize;
	}

	virtual void SetBodyLength(uint32_t nLength)
	{
		mSize = nLength;
	}

private:
	uint32_t mSize;
	uint16_t mMsgID;
};

