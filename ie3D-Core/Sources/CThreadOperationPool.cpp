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
    m_thread = std::thread(&CThreadOperationPool::updateThread, this);
}

CThreadOperationPool::~CThreadOperationPool(void)
{
    
}

void CThreadOperationPool::addOperation(CSharedThreadOperationRef operation, E_THREAD_OPERATION_QUEUE operationQueue)
{
    std::unique_lock<std::mutex> mutexLock(m_mutex);
    m_operations.at(operationQueue).push(operation);
    mutexLock.unlock();
}

void CThreadOperationPool::updateThread(void)
{
    while (m_isRunning)
    {
        if(!CThreadOperationPool::isQueueEmpty(E_THREAD_OPERATION_QUEUE_BACKGROUND))
        {
            CSharedThreadOperation operation = CThreadOperationPool::nextOperation(E_THREAD_OPERATION_QUEUE_BACKGROUND);
            if(operation == nullptr)
            {
                continue;
            }
            CSharedThreadOperation dependencyOperation = operation->nextOperation();
            if(dependencyOperation->m_operationQueue == E_THREAD_OPERATION_QUEUE_BACKGROUND)
            {
                if(!dependencyOperation->getIsCanceled())
                {
                    dependencyOperation->m_executionBlock();
                }
                operation->popOperation();
            }
            if(dependencyOperation == operation)
            {
                CThreadOperationPool::popOperation(E_THREAD_OPERATION_QUEUE_BACKGROUND);
            }
        }
        
        if(!CThreadOperationPool::isQueueEmpty(E_THREAD_OPERATION_QUEUE_MAIN))
        {
            CSharedThreadOperation operation = CThreadOperationPool::nextOperation(E_THREAD_OPERATION_QUEUE_MAIN);
            if(operation == nullptr)
            {
                continue;
            }
            CSharedThreadOperation dependencyOperation = operation->nextOperation();
            if(dependencyOperation->m_operationQueue == E_THREAD_OPERATION_QUEUE_BACKGROUND)
            {
                if(!dependencyOperation->getIsCanceled())
                {
                    dependencyOperation->m_executionBlock();
                }
                operation->popOperation();
            }
        }
    }
}

void CThreadOperationPool::update(void)
{
    if(!CThreadOperationPool::isQueueEmpty(E_THREAD_OPERATION_QUEUE_MAIN))
    {
        CSharedThreadOperation operation = CThreadOperationPool::nextOperation(E_THREAD_OPERATION_QUEUE_MAIN);
        if(operation == nullptr)
        {
            return;
        }
        CSharedThreadOperation dependencyOperation = operation->nextOperation();
        if(dependencyOperation->m_operationQueue == E_THREAD_OPERATION_QUEUE_MAIN)
        {
            if(!dependencyOperation->getIsCanceled())
            {
                dependencyOperation->m_executionBlock();
            }
            operation->popOperation();
        }
        if(dependencyOperation == operation)
        {
            CThreadOperationPool::popOperation(E_THREAD_OPERATION_QUEUE_MAIN);
        }
    }
    
    if(!CThreadOperationPool::isQueueEmpty(E_THREAD_OPERATION_QUEUE_BACKGROUND))
    {
        CSharedThreadOperation operation = CThreadOperationPool::nextOperation(E_THREAD_OPERATION_QUEUE_BACKGROUND);
        if(operation == nullptr)
        {
            return;
        }
        CSharedThreadOperation dependencyOperation = operation->nextOperation();
        if(dependencyOperation->m_operationQueue == E_THREAD_OPERATION_QUEUE_MAIN)
        {
            if(!dependencyOperation->getIsCanceled())
            {
                dependencyOperation->m_executionBlock();
            }
            operation->popOperation();
        }
    }
}

CSharedThreadOperation CThreadOperationPool::nextOperation(E_THREAD_OPERATION_QUEUE operationQueue)
{
    std::unique_lock<std::mutex> mutexLock(m_mutex);
    CSharedThreadOperation operation = m_operations.empty() ? nullptr : m_operations.at(operationQueue).front();
    mutexLock.unlock();
    
    return operation;
}

void CThreadOperationPool::popOperation(E_THREAD_OPERATION_QUEUE operationQueue)
{
    std::unique_lock<std::mutex> mutexLock(m_mutex);
    m_operations.at(E_THREAD_OPERATION_QUEUE_MAIN).pop();
    mutexLock.unlock();
}

bool CThreadOperationPool::isQueueEmpty(E_THREAD_OPERATION_QUEUE operationQueue)
{
    std::unique_lock<std::mutex> mutexLock(m_mutex);
    bool isQueueEmpty = m_operations.at(operationQueue).empty();
    mutexLock.unlock();
    return isQueueEmpty;
}

