//
//  thread_concurrency_queue.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 6/12/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef thread_concurrency_queue_h
#define thread_concurrency_queue_h

#include "thread_concurrency_common.h"
#include "thread_concurrency_task.h"

class thread_concurrency_main_queue
{
private:
    
protected:
    
    friend void thread_concurrency_main_queue_update(void);
    
    std::string m_guid;
    std::queue<std::shared_ptr<i_thread_concurrency_task> > m_queue;
    
    void _Update(void);

public:
    
    thread_concurrency_main_queue(const std::string& _guid);
    virtual ~thread_concurrency_main_queue(void);
    
    virtual void append_task(std::shared_ptr<i_thread_concurrency_task> _thread_concurrency_task);
};


class thread_concurrency_queue : public thread_concurrency_main_queue
{
private:

protected:
    
    std::mutex m_mutex;
    std::thread m_thread;
    std::atomic_int m_running;
    
    void _Thread(void);
    
public:
    
    thread_concurrency_queue(const std::string& _guid);
    ~thread_concurrency_queue(void);
    
    void append_task(std::shared_ptr<i_thread_concurrency_task> _thread_concurrency_task);
};


#endif 
