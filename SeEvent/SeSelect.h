#pragma once

#ifndef _WIN32
#include <sys/select.h>
#endif

#include "SePlatForm.h"
#include "Se.h"

struct SeSelectOp
{
    fd_set rfds, wfds;
    fd_set _rfds, _wfds;
};

class SeSelect : public SeEventOp
{
public:
	virtual bool InitOp();
	virtual bool AddEvent(socket_t fd, int mask);
	virtual bool DelEvent(socket_t fd, int mask);
	virtual bool Dispatch(struct timeval* tv);
	virtual bool Clear();
protected:

private:
	SeSelectOp mSelectOp;
};