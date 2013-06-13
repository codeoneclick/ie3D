//
//  thread_concurrency.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 6/12/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "thread_concurrency.h"


thread_concurrency::thread_concurrency(void)
{
    m_pool[THREAD_CONCURRENCY_QUEUE_PRIORITY_LOW] = std::make_shared<thread_concurrency_queue>("low");
    m_pool[THREAD_CONCURRENCY_QUEUE_PRIORITY_DEFAULT] = std::make_shared<thread_concurrency_queue>("default");
    m_pool[THREAD_CONCURRENCY_QUEUE_PRIORITY_HIGH] = std::make_shared<thread_concurrency_queue>("high");
}

thread_concurrency::~thread_concurrency(void)
{
    
}

std::shared_ptr<thread_concurrency_queue> thread_concurrency::get_thread_concurrency_queue(THREAD_CONCURRENCY_QUEUE_PRIORITY _thread_concurrency_priority)
{
    return m_pool[_thread_concurrency_priority];
}