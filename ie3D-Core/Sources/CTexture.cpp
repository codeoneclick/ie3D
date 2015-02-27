//
//  CTexture.cpp
//  gEngine-Core
//
//  Created by Sergey Sergeev on 5/7/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CTexture.h"


CTextureData::CTextureData(ui32 width,
                           ui32 height,
                           ui8* data,
                           GLenum format,
                           ui32 bpp,
                           ui32 mips,
                           bool isCompressed) : IResourceData(E_RESOURCE_DATA_CLASS_TEXTURE_DATA),
m_textureId(0),
m_width(width),
m_height(height),
m_data(data),
m_format(format),
m_bpp(bpp),
m_mips(mips),
m_isCompressed(isCompressed)
{
    
}

CTextureData::CTextureData(ui32 textureId) : IResourceData(E_RESOURCE_DATA_CLASS_TEXTURE_DATA),
m_textureId(textureId),
m_data(nullptr)
{
    
}

CTextureData::~CTextureData(void)
{
    delete[] m_data;
}

ui32 CTextureData::getWidth(void) const
{
    return m_width;
}

ui32 CTextureData::getHeight(void) const
{
    return m_height;
}

const ui8* CTextureData::getData(void) const
{
    return m_data;
}

ui32 CTextureData::getTextureId(void) const
{
    return m_textureId;
}

GLenum CTextureData::getFormat(void) const
{
    return m_format;
}

ui32 CTextureData::getBPP(void) const
{
    return m_bpp;
}

ui32 CTextureData::getMips(void) const
{
    return m_mips;
}

bool CTextureData::isCompressed(void) const
{
    return m_isCompressed;
}

CTexture::CTexture(const std::string& guid) : IResource(E_RESOURCE_CLASS_TEXTURE, guid),
m_textureData(nullptr),
m_textureId(0),
m_presettedWrapMode(GL_REPEAT),
m_settedWrapMode(0),
m_presettedMagFilter(GL_NEAREST),
m_settedMagFilter(0),
m_pressetedMinFilter(GL_NEAREST),
m_settedMinFilter(0)
{
    
}

CSharedTexture CTexture::constructCustomTexture(const std::string& guid,
                                             ui32 textureId,
                                             ui32 width,
                                             ui32 height)
{
    CSharedTexture texture = std::make_shared<CTexture>(guid);
    texture->m_textureId = textureId;
    texture->m_textureData = std::make_shared<CTextureData>(width, height, nullptr,
                                                            0, 0, 0, false);
    texture->m_status |= E_RESOURCE_STATUS_LOADED;
    texture->m_status |= E_RESOURCE_STATUS_COMMITED;
    return texture;
}

CTexture::~CTexture(void)
{
    ieDeleteTextures(1, &m_textureId);
}

void CTexture::onResourceDataSerializationFinished(ISharedResourceDataRef resourceData)
{
    assert(resourceData != nullptr);
    switch(resourceData->getResourceDataClass())
    {
        case E_RESOURCE_DATA_CLASS_TEXTURE_DATA:
        {
            m_textureData = std::static_pointer_cast<CTextureData>(resourceData);
            m_status |= E_RESOURCE_STATUS_LOADED;
        }
            break;
        default:
        {
            assert(false);
        }
            break;
    }
}

void CTexture::onResourceDataCommitFinished(ISharedResourceDataRef resourceData)
{
    assert(resourceData != nullptr);
    switch(resourceData->getResourceDataClass())
    {
        case E_RESOURCE_DATA_CLASS_TEXTURE_DATA:
        {
            CSharedTextureData textureData = std::static_pointer_cast<CTextureData>(resourceData);
            m_textureId = textureData->getTextureId();
            assert(m_textureId != 0);
            m_status |= E_RESOURCE_STATUS_COMMITED;
        }
            break;
        default:
        {
            assert(false);
        }
            break;
    }
}

ui32 CTexture::getWidth(void) const
{
    return IResource::isLoaded() ? m_textureData->getWidth() : 0;
}

ui32 CTexture::getHeight(void) const
{
    return IResource::isLoaded() ? m_textureData->getHeight() : 0;
}

const ui8* CTexture::getData(void) const
{
    return IResource::isLoaded() ? m_textureData->getData() : nullptr;
}

ui32 CTexture::getTextureId(void) const
{
    return m_textureId;
}

GLenum CTexture::getFormat(void) const
{
    return IResource::isLoaded() ? m_textureData->getFormat() : 0;
}

ui32 CTexture::getBPP(void) const
{
    return IResource::isLoaded() ? m_textureData->getBPP() : 0;
}

ui32 CTexture::getNumMips(void) const
{
    return IResource::isLoaded() ? m_textureData->getMips() : 0;
}

bool CTexture::isCompressed(void) const
{
    return IResource::isLoaded() ? m_textureData->isCompressed() : false;
}

void CTexture::setWrapMode(ui32 wrapMode)
{
    m_presettedWrapMode = wrapMode;
}

void CTexture::setMagFilter(ui32 magFilter)
{
    m_presettedMagFilter = magFilter;
}

void CTexture::setMinFilter(ui32 minFilter)
{
    m_pressetedMinFilter = minFilter;
}

void CTexture::bind(void) const
{
    if(IResource::isLoaded() && IResource::isCommited())
    {
        ieBindTexture(GL_TEXTURE_2D, m_textureId);
        if(m_settedWrapMode == 0 || m_presettedWrapMode != m_settedWrapMode)
        {
            m_settedWrapMode = m_presettedWrapMode;
            ieTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_settedWrapMode);
            ieTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_settedWrapMode);
        }
        if(m_settedMagFilter == 0 || m_presettedMagFilter != m_settedMagFilter)
        {
            m_settedMagFilter = m_presettedMagFilter;
            ieTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_settedMagFilter);
        }
        if(m_settedMinFilter == 0 || m_pressetedMinFilter != m_settedMinFilter)
        {
            m_settedMinFilter = m_pressetedMinFilter;
            ieTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_settedMinFilter);
        }
    }
}
void CTexture::unbind(void) const
{
    if(IResource::isLoaded() && IResource::isCommited())
    {
        ieBindTexture(GL_TEXTURE_2D, NULL);
    }
}
