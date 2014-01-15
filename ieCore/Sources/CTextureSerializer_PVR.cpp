//
//  CTextureSerializer_PVR.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/16/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CTextureSerializer_PVR.h"
#include "CTexture.h"
#include "PVRTTexture.h"

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
    
    std::istream* filestream = IResourceSerializer::_LoadData(m_filename);
    
#if defined(__NDK__)
    i64 size = static_cast<std::memstream*>(filestream)->size();
#else
    filestream->seekg(0, std::ios::end);
    i64 size = filestream->tellg();
    filestream->seekg(0, std::ios::beg);
#endif
    
	ui8* sourcedata = new ui8[size];
	filestream->read((char*)sourcedata, static_cast<i32>(size));
    IResourceSerializer::_FreeData(filestream);
    
    std::shared_ptr<CTexture> texture = std::static_pointer_cast<CTexture >(m_resource);
    std::shared_ptr<CTextureHeader> textureheader = std::make_shared<CTextureHeader>();
    
	if(*(PVRTuint32*)sourcedata != PVRTEX3_IDENT)
	{
        PVR_Texture_Header* header;
		header = (PVR_Texture_Header*)sourcedata;
        switch (header->dwpfFlags & PVRTEX_PIXELTYPE)
        {
            case OGL_PVRTC2:
                if(header->dwAlphaBitMask)
                {
                    textureheader->_Set_Bpp(2);
                    textureheader->_Set_Format(GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG);
                    textureheader->_Set_IsCompressed(true);
                }
                else
                {
                    textureheader->_Set_Bpp(2);
                    textureheader->_Set_Format(GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG);
                    textureheader->_Set_IsCompressed(true);
                }
                break;
            case OGL_PVRTC4:
                if(header->dwAlphaBitMask)
                {
                    textureheader->_Set_Bpp(4);
                    textureheader->_Set_Format(GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG);
                    textureheader->_Set_IsCompressed(true);
                }
                else
                {
                    textureheader->_Set_Bpp(4);
                    textureheader->_Set_Format(GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG);
                    textureheader->_Set_IsCompressed(true);
                }
                break;
			case OGL_RGBA_8888:
                {
                    textureheader->_Set_Bpp(32);
                    textureheader->_Set_Format(GL_RGBA);
                    textureheader->_Set_IsCompressed(false);
                }
				break;
            default:
                {
                    m_status = E_SERIALIZER_STATUS_FAILURE;
                }
                break;
        }
        
        ui8* texturedata = new ui8[size - header->dwHeaderSize];
        memcpy(texturedata, sourcedata + header->dwHeaderSize, static_cast<ui32>(size) - header->dwHeaderSize);
        
        textureheader->_Set_Width(header->dwWidth);
        textureheader->_Set_Height(header->dwHeight);
        textureheader->_Set_Data(texturedata);
        textureheader->_Set_NumMips(header->dwMipMapCount);
        texture->Set_Header(textureheader);
        
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
                    textureheader->_Set_Bpp(2);
                    textureheader->_Set_Format(GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG);
                    textureheader->_Set_IsCompressed(true);
				}
                    break;
                case 1:
				{
                    textureheader->_Set_Bpp(2);
                    textureheader->_Set_Format(GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG);
                    textureheader->_Set_IsCompressed(true);
				}
                    break;
                case 2:
				{
                    textureheader->_Set_Bpp(4);
                    textureheader->_Set_Format(GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG);
                    textureheader->_Set_IsCompressed(true);
				}
                    break;
                case 3:
				{
                    textureheader->_Set_Bpp(4);
                    textureheader->_Set_Format(GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG);
                    textureheader->_Set_IsCompressed(true);
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
                textureheader->_Set_Bpp(32);
                textureheader->_Set_Format(GL_RGBA);
                textureheader->_Set_IsCompressed(false);
			}
			else
			{
				m_status = E_SERIALIZER_STATUS_FAILURE;
			}
		}
        
        ui8* texturedata = new ui8[size - (PVRTEX3_HEADERSIZE + header->u32MetaDataSize)];
        memcpy(texturedata, sourcedata + (PVRTEX3_HEADERSIZE + header->u32MetaDataSize), static_cast<ui32>(size) - (PVRTEX3_HEADERSIZE + header->u32MetaDataSize));
        
        textureheader->_Set_Width(header->u32Width);
        textureheader->_Set_Height(header->u32Height);
        textureheader->_Set_Data(texturedata);
        textureheader->_Set_NumMips(header->u32MIPMapCount);
        texture->Set_Header(textureheader);
        
        m_status = E_SERIALIZER_STATUS_SUCCESS;
    }
    delete[] sourcedata;
}