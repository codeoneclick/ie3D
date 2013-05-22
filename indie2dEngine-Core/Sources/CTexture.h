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

class CTexture final : public IResource
{
private:
    
protected:
    
    friend class CTextureSerializer_PVR;
    friend class CTextureCommiter_PVR;
    friend class CTextureLoadingOperation;
    
    ui32 m_width;
    ui32 m_height;
    ui8* m_data;
    
    GLenum m_format;
    i32 m_bpp;
	ui32 m_numMips;
    bool m_isCompressed;
    
    ui32 m_handle;
    bool m_isWrap;
    
    inline void _Set_Data(ui8* _data)
    {
        m_data = _data;
    };
    
    inline void _Set_Format(GLenum _format)
    {
        m_format = _format;
    };
    
    inline GLenum _Get_Format(void)
    {
        return m_format;
    };
    
    inline void _Set_Bpp(i32 _bpp)
    {
        m_bpp = _bpp;
    };
    
    inline i32 _Get_Bpp(void)
    {
        return m_bpp;
    };
    
    inline void _Set_NumMips(ui32 _numMips)
    {
        m_numMips = _numMips;
    };
    
    inline ui32 _Get_NumMips(void)
    {
        return m_numMips;
    };
    
    inline void _Set_IsCompressed(bool _isCompressed)
    {
        m_isCompressed = _isCompressed;
    };
    
    inline bool _Get_IsCompressed(void)
    {
        return m_isCompressed;
    };
    
    inline void _Set_Handle(ui32 _handle)
    {
        m_handle = _handle;
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
    
    CTexture(const std::string& _guid);
    ~CTexture(void);
    
    inline const ui32 Get_Handle(void)
    {
        return m_handle;
    };
    
    inline const ui32 Get_Width(void)
    {
        return m_width;
    };
    
    inline const ui32 Get_Height(void)
    {
        return m_height;
    };
    
    inline void Set_IsWrap(bool _value)
    {
        m_isWrap = _value;
    };
    
    void Bind(void) const;
    void Unbind(void) const;
};

#endif
