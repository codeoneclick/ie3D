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

IResourceLoader::IResourceLoader(void) :
m_atomic(1),
m_thread(&IResourceLoader::_Thread, this)
{
    
}

IResourceLoader::~IResourceLoader(void)
{
    m_atomic = 0;
    m_thread.join();
    m_operationsQueue.clear();
    m_resourceContainer.clear();
}

void IResourceLoader::_Thread(void)
{
    while (m_atomic)
    {
        for(auto iterator : m_operationsQueue)
        {
            std::shared_ptr<IResourceLoadingOperation> operation = iterator.second;
            assert(operation->Get_Status() == E_RESOURCE_LOADING_OPERATION_STATUS_UNKNOWN);
            std::shared_ptr<IResource> resource = operation->Start();
            if(operation->Get_Status() == E_RESOURCE_LOADING_OPERATION_STATUS_SUCCESS)
            {
                
            }
        }
    }
}

void IResourceLoader::_Update(void)
{

}

void IResourceLoader::UnloadResource(std::shared_future<std::shared_ptr<IResource> > _resource)
{

}