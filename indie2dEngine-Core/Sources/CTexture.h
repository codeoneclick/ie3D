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
    
    ui32 m_width;
    ui32 m_height;
    ui32 m_handle;
    bool m_isWrap;
    
public:
    
    CTexture(const std::string& _guid);
    ~CTexture(void);
    
    void Link(ui32 _handle, ui32 _width, ui32 _height);
    
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
