//
//  thread_concurrency.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 6/12/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "thread_concurrency.h"

std::shared_ptr<thread_concurrency> thread_concurrency::instance(void)
{
    static std::once_flag flag;
    static std::shared_ptr<thread_concurrency> instance_ = nullptr;
    std::call_once(flag, []()
                   {
                       instance_ = std::make_shared<thread_concurrency>();
                   });
    return instance_;
}


thread_concurrency::thread_concurrency(void)
{
    m_mainQueue = std::make_shared<thread_concurrency_main_queue>("main.queue");
    
    m_poolQueue[THREAD_CONCURRENCY_QUEUE_PRIORITY_LOW] = std::make_shared<thread_concurrency_queue>("low.priority.queue");
    m_poolQueue[THREAD_CONCURRENCY_QUEUE_PRIORITY_DEFAULT] = std::make_shared<thread_concurrency_queue>("default.priority.queue");
    m_poolQueue[THREAD_CONCURRENCY_QUEUE_PRIORITY_HIGH] = std::make_shared<thread_concurrency_queue>("high.priority.queue");
}

thread_concurrency::~thread_concurrency(void)
{
    
}

std::shared_ptr<thread_concurrency_queue> thread_concurrency::get_thread_concurrency_queue(THREAD_CONCURRENCY_QUEUE_PRIORITY _thread_concurrency_priority)
{
    assert(m_poolQueue[_thread_concurrency_priority] != nullptr);
    return m_poolQueue[_thread_concurrency_priority];
}
 
std::shared_ptr<thread_concurrency_main_queue> thread_concurrency::get_thread_concurrency_main_queue(void)
{
    assert(m_mainQueue != nullptr);
    return m_mainQueue;
}

void thread_concurrency_init(void)
{
    thread_concurrency::instance();
}

void thread_concurrency_main_queue_update(void)
{
    assert(thread_concurrency::instance()->m_mainQueue != nullptr);
    thread_concurrency::instance()->m_mainQueue->_Update();
}

std::shared_ptr<thread_concurrency_queue> get_thread_concurrency_queue(THREAD_CONCURRENCY_QUEUE_PRIORITY _thread_concurrency_priority)
{
    return thread_concurrency::instance()->get_thread_concurrency_queue(_thread_concurrency_priority);
}

std::shared_ptr<thread_concurrency_main_queue> get_thread_concurrency_main_queue(void)
{
    return thread_concurrency::instance()->get_thread_concurrency_main_queue();
}
