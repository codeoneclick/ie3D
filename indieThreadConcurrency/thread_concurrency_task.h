//
//  thread_concurrency_task.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 6/13/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef thread_concurrency_task_h
#define thread_concurrency_task_h

#include "thread_concurrency_common.h"

class i_thread_concurrency_task
{
private:
    
protected:
    
public:
    
    i_thread_concurrency_task(void) { };
    virtual ~i_thread_concurrency_task(void) { };
    
    virtual void execute(void) { assert(false); };
};

template<class FUCTION, class ARGS> class thread_concurrency_task : public i_thread_concurrency_task
{
protected:
    
    FUCTION m_function;
    ARGS m_args;
    
public:
    thread_concurrency_task(FUCTION _function, ARGS _args)
    {
        m_function = _function;
        m_args = _args;
        std::cout<<"[thread_concurrency_task created]"<<std::endl;
    };
    
    ~thread_concurrency_task(void)
    {
        std::cout<<"[~thread_concurrency_task deleted]"<<std::endl;
    };
    
    void execute(void)
    {
        m_function(m_args);
        std::thread::id this_id = std::this_thread::get_id();
        std::cout << "[execute] thread : "<<this_id<<std::endl;
    };
};


#endif
