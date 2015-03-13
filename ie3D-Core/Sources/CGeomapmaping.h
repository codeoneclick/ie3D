//
//  CGeomapmaping.h
//  ie3D-Core
//
//  Created by sergey.sergeev on 3/5/15.
//  Copyright (c) 2015 Sergey Sergeev. All rights reserved.
//

#ifndef CGeomapmaping_h
#define CGeomapmaping_h

#include "HCommon.h"
#include "HDeclaration.h"

namespace ie
{
    class heightmap
    {
    public:
        
        struct uncopressedVertex
        {
            glm::lowp_uvec2 m_positionXZ;
            f32 height;
            glm::lowp_vec3 m_normal;
            glm::lowp_ivec2 m_texcoord;
            std::vector<ui32> m_containInFace;
            
            uncopressedVertex(void) = default;
            ~uncopressedVertex(void)
            {
                std::vector<ui32> eraser;
                m_containInFace.swap(eraser);
            };
        };
        
        struct compressedVertex
        {
            glm::uint32 m_positionXZ;
            f32 height;
            glm::uint32 m_normal;
            glm::uint32 m_texcoord;
            
            compressedVertex(void) = default;
            ~compressedVertex(void) = default;
        };
        
        struct face
        {
            glm::lowp_vec3 m_normal;
            std::array<ui32, 3> m_indexes;
            
            face(void) = default;
            ~face(void) = default;
        };
        
    private:
        
    protected:
        
    public:
        
        heightmap(void);
        ~heightmap(void);
    };
};

#endif
