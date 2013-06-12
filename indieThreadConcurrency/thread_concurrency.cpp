//
//  thread_concurrency.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 6/12/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "thread_concurrency.h"
#include "thread_concurrency_queue.h"

thread_concurrency::thread_concurrency(void)
{
    
}

thread_concurrency::~thread_concurrency(void)
{
    
}

template<typename RESULT, typename ...ARGS>
void thread_concurrency::execute(thread_concurrency_queue _queue, std::function<RESULT (ARGS...)> _function)
{
    
}