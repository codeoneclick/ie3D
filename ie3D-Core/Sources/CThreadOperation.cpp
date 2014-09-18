//
//  CThreadOperation.cpp
//  ie3D-Core
//
//  Created by sergey.sergeev on 8/19/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#include "CThreadOperation.h"
#include "CThreadOperationPool.h"

CThreadOperation::CThreadOperation(E_THREAD_OPERATION_QUEUE operationQueue) :
m_operationQueue(operationQueue),
m_executionBlock(nullptr),
m_cancelBlock(nullptr),
m_status(E_THREAD_OPERATION_STATUS_NONE)
{
    
}

CThreadOperation::~CThreadOperation(void)
{
    m_executionBlock = nullptr;
    m_cancelBlock = nullptr;
    while (!m_dependecies.empty()) {
        m_dependecies.pop();
    }
    m_uniqueDependecies.clear();
}

void CThreadOperation::setExecutionBlock(std::function<void(void)> callback)
{
    m_executionBlock = callback;
}

void CThreadOperation::setCancelBlock(std::function<void(void)> callback)
{
    m_cancelBlock = callback;
}

void CThreadOperation::addDependency(CSharedThreadOperationRef operation)
{
    std::lock_guard<std::mutex> lockGuard(m_mutex);
    m_dependecies.push(operation);
    m_uniqueDependecies.insert(operation);
}

CSharedThreadOperation CThreadOperation::nextOperation(void)
{
    if(!CThreadOperation::isQueueEmpty())
    {
        std::lock_guard<std::mutex> lockGuard(m_mutex);
        CSharedThreadOperation operation = m_dependecies.front();
        return operation->nextOperation();
    }
    return shared_from_this();
}

bool CThreadOperation::popOperation(void)
{
    if(!CThreadOperation::isQueueEmpty())
    {
        CSharedThreadOperation operation = m_dependecies.front();
        if(operation->popOperation())
        {
            std::lock_guard<std::mutex> lockGuard(m_mutex);
            m_dependecies.pop();
            m_uniqueDependecies.erase(operation);
        }
    }
    else
    {
        return true;
    }
    return false;
}

void CThreadOperation::execute(void)
{
    assert(m_executionBlock != nullptr);
    if(m_executionBlock != nullptr)
    {
        m_status |= E_THREAD_OPERATION_STATUS_EXECUTED;
        m_executionBlock();
        m_status |= E_THREAD_OPERATION_STATUS_COMPLETED;
    }
}

void CThreadOperation::addToExecutionQueue(void)
{
    CThreadOperationPool::sharedInstance()->addOperation(shared_from_this(), m_operationQueue);
}

void CThreadOperation::cancel(void)
{
    for(const auto& operation : m_uniqueDependecies)
    {
        operation->cancel();
    }
    if(m_cancelBlock != nullptr)
    {
        m_cancelBlock();
    }
    if(!CThreadOperation::isExecuted())
    {
        m_status |= E_THREAD_OPERATION_STATUS_COMPLETED;
    }
    m_status |= E_THREAD_OPERATION_STATUS_CANCELED;
}

bool CThreadOperation::isExecuted(void)
{
    return m_status & E_THREAD_OPERATION_STATUS_EXECUTED;
}

bool CThreadOperation::isCanceled(void)
{
    return m_status & E_THREAD_OPERATION_STATUS_CANCELED;
}

bool CThreadOperation::isCompleted(void)
{
    return m_status & E_THREAD_OPERATION_STATUS_COMPLETED;
}

bool CThreadOperation::isQueueEmpty(void)
{
    std::lock_guard<std::mutex> lockGuard(m_mutex);
    return m_dependecies.empty();
}

E_THREAD_OPERATION_QUEUE CThreadOperation::getOperationQueueName(void)
{
    return m_operationQueue;
}