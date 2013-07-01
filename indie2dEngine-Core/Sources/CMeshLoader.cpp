//
//  CMeshLoader.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/26/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CMeshLoader.h"
#include "CMesh.h"
#include "CMeshLoadingOperation.h"

CMeshLoader::CMeshLoader(void)
{
    
}

CMeshLoader::~CMeshLoader(void)
{
    
}

std::shared_ptr<CMesh> CMeshLoader::StartLoadOperation(const std::string &_filename)
{
    std::string guid = _filename;
    std::shared_ptr<CMesh> resource = nullptr;
    if(m_resourceContainer.find(guid) != m_resourceContainer.end())
    {
        resource = std::static_pointer_cast<CMesh>(m_resourceContainer.find(guid)->second);
    }
    else
    {
        resource = std::make_shared<CMesh>(guid);
        std::shared_ptr<IResourceLoadingOperation> operation = std::make_shared<CMeshLoadingOperation>(_filename, resource);
        m_resourceContainer.insert(std::make_pair(guid, resource));
        m_operationsQueue.insert(std::make_pair(guid, operation));
    }
    return resource;
}