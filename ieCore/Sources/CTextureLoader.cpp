//
//  CTextureLoader.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/23/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CTextureLoader.h"
#include "CTexture.h"
#include "CTextureLoadingOperation.h"

CTextureLoader::CTextureLoader(void)
{
    
}

CTextureLoader::~CTextureLoader(void)
{
    
}

std::shared_ptr<CTexture> CTextureLoader::StartLoadOperation(const std::string &_filename)
{
    std::string guid = _filename;
    std::shared_ptr<CTexture> resource = nullptr;
    if(m_resourceContainer.find(guid) != m_resourceContainer.end())
    {
        resource = std::static_pointer_cast<CTexture>(m_resourceContainer.find(guid)->second);
    }
    else
    {
        resource = std::make_shared<CTexture>(guid);
        std::shared_ptr<IResourceLoadingOperation> operation = std::make_shared<CTextureLoadingOperation>(_filename, resource);
        m_resourceContainer.insert(std::make_pair(guid, resource));
        m_operationsQueue.insert(std::make_pair(guid, operation));
    }
    return resource;
}