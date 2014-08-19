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
    m_operations.at(operationQueue).push(operation);
}

void CThreadOperationPool::updateThread(void)
{
    while (m_isRunning)
    {
        if(!m_operations.at(E_THREAD_OPERATION_QUEUE_BACKGROUND).empty())
        {
            CSharedThreadOperation operation = CThreadOperationPool::nextOperation(E_THREAD_OPERATION_QUEUE_BACKGROUND);
            if(operation == nullptr)
            {
                continue;
            }
            CSharedThreadOperation dependencyOperation = operation->nextOperation();
            if(dependencyOperation->m_operationQueue == E_THREAD_OPERATION_QUEUE_BACKGROUND)
            {
                dependencyOperation->m_executionBlock();
                operation->popOperation();
            }
            if(dependencyOperation == operation)
            {
                CThreadOperationPool::popOperation(E_THREAD_OPERATION_QUEUE_BACKGROUND);
            }
        }
        
        if(!m_operations.at(E_THREAD_OPERATION_QUEUE_MAIN).empty())
        {
            CSharedThreadOperation operation = m_operations.at(E_THREAD_OPERATION_QUEUE_MAIN).front();
            if(operation == nullptr)
            {
                continue;
            }
            CSharedThreadOperation dependencyOperation = operation->nextOperation();
            if(dependencyOperation->m_operationQueue == E_THREAD_OPERATION_QUEUE_BACKGROUND)
            {
                dependencyOperation->m_executionBlock();
                operation->popOperation();
            }
        }
    }
}

void CThreadOperationPool::update(void)
{
    if(!m_operations.at(E_THREAD_OPERATION_QUEUE_MAIN).empty())
    {
        CSharedThreadOperation operation = CThreadOperationPool::nextOperation(E_THREAD_OPERATION_QUEUE_MAIN);
        if(operation == nullptr)
        {
            return;
        }
        CSharedThreadOperation dependencyOperation = operation->nextOperation();
        if(dependencyOperation->m_operationQueue == E_THREAD_OPERATION_QUEUE_MAIN)
        {
            dependencyOperation->m_executionBlock();
            operation->popOperation();
        }
        if(dependencyOperation == operation)
        {
            CThreadOperationPool::popOperation(E_THREAD_OPERATION_QUEUE_MAIN);
        }
    }
    
    if(!m_operations.at(E_THREAD_OPERATION_QUEUE_BACKGROUND).empty())
    {
        CSharedThreadOperation operation = m_operations.at(E_THREAD_OPERATION_QUEUE_BACKGROUND).front();
        if(operation == nullptr)
        {
            return;
        }
        CSharedThreadOperation dependencyOperation = operation->nextOperation();
        if(dependencyOperation->m_operationQueue == E_THREAD_OPERATION_QUEUE_MAIN)
        {
            dependencyOperation->m_executionBlock();
            operation->popOperation();
        }
    }
}

CSharedThreadOperation CThreadOperationPool::nextOperation(E_THREAD_OPERATION_QUEUE operationQueue)
{
    m_mutex.lock();
    CSharedThreadOperation operation = m_operations.at(E_THREAD_OPERATION_QUEUE_MAIN).front();
    m_mutex.unlock();
    return operation;
}

void CThreadOperationPool::popOperation(E_THREAD_OPERATION_QUEUE operationQueue)
{
    m_mutex.lock();
    m_operations.at(E_THREAD_OPERATION_QUEUE_MAIN).pop();
    m_mutex.unlock();
}

