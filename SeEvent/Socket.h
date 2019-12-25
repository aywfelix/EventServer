#pragma once

#include "SocketDefine.h"
#include "SePlatForm.h"

class Socket
{
public:

	void SetSocket(socket_t sock, struct sockaddr_in& addr);


	void CreateFd();
	void CloseSocket();

	// server
	bool Listen(UINT port);
	bool Accept(socket_t& connfd, struct sockaddr_in& addr);
	// client
	bool Connect(const char* ip, UINT port);

	int Send(const char* buf, int len);
	int Recv(char* buf, int len);

	// TODO udp
	socket_t GetFd() { return m_fd; }
	void SetSocketOptions();
private:
	void SetNonBlock();
	// set socket options
	void SetReUseAddr();
	void SetNodelay();
	void SetKeepAlive(INT32 interval = 15);  // 15 seconds
	void SetBufferSize(UINT32 send_size, UINT32 recv_size);
private:
	const char* m_ip;
	UINT m_port;
	socket_t m_fd;
	struct sockaddr_in m_sockAddr;
};
