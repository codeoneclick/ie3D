//
//  thread_concurrency_queue.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 6/12/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "thread_concurrency_queue.h"
#include "thread_concurrency_task.h"

thread_concurrency_main_queue::thread_concurrency_main_queue(const std::string& _guid) :
m_guid(_guid)
{
    
}

thread_concurrency_main_queue::~thread_concurrency_main_queue(void)
{
    
}

void thread_concurrency_main_queue::append_task(std::shared_ptr<i_thread_concurrency_task> _thread_concurrency_task)
{
    m_queue.push(_thread_concurrency_task);
}

void thread_concurrency_main_queue::_Update(void)
{
    while(!m_queue.empty())
    {
        std::shared_ptr<i_thread_concurrency_task> thread_concurrency_task_ =  m_queue.front();
        m_queue.pop();
        thread_concurrency_task_->execute();
    }
}

thread_concurrency_queue::thread_concurrency_queue(const std::string& _guid) :
thread_concurrency_main_queue(_guid),
m_running(1),
m_thread(&thread_concurrency_queue::_Thread, this)
{
    
}

thread_concurrency_queue::~thread_concurrency_queue(void)
{
    m_running = 0;
    m_thread.join();
}

void thread_concurrency_queue::append_task(std::shared_ptr<i_thread_concurrency_task> _thread_concurrency_task)
{
    m_mutex.lock();
    m_queue.push(_thread_concurrency_task);
    m_mutex.unlock();
}

void thread_concurrency_queue::_Thread(void)
{
    while (m_running)
    {
        if(!m_queue.empty())
        {
            m_mutex.lock();
            std::shared_ptr<i_thread_concurrency_task> thread_concurrency_task_ =  m_queue.front();
            m_queue.pop();
            m_mutex.unlock();
            thread_concurrency_task_->execute();
        }
        else
        {
            std::this_thread::yield();
        }
    }
}