//
//  CVBO.h
//  ie3D-Core
//
//  Created by sergey.sergeev on 3/5/15.
//  Copyright (c) 2015 Sergey Sergeev. All rights reserved.
//

#ifndef CVBO_h
#define CVBO_h

#include "IResource.h"
#include "HDeclaration.h"

namespace ie
{
    class vbo : public IResourceData
    {
    public:
        
        enum E_DECL
        {
            E_DECL_POS_3F32_TEX_UI32_NORM_UI32_COL_4UI8_EXT_4UI8 = 0,
            E_DECL_POS_2I16_H_F32_TEX_UI32_NORM_UI32
        };
        
        struct DECL
        {
            
        };
        
        struct POS_3F32_TEX_UI32_NORM_UI32_COL_4UI8_EXT_4UI8 : public DECL
        {
            glm::vec3 m_position;   // 12 +
            glm::uint32 m_texcoord; // 4 +
            glm::uint32 m_normal;   // 4 +
            glm::u8vec4 m_color;    // 4 +
            glm::u8vec4 m_extra;    // 4 = 28
        };
        
        struct POS_2I16_H_F32_TEX_UI32_NORM_UI32 : public DECL
        {
            glm::lowp_ivec2 m_position; // 4 +
            glm::lowp_float m_height;   // 4 +
            glm::uint32 m_texcoord;     // 4 +
            glm::uint32 m_normal;       // 4 = 16
        };
        
    private:
        
    protected:
        
        ui32 m_vramAddress;
        DECL* m_data;
        
        ui32 m_allocatedSize;
        ui32 m_usedSize;
        
        E_DECL m_declType;
        ui32 m_declSize;
        
        GLenum m_mode;
        
        static void bind(i8 attr, GLenum attrType, i32 attrSize, i32 declSize, bool normalized, GLvoid* ptr);
        static void bind_POS_3F32_TEX_UI32_NORM_UI32_COL_4UI8_EXT_4UI8(const std::array<i8, E_SHADER_ATTRIBUTE_MAX>& attributes,
                                                                       ui32 declSize);
        
        static void bind_POS_2I16_H_F32_TEX_UI32_NORM_UI32(const std::array<i8, E_SHADER_ATTRIBUTE_MAX>& attributes,
                                                           ui32 declSize);
        
    public:
        
        vbo(E_DECL decl, ui32 sizeToAllocate, GLenum mode);
        ~vbo(void);
        
        ui32 getAllocatedSize(void) const;
        ui32 getUsedSize(void) const;
        
        DECL* lock(void) const;
        void unlock(ui32 sizeToUse = 0);
        
        void bind(const std::array<i8, E_SHADER_ATTRIBUTE_MAX>& attributes) const;
        void unbind(const std::array<i8, E_SHADER_ATTRIBUTE_MAX>& attributes) const;
    };
};


#endif
