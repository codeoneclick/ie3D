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
IResource(E_RESOURCE_TYPE_TEXTURE, _guid),
m_handle(0),
m_header(nullptr),
m_wrap(GL_REPEAT)
{
    
}

CTexture::~CTexture(void)
{
    glDeleteTextures(1, &m_handle);
}

void CTexture::Bind(void) const
{
    if((m_status & E_RESOURCE_STATUS_LOADED) && (m_status & E_RESOURCE_STATUS_COMMITED))
    {
        glBindTexture(GL_TEXTURE_2D, m_handle);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_wrap);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_wrap);
    }
}

void CTexture::Unbind(void) const
{
    if((m_status & E_RESOURCE_STATUS_LOADED) && (m_status & E_RESOURCE_STATUS_COMMITED))
    {
        glBindTexture(GL_TEXTURE_2D, NULL);
    }
}
