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
    THREAD_CONCURRENCY_QUEUE_PRIORITY_MAX,
    THREAD_CONCURRENCY_QUEUE_PRIORITY_MAIN
};

#define THREAD_CONCURRENCY_QUEUE_LOW "THREAD_CONCURRENCY_QUEUE_LOW"
#define THREAD_CONCURRENCY_QUEUE_DEFAULT "THREAD_CONCURRENCY_QUEUE_DEFAULT"
#define THREAD_CONCURRENCY_QUEUE_HIGH "THREAD_CONCURRENCY_QUEUE_HIGH"
#define THREAD_CONCURRENCY_QUEUE_MAIN "THREAD_CONCURRENCY_QUEUE_MAIN"

class thread_concurrency_queue;
class thread_concurrency_main_queue;

void thread_concurrency_init(void);
void thread_concurrency_main_queue_update(void);
std::shared_ptr<thread_concurrency_queue> get_thread_concurrency_queue(THREAD_CONCURRENCY_QUEUE_PRIORITY _thread_concurrency_priority);
std::shared_ptr<thread_concurrency_main_queue> get_thread_concurrency_main_queue(void);
template<class... ARGS>
void thread_concurrency_dispatch(std::shared_ptr<thread_concurrency_queue> _queue, std::function<void(ARGS... args)> _function, ARGS... args);

#endif
