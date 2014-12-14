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

class CTexture : public IResource
{
private:
    
protected:
    
    CSharedTextureData m_textureData;
    ui32 m_textureId;
    
    mutable ui32 m_settedWrapMode;
    ui32 m_presettedWrapMode;
    
    mutable ui32 m_settedMagFilter;
    ui32 m_presettedMagFilter;
    
    mutable ui32 m_settedMinFilter;
    ui32 m_pressetedMinFilter;
    
    void onResourceDataSerializationFinished(ISharedResourceDataRef resourceData);
    
    void onResourceDataCommitFinished(ISharedResourceDataRef resourceData);
    
public:
    
    CTexture(const std::string& guid);
    
    static CSharedTexture constructCustomTexture(const std::string& guid,
                                                 ui32 textureId,
                                                 ui32 width,
                                                 ui32 height);
    
    virtual ~CTexture(void);
    
    ui32 getWidth(void) const;
    ui32 getHeight(void) const;
    
    const ui8* getData(void) const;
    ui32 getTextureId(void) const;
    
    GLenum getFormat(void) const;
    ui32 getBPP(void) const;
    ui32 getNumMips(void) const;
    
    bool isCompressed(void) const;
    
    void setWrapMode(ui32 wrapMode);
    void setMagFilter(ui32 magFilter);
    void setMinFilter(ui32 minFilter);
    
    virtual void bind(void) const;
    virtual void unbind(void) const;
};

#endif
