//
//  thread_concurrency.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 6/12/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef thread_concurrency_h
#define thread_concurrency_h

#include "thread_concurrency_common.h"
#include "thread_concurrency_queue.h"
#include "thread_concurrency_task.h"

class thread_concurrency
{
private:
    
protected:
    
    friend void thread_concurrency_init(void);
    friend void thread_concurrency_main_queue_update(void);
    friend std::shared_ptr<thread_concurrency_queue> get_thread_concurrency_queue(THREAD_CONCURRENCY_QUEUE_PRIORITY _thread_concurrency_priority);
    friend std::shared_ptr<thread_concurrency_main_queue> get_thread_concurrency_main_queue(void);
    template<class... ARGS>
    friend void thread_concurrency_dispatch(std::shared_ptr<thread_concurrency_queue> _queue, std::function<void(ARGS... args)> _function, ARGS... args);
    
    std::shared_ptr<thread_concurrency_main_queue> m_mainQueue;
    std::shared_ptr<thread_concurrency_queue> m_poolQueue[THREAD_CONCURRENCY_QUEUE_PRIORITY_MAX];
    
    static std::shared_ptr<thread_concurrency> instance(void);
    
    std::shared_ptr<thread_concurrency_queue> get_thread_concurrency_queue(THREAD_CONCURRENCY_QUEUE_PRIORITY _thread_concurrency_priority);
    std::shared_ptr<thread_concurrency_main_queue> get_thread_concurrency_main_queue(void);
    
    template<class... ARGS>
    void dispatch(std::shared_ptr<thread_concurrency_queue> _queue, std::function<void(ARGS... args)> _function, ARGS... args);

public:
    
    thread_concurrency(void);
    ~thread_concurrency(void);
    
};

template<class... ARGS>
void thread_concurrency::dispatch(std::shared_ptr<thread_concurrency_queue> _queue, std::function<void(ARGS... args)> _function, ARGS... args)
{
    std::shared_ptr<thread_concurrency_task<std::function<void(ARGS...)>, ARGS... > > thread_concurrency_task_ = std::make_shared<thread_concurrency_task<std::function<void(ARGS...)>, ARGS... > >(_function, std::forward<ARGS>(args)...);
    _queue->append_task(thread_concurrency_task_);
};

template<class... ARGS>
void thread_concurrency_dispatch(std::shared_ptr<thread_concurrency_queue> _queue, std::function<void(ARGS... args)> _function, ARGS... args)
{
    thread_concurrency::instance()->dispatch(_queue, _function, std::forward<ARGS>(args)...);
};

#endif
