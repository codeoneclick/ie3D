//
//  CTextureCommiter_PVR.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/17/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CTextureCommiter_PVR.h"
#include "CTexture.h"

CTextureCommiter_PVR::CTextureCommiter_PVR(const std::string& _guid, std::shared_ptr<CTextureHeader> _header, std::shared_ptr<IResource> _resource) :
IResourceCommiter(_guid, _resource),
m_header(_header)
{
    
}

CTextureCommiter_PVR::~CTextureCommiter_PVR(void)
{
    
}

void CTextureCommiter_PVR::Commit(void)
{
    m_status = E_COMMITER_STATUS_INPROGRESS;
    assert(m_resource != nullptr);
    assert(m_header != nullptr);
    std::shared_ptr<CTexture> texture = std::static_pointer_cast<CTexture >(m_resource);
    
    ui32 handle = 0;
    glGenTextures(1, &handle);
    glBindTexture(GL_TEXTURE_2D, handle);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    i32 width = m_header->Get_Width();
    i32 height = m_header->Get_Height();
    ui8* data = m_header->_Get_Data();
    
	for (ui32 mip = 0; mip < m_header->Get_NumMips() && width > 0 && height > 0; ++mip)
	{
		GLsizei size = MAX_VALUE(32, static_cast<i32>(width) * static_cast<i32>(height) * m_header->Get_Bpp() / 8);
		m_header->Get_IsCompressed() ? glCompressedTexImage2D(GL_TEXTURE_2D, mip, m_header->Get_Format(), width, height, 0, size, data) :
        glTexImage2D(GL_TEXTURE_2D, mip, m_header->Get_Format(), width, height, 0, m_header->Get_Format(), GL_UNSIGNED_BYTE, data);
		data += size;
		width >>= 1; height >>= 1;
	}
    
    texture->Set_Handle(handle);
    m_status = E_COMMITER_STATUS_SUCCESS;
}