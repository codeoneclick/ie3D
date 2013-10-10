//
//  gcdppQueue_t.cpp
//  gcdpp
//
//  Created by Sergey Sergeev on 6/26/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "gcdppQueue_t.h"

using namespace gcdpp;

gcdppQueue_t::gcdppQueue_t(const std::string& _guid) :
gcdppQueue_i(_guid),
m_running(1),
m_thread(&gcdppQueue_t::_Thread, this)
{
    
}

gcdppQueue_t::~gcdppQueue_t(void)
{
    m_running = 0;
    m_thread.join();
}

void gcdppQueue_t::AppendTask(std::shared_ptr<gcdppTask_i> _task)
{
    m_mutex.lock();
    m_queue.push(_task);
    m_mutex.unlock();
}

void gcdppQueue_t::_Thread(void)
{
    while (m_running)
    {
        if(!m_queue.empty())
        {
            m_mutex.lock();
            std::shared_ptr<gcdppTask_i> task_ = m_queue.front();
            m_queue.pop();
            m_mutex.unlock();
            task_->Execute();
        }
        else
        {
            std::this_thread::yield();
        }
    }
}