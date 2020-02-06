#pragma once

#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>

struct SeKqueueOp
{
    int kqfd;
    struct kevent *events;
};