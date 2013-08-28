//
//  CShaderLoader.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/17/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CShaderLoader.h"
#include "CShaderLoadingOperation.h"
#include "CShader.h"

CShaderLoader::CShaderLoader(void)
{
    
}

CShaderLoader::~CShaderLoader(void)
{
    
}

std::shared_ptr<CShader> CShaderLoader::StartLoadOperation(const std::string &_vsFilename, const std::string &_fsFilename)
{
    std::string guid = std::string().append(_vsFilename).append(_fsFilename);
    std::shared_ptr<CShader> resource = nullptr;
    if(m_resourceContainer.find(guid) != m_resourceContainer.end())
    {
        resource = std::static_pointer_cast<CShader>(m_resourceContainer.find(guid)->second);
    }
    else
    {
        resource = std::make_shared<CShader>(guid);
        std::shared_ptr<IResourceLoadingOperation> operation = std::make_shared<CShaderLoadingOperation>(_vsFilename, _fsFilename, resource);
        m_resourceContainer.insert(std::make_pair(guid, resource));
        m_operationsQueue.insert(std::make_pair(guid, operation));
    }
    return resource;
}