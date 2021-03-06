//
//  CHeightmapContainer.hpp
//  ie3D-Core
//
//  Created by sergey.sergeev on 5/11/15.
//  Copyright (c) 2015 Sergey Sergeev. All rights reserved.
//

inline glm::ivec2 CHeightmapContainer::getMainSize(void) const
{
    return m_mainSize;
};

inline glm::ivec2 CHeightmapContainer::getChunksNum(void) const
{
    return m_chunksNum;
};

inline glm::ivec2 CHeightmapContainer::getChunkSize(void) const
{
    return m_chunkSize;
};

inline glm::ivec2 CHeightmapContainer::getChunkLODSize(E_LANDSCAPE_CHUNK_LOD LOD) const
{
    assert(LOD >= 0 && LOD < E_LANDSCAPE_CHUNK_LOD_MAX);
    return m_chunkLODsSizes[LOD];
}

inline glm::ivec2 CHeightmapContainer::getTexturesLODSize(E_LANDSCAPE_CHUNK_LOD LOD) const
{
    assert(LOD >= 0 && LOD < E_LANDSCAPE_CHUNK_LOD_MAX);
    return m_texturesLODsSizes[LOD];
}

inline f32 CHeightmapContainer::getMaxHeight(void) const
{
    auto max = std::max_element(m_compressedVertices, m_compressedVertices + m_mainSize.x * m_mainSize.y, [](SCompressedVertex const& value_01,
                                                                                                             SCompressedVertex const& value_02) {
        return value_01.m_position.y > value_02.m_position.y;
    });
    return max->m_position.y;
}

inline f32 CHeightmapContainer::getMinHeight(void) const
{
    auto min = std::max_element(m_compressedVertices, m_compressedVertices + m_mainSize.x * m_mainSize.y, [] (SCompressedVertex const& value_01,
                                                                                                              SCompressedVertex const& value_02) {
        return value_01.m_position.y < value_02.m_position.y;
    });
    return min->m_position.y;
}

inline CHeightmapContainer::SUncomressedVertex* CHeightmapContainer::getUncopressedVertices(void) const
{
    assert(m_uncompressedVertices != nullptr);
    return m_uncompressedVertices;
};

inline CHeightmapContainer::SCompressedVertex* CHeightmapContainer::getCompressedVertices(void) const
{
    assert(m_compressedVertices != nullptr);
    return m_compressedVertices;
};

inline CHeightmapContainer::SFace* CHeightmapContainer::getFaces(void) const
{
    assert(m_faces != nullptr);
    return m_faces;
};

inline void CHeightmapContainer::attachUncompressedVertexToVBO(i32 i, i32 j, ui32 vboIndex, ui32 vboVertexIndex)
{
    i32 index = i + j * m_mainSize.x;
    assert(m_uncompressedVertices[index].m_containsInVBOSize <= kMaxContainsInVBO);
    m_uncompressedVertices[index].m_containsInVBO[m_uncompressedVertices[index].m_containsInVBOSize++] = glm::ivec2(vboIndex, vboVertexIndex);
}

inline glm::ivec2* CHeightmapContainer::attachedVerticesToVBO(i32 i, i32 j, ui8 *size) const
{
    i32 index = i + j * m_mainSize.x;
    if(m_uncompressedVertices[index].m_containsInVBOSize == 0 || m_uncompressedVertices[index].m_containsInVBOSize > kMaxContainsInVBO)
    {
        assert(m_uncompressedVertices[index].m_containsInVBOSize != 0 && m_uncompressedVertices[index].m_containsInVBOSize <= kMaxContainsInVBO);
        *size = 0;
        return nullptr;
    }
    *size = m_uncompressedVertices[index].m_containsInVBOSize;
    return m_uncompressedVertices[index].m_containsInVBO;
}

inline void CHeightmapContainer::attachUncompressedVertexToFace(i32 i, i32 j, ui32 faceIndex)
{
    i32 index = i + j * m_mainSize.x;
    assert(m_uncompressedVertices[index].m_containsInFaceSize <= kMaxContainsInFace);
    m_uncompressedVertices[index].m_containsInFace[m_uncompressedVertices[index].m_containsInFaceSize++] = faceIndex;
}

inline ui32* CHeightmapContainer::attachedVerticesToFace(i32 i, i32 j, ui8 *size) const
{
    i32 index = i + j * m_mainSize.x;
    assert(m_uncompressedVertices[index].m_containsInFaceSize != 0 && m_uncompressedVertices[index].m_containsInFaceSize <= kMaxContainsInFace);
    *size = m_uncompressedVertices[index].m_containsInFaceSize;
    return m_uncompressedVertices[index].m_containsInFace;
}

inline glm::vec3 CHeightmapContainer::getVertexPosition(ui32 i, ui32 j) const
{
    return m_compressedVertices[i + j * m_mainSize.x].m_position;
};

inline glm::uint32 CHeightmapContainer::getCompressedVertexTexcoord(ui32 i, ui32 j) const
{
    return m_compressedVertices[i + j * m_mainSize.x].m_texcoord;
};

inline glm::vec2 CHeightmapContainer::getUncompressedVertexTexcoord(ui32 i, ui32 j) const
{
    return glm::unpackUnorm2x16(m_compressedVertices[i + j * m_mainSize.x].m_texcoord);
};

inline glm::uint32 CHeightmapContainer::getCompressedVertexNormal(ui32 i, ui32 j) const
{
    return m_compressedVertices[i + j * m_mainSize.x].m_normal;
};

inline glm::vec3 CHeightmapContainer::getUncompressedVertexNormal(ui32 i, ui32 j) const
{
    glm::vec4 normal = glm::unpackSnorm4x8(m_compressedVertices[i + j * m_mainSize.x].m_normal);
    return glm::vec3(normal.x, normal.y, normal.z);
};

inline std::shared_ptr<CHeightmapContainer::CHeightmapVBOMMAP> CHeightmapContainer::getVBOMmap(i32 index) const
{
    assert(index >=0 && index < m_vbosMMAP.size());
    return m_vbosMMAP[index];
};

inline std::shared_ptr<CHeightmapContainer::CHeightmapIBOMMAP> CHeightmapContainer::getIBOMmap(i32 index, E_LANDSCAPE_CHUNK_LOD LOD) const
{
    assert(LOD >= 0 && LOD < E_LANDSCAPE_CHUNK_LOD_MAX);
    assert(index >=0 && index < m_ibosMMAP.size());
    return m_ibosMMAP[index][LOD];
};

inline std::shared_ptr<CHeightmapContainer::CHeightmapTextureMMAP_RGB565>CHeightmapContainer:: getSplattingMTexturesMmap(i32 index) const
{
    assert(index >=0 && index < m_splattingMTexturesMMAP.size());
    return m_splattingMTexturesMMAP[index];
};

inline std::shared_ptr<CHeightmapContainer::CHeightmapTextureMMAP_RGB565> CHeightmapContainer::getSplattingDTexturesMmap(i32 index, E_LANDSCAPE_CHUNK_LOD LOD) const
{
    assert(LOD >= 0 && LOD < E_LANDSCAPE_CHUNK_LOD_MAX);
    assert(index >=0 && index < m_splattingDTexturesMMAP.size());
    return m_splattingDTexturesMMAP[index][LOD];
};

inline std::shared_ptr<CHeightmapContainer::CHeightmapTextureMMAP_RGBA8> CHeightmapContainer::getSplattingNTexturesMmap(i32 index, E_LANDSCAPE_CHUNK_LOD LOD) const
{
    assert(LOD >= 0 && LOD < E_LANDSCAPE_CHUNK_LOD_MAX);
    assert(index >=0 && index < m_splattingNTexturesMMAP.size());
    return m_splattingNTexturesMMAP[index][LOD];
};

inline CSharedTexture CHeightmapContainer::getDeepTexture(void) const
{
    assert(m_deepTexture != nullptr);
    return m_deepTexture;
};
