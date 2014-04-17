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

CTextureSerializer_PVR::CTextureSerializer_PVR(const std::string& filename,
                                               ISharedResourceRef resource) :
IResourceSerializer(filename, resource),
m_filename(filename)
{
    
}

CTextureSerializer_PVR::~CTextureSerializer_PVR(void)
{
    
}

void CTextureSerializer_PVR::serialize(void)
{
    IResourceSerializer::onResourceDataSerializationStatusChanged(nullptr, E_RESOURCE_DATA_STATUS_STARTED);
    assert(m_resource != nullptr);
    m_status = E_SERIALIZER_STATUS_INPROGRESS;
    
    std::shared_ptr<std::istream> filestream = IResourceSerializer::openStream(m_filename);
    
#if defined(__NDK__)
    i64 size = static_cast<std::memstream*>(filestream)->size();
#else
    filestream->seekg(0, std::ios::end);
    i64 size = filestream->tellg();
    filestream->seekg(0, std::ios::beg);
#endif
    
	ui8* sourcedata = new ui8[size];
	filestream->read((char*)sourcedata, static_cast<i32>(size));
    IResourceSerializer::closeStream(filestream);
    
    std::shared_ptr<CTexture> texture = std::static_pointer_cast<CTexture>(m_resource);
    ui32 bpp; GLenum format; bool isCompressed;
    
    CSharedTextureData textureData = nullptr;
    
	if(*(PVRTuint32*)sourcedata != PVRTEX3_IDENT)
	{
        PVR_Texture_Header* header;
		header = (PVR_Texture_Header*)sourcedata;
        switch (header->dwpfFlags & PVRTEX_PIXELTYPE)
        {
            case OGL_PVRTC2:
                if(header->dwAlphaBitMask)
                {
                    bpp = 2;
                    format = GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG;
                    isCompressed = true;
                }
                else
                {
                    bpp = 2;
                    format = GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG;
                    isCompressed = true;
                }
                break;
            case OGL_PVRTC4:
                if(header->dwAlphaBitMask)
                {
                    bpp = 4;
                    format = GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG;
                    isCompressed = true;
                }
                else
                {
                    bpp = 4;
                    format = GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG;
                    isCompressed = true;
                }
                break;
			case OGL_RGBA_8888:
                {
                    bpp = 32;
                    format = GL_RGBA;
                    isCompressed = false;
                }
				break;
            default:
                {
                    m_status = E_SERIALIZER_STATUS_FAILURE;
                    return;
                }
                break;
        }
        
        ui8* texturedata = new ui8[size - header->dwHeaderSize];
        memcpy(texturedata, sourcedata + header->dwHeaderSize, static_cast<ui32>(size) - header->dwHeaderSize);
        textureData = std::make_shared<CTextureData>(header->dwWidth,
                                                     header->dwHeight,
                                                     texturedata,
                                                     format,
                                                     bpp,
                                                     header->dwMipMapCount,
                                                     isCompressed);
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
                    bpp = 2;
                    format = GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG;
                    isCompressed = true;
				}
                    break;
                case 1:
				{
                    bpp = 2;
                    format = GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG;
                    isCompressed = true;
				}
                    break;
                case 2:
				{
                    bpp = 4;
                    format = GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG;
                    isCompressed = true;
				}
                    break;
                case 3:
				{
                    bpp = 4;
                    format = GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG;
                    isCompressed = true;
				}
                    break;
                default:
				{
					m_status = E_SERIALIZER_STATUS_FAILURE;
                    return;
				}
                    break;
			}
		}
		else
		{
			EPVRTVariableType channelType = (EPVRTVariableType)header->u32ChannelType;
			if(channelType == ePVRTVarTypeUnsignedByteNorm)
			{
                bpp = 32;
                format = GL_RGBA;
                isCompressed = false;
			}
			else
			{
				m_status = E_SERIALIZER_STATUS_FAILURE;
			}
		}
        
        ui8* texturedata = new ui8[size - (PVRTEX3_HEADERSIZE + header->u32MetaDataSize)];
        memcpy(texturedata, sourcedata + (PVRTEX3_HEADERSIZE + header->u32MetaDataSize), static_cast<ui32>(size) - (PVRTEX3_HEADERSIZE + header->u32MetaDataSize));
        
        textureData = std::make_shared<CTextureData>(header->u32Width,
                                                     header->u32Height,
                                                     texturedata,
                                                     format,
                                                     bpp,
                                                     header->u32MIPMapCount,
                                                     isCompressed);
    }
    IResourceSerializer::onResourceDataSerializationStatusChanged(textureData, E_RESOURCE_DATA_STATUS_PROGRESS);
    m_status = E_SERIALIZER_STATUS_SUCCESS;
    IResourceSerializer::onResourceDataSerializationStatusChanged(nullptr, E_RESOURCE_DATA_STATUS_FINISHED);
    delete[] sourcedata;
}