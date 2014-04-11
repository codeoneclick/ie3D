//
//  CTexture.h
//  gEngine-Core
//
//  Created by Sergey Sergeev on 5/7/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CTexture_h
#define CTexture_h

#include "IResource.h"
#include "HDeclaration.h"

class CTextureData final : public IResourceData
{
private:
    
protected:
    
    ui32 m_textureId;
    ui32 m_width;
    ui32 m_height;
    ui8* m_data;
    GLenum m_format;
    ui32 m_bpp;
    ui32 m_mips;
    bool m_isCompressed;
    
public:
    
    CTextureData(ui32 width,
                 ui32 height,
                 ui8* data,
                 GLenum format,
                 ui32 bpp,
                 ui32 mips,
                 bool isCompressed);
    
    CTextureData(ui32 textureId);
    
    ~CTextureData(void);
    
    ui32 getWidth(void) const;
    ui32 getHeight(void) const;
    
    const ui8* getData(void) const;
    ui32 getTextureId(void) const;
    
    GLenum getFormat(void) const;
    ui32 getBPP(void) const;
    ui32 getMips(void) const;
    
    bool isCompressed(void) const;
};

class CTexture final : public IResource
{
private:
    
protected:
    
    CSharedTextureData m_textureData;
    ui32 m_textureId;
    
    mutable ui32 m_settedWrapMode;
    ui32 m_presettedWrapMode;
    
    void onResourceDataSerialized(ISharedResourceDataRef resourceData,
                                  E_RESOURCE_DATA_STATUS status);
    
    void onResourceDataCommited(ISharedResourceDataRef resourceData,
                                E_RESOURCE_DATA_STATUS status);
    
public:
    
    CTexture(const std::string& guid);
    CTexture(const std::string& guid,
             ui32 textureId,
             ui32 witdh,
             ui32 height);
    
    ~CTexture(void);
    
    ui32 getTextureId(void) const;
    
    ui32 getTextureWidth(void) const;
    ui32 getTextureHeight(void) const;
    
    void setWrapMode(ui32 wrapMode);
    
    void bind(void) const;
    void unbind(void) const;
};

#endif
