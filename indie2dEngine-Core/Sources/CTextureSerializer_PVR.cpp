//
//  CTextureSerializer_PVR.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/16/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CTextureSerializer_PVR.h"
#include "CCommonOS.h"
#include "CTexture.h"
#include <pvrtextool/include/PVRTexture.h>

CTextureSerializer_PVR::CTextureSerializer_PVR(const std::string& _filename, std::shared_ptr<IResource> _resource) :
IResourceSerializer(_filename, _resource),
m_filename(_filename)
{
    
}

CTextureSerializer_PVR::~CTextureSerializer_PVR(void)
{
    
}

void CTextureSerializer_PVR::Serialize(void)
{
    assert(m_resource != nullptr);
    m_status = E_SERIALIZER_STATUS_INPROGRESS;
    
    std::string path(Get_BundlePath());
    std::string filename(path);
    filename.append(m_filename);
    
    std::ifstream filestream;
    filestream.open(filename.c_str());
    if (!filestream.is_open())
    {
        m_status = E_SERIALIZER_STATUS_FAILURE;
        return;
    }
    
    filestream.seekg(0, std::ios::end);
    i32 size = filestream.tellg();
    filestream.seekg(0, std::ios::beg);
	ui8* sourcedata = new ui8[size];
	filestream.read((char*)sourcedata, size);
    filestream.close();
    
    std::shared_ptr<CTexture> texture = std::static_pointer_cast<CTexture >(m_resource);
    
	if(*(PVRTuint32*)sourcedata != PVRTEX3_IDENT)
	{
        PVR_Texture_Header* header;
		header = (PVR_Texture_Header*)sourcedata;
        switch (header->dwpfFlags & PVRTEX_PIXELTYPE)
        {
            case OGL_PVRTC2:
                if(header->dwAlphaBitMask)
                {
                    texture->_Set_Bpp(2);
                    texture->_Set_Format(GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG);
                    texture->_Set_IsCompressed(true);
                }
                else
                {
                    texture->_Set_Bpp(2);
                    texture->_Set_Format(GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG);
                    texture->_Set_IsCompressed(true);
                }
                break;
            case OGL_PVRTC4:
                if(header->dwAlphaBitMask)
                {
                    texture->_Set_Bpp(4);
                    texture->_Set_Format(GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG);
                    texture->_Set_IsCompressed(true);
                }
                else
                {
                    texture->_Set_Bpp(4);
                    texture->_Set_Format(GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG);
                    texture->_Set_IsCompressed(true);
                }
                break;
			case OGL_RGBA_8888:
                {
                    texture->_Set_Bpp(32);
                    texture->_Set_Format(GL_RGBA);
                    texture->_Set_IsCompressed(false);
                }
				break;
            default:
                {
                    m_status = E_SERIALIZER_STATUS_FAILURE;
                }
                break;
        }
        
        ui8* texturedata = new ui8[size - header->dwHeaderSize];
        memcpy(sourcedata + header->dwHeaderSize, texturedata, size - header->dwHeaderSize);
        
        texture->_Set_Width(header->dwWidth);
        texture->_Set_Height(header->dwHeight);
        texture->_Set_Data(texturedata);
        texture->_Set_NumMips(header->dwMipMapCount);
        
        m_status = E_SERIALIZER_STATUS_SUCCESS;
    }
    else
    {
        PVRTextureHeaderV3* header = (PVRTextureHeaderV3*)sourcedata;
        PVRTuint64 pixelFormat = header->u64PixelFormat;
		PVRTuint64 pixelFormatPartHigh = pixelFormat & PVRTEX_PFHIGHMASK;
		if (pixelFormatPartHigh == 0)
		{
			switch (pixelFormat)
			{
                case 0:
				{
                    texture->_Set_Bpp(2);
                    texture->_Set_Format(GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG);
                    texture->_Set_IsCompressed(true);
				}
                    break;
                case 1:
				{
                    texture->_Set_Bpp(2);
                    texture->_Set_Format(GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG);
                    texture->_Set_IsCompressed(true);
				}
                    break;
                case 2:
				{
                    texture->_Set_Bpp(4);
                    texture->_Set_Format(GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG);
                    texture->_Set_IsCompressed(true);
				}
                    break;
                case 3:
				{
                    texture->_Set_Bpp(4);
                    texture->_Set_Format(GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG);
                    texture->_Set_IsCompressed(true);
				}
                    break;
                default:
				{
					m_status = E_SERIALIZER_STATUS_FAILURE;
				}
                    break;
			}
		}
		else
		{
			EPVRTVariableType channelType = (EPVRTVariableType)header->u32ChannelType;
			if(channelType == ePVRTVarTypeUnsignedByteNorm)
			{
                texture->_Set_Bpp(32);
                texture->_Set_Format(GL_RGBA);
                texture->_Set_IsCompressed(false);
			}
			else
			{
				m_status = E_SERIALIZER_STATUS_FAILURE;
			}
		}
        
        ui8* texturedata = new ui8[size - (PVRTEX3_HEADERSIZE + header->u32MetaDataSize)];
        memcpy(sourcedata + (PVRTEX3_HEADERSIZE + header->u32MetaDataSize), texturedata, size - (PVRTEX3_HEADERSIZE + header->u32MetaDataSize));
        
        texture->_Set_Width(header->u32Width);
        texture->_Set_Height(header->u32Height);
        texture->_Set_Data(texturedata);
        texture->_Set_NumMips(header->u32MIPMapCount);
        
        m_status = E_SERIALIZER_STATUS_SUCCESS;
    }
    delete[] sourcedata;
}