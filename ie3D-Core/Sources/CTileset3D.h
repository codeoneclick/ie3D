//
//  CTileset3D.h
//  ie3D-Core
//
//  Created by sergey.sergeev on 3/4/15.
//  Copyright (c) 2015 Sergey Sergeev. All rights reserved.
//

#ifndef CTileset3D_h
#define CTileset3D_h

#include "HCommon.h"
#include "HEnums.h"
#include "HDeclaration.h"
#include "CTileset2D.h"

class CTileset3DVertex
{
private:
    
protected:
    
public:
    
    CTileset3DVertex(void) = default;
    ~CTileset3DVertex(void) = default;
    
    glm::vec3 m_position;
    glm::uint32 m_normal;
    glm::uint32 m_texcoord;
};

class CTileset3DQuad
{
private:
    
protected:
    
public:
    
    CTileset3DQuad(void) = default;
    ~CTileset3DQuad(void) = default;
    
    std::bitset<4> *m_bitset;
    ui32 *m_bitsetType;
    std::array<CTileset3DVertex, 4> m_vertices;
};

class CTileset3D
{
private:
    
    struct SUncompressedVertex
    {
        glm::vec3 m_position;
        glm::vec3 m_normal;
        std::vector<ui32> m_containInFace;
        
        SUncompressedVertex(void) = default;
        ~SUncompressedVertex(void)
        {
            std::vector<ui32> eraser;
            m_containInFace.swap(eraser);
        };
    };
    
    struct SCompressedVertex
    {
        glm::vec3 m_position;
        glm::uint32 m_normal;
        
        SCompressedVertex(void) = default;
        ~SCompressedVertex(void) = default;
    };
    
    struct SFace
    {
        glm::vec3 m_normal;
        std::array<ui32, 3> m_indexes;
        
        SFace(void) = default;
        ~SFace(void) = default;
    };
    
protected:
    
    glm::ivec2 m_size;
    std::vector<std::shared_ptr<CTileset3DQuad>> m_quads;
    
    std::shared_ptr<CTileset2D> m_tileset2D;
    
    void generateQuadsVerticesFromRawHeights(const std::vector<f32>& heights);
    void generateQuads(const std::vector<SUncompressedVertex>& uncompressedVertices);
    
public:
    
    CTileset3D(CSharedTextureRef texture, const glm::ivec2& size, const std::vector<f32>& heights);
    ~CTileset3D(void);
    
    CSharedMesh m_mesh;
};

#endif
