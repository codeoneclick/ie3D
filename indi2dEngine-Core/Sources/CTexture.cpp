//
//  CTexture.cpp
//  gEngine-Core
//
//  Created by Sergey Sergeev on 5/7/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CTexture.h"

CTexture::CTexture(const std::string& _guid) :
IResource(E_RESOURCE_TYPE_TEXTURE, _guid),
m_handle(0),
m_width(0),
m_height(0),
m_isWrap(false)
{
    
}

CTexture::~CTexture(void)
{
    glDeleteTextures(1, &m_handle);
}

void CTexture::Link(ui32 _handle, ui32 _width, ui32 _height)
{
    m_handle = _handle;
    m_width = _width;
    m_height = _height;
    m_isLinked = true;
}

void CTexture::Bind(void) const
{
    glBindTexture(GL_TEXTURE_2D, m_handle);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_isWrap ? GL_CLAMP_TO_EDGE : GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_isWrap ? GL_CLAMP_TO_EDGE : GL_REPEAT);
}

void CTexture::Unbind(void) const
{
    glBindTexture(GL_TEXTURE_2D, NULL);
}
