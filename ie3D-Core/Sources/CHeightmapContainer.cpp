//
//  CHeightmapContainer.cpp
//  ie3D-Core
//
//  Created by sergey.sergeev on 5/11/15.
//  Copyright (c) 2015 Sergey Sergeev. All rights reserved.
//

#include "CHeightmapContainer.h"
#include "CHeightmapLoader.h"
#include "CHeightmapTextureGenerator.h"
#include "CTexture.h"

const f32 CHeightmapContainer::kRaise = 32.0f;
const f32 CHeightmapContainer::kDeep = CHeightmapContainer::kRaise * 0.25;

const f32 CHeightmapContainer::kLayerSection01 = 0.33;
const f32 CHeightmapContainer::kLayerSection02 = 0.66;
const f32 CHeightmapContainer::kLayerSectionOffset = 0.11;

static const std::string kDeepTextureName = "deep.texture";

CHeightmapContainer::CHeightmapContainer(void) :
m_uncompressedVertices(nullptr),
m_compressedVertices(nullptr),
m_faces(nullptr),
m_uncompressedVerticesMMAPDescriptor(nullptr),
m_compressedVerticesMMAPDescriptor(nullptr),
m_facesMMAPDescriptor(nullptr),
m_vbosMMAPDescriptor(nullptr),
m_ibosMMAPDescriptor(nullptr),
m_splattingMTexturesMMAPDescriptor(nullptr),
m_splattingDTexturesMMAPDescriptor(nullptr),
m_splattingNTexturesMMAPDescriptor(nullptr),
m_mainSize(0),
m_deepTexture(nullptr)
{
    
}

CHeightmapContainer::~CHeightmapContainer(void)
{
    CHeightmapContainer::eraseGeometry();
    CHeightmapContainer::eraseMTextures();
    CHeightmapContainer::eraseDTextures();
    CHeightmapContainer::eraseNTextures();
}

void CHeightmapContainer::init(const glm::ivec2& size)
{
    m_mainSize = size;
    
    m_chunkSize = glm::ivec2(MIN_VALUE(size.x, kMaxChunkSize),
                             MIN_VALUE(size.y, kMaxChunkSize));
    
    m_chunkLODsSizes.at(E_LANDSCAPE_CHUNK_LOD_01) = glm::ivec2(MIN_VALUE(size.x, m_chunkSize.x),
                                                               MIN_VALUE(size.y, m_chunkSize.y));
    
    m_chunkLODsSizes.at(E_LANDSCAPE_CHUNK_LOD_02) = glm::ivec2(MIN_VALUE(size.x, floor(static_cast<f32>(kMaxChunkSize) / 2.0f)),
                                                               MIN_VALUE(size.y, floor(static_cast<f32>(kMaxChunkSize) / 2.0f)));
    
    m_chunkLODsSizes.at(E_LANDSCAPE_CHUNK_LOD_03) = glm::ivec2(MIN_VALUE(size.x, ceil(static_cast<f32>(kMaxChunkSize) / 4.0f)),
                                                               MIN_VALUE(size.y, ceil(static_cast<f32>(kMaxChunkSize) / 4.0f)));
    
    m_chunkLODsSizes.at(E_LANDSCAPE_CHUNK_LOD_04) = glm::ivec2(MIN_VALUE(size.x, floor(static_cast<f32>(kMaxChunkSize) / 8.0f)),
                                                               MIN_VALUE(size.y, floor(static_cast<f32>(kMaxChunkSize) / 8.0f)));
    
    m_texturesLODsSizes.at(E_LANDSCAPE_CHUNK_LOD_01) = CHeightmapTextureGenerator::kSplattingTextureSize_LOD1;
    m_texturesLODsSizes.at(E_LANDSCAPE_CHUNK_LOD_02) = CHeightmapTextureGenerator::kSplattingTextureSize_LOD2;
    m_texturesLODsSizes.at(E_LANDSCAPE_CHUNK_LOD_03) = CHeightmapTextureGenerator::kSplattingTextureSize_LOD3;
    m_texturesLODsSizes.at(E_LANDSCAPE_CHUNK_LOD_04) = CHeightmapTextureGenerator::kSplattingTextureSize_LOD4;
    
    m_chunksNum = glm::ivec2(size.x / (m_chunkSize.x - 1),
                             size.y / (m_chunkSize.y - 1));
    
    m_uncompressedVertices = new SUncomressedVertex[m_mainSize.x * m_mainSize.y];
    m_compressedVertices = new SCompressedVertex[m_mainSize.x * m_mainSize.y];
    m_faces = new SFace[(m_mainSize.x - 1) * (m_mainSize.y - 1) * 2];
    
    m_vbosMMAP.resize(m_chunksNum.x * m_chunksNum.y);
    m_ibosMMAP.resize(m_chunksNum.x * m_chunksNum.y);
    
    m_splattingMTexturesMMAP.resize(m_chunksNum.x * m_chunksNum.y);
    m_splattingDTexturesMMAP.resize(m_chunksNum.x * m_chunksNum.y);
    m_splattingNTexturesMMAP.resize(m_chunksNum.x * m_chunksNum.y);
}

void CHeightmapContainer::eraseGeometry(void)
{
    if(m_uncompressedVerticesMMAPDescriptor != 0)
    {
        m_uncompressedVerticesMMAPDescriptor->deallocate();
        m_uncompressedVerticesMMAPDescriptor = nullptr;
    }
    else
    {
        delete [] m_uncompressedVertices;
    }
    m_uncompressedVertices = nullptr;
    
    if(m_compressedVerticesMMAPDescriptor != nullptr)
    {
        m_compressedVerticesMMAPDescriptor->deallocate();
        m_compressedVerticesMMAPDescriptor = nullptr;
    }
    else
    {
        delete [] m_compressedVertices;
    }
    m_compressedVertices = nullptr;
    
    if(m_facesMMAPDescriptor != nullptr)
    {
        m_facesMMAPDescriptor->deallocate();
        m_facesMMAPDescriptor = nullptr;
    }
    else
    {
        delete [] m_faces;
    }
    m_faces = nullptr;
    
    if(m_vbosMMAPDescriptor != nullptr)
    {
        m_vbosMMAPDescriptor->deallocate();
        m_vbosMMAPDescriptor = nullptr;
    }
    
    if(m_ibosMMAPDescriptor != nullptr)
    {
        m_ibosMMAPDescriptor->deallocate();
        m_ibosMMAPDescriptor = nullptr;
    }
}

void CHeightmapContainer::eraseMTextures(void)
{
    if(m_splattingMTexturesMMAPDescriptor)
    {
        m_splattingMTexturesMMAPDescriptor->deallocate();
        m_splattingMTexturesMMAPDescriptor = nullptr;
    }
}

void CHeightmapContainer::eraseDTextures(void)
{
    if(m_splattingDTexturesMMAPDescriptor)
    {
        m_splattingDTexturesMMAPDescriptor->deallocate();
        m_splattingDTexturesMMAPDescriptor = nullptr;
    }
}

void CHeightmapContainer::eraseNTextures(void)
{
    if(m_splattingNTexturesMMAPDescriptor)
    {
        m_splattingNTexturesMMAPDescriptor->deallocate();
        m_splattingNTexturesMMAPDescriptor = nullptr;
    }
}

void CHeightmapContainer::mmapGeometry(const std::string& filename)
{
    CHeightmapContainer::eraseGeometry();
    
    m_compressedVerticesMMAPDescriptor = std::make_shared<CMmap>();
    m_compressedVertices = static_cast<SCompressedVertex*>(m_compressedVerticesMMAPDescriptor->allocate(CHeightmapLoader::getCompressedVerticesMMAPFilename(filename)));
    if (!m_compressedVertices)
    {
        assert(false);
    }
    
    m_uncompressedVerticesMMAPDescriptor = std::make_shared<CMmap>();
    m_uncompressedVertices = static_cast<SUncomressedVertex*>(m_uncompressedVerticesMMAPDescriptor->allocate(CHeightmapLoader::getUncompressedVerticesMMAPFilename(filename)));
    if (!m_uncompressedVertices)
    {
        assert(false);
    }
    
    m_facesMMAPDescriptor = std::make_shared<CMmap>();
    m_faces = static_cast<SFace*>(m_facesMMAPDescriptor->allocate(CHeightmapLoader::getFacesMMAPFilename(filename)));
    if (!m_faces)
    {
        assert(false);
    }
    
    m_vbosMMAPDescriptor = std::make_shared<CMmap>();
    m_vbosMMAPDescriptor->allocate(CHeightmapLoader::getVBOsMMAPFilename(filename));
    
    ui32 offset = 0;
    for(ui32 i = 0; i < m_chunksNum.x; ++i)
    {
        for(ui32 j = 0; j < m_chunksNum.y; ++j)
        {
            m_vbosMMAP[i + j * m_chunksNum.x] = std::make_shared<CHeightmapVBOMMAP>(m_vbosMMAPDescriptor);
            m_vbosMMAP[i + j * m_chunksNum.x]->setSize(m_chunkSize.x * m_chunkSize.y);
            m_vbosMMAP[i + j * m_chunksNum.x]->setOffset(offset);
            offset += m_chunkSize.x * m_chunkSize.y;
        }
    }
    
    m_ibosMMAPDescriptor = std::make_shared<CMmap>();
    m_ibosMMAPDescriptor->allocate(CHeightmapLoader::getIBOsMMAPFilename(filename));
    
    offset = 0;
    for(ui32 i = 0; i < m_chunksNum.x; ++i)
    {
        for(ui32 j = 0; j < m_chunksNum.y; ++j)
        {
            for(ui32 k = 0; k < E_LANDSCAPE_CHUNK_LOD_MAX; ++k)
            {
                glm::ivec2 currentChunkSize = glm::ivec2(m_chunkLODsSizes[k].x % 2 == 0 ? m_chunkLODsSizes[k].x : m_chunkLODsSizes[k].x - 1,
                                                         m_chunkLODsSizes[k].y % 2 == 0 ? m_chunkLODsSizes[k].y : m_chunkLODsSizes[k].y - 1);
                
                glm::ivec2 extendedChunkSize = currentChunkSize;
                extendedChunkSize -= glm::ivec2(k != E_LANDSCAPE_CHUNK_LOD_01 ? 2 : 0,
                                                k != E_LANDSCAPE_CHUNK_LOD_01 ? 2 : 0);
                
                ui32 additionalIndicesCount = k != E_LANDSCAPE_CHUNK_LOD_01 ? 12 * (m_chunkSize.x - 1 + currentChunkSize.x) : 0;
                ui32 indicesCount = extendedChunkSize.x * extendedChunkSize.y * 6 + additionalIndicesCount;
                
                m_ibosMMAP[i + j * m_chunksNum.x][k] = std::make_shared<CHeightmapIBOMMAP>(m_ibosMMAPDescriptor);
                m_ibosMMAP[i + j * m_chunksNum.x][k]->setSize(indicesCount);
                m_ibosMMAP[i + j * m_chunksNum.x][k]->setOffset(offset);
                offset += indicesCount * 2;
            }
        }
    }
}

void CHeightmapContainer::mmapMTextures(const std::string& filename)
{
    CHeightmapContainer::eraseMTextures();
    
    m_splattingMTexturesMMAPDescriptor = std::make_shared<CMmap>();
    m_splattingMTexturesMMAPDescriptor->allocate(CHeightmapLoader::getSplattingTexturesM_MMapFilename(filename));
    
    ui32 offset = 0;
    for(ui32 i = 0; i < m_chunksNum.x; ++i)
    {
        for(ui32 j = 0; j < m_chunksNum.y; ++j)
        {
            m_splattingMTexturesMMAP[i + j * m_chunksNum.x] = std::make_shared<CHeightmapTextureMMAP_RGB565>(m_splattingMTexturesMMAPDescriptor);
            m_splattingMTexturesMMAP[i + j * m_chunksNum.x]->setSize(CHeightmapTextureGenerator::kSplattingTextureMaskSize.x * CHeightmapTextureGenerator::kSplattingTextureMaskSize.y);
            m_splattingMTexturesMMAP[i + j * m_chunksNum.x]->setOffset(offset);
            offset += CHeightmapTextureGenerator::kSplattingTextureMaskSize.x * CHeightmapTextureGenerator::kSplattingTextureMaskSize.y;
        }
    }
}

void CHeightmapContainer::mmapDTextures(const std::string& filename)
{
    CHeightmapContainer::eraseDTextures();
    
    m_splattingDTexturesMMAPDescriptor = std::make_shared<CMmap>();
    m_splattingDTexturesMMAPDescriptor->allocate(CHeightmapLoader::getSplattingTexturesD_MMapFilename(filename));
    
    ui32 offset = 0;
    for(ui32 i = 0; i < m_chunksNum.x; ++i)
    {
        for(ui32 j = 0; j < m_chunksNum.y; ++j)
        {
            for(ui32 k = 0; k < E_LANDSCAPE_CHUNK_LOD_MAX; ++k)
            {
                m_splattingDTexturesMMAP[i + j * m_chunksNum.x][k] = std::make_shared<CHeightmapTextureMMAP_RGBA8>(m_splattingDTexturesMMAPDescriptor);
                m_splattingDTexturesMMAP[i + j * m_chunksNum.x][k]->setSize(CHeightmapContainer::getTexturesLODSize(static_cast<E_LANDSCAPE_CHUNK_LOD>(k)).x *
                                                                           CHeightmapContainer::getTexturesLODSize(static_cast<E_LANDSCAPE_CHUNK_LOD>(k)).y *
                                                                           CHeightmapTextureGenerator::kSplattingTextureChannels);
                m_splattingDTexturesMMAP[i + j * m_chunksNum.x][k]->setOffset(offset);
                offset += CHeightmapContainer::getTexturesLODSize(static_cast<E_LANDSCAPE_CHUNK_LOD>(k)).x *
                CHeightmapContainer::getTexturesLODSize(static_cast<E_LANDSCAPE_CHUNK_LOD>(k)).y * CHeightmapTextureGenerator::kSplattingTextureChannels;
            }
        }
    }
}

void CHeightmapContainer::mmapNTextures(const std::string& filename)
{
    CHeightmapContainer::eraseNTextures();
    
    m_splattingNTexturesMMAPDescriptor = std::make_shared<CMmap>();
    m_splattingNTexturesMMAPDescriptor->allocate(CHeightmapLoader::getSplattingTexturesN_MMapFilename(filename));
    
    ui32 offset = 0;
    for(ui32 i = 0; i < m_chunksNum.x; ++i)
    {
        for(ui32 j = 0; j < m_chunksNum.y; ++j)
        {
            for(ui32 k = 0; k < E_LANDSCAPE_CHUNK_LOD_MAX; ++k)
            {
                m_splattingNTexturesMMAP[i + j * m_chunksNum.x][k] = std::make_shared<CHeightmapTextureMMAP_RGBA8>(m_splattingNTexturesMMAPDescriptor);
                m_splattingNTexturesMMAP[i + j * m_chunksNum.x][k]->setSize(CHeightmapContainer::getTexturesLODSize(static_cast<E_LANDSCAPE_CHUNK_LOD>(k)).x *
                                                                            CHeightmapContainer::getTexturesLODSize(static_cast<E_LANDSCAPE_CHUNK_LOD>(k)).y *
                                                                            CHeightmapTextureGenerator::kSplattingTextureChannels);
                m_splattingNTexturesMMAP[i + j * m_chunksNum.x][k]->setOffset(offset);
                offset += CHeightmapContainer::getTexturesLODSize(static_cast<E_LANDSCAPE_CHUNK_LOD>(k)).x *
                CHeightmapContainer::getTexturesLODSize(static_cast<E_LANDSCAPE_CHUNK_LOD>(k)).y * CHeightmapTextureGenerator::kSplattingTextureChannels;
            }
        }
    }
}

void CHeightmapContainer::createDeepTexture(void)
{
    ui32 textureId;
    ieGenTextures(1, &textureId);
    m_deepTexture = CTexture::constructCustomTexture(kDeepTextureName, textureId,
                                                     m_mainSize.x, m_mainSize.y);
    
    m_deepTexture->setWrapMode(GL_CLAMP_TO_EDGE);
    m_deepTexture->setMagFilter(GL_LINEAR);
    m_deepTexture->setMinFilter(GL_LINEAR);
}
