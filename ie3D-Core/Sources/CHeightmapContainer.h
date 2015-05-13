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
#include "CMmap.h"
#include "CVertexBuffer.h"
#include "CIndexBuffer.h"

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
    
    class CHeightmapMMAP
    {
    private:
        
    protected:
        
        std::shared_ptr<CMmap> m_descriptor;
        
        ui32 m_size;
        ui32 m_offset;
        
    public:
        
        CHeightmapMMAP(const std::shared_ptr<CMmap>& descriptor);
        virtual ~CHeightmapMMAP(void) = default;
        
        inline void setSize(ui32 size) { m_size = size; };
        inline void setOffset(ui32 offset) { m_offset = offset; }
        inline ui32 getSize(void) const { return m_size; };
    };
    
    class CHeightmapVBOMMAP : public CHeightmapMMAP
    {
    private:
        
    protected:
        
    public:
        
        CHeightmapVBOMMAP(const std::shared_ptr<CMmap>& descriptor) : CHeightmapMMAP(descriptor) { };
        ~CHeightmapVBOMMAP(void) = default;
        
        inline SAttributeVertex* getPointer(void) const
        {
            SAttributeVertex* pointer = (SAttributeVertex* )m_descriptor->pointer();
            assert(pointer != nullptr);
            
            return pointer + m_offset;
        };
    };
    
    class CHeightmapIBOMMAP : public CHeightmapMMAP
    {
    private:
        
        
    protected:
        
    public:
        
        CHeightmapIBOMMAP(const std::shared_ptr<CMmap>& descriptor) : CHeightmapMMAP(descriptor) { };
        ~CHeightmapIBOMMAP(void) = default;
        
        inline ui16* getSourcePointer(void) const
        {
            ui16* pointer = (ui16* )m_descriptor->pointer();
            assert(pointer != nullptr);
            
            return pointer + m_offset;
        };
        
        inline ui16* getOriginPointer(void) const
        {
            ui16* pointer = (ui16* )m_descriptor->pointer();
            assert(pointer != nullptr);
            
            return pointer + m_offset + m_size;
        };
        
        inline void updateSourcePointer(void)
        {
            ui16* sourcePointer = CHeightmapIBOMMAP::getSourcePointer();
            ui16* originPointer = CHeightmapIBOMMAP::getOriginPointer();
            memcpy(sourcePointer, originPointer, m_size * sizeof(ui16));
        }
    };
    
private:
    
    SUncomressedVertex* m_uncompressedVertices;
    SFace* m_faces;
    SCompressedVertex* m_compressedVertices;
    
    std::shared_ptr<CMmap> m_uncompressedVerticesMMAPDescriptor;
    std::shared_ptr<CMmap> m_facesMMAPDescriptor;
    std::shared_ptr<CMmap> m_compressedVerticesMMAPDescriptor;
    
    std::shared_ptr<CMmap> m_vbosMMAPDescriptor;
    std::shared_ptr<CMmap> m_ibosMMAPDescriptor;
    
    std::vector<std::shared_ptr<CHeightmapVBOMMAP>> m_vbosMMAP;
    std::vector<std::array<std::shared_ptr<CHeightmapIBOMMAP>, E_LANDSCAPE_CHUNK_LOD_MAX>> m_ibosMMAP;
    
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
    
    inline std::shared_ptr<CHeightmapVBOMMAP> getVBOMmap(i32 index) const;
    inline std::shared_ptr<CHeightmapIBOMMAP> getIBOMmap(i32 index, E_LANDSCAPE_CHUNK_LOD LOD) const;
};

#include "CHeightmapContainer.hpp"

#endif
