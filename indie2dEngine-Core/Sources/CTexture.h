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
    
public:
    CTextureData(ui32 width,
                 ui32 height,
                 ui8* data,
                 GLenum format,
                 ui32 bpp,
                 ui32 mips,
                 bool isCompressed);
    
    ~CTextureData(void);
    
    ui32 getWidth(void) const;
    ui32 getHeight(void) const;
    
    const ui8* getData(void) const;
    
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
    ui32 m_handle;
    
    mutable ui32 m_settedWrapMode;
    ui32 m_presettedWrapMode;
    
    void onResourceDataSerialized(ISharedResourceDataRef resourceData,
                                  E_RESOURCE_DATA_STATUS status);
    
    void onResourceDataCommited(ISharedResourceDataRef resourceData,
                                E_RESOURCE_DATA_STATUS status);
    
public:
    
    CTexture(const std::string& _guid);
    ~CTexture(void);
    
    std::shared_ptr<CTextureHeader> Get_Header(void) const;
    ui32 Get_Handle(void) const;
    ui32 Get_Width(void) const;
    ui32 Get_Height(void) const;
    ui32 Get_WrapMode(void) const;
    
    void Set_Header(const std::shared_ptr<CTextureHeader>& _header);
    void Set_Handle(ui32 _handle);
    void Set_WrapMode(ui32 _wrapMode);
    
    void Bind(void) const;
    void Unbind(void) const;
};

#endif
