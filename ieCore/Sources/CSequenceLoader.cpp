//
//  CSequenceLoader.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 8/13/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CSequenceLoader.h"
#include "CSequence.h"
#include "CSequenceLoadingOperation.h"

CSequenceLoader::CSequenceLoader(void)
{
    
}

CSequenceLoader::~CSequenceLoader(void)
{
    
}

std::shared_ptr<CSequence> CSequenceLoader::StartLoadOperation(const std::string &_filename)
{
    std::string guid = _filename;
    std::shared_ptr<CSequence> resource = nullptr;
    if(m_resourceContainer.find(guid) != m_resourceContainer.end())
    {
        resource = std::static_pointer_cast<CSequence>(m_resourceContainer.find(guid)->second);
    }
    else
    {
        resource = std::make_shared<CSequence>(guid);
        std::shared_ptr<IResourceLoadingOperation> operation = std::make_shared<CSequenceLoadingOperation>(_filename, resource);
        m_resourceContainer.insert(std::make_pair(guid, resource));
        m_operationsQueue.insert(std::make_pair(guid, operation));
    }
    return resource;
}