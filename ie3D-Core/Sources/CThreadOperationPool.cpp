//
//  CThreadOperationPool.cpp
//  ie3D-Core
//
//  Created by sergey.sergeev on 8/19/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#include "CThreadOperationPool.h"
#include "CThreadOperation.h"

CThreadOperationPool* CThreadOperationPool::m_sharedInstance = nullptr;

CThreadOperationPool* CThreadOperationPool::sharedInstance(void)
{
    if(m_sharedInstance == nullptr)
    {
        m_sharedInstance = new CThreadOperationPool();
    }
    return m_sharedInstance;
}

CThreadOperationPool::CThreadOperationPool(void)
{
    m_isRunning = 1;
    for(ui32 index = 0; index < MAX_THREADS_PER_QUEUE; ++index)
    {
        m_threads.at(index) = std::thread([this, index](void) {
            CThreadOperationPool::updateThread(index);
        });
    }
}

CThreadOperationPool::~CThreadOperationPool(void)
{
    
}

void CThreadOperationPool::addOperation(CSharedThreadOperationRef operation, E_THREAD_OPERATION_QUEUE operationQueue)
{
    std::lock_guard<std::mutex> lockGuard(m_mutex);
    ui32 threadId = 0;
    size_t minOperationCount = m_operations.at(0).at(operationQueue).size();
    for (ui32 i = 0; i < m_operations.size(); ++i)
    {
        if(m_operations.at(i).at(operationQueue).size() < minOperationCount)
        {
            minOperationCount = m_operations.at(i).at(operationQueue).size();
            threadId = i;
        }
    }
    m_operations.at(threadId).at(operationQueue).push(operation);
}

void CThreadOperationPool::updateThread(ui32 threadId)
{
    while (m_isRunning)
    {
        if(!CThreadOperationPool::isQueueEmpty(threadId, E_THREAD_OPERATION_QUEUE_BACKGROUND))
        {
            CSharedThreadOperation operation = CThreadOperationPool::nextOperation(threadId, E_THREAD_OPERATION_QUEUE_BACKGROUND);
            if(operation == nullptr)
            {
                continue;
            }
            CSharedThreadOperation dependencyOperation = operation->nextOperation();
            if(dependencyOperation->getOperationQueueName() == E_THREAD_OPERATION_QUEUE_BACKGROUND)
            {
                if(!dependencyOperation->isCanceled())
                {
                    dependencyOperation->execute();
                }
                operation->popOperation();
            }
            if(dependencyOperation == operation)
            {
                CThreadOperationPool::popOperation(threadId, E_THREAD_OPERATION_QUEUE_BACKGROUND);
            }
        }
        
        if(!CThreadOperationPool::isQueueEmpty(threadId, E_THREAD_OPERATION_QUEUE_MAIN))
        {
            CSharedThreadOperation operation = CThreadOperationPool::nextOperation(threadId, E_THREAD_OPERATION_QUEUE_MAIN);
            if(operation == nullptr)
            {
                continue;
            }
            CSharedThreadOperation dependencyOperation = operation->nextOperation();
            if(dependencyOperation->getOperationQueueName() == E_THREAD_OPERATION_QUEUE_BACKGROUND)
            {
                if(!dependencyOperation->isCanceled())
                {
                    dependencyOperation->execute();
                }
                operation->popOperation();
            }
        }
    }
}

void CThreadOperationPool::update(void)
{
    for(ui32 threadId = 0; threadId < m_threads.size(); ++threadId)
    {
        if(!CThreadOperationPool::isQueueEmpty(threadId, E_THREAD_OPERATION_QUEUE_MAIN))
        {
            CSharedThreadOperation operation = CThreadOperationPool::nextOperation(threadId, E_THREAD_OPERATION_QUEUE_MAIN);
            if(operation == nullptr)
            {
                return;
            }
            CSharedThreadOperation dependencyOperation = operation->nextOperation();
            if(dependencyOperation->getOperationQueueName() == E_THREAD_OPERATION_QUEUE_MAIN)
            {
                if(!dependencyOperation->isCanceled())
                {
                    dependencyOperation->execute();
                }
                operation->popOperation();
            }
            if(dependencyOperation == operation)
            {
                CThreadOperationPool::popOperation(threadId, E_THREAD_OPERATION_QUEUE_MAIN);
            }
        }
        
        if(!CThreadOperationPool::isQueueEmpty(threadId, E_THREAD_OPERATION_QUEUE_BACKGROUND))
        {
            CSharedThreadOperation operation = CThreadOperationPool::nextOperation(threadId, E_THREAD_OPERATION_QUEUE_BACKGROUND);
            if(operation == nullptr)
            {
                return;
            }
            CSharedThreadOperation dependencyOperation = operation->nextOperation();
            if(dependencyOperation->getOperationQueueName() == E_THREAD_OPERATION_QUEUE_MAIN)
            {
                if(!dependencyOperation->isCanceled())
                {
                    dependencyOperation->execute();
                }
                operation->popOperation();
            }
        }
    }
}

CSharedThreadOperation CThreadOperationPool::nextOperation(ui32 threadId, E_THREAD_OPERATION_QUEUE operationQueue)
{
    std::lock_guard<std::mutex> lockGuard(m_mutex);
    CSharedThreadOperation operation = m_operations.at(threadId).at(operationQueue).empty() ? nullptr : m_operations.at(threadId).at(operationQueue).front();
    return operation;
}

void CThreadOperationPool::popOperation(ui32 threadId, E_THREAD_OPERATION_QUEUE operationQueue)
{
    std::lock_guard<std::mutex> lockGuard(m_mutex);
    m_operations.at(threadId).at(operationQueue).pop();
}

bool CThreadOperationPool::isQueueEmpty(ui32 threadId, E_THREAD_OPERATION_QUEUE operationQueue)
{
    std::lock_guard<std::mutex> lockGuard(m_mutex);
    return m_operations.at(threadId).at(operationQueue).empty();
}

