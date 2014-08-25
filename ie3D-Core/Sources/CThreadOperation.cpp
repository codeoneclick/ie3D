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
m_isDone(false),
m_isCanceled(false)
{
    
}

CThreadOperation::~CThreadOperation(void)
{
    
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
    std::unique_lock<std::mutex> mutexLock(m_mutex);
    m_dependecies.push(operation);
    m_dependeciesReferences.insert(operation);
    mutexLock.unlock();
}

CSharedThreadOperation CThreadOperation::nextOperation(void)
{
    if(!CThreadOperation::isQueueEmpty())
    {
        std::unique_lock<std::mutex> mutexLock(m_mutex);
        CSharedThreadOperation operation = m_dependecies.front();
        mutexLock.unlock();
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
            std::unique_lock<std::mutex> mutexLock(m_mutex);
            m_dependecies.pop();
            m_dependeciesReferences.erase(operation);
            mutexLock.unlock();
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
    CThreadOperationPool::sharedInstance()->addOperation(shared_from_this(), m_operationQueue);
}

void CThreadOperation::cancel(void)
{
    for(const auto& operation : m_dependeciesReferences)
    {
        operation->cancel();
    }
    m_isCanceled = true;
}

bool CThreadOperation::getIsDone(void)
{
    return m_isDone;
}

bool CThreadOperation::getIsCanceled(void)
{
    return m_isCanceled;
}

bool CThreadOperation::isQueueEmpty(void)
{
    std::unique_lock<std::mutex> mutexLock(m_mutex);
    bool isQueueEmpty = m_dependecies.empty();
    mutexLock.unlock();
    return isQueueEmpty;
}