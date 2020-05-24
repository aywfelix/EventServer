/*
 * @Author: jia.lai
 * @Date: 2019-12-25 21:19:52
 * @LastEditTime: 2020-05-23 21:27:58
 * @Description: 
 * @Version: 1.0
 */
#pragma once

#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>

struct SeKqueueOp
{
    int kqfd;
    struct kevent *events;
};