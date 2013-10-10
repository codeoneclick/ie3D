//
//  gcdppImpl.cpp
//  gcdpp
//
//  Created by Sergey Sergeev on 6/26/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "gcdppImpl.h"

using namespace gcdpp;

std::shared_ptr<gcdppImpl> gcdppImpl::Instance(void)
{
    static std::once_flag flag;
    static std::shared_ptr<gcdppImpl> instance = nullptr;
    std::call_once(flag, []()
                   {
                       instance = std::make_shared<gcdppImpl>();
                   });
    return instance;
}


gcdppImpl::gcdppImpl(void)
{
    m_poolQueue[queue::GCDPP_DISPATCH_QUEUE_PRIORITY_MAIN] = std::make_shared<gcdppMainQueue_t>(GCDPP_DISPATCH_QUEUE_PRIORITY_MAIN_STR);
    m_poolQueue[queue::GCDPP_DISPATCH_QUEUE_PRIORITY_LOW] = std::make_shared<gcdppQueue_t>(GCDPP_DISPATCH_QUEUE_PRIORITY_LOW_STR);
    m_poolQueue[queue::GCDPP_DISPATCH_QUEUE_PRIORITY_DEFAULT] = std::make_shared<gcdppQueue_t>(GCDPP_DISPATCH_QUEUE_PRIORITY_DEFAULT_STR);
    m_poolQueue[queue::GCDPP_DISPATCH_QUEUE_PRIORITY_HIGH] = std::make_shared<gcdppQueue_t>(GCDPP_DISPATCH_QUEUE_PRIORITY_HIGH_STR);
}

gcdppImpl::~gcdppImpl(void)
{
    
}

std::shared_ptr<gcdppQueue_t> gcdppImpl::GetGlobalQueue(queue::GCDPP_DISPATCH_QUEUE_PRIORITY _priority)
{
    assert(m_poolQueue[_priority] != nullptr);
    return std::static_pointer_cast<gcdppQueue_t>(m_poolQueue[_priority]);
}

std::shared_ptr<gcdppMainQueue_t> gcdppImpl::GetMainQueue(void)
{
    assert(m_poolQueue[queue::GCDPP_DISPATCH_QUEUE_PRIORITY_MAIN] != nullptr);
    return std::static_pointer_cast<gcdppMainQueue_t>(m_poolQueue[queue::GCDPP_DISPATCH_QUEUE_PRIORITY_MAIN]);
}
