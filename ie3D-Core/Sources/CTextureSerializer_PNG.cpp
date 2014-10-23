//
//  CTextureSerializer_PNG.cpp
//  ie3D-Core
//
//  Created by sergey.sergeev on 7/10/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#include "CTextureSerializer_PNG.h"
#include "CTexture.h"
#include <libpng-1.6.9/pngstruct.h>

CTextureSerializer_PNG::CTextureSerializer_PNG(const std::string& filename,
                                               ISharedResourceRef resource) :
IResourceSerializer(filename, resource),
m_filename(filename)
{
    
}

CTextureSerializer_PNG::~CTextureSerializer_PNG(void)
{
    
}

void png_read_fn(png_structp pngstruct, png_bytep data,
                 png_size_t size)
{
    if(pngstruct->io_ptr == NULL)
    {
        return;
    }
    
    std::istream* filestream = (std::istream*)pngstruct->io_ptr;
    filestream->read((char *)data, static_cast<i32>(size));
}

void CTextureSerializer_PNG::serialize(void)
{
    assert(m_resource != nullptr);
    m_status = E_SERIALIZER_STATUS_INPROGRESS;
    
    std::shared_ptr<std::istream> filestream = IResourceSerializer::openStream(m_filename, &m_status);
    
#if defined(__NDK__)
    i64 size = static_cast<std::memstream*>(filestream)->size();
#else

#endif
    
    std::shared_ptr<CTexture> texture = std::static_pointer_cast<CTexture>(m_resource);
    
    png_byte header[8];
    filestream->read((char *)header, 8);
    
    if (png_sig_cmp(header, 0, 8))
    {
        std::cout<<"error: "<<m_filename<<"is not a PNG."<<std::endl;
        m_status = E_SERIALIZER_STATUS_FAILURE;
        return;
    }
    
    png_structp pngstruct = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!pngstruct)
    {
        std::cout<<"error: png_create_read_struct returned 0."<<std::endl;
        m_status = E_SERIALIZER_STATUS_FAILURE;
        return;
    }
    
    png_infop pnginfo = png_create_info_struct(pngstruct);
    if (!pnginfo)
    {
        std::cout<<"error: png_create_info_struct returned 0."<<std::endl;
        png_destroy_read_struct(&pngstruct, (png_infopp)NULL, (png_infopp)NULL);
        m_status = E_SERIALIZER_STATUS_FAILURE;
        return;
    }
    
    png_infop pngendinfo = png_create_info_struct(pngstruct);
    if (!pngendinfo)
    {
        std::cout<<"error: png_create_info_struct returned 0."<<std::endl;
        png_destroy_read_struct(&pngstruct, &pnginfo, (png_infopp) NULL);
        m_status = E_SERIALIZER_STATUS_FAILURE;
        return;
    }
    
    if (setjmp(png_jmpbuf(pngstruct)))
    {
        std::cout<<"error from libpng."<<std::endl;
        png_destroy_read_struct(&pngstruct, &pnginfo, &pngendinfo);
        m_status = E_SERIALIZER_STATUS_FAILURE;
        return;
    }
    
    png_set_read_fn(pngstruct, filestream.get(), png_read_fn);
    
    filestream->seekg(0, std::ios::beg);
    
    png_read_info(pngstruct, pnginfo);
    
    i32 bitdepth, colortype;
    ui32 width, height, format;
    png_byte* data;
    
    png_get_IHDR(pngstruct, pnginfo, &width, &height, &bitdepth, &colortype,
                 NULL, NULL, NULL);
    format = colortype;
    
    png_read_update_info(pngstruct, pnginfo);
    png_size_t rowbytes = png_get_rowbytes(pngstruct, pnginfo);
    rowbytes += 3 - ((rowbytes - 1) % 4);
    
    data = new png_byte[rowbytes * height * sizeof(png_byte) + 15];
    if (data == nullptr)
    {
        std::cout<<"error: could not allocate memory for PNG image data."<<std::endl;
        png_destroy_read_struct(&pngstruct, &pnginfo, &pngendinfo);
        m_status = E_SERIALIZER_STATUS_FAILURE;
        return;
    }
    
    png_bytep *rowpointers = new png_bytep[height * sizeof(png_bytep)];
    if (rowpointers == nullptr)
    {
        std::cout<<"error: could not allocate memory for PNG row pointers."<<std::endl;
        png_destroy_read_struct(&pngstruct, &pnginfo, &pngendinfo);
        delete[] data;
        m_status = E_SERIALIZER_STATUS_FAILURE;
        return;
    }
    for (ui32 i = 0; i < height; ++i)
    {
        rowpointers[height - 1 - i] = data + i * rowbytes;
    }
    png_read_image(pngstruct, rowpointers);
    png_destroy_read_struct(&pngstruct, &pnginfo, &pngendinfo);
    delete[] rowpointers;
    
    switch (format)
    {
        case PNG_COLOR_TYPE_RGB:
        {
            format = GL_RGB;
        }
            break;
        case PNG_COLOR_TYPE_RGB_ALPHA:
        {
            format = GL_RGBA;
        }
            break;
        default:
            assert(false);
            break;
    }
    
    IResourceSerializer::closeStream(filestream);
    
    CSharedTextureData textureData = std::make_shared<CTextureData>(width,
                                                                    height,
                                                                    data,
                                                                    format,
                                                                    8,
                                                                    0,
                                                                    false);
    
    m_status = E_SERIALIZER_STATUS_SUCCESS;
    IResourceSerializer::onResourceDataSerializationFinished(textureData);
}
