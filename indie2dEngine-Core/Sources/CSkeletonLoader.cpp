//
//  CSkeletonLoader.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 8/13/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CSkeletonLoader.h"
#include "CSkeleton.h"
#include "CSkeletonLoadingOperation.h"

CSkeletonLoader::CSkeletonLoader(void)
{
    
}

CSkeletonLoader::~CSkeletonLoader(void)
{
    
}

std::shared_ptr<CSkeleton> CSkeletonLoader::StartLoadOperation(const std::string &_filename)
{
    std::string guid = _filename;
    std::shared_ptr<CSkeleton> resource = nullptr;
    if(m_resourceContainer.find(guid) != m_resourceContainer.end())
    {
        resource = std::static_pointer_cast<CSkeleton>(m_resourceContainer.find(guid)->second);
    }
    else
    {
        resource = std::make_shared<CSkeleton>(guid);
        std::shared_ptr<IResourceLoadingOperation> operation = std::make_shared<CSkeletonLoadingOperation>(_filename, resource);
        m_resourceContainer.insert(std::make_pair(guid, resource));
        m_operationsQueue.insert(std::make_pair(guid, operation));
    }
    return resource;
}