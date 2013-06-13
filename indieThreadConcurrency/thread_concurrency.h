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
    
    std::shared_ptr<thread_concurrency_queue> m_pool[THREAD_CONCURRENCY_QUEUE_PRIORITY_MAX];
    
public:
    
    thread_concurrency(void);
    ~thread_concurrency(void);
    
    template<class RESULT, class... ARGS>
    void dispatch(std::shared_ptr<thread_concurrency_queue> _queue, std::function<RESULT(ARGS... args)> _function, ARGS... args);
    
    std::shared_ptr<thread_concurrency_queue> get_thread_concurrency_queue(THREAD_CONCURRENCY_QUEUE_PRIORITY _thread_concurrency_priority);
};


template<class RESULT, class... ARGS>
void thread_concurrency::dispatch(std::shared_ptr<thread_concurrency_queue> _queue, std::function<RESULT(ARGS... args)> _function, ARGS... args)
{
    std::shared_ptr<thread_concurrency_task<std::function<RESULT(ARGS...)>, ARGS... > > thread_concurrency_task_ = std::make_shared<thread_concurrency_task<std::function<RESULT(ARGS...)>, ARGS... > >(_function, std::forward<ARGS>(args)...);
    _queue->append_task(thread_concurrency_task_);
};

#endif
