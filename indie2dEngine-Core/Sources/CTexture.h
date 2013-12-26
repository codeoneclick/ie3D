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

class CTextureHeader final
{
private:
    
protected:
    
    friend class CRenderOperationWorldSpace;
    friend class CRenderOperationScreenSpace;
    friend class CTextureSerializer_PVR;
    friend class CTextureCommiter_PVR;
    friend class CTextureLoadingOperation;
    friend class CHeightmapProcessor;
    friend class CTexture;
    
    ui32 m_width;
    ui32 m_height;
    ui8* m_data;
    
    GLenum m_format;
    i32 m_bpp;
	ui32 m_numMips;
    bool m_isCompressed;
    
    inline void _Set_Data(ui8* _data)
    {
        m_data = _data;
    };
    
    inline ui8* _Get_Data(void)
    {
        return m_data;
    };
    
    inline void _Set_Format(GLenum _format)
    {
        m_format = _format;
    };
    
    inline void _Set_Bpp(i32 _bpp)
    {
        m_bpp = _bpp;
    };
    
    inline void _Set_NumMips(ui32 _numMips)
    {
        m_numMips = _numMips;
    };
    
    inline void _Set_IsCompressed(bool _isCompressed)
    {
        m_isCompressed = _isCompressed;
    };
    
    inline void _Set_Width(ui32 _width)
    {
        m_width = _width;
    };
    
    inline void _Set_Height(ui32 _height)
    {
        m_height = _height;
    };
    
public:
    
    CTextureHeader(void);
    ~CTextureHeader(void);
    
    inline ui32 Get_Width(void)
    {
        return m_width;
    };
    
    inline ui32 Get_Height(void)
    {
        return m_height;
    };
    
    inline GLenum Get_Format(void)
    {
        return m_format;
    };
    
    inline i32 Get_Bpp(void)
    {
        return m_bpp;
    };

    inline ui32 Get_NumMips(void)
    {
        return m_numMips;
    };

    inline bool Get_IsCompressed(void)
    {
        return m_isCompressed;
    };
};

class I_RO_Texture
{
private:
    
protected:
    
public:
    
    I_RO_Texture(void) = default;
    ~I_RO_Texture(void) = default;
    
    virtual std::shared_ptr<CTextureHeader> Get_Header(void) const = 0;
    virtual ui32 Get_Handle(void) const = 0;
    virtual ui32 Get_Width(void) const = 0;
    virtual ui32 Get_Height(void) const = 0;
    virtual ui32 Get_WrapMode(void) const = 0;
};

class I_WO_Texture
{
private:
    
protected:
    
public:
    
    I_WO_Texture(void) = default;
    ~I_WO_Texture(void) = default;
    
    virtual void Set_Header(const std::shared_ptr<CTextureHeader>& _header) = 0;
    virtual void Set_Handle(ui32 _handle) = 0;
    virtual void Set_WrapMode(ui32 _wrapMode) = 0;
};

class CTexture final :
public IResource,
public I_RO_Texture,
public I_WO_Texture
{
private:
    
protected:
    
    std::shared_ptr<CTextureHeader> m_header;
    ui32 m_handle;
    
    mutable ui32 m_settedWrapMode;
    ui32 m_presettedWrapMode;
    
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
