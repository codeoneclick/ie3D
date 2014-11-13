//
//  CResourceLoader.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/17/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CResourceLoader.h"
#include "CTexture.h"
#include "CShader.h"
#include "CMesh.h"
#include "CAnimationSequence.h"
#include "CTextureLoadingOperation.h"
#include "CShaderLoadingOperation.h"
#include "CMeshLoadingOperation.h"
#include "CAnimationSequenceLoadingOperation.h"

CResourceLoader::CResourceLoader(void)
{
    m_isRunning = 1;
    m_thread = std::thread(&CResourceLoader::onThreadUpdate, this);
}

CResourceLoader::~CResourceLoader(void)
{
    m_isRunning = 0;
    m_thread.join();
    m_operationsQueue.clear();
    m_resourceContainer.clear();
}

void CResourceLoader::onThreadUpdate(void)
{
    while (m_isRunning)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        for(const auto& iterator : m_operationsQueue)
        {
            std::shared_ptr<IResourceLoadingOperation> operation = iterator.second;
            if(operation->getStatus() == E_RESOURCE_LOADING_OPERATION_STATUS_UNKNOWN)
            {
                operation->serialize();
            }
        }
        if(m_operationsQueue.empty())
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(128));
        }
    }
}

void CResourceLoader::_OnGameLoopUpdate(f32 deltatime)
{
    auto iterator = m_operationsQueue.begin();
    while(iterator != m_operationsQueue.end())
    {
        std::shared_ptr<IResourceLoadingOperation> operation = iterator->second;
        if(operation->getStatus() == E_RESOURCE_LOADING_OPERATION_STATUS_WAITING)
        {
            operation->commit();
            std::lock_guard<std::mutex> lock(m_mutex);
            m_operationsQueue.erase(iterator++);
        }
        else if(operation->getStatus() == E_RESOURCE_LOADING_OPERATION_STATUS_FAILURE ||
                operation->getStatus() == E_RESOURCE_LOADING_OPERATION_STATUS_SUCCESS)
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

CSharedTexture CResourceLoader::startTextureLoadingOperation(const std::string& filename)
{
    std::string guid = filename;
    CSharedTexture resource = nullptr;
    if(m_resourceContainer.find(guid) != m_resourceContainer.end())
    {
        resource = std::static_pointer_cast<CTexture>(m_resourceContainer.find(guid)->second);
    }
    else
    {
        resource = std::make_shared<CTexture>(guid);
        ISharedResourceLoadingOperation operation = std::make_shared<CTextureLoadingOperation>(filename,
                                                                                               resource);
        m_resourceContainer.insert(std::make_pair(guid, resource));
        m_operationsQueue.insert(std::make_pair(guid, operation));
    }
    return resource;
}

CSharedShader CResourceLoader::startShaderLoadingOperation(const std::string& vsFilename, const std::string& fsFilename)
{
    std::string guid = std::string().append(vsFilename).append(fsFilename);
    CSharedShader resource = nullptr;
    if(m_resourceContainer.find(guid) != m_resourceContainer.end())
    {
        resource = std::static_pointer_cast<CShader>(m_resourceContainer.find(guid)->second);
    }
    else
    {
        resource = std::make_shared<CShader>(guid);
        ISharedResourceLoadingOperation operation = std::make_shared<CShaderLoadingOperation>(vsFilename,
                                                                                              fsFilename,
                                                                                              resource);
        m_resourceContainer.insert(std::make_pair(guid, resource));
        m_operationsQueue.insert(std::make_pair(guid, operation));
    }
    return resource;
}

CSharedMesh CResourceLoader::startMeshLoadingOperation(const std::string& filename)
{
    std::string guid = filename;
    CSharedMesh resource = nullptr;
    if(m_resourceContainer.find(guid) != m_resourceContainer.end())
    {
        resource = std::static_pointer_cast<CMesh>(m_resourceContainer.find(guid)->second);
    }
    else
    {
        resource = std::make_shared<CMesh>(guid);
        ISharedResourceLoadingOperation operation = std::make_shared<CMeshLoadingOperation>(filename,
                                                                                            resource);
        m_resourceContainer.insert(std::make_pair(guid, resource));
        m_operationsQueue.insert(std::make_pair(guid, operation));
    }
    return resource;
}

CSharedAnimationSequence CResourceLoader::startAnimationSequenceLoadingOperation(const std::string& filename)
{
    std::string guid = filename;
    CSharedAnimationSequence resource = nullptr;
    if(m_resourceContainer.find(guid) != m_resourceContainer.end())
    {
        resource = std::static_pointer_cast<CAnimationSequence>(m_resourceContainer.find(guid)->second);
    }
    else
    {
        resource = std::make_shared<CAnimationSequence>(guid);
        ISharedResourceLoadingOperation operation = std::make_shared<CAnimationSequenceLoadingOperation>(filename,
                                                                                                         resource);
        m_resourceContainer.insert(std::make_pair(guid, resource));
        m_operationsQueue.insert(std::make_pair(guid, operation));
    }
    return resource;
}

void CResourceLoader::unloadResource(ISharedResourceRef resource)
{

}
