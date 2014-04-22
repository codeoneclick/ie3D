//
//  IScreenSpaceTextureAccessor.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/28/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "IScreenSpaceTextureAccessor.h"
#include "CTexture.h"

IScreenSpaceTextureAccessor::IScreenSpaceTextureAccessor(void)
{

}

IScreenSpaceTextureAccessor::~IScreenSpaceTextureAccessor(void)
{
    m_customTextures.clear();
}


void IScreenSpaceTextureAccessor::addCustomTexture(CSharedTextureRef texture)
{
    m_customTextures.insert(std::make_pair(texture->getGuid(), texture));
}

CSharedTexture IScreenSpaceTextureAccessor::getCustomTexture(const std::string& guid)
{
    CSharedTexture texture = nullptr;
    const auto& iterator = m_customTextures.find(guid);
    if(iterator != m_customTextures.end())
    {
        texture = iterator->second;
    }
    return texture;
}