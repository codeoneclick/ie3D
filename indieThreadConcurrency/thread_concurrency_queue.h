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

class thread_concurrency_queue
{
private:
    
protected:
    
    std::string m_guid;
    
public:
    
    thread_concurrency_queue(const std::string& _guid);
    ~thread_concurrency_queue(void);
};

#endif 
