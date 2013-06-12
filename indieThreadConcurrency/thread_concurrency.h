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

class thread_concurrency_queue;

class thread_concurrency
{
private:
    
protected:
    
public:
    
    thread_concurrency(void);
    ~thread_concurrency(void);
    
    template<typename RESULT, typename ...ARGS>
    void execute(thread_concurrency_queue _queue, std::function<RESULT(ARGS... args)> _function);
    
};

#endif
