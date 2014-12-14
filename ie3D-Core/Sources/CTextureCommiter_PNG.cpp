//
//  CTextureCommiter_PNG.cpp
//  ie3D-Core
//
//  Created by sergey.sergeev on 7/10/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#include "CTextureCommiter_PNG.h"
#include "CTexture.h"

CTextureCommiter_PNG::CTextureCommiter_PNG(const std::string& guid, ISharedResourceRef resource) :
IResourceCommiter(guid, resource)
{
    
}

CTextureCommiter_PNG::~CTextureCommiter_PNG(void)
{
    
}

void CTextureCommiter_PNG::commit(void)
{
    m_status = E_COMMITER_STATUS_INPROGRESS;
    assert(m_resource != nullptr);
    CSharedTexture texture = std::static_pointer_cast<CTexture>(m_resource);
    
    ui32 textureId = 0;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    
    i32 width = texture->getWidth();
    i32 height = texture->getHeight();
    i32 format = texture->getFormat();
    const ui8* data = texture->getData();
    
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0,
                 format, GL_UNSIGNED_BYTE, (GLvoid*)&data[0]);
    glGenerateMipmap(GL_TEXTURE_2D);
    
    GLenum error = glGetError();
    assert(error == GL_NO_ERROR);
    
    m_status = E_COMMITER_STATUS_SUCCESS;
    
    IResourceCommiter::onResourceDataCommitFinished(std::make_shared<CTextureData>(textureId));
}