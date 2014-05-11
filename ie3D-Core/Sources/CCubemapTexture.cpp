//
//  CCubemapTexture.cpp
//  ie3D-Core
//
//  Created by Sergey Sergeev on 5/6/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#include "CCubemapTexture.h"

CCubemapTexture::CCubemapTexture(CSharedTextureRef xpositive, CSharedTextureRef xnegative,
                                 CSharedTextureRef ypositive, CSharedTextureRef ynegative,
                                 CSharedTextureRef zpositive, CSharedTextureRef znegative) :
CTexture(xpositive->getGuid() +
         xnegative->getGuid() +
         ypositive->getGuid() +
         ynegative->getGuid() +
         zpositive->getGuid() +
         znegative->getGuid()),
m_xpositive(xpositive),
m_xnegative(xnegative),
m_ypositive(ypositive),
m_ynegative(ynegative),
m_zpositive(zpositive),
m_znegative(znegative)
{
    glGenTextures(1, &m_textureId);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureId);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    m_textureData = std::make_shared<CTextureData>(0, 0, nullptr,
                                                   0, 0, 0, false);
}

CCubemapTexture::~CCubemapTexture(void)
{
    
}

CSharedCubemapTexture CCubemapTexture::constructCustomCubemapTexture(CSharedTextureRef xpositive, CSharedTextureRef xnegative,
                                                                     CSharedTextureRef ypositive, CSharedTextureRef ynegative,
                                                                     CSharedTextureRef zpositive, CSharedTextureRef znegative)
{
    CSharedCubemapTexture texture = std::make_shared<CCubemapTexture>(xpositive, xnegative,
                                                                      ypositive, ynegative,
                                                                      zpositive, znegative);
    xpositive->registerLoadingHandler(texture);
    xnegative->registerLoadingHandler(texture);
    ypositive->registerLoadingHandler(texture);
    ynegative->registerLoadingHandler(texture);
    zpositive->registerLoadingHandler(texture);
    znegative->registerLoadingHandler(texture);
    return texture;
}

void CCubemapTexture::onResourceLoaded(ISharedResourceRef resource, bool success)
{
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureId);
    
    CSharedTexture texture = std::static_pointer_cast<CTexture>(resource);
    GLenum face = GL_TEXTURE_CUBE_MAP_POSITIVE_X;
    
    if(resource == m_xpositive)
    {
        face = GL_TEXTURE_CUBE_MAP_POSITIVE_X;
    }
    else if(resource == m_xnegative)
    {
        face = GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
    }
    else if(resource == m_ypositive)
    {
        face = GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
    }
    else if(resource == m_ynegative)
    {
        face = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
    }
    else if(resource == m_zpositive)
    {
        face = GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
    }
    else if(resource == m_znegative)
    {
        face = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;
    }
    
    i32 width = texture->getWidth();
    i32 height = texture->getHeight();
    const ui8* data = texture->getData();
    
	for (ui32 mip = 0; mip < texture->getNumMips() && width > 0 && height > 0; ++mip)
	{
		GLsizei size = MAX_VALUE(32, static_cast<i32>(width) * static_cast<i32>(height) * texture->getBPP() / 8);
		texture->isCompressed() ?
        glCompressedTexImage2D(face, mip, texture->getFormat(), width, height, 0, size, data) :
        glTexImage2D(face, mip, texture->getFormat(), width, height, 0, texture->getFormat(), GL_UNSIGNED_BYTE, data);
		data += size;
		width >>= 1; height >>= 1;
	}
    
    if(m_xpositive->isCommited() && m_xpositive->isLoaded() &&
       m_xnegative->isCommited() && m_xnegative->isLoaded() &&
       m_ypositive->isCommited() && m_ypositive->isLoaded() &&
       m_ynegative->isCommited() && m_ynegative->isLoaded() &&
       m_zpositive->isCommited() && m_zpositive->isLoaded() &&
       m_znegative->isCommited() && m_znegative->isLoaded())
    {
        m_status |= E_RESOURCE_STATUS_LOADED;
        m_status |= E_RESOURCE_STATUS_COMMITED;
    }
}

void CCubemapTexture::bind(void) const
{
    if(IResource::isLoaded() && IResource::isCommited())
    {
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureId);
    }
}

void CCubemapTexture::unbind(void) const
{
    if(IResource::isLoaded() && IResource::isCommited())
    {
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    }
}
