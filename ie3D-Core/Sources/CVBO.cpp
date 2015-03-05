//
//  CVBO.cpp
//  ie3D-Core
//
//  Created by sergey.sergeev on 3/5/15.
//  Copyright (c) 2015 Sergey Sergeev. All rights reserved.
//

#include "CVBO.h"
#include "CShader.h"

namespace ie
{
    vbo::vbo(vbo::E_DECL decl, ui32 sizeToAllocate, GLenum mode) : IResourceData(E_RESOURCE_DATA_CLASS_VERTEX_BUFFER_DATA),
    m_allocatedSize(sizeToAllocate),
    m_usedSize(0),
    m_declType(decl),
    m_mode(mode)
    {
        assert(m_allocatedSize > 0);
        
        ieGenBuffers(1, &m_vramAddress);
        ieBindBuffer(GL_ARRAY_BUFFER, m_vramAddress);
        
        switch (m_declType)
        {
            case E_DECL_POS_3F32_TEX_UI32_NORM_UI32_COL_4UI8_EXT_4UI8:
            {
                m_data = new POS_3F32_TEX_UI32_NORM_UI32_COL_4UI8_EXT_4UI8[m_allocatedSize];
                m_declSize = sizeof(POS_3F32_TEX_UI32_NORM_UI32_COL_4UI8_EXT_4UI8);
            }
                break;
            case E_DECL_POS_2I16_H_F32_TEX_UI32_NORM_UI32:
            {
                m_data = new POS_2I16_H_F32_TEX_UI32_NORM_UI32[m_allocatedSize];
                m_declSize = sizeof(POS_2I16_H_F32_TEX_UI32_NORM_UI32);
            }
                break;
            default:
                assert(false);
                break;
        }
        memset(m_data, 0x0, m_declSize * m_allocatedSize);
        ieBufferData(GL_ARRAY_BUFFER, m_declSize * m_allocatedSize, m_data, m_mode);
    }
    
    vbo::~vbo(void)
    {
        ieDeleteBuffers(1, &m_declSize);
        delete[] m_data;
    }
    
    ui32 vbo::getAllocatedSize(void) const
    {
        return m_allocatedSize;
    }
    
    ui32 vbo::getUsedSize(void) const
    {
        return m_usedSize;
    }
    
    vbo::DECL* vbo::lock(void) const
    {
        assert(m_data);
        return m_data;
    }
    
    void vbo::unlock(ui32 sizeToUse)
    {
        assert(m_data != nullptr);
        assert(m_allocatedSize != 0);
        m_usedSize = sizeToUse > 0 && sizeToUse < m_allocatedSize ? sizeToUse : m_allocatedSize;
        ieBindBuffer(GL_ARRAY_BUFFER, m_vramAddress);
        ieBufferSubData(GL_ARRAY_BUFFER, 0, m_declSize * m_usedSize, m_data);
    }
    
    void vbo::bind(i8 attr, GLenum attrType, i32 attrSize, i32 declSize, bool normalized, GLvoid* ptr)
    {
        if(attr >= 0)
        {
            ieEnableVertexAttribArray(attr);
            ieVertexAttribPointer(attr, attrSize, attrType, normalized, declSize, ptr);
        }
    }
    
    void vbo::bind_POS_3F32_TEX_UI32_NORM_UI32_COL_4UI8_EXT_4UI8(const std::array<i8, E_SHADER_ATTRIBUTE_MAX>& attributes,
                                                                 ui32 declSize)
    {
        vbo::bind(attributes[CShader::E_ATTRIBUTE_POSITION_3F], GL_FLOAT, 3, declSize, false,
                  (GLvoid*)offsetof(POS_3F32_TEX_UI32_NORM_UI32_COL_4UI8_EXT_4UI8, m_position));
        
        vbo::bind(attributes[CShader::E_ATTRIBUTE_TEXCOORD_UI32], GL_UNSIGNED_SHORT, 2, declSize, true,
                  (GLvoid*)offsetof(POS_3F32_TEX_UI32_NORM_UI32_COL_4UI8_EXT_4UI8, m_texcoord));
        
        vbo::bind(attributes[CShader::E_ATTRIBUTE_NORMAL_UI32], GL_BYTE, 4, declSize, true,
                  (GLvoid*)offsetof(POS_3F32_TEX_UI32_NORM_UI32_COL_4UI8_EXT_4UI8, m_normal));
        
        vbo::bind(attributes[CShader::E_ATTRIBUTE_COLOR_4UI8], GL_UNSIGNED_BYTE, 4, declSize, true,
                  (GLvoid*)offsetof(POS_3F32_TEX_UI32_NORM_UI32_COL_4UI8_EXT_4UI8, m_color));
        
        vbo::bind(attributes[CShader::E_ATTRIBUTE_EXTRA_4UI8], GL_UNSIGNED_BYTE, 4, declSize, true,
                  (GLvoid*)offsetof(POS_3F32_TEX_UI32_NORM_UI32_COL_4UI8_EXT_4UI8, m_extra));
    }
    
    void vbo::bind_POS_2I16_H_F32_TEX_UI32_NORM_UI32(const std::array<i8, E_SHADER_ATTRIBUTE_MAX>& attributes,
                                                     ui32 declSize)
    {
        vbo::bind(attributes[CShader::E_ATTRIBUTE_POSITION_2I16], GL_SHORT, 2, declSize, false,
                  (GLvoid*)offsetof(POS_2I16_H_F32_TEX_UI32_NORM_UI32, m_position));
        
        vbo::bind(attributes[CShader::E_ATTRIBUTE_HEIGHT_F32], GL_FLOAT, 1, declSize, false,
                  (GLvoid*)offsetof(POS_2I16_H_F32_TEX_UI32_NORM_UI32, m_height));
        
        vbo::bind(attributes[CShader::E_ATTRIBUTE_TEXCOORD_UI32], GL_UNSIGNED_SHORT, 2, declSize, true,
                  (GLvoid*)offsetof(POS_2I16_H_F32_TEX_UI32_NORM_UI32, m_texcoord));
        
        vbo::bind(attributes[CShader::E_ATTRIBUTE_NORMAL_UI32], GL_BYTE, 4, declSize, true,
                  (GLvoid*)offsetof(POS_2I16_H_F32_TEX_UI32_NORM_UI32, m_normal));
    }

    void vbo::bind(const std::array<i8, E_SHADER_ATTRIBUTE_MAX>& attributes) const
    {
        if(m_usedSize != 0)
        {
            ieBindBuffer(GL_ARRAY_BUFFER, m_vramAddress);
            switch (m_declType)
            {
                case E_DECL_POS_3F32_TEX_UI32_NORM_UI32_COL_4UI8_EXT_4UI8:
                {
                    vbo::bind_POS_3F32_TEX_UI32_NORM_UI32_COL_4UI8_EXT_4UI8(attributes, m_declSize);
                }
                    break;
                case E_DECL_POS_2I16_H_F32_TEX_UI32_NORM_UI32:
                {
                    vbo::bind_POS_2I16_H_F32_TEX_UI32_NORM_UI32(attributes, m_declSize);
                }
                    break;
                default:
                    assert(false);
                    break;
            }
        }
    }
    
    void vbo::unbind(const std::array<i8, E_SHADER_ATTRIBUTE_MAX>& attributes) const
    {
        if(m_usedSize != 0)
        {
            ieBindBuffer(GL_ARRAY_BUFFER, m_vramAddress);
            for(ui32 i = 0; i < attributes.size(); ++i)
            {
                if(attributes[i] > 0)
                {
                    ieDisableVertexAttribArray(attributes[i]);
                }
            }
            ieBindBuffer(GL_ARRAY_BUFFER, NULL);
        }
    }
};
