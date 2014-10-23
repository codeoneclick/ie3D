//
//  CTextureCommiter_PVR.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/17/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CTextureCommiter_PVR.h"
#include "CTexture.h"

CTextureCommiter_PVR::CTextureCommiter_PVR(const std::string& guid, ISharedResourceRef resource) :
IResourceCommiter(guid, resource)
{
    
}

CTextureCommiter_PVR::~CTextureCommiter_PVR(void)
{
    
}

void CTextureCommiter_PVR::commit(void)
{
    m_status = E_COMMITER_STATUS_INPROGRESS;
    assert(m_resource != nullptr);
    CSharedTexture texture = std::static_pointer_cast<CTexture>(m_resource);
    
    ui32 textureId = 0;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    i32 width = texture->getWidth();
    i32 height = texture->getHeight();
    const ui8* data = texture->getData();
    
	for (ui32 mip = 0; mip < texture->getNumMips() && width > 0 && height > 0; ++mip)
	{
		GLsizei size = MAX_VALUE(32, static_cast<i32>(width) * static_cast<i32>(height) * texture->getBPP() / 8);
		texture->isCompressed() ?
        glCompressedTexImage2D(GL_TEXTURE_2D, mip, texture->getFormat(), width, height, 0, size, data) :
        glTexImage2D(GL_TEXTURE_2D, mip, texture->getFormat(), width, height, 0, texture->getFormat(), GL_UNSIGNED_BYTE, data);
		data += size;
		width >>= 1; height >>= 1;
	}
    
    GLenum error = glGetError();
    assert(error == GL_NO_ERROR);
    
    m_status = E_COMMITER_STATUS_SUCCESS;
    IResourceCommiter::onResourceDataCommitFinished(std::make_shared<CTextureData>(textureId));
}