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

#define MAX_FD_NUM 5000

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


enum ServerType
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

using NET_RECEIVE_FUNCTOR = std::function<void(const socket_t sock_fd, const int msg_id, const char* msg, const size_t msg_len)>;
using NET_RECEIVE_FUNCTOR_PTR = std::shared_ptr<NET_RECEIVE_FUNCTOR>;

using NET_EVENT_FUNCTOR = std::function<void(const socket_t sock_fd, const SE_NET_EVENT nEvent, SeNet* pNet)>;
using NET_EVENT_FUNCTOR_PTR = std::shared_ptr<NET_EVENT_FUNCTOR>;


// Message Head[ MsgID(2) | MsgSize(4) ]
#define MSG_HEAD_LEN 6

class IMsgHead
{
public:
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
		m_size = 0;
		m_msgid = 0;
	}
	NetMsgHead(uint16_t MsgID, uint32_t Size):m_msgid(MsgID), m_size(Size){}

	// Message Head[ MsgID(2) | MsgSize(4) ]
	virtual void EnCode(char* strData)
	{
		uint16_t msg_id = Htons(m_msgid);
		memcpy(strData, &msg_id, sizeof(msg_id));

		uint32_t nPackSize = m_size + MSG_HEAD_LEN;
		uint32_t nSize = Htonl(nPackSize);
		memcpy(strData + 2, &nSize, sizeof(nSize));
	}

	// Message Head[ MsgID(2) | MsgSize(4) ]
	virtual void DeCode(const char* strData)
	{
		uint16_t msg_id = 0;
		memcpy(&msg_id, strData, sizeof(m_msgid));
		m_msgid = Ntohs(msg_id);

		uint32_t nPackSize = 0;
		memcpy(&nPackSize, strData + 2, sizeof(m_size));
		m_size = Ntohl(nPackSize) - MSG_HEAD_LEN;
	}

	virtual uint16_t GetMsgID() const
	{
		return m_msgid;
	}

	virtual void SetMsgID(uint16_t msg_id)
	{
		m_msgid = msg_id;
	}

	virtual uint32_t GetBodyLength() const
	{
		return m_size;
	}

	virtual void SetBodyLength(uint32_t msg_length)
	{
		m_size = msg_length;
	}

private:
	uint32_t m_size;
	uint16_t m_msgid;
};