//
//  CTexture.cpp
//  gEngine-Core
//
//  Created by Sergey Sergeev on 5/7/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CTexture.h"

CTextureHeader::CTextureHeader(void) :
m_data(nullptr),
m_width(0),
m_height(0)
{
    
}

CTextureHeader::~CTextureHeader(void)
{
    delete[] m_data;
}

CTexture::CTexture(const std::string& _guid) :
IResource(E_RESOURCE_CLASS_TEXTURE, _guid),
m_handle(0),
m_header(nullptr),
m_presettedWrap(GL_REPEAT),
m_settedWrap(0)
{
    
}

CTexture::~CTexture(void)
{
    glDeleteTextures(1, &m_handle);
}

void CTexture::Bind(void) const 
{
    if(IResource::IsLoaded() && IResource::IsCommited())
    {
        glBindTexture(GL_TEXTURE_2D, m_handle);
        if(m_settedWrap == 0 || m_presettedWrap != m_settedWrap)
        {
            m_settedWrap = m_presettedWrap;
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_settedWrap);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_settedWrap);
        }
    }
}

void CTexture::Unbind(void) const
{
    if(IResource::IsLoaded() && IResource::IsCommited())
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}
