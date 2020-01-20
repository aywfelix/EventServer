#include "SeSelect.h"
#include "Socket.h"

bool SeSelect::InitOp()
{
	FD_ZERO(&mSelectOp.rfds);
	FD_ZERO(&mSelectOp.wfds);
	return true;
}

bool SeSelect::AddEvent(socket_t fd, int mask)
{
	if (mask & EV_READ)
	{
		FD_SET(fd, &mSelectOp.rfds);
	}
	if (mask & EV_WRITE)
	{
		FD_SET(fd, &mSelectOp.wfds);
	}
	return true;
}

bool SeSelect::DelEvent(socket_t fd, int mask)
{
	if (mask & EV_READ)
	{
		FD_CLR(fd, &mSelectOp.rfds);
	}
	if (mask & EV_WRITE)
	{
		FD_CLR(fd, &mSelectOp.wfds);
	}
	return true;
}

bool  SeSelect::Dispatch(struct timeval* tv)
{
	int ret = 0;
	memcpy(&mSelectOp._rfds, &mSelectOp.rfds, sizeof(fd_set));
	memcpy(&mSelectOp._wfds, &mSelectOp.wfds, sizeof(fd_set));

	ret = select((int)m_maxfd + 1,&mSelectOp._rfds, &mSelectOp._wfds, NULL, tv);
	if (ret == -1)
	{
		if (SOCKET_ERR_RW_RETRIABLE(errno))
		{
			return true;
		}
		fprintf(stderr, "select error %d:%s", errno, strerror(errno));
		return false;
	}
	if (ret > 0) {
		for (socket_t iter = 0; iter <= m_maxfd; iter++)
		{
			int mask = 0;
			if (FD_ISSET(iter, &mSelectOp._rfds))
			{
				mask |= EV_READ;
			}
			if (FD_ISSET(iter, &mSelectOp._wfds))
			{
				mask |= EV_WRITE;
			}
			if (mask == 0)
			{
				continue;
			}
			SetEvent(iter, mask);
		}
	}
	return true;
}

bool SeSelect::Clear()
{
	for (int i=0;i<= m_maxfd; i++)
	{
		if (FD_ISSET(i, &mSelectOp.rfds))
		{
			FD_CLR(i, &mSelectOp.rfds);
		}
		if (FD_ISSET(i, &mSelectOp.wfds))
		{
			FD_CLR(i, &mSelectOp.wfds);
		}
	}
	return true;
}