#include "Socket.h"
#include "Assertx.h"


void Socket::SetSocket(socket_t sock, struct sockaddr_in& addr)
{
	m_fd = sock;
	memcpy(&m_sockAddr, &addr, sizeof(addr));
}

void Socket::SetNonBlock()
{
#ifdef SF_PLATFORM_WIN
	ULONG ul = 1;
	ioctlsocket(m_fd, FIONBIO, &ul);
#else
	socket_t oldfd = fcntl(m_fd, F_GETFL);
	fcntl(m_fd, F_SETFL, oldfd | O_NONBLOCK);
#endif
}

void Socket::CreateFd()
{
	m_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	Assert(m_fd > 0);
}

void Socket::CloseSocket()
{
#ifdef SF_PLATFORM_WIN
	shutdown(m_fd, SD_BOTH);
	closesocket(m_fd);
#else
	shutdown(m_fd, SHUT_RDWR);
	close(m_fd);
#endif
	m_fd = INVALID_SOCKET;
}

void Socket::BindAddr(struct sockaddr_in& addr, uint32_t port, const char* ip)
{
	m_port = port;
	addr.sin_family = AF_INET;
	addr.sin_port = m_port;
	if (ip != nullptr)
		inet_pton(AF_INET, ip, &addr.sin_addr);
	else
		addr.sin_addr.s_addr = htonl(INADDR_ANY);
	//m_sockAddr.sin_addr.s_addr = inet_addr("192.168.43.38");
}

bool Socket::Listen(uint32_t port)
{
	BindAddr(m_sockAddr, port);
	if (bind(m_fd, (const sockaddr*)&m_sockAddr, sizeof(m_sockAddr)) < 0)
	{
		return false;
	}
	if (listen(m_fd, SOMAXCONN) < 0)
	{
		return false;
	}
	return true;
}

bool Socket::Accept(socket_t& connfd, struct sockaddr_in& addr)
{
	int32_t len = sizeof(addr);
	if ((connfd = accept(m_fd, (struct sockaddr*) & addr, &len)) < 0)
	{
		return false;
	}
	return true;
}

bool Socket::Connect(const char* ip, uint32_t port)
{
	Assert(ip != nullptr);
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = port;
	inet_pton(AF_INET, ip, &addr.sin_addr);
	if ((connect(m_fd, (struct sockaddr*) & addr, sizeof(addr))) < 0)
	{
		return false;
	}
	return true;
}

int Socket::Send(const char* buf, int len)
{
	return send(m_fd, buf, len, 0);
}

int Socket::Recv(char* buf, int len)
{
	return recv(m_fd, buf, len, 0);
}

void Socket::SetNodelay()
{
#if defined SF_PLATFORM_LINUX
	const int optval = 1;
	setsockopt(m_fd, SOL_SOCKET, TCP_NODELAY, (const char*)&optval, sizeof(optval));
#endif	
}

void Socket::SetKeepAlive(uint32_t interval)
{
	int val = 1;
	setsockopt(m_fd, SOL_SOCKET, SO_KEEPALIVE, (const char*)&val, sizeof(val));
	val = interval;

#ifdef SF_PLATFORM_LINUX
	setsockopt(m_fd, IPPROTO_TCP, TCP_KEEPIDLE, &val, sizeof(val));
	val = interval / 3;
	if (val == 0) val = 1;
	setsockopt(fd, IPPROTO_TCP, TCP_KEEPINTVL, &val, sizeof(val));
	val = 5;
	setsockopt(fd, IPPROTO_TCP, TCP_KEEPCNT, &val, sizeof(val));
#endif
}

void Socket::SetBufferSize(uint32_t send_size, uint32_t recv_size)
{
	setsockopt(m_fd, SOL_SOCKET, SO_SNDBUF, (const char*)&send_size, sizeof(send_size));
	setsockopt(m_fd, SOL_SOCKET, SO_RCVBUF, (const char*)&recv_size, sizeof(recv_size));
}

void Socket::SetReUseAddr()
{
	const int optval = 1;
	setsockopt(m_fd, SOL_SOCKET, SO_REUSEADDR, (const char*)&optval, sizeof(optval));
}

void Socket::SetSocketOptions()
{
	SetNonBlock();
	SetReUseAddr();
	SetNodelay();
	SetKeepAlive(SOCKET_KEEP_ALIVE_INTERVAL);
	SetBufferSize(SOCKET_BUFFER_SIZE, SOCKET_BUFFER_SIZE);
}

void Socket::CreateUDPFd()
{
	m_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	Assert(m_fd > 0);
}

int Socket::RecvFrom(char* buf, int len)
{
	int addr_len = sizeof(struct sockaddr_in);
	return recvfrom(m_fd, buf, len, 0, (struct sockaddr*)&m_sockAddr, &addr_len);
}

int Socket::SendTo(const char* buf, int len, const struct sockaddr& addr)
{
	return sendto(m_fd, buf, len, 0, &addr, sizeof(addr));
}

void Socket::SetBroadCast()
{
	const int on = 1;
	setsockopt(m_fd, SOL_SOCKET, SO_BROADCAST, (const char*)&on, sizeof(on));
}