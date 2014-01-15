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
m_presettedWrapMode(GL_REPEAT),
m_settedWrapMode(0)
{
    
}

CTexture::~CTexture(void)
{
    glDeleteTextures(1, &m_handle);
}

std::shared_ptr<CTextureHeader> CTexture::Get_Header(void) const
{
    assert(m_header != nullptr);
    return m_header;
}

ui32 CTexture::Get_Handle(void) const
{
    return m_handle;
}

ui32 CTexture::Get_Width(void) const
{
    assert(m_header != nullptr);
    return m_header->Get_Width();
}

ui32 CTexture::Get_Height(void) const
{
    assert(m_header != nullptr);
    return m_header->Get_Height();
}

ui32 CTexture::Get_WrapMode(void) const
{
    return m_settedWrapMode;
}

void CTexture::Set_Header(const std::shared_ptr<CTextureHeader>& _header)
{
    assert(_header != nullptr);
    m_header = _header;
    m_status |= E_RESOURCE_STATUS_LOADED;
}

void CTexture::Set_Handle(ui32 _handle)
{
    m_handle = _handle;
    m_status |= E_RESOURCE_STATUS_COMMITED;
}

void CTexture::Set_WrapMode(ui32 _wrapMode)
{
    m_presettedWrapMode = _wrapMode;
}

void CTexture::Bind(void) const
{
    if(IResource::IsLoaded() && IResource::IsCommited())
    {
        glBindTexture(GL_TEXTURE_2D, m_handle);
        if(m_settedWrapMode == 0 || m_presettedWrapMode != m_settedWrapMode)
        {
            m_settedWrapMode = m_presettedWrapMode;
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_settedWrapMode);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_settedWrapMode);
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
