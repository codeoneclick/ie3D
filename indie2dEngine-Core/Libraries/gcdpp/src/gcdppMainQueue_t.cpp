//
//  gcdppMainQueue_t.cpp
//  gcdpp
//
//  Created by Sergey Sergeev on 6/26/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "gcdppMainQueue_t.h"

using namespace gcdpp;

gcdppMainQueue_t::gcdppMainQueue_t(const std::string& _guid) :
gcdppQueue_i(_guid)
{
    
}

gcdppMainQueue_t::~gcdppMainQueue_t(void)
{
    
}

void gcdppMainQueue_t::AppendTask(std::shared_ptr<gcdppTask_i> _task)
{
    assert(_task != nullptr);
    m_queue.push(_task);
}

void gcdppMainQueue_t::_Update(void)
{
    while(!m_queue.empty())
    {
        m_queue.front()->Execute();
        m_queue.pop();
    }
}
