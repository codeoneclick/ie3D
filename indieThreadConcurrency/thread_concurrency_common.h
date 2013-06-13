//
//  thread_concurrency_common.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 6/12/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef thread_concurrency_common_h
#define thread_concurrency_common_h

#include <map>
#include <set>
#include <queue>
#include <string>
#include <vector>
#include <numeric>
#include <future>
#include <algorithm>
#include <mutex>
#include <thread>
#include <fstream>
#include <strstream>
#include <sstream>
#include <iostream>
#include <assert.h>

enum THREAD_CONCURRENCY_QUEUE_PRIORITY
{
    THREAD_CONCURRENCY_QUEUE_PRIORITY_LOW = 1,
    THREAD_CONCURRENCY_QUEUE_PRIORITY_DEFAULT,
    THREAD_CONCURRENCY_QUEUE_PRIORITY_HIGH,
    THREAD_CONCURRENCY_QUEUE_PRIORITY_MAX
};

#define THREAD_CONCURRENCY_QUEUE_LOW "THREAD_CONCURRENCY_QUEUE_LOW"
#define THREAD_CONCURRENCY_QUEUE_DEFAULT "THREAD_CONCURRENCY_QUEUE_DEFAULT"
#define THREAD_CONCURRENCY_QUEUE_HIGH "THREAD_CONCURRENCY_QUEUE_HIGH"

#endif
