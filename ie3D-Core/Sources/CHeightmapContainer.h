//
//  CHeightmapContainer.h
//  ie3D-Core
//
//  Created by sergey.sergeev on 5/11/15.
//  Copyright (c) 2015 Sergey Sergeev. All rights reserved.
//

#ifndef CHeightmapContainer_h
#define CHeightmapContainer_h

#include "HCommon.h"
#include "HEnums.h"

class CHeightmapContainer
{
public:
    
    static const ui8 kMaxContainsInFace = 32;
    static const ui8 kMaxContainsInVBO = 4;
    static const ui8 kMaxChunkSize = 65;
    
    struct SUncomressedVertex
    {
        glm::vec3 m_position;
        glm::vec3 m_normal;
        glm::vec2 m_texcoord;
        
        std::vector<ui32> m_containsInFace;
        std::vector<glm::ivec2> m_containsInVBO;
        
        SUncomressedVertex(void) = default;
        ~SUncomressedVertex(void) = default;
        
        SUncomressedVertex(const SUncomressedVertex& copy) = delete;
        SUncomressedVertex(SUncomressedVertex&& copy) = delete;
        SUncomressedVertex& operator = (const SUncomressedVertex& copy) = delete;
        SUncomressedVertex& operator = (SUncomressedVertex&& copy) = delete;
    };
    
    struct SCompressedVertex
    {
        glm::vec3 m_position;
        glm::uint32 m_normal;
        glm::uint32 m_texcoord;
        
        SCompressedVertex(void) = default;
        ~SCompressedVertex(void) = default;
    };
    
    struct SFace
    {
        glm::vec3 m_normal;
        ui32 m_indexes[3];
        
        SFace(void) = default;
        ~SFace(void) = default;
        
        SFace(const SFace& copy) = delete;
        SFace(SFace&& copy) = delete;
        SFace& operator = (const SFace& copy) = delete;
        SFace& operator = (SFace&& copy) = delete;
    };
    
private:
    
    SUncomressedVertex* m_uncompressedVertices;
    SFace* m_faces;
    SCompressedVertex* m_compressedVertices;
    
    i32 m_uncompressedVerticesFiledescriptor;
    i32 m_facesFiledescriptor;
    i32 m_compressedVerticesFiledescriptor;
    
    glm::ivec2 m_size;
    
    glm::ivec2 m_chunksNum;
    glm::ivec2 m_chunkSize;
    std::array<glm::ivec2, E_LANDSCAPE_CHUNK_LOD_MAX> m_chunkLODsSizes;
    
    void erase(void);
    
protected:
    
public:
    
    CHeightmapContainer(void);
    ~CHeightmapContainer(void);
    
    void create(const glm::ivec2& size);
    void mmap(const std::string& filename);
    
    inline glm::ivec2 getSize(void) const;
    inline glm::ivec2 getChunksNum(void) const;
    inline glm::ivec2 getChunkSize(void) const;
    inline glm::ivec2 getChunkLODSize(E_LANDSCAPE_CHUNK_LOD LOD) const;
    
    inline SUncomressedVertex* getUncopressedVertices(void) const;
    inline SCompressedVertex* getCompressedVertices(void) const;
    inline SFace* getFaces(void) const;
    
    inline void attachUncompressedVertexToVBO(i32 i, i32 j, ui32 vboIndex, ui32 vboVertexIndex);
    inline std::vector<glm::ivec2> attachedVerticesToVBO(i32 i, i32 j);
    
    inline glm::vec3 getVertexPosition(ui32 i, ui32 j) const;
    inline glm::uint32 getCompressedVertexTexcoord(ui32 i, ui32 j) const;
    inline glm::vec2 getUncompressedVertexTexcoord(ui32 i, ui32 j) const;
    inline glm::uint32 getCompressedVertexNormal(ui32 i, ui32 j) const;
    inline glm::vec3 getUncompressedVertexNormal(ui32 i, ui32 j) const;
};

#include "CHeightmapContainer.hpp"

#endif
