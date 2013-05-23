//
//  IResourceLoader.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/17/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "IResourceLoader.h"
#include "IResource.h"
#include "IResourceLoadingOperation.h"
#include "CGameLoopExecutor.h"

IResourceLoader::IResourceLoader(void) :
m_isRunning(1),
m_thread(&IResourceLoader::_Thread, this)
{
    ConnectToGameLoop(std::shared_ptr<IResourceLoader>(this));
}

IResourceLoader::~IResourceLoader(void)
{
    m_isRunning = 0;
    m_thread.join();
    m_operationsQueue.clear();
    m_resourceContainer.clear();
    DisconnectFromGameLoop(std::shared_ptr<IResourceLoader>(this));
}

void IResourceLoader::_Thread(void)
{
    while (m_isRunning)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        for(auto iterator : m_operationsQueue)
        {
            std::shared_ptr<IResourceLoadingOperation> operation = iterator.second;
            if(operation->Get_Status() == E_RESOURCE_LOADING_OPERATION_STATUS_UNKNOWN)
            {
                operation->Serialize();
            }
        }
        if(m_operationsQueue.empty())
        {
            std::this_thread::yield();
        }
    }
}

void IResourceLoader::_OnGameLoopUpdate(f32 _deltatime)
{
    auto iterator = m_operationsQueue.begin();
    while(iterator != m_operationsQueue.end())
    {
        std::shared_ptr<IResourceLoadingOperation> operation = iterator->second;
        if(operation->Get_Status() == E_RESOURCE_LOADING_OPERATION_STATUS_WAITING)
        {
            operation->Commit();
            std::lock_guard<std::mutex> lock(m_mutex);
            m_operationsQueue.erase(iterator++);
        }
        else if(operation->Get_Status() == E_RESOURCE_LOADING_OPERATION_STATUS_FAILURE ||
                operation->Get_Status() == E_RESOURCE_LOADING_OPERATION_STATUS_SUCCESS)
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_operationsQueue.erase(iterator++);
        }
        else
        {
            ++iterator;
        }
    }
}

void IResourceLoader::UnloadResource(std::shared_ptr<IResource> _resource)
{

}
