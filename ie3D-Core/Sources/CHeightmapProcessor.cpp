//
//  CHeightmapProcessor.cpp
//  iGaia
//
//  Created by Sergey Sergeev on 3/3/13.
//
//

#include "CHeightmapProcessor.h"
#include "CTexture.h"
#include "CMaterial.h"
#include "CMesh.h"
#include "CVertexBuffer.h"
#include "CIndexBuffer.h"
#include "IRenderTechniqueImporter.h"
#include "IRenderTechniqueAccessor.h"
#include "CConfigurationGameObjects.h"
#include "CQuadTree.h"
#include "CThreadOperation.h"

#if defined(__IOS__)

#import <UIKit/UIKit.h>

#elif defined(__OSX__)

#include <Cocoa/Cocoa.h>

#endif

CHeightmapData::CHeightmapData(const std::string& filename) :
m_maxHeight(-FLT_MAX),
m_minHeight(FLT_MAX)
{
    ui8* data = nullptr;
#if defined(__IOS__)
    
    UIImage* image = [UIImage imageNamed:[NSString stringWithCString:"map_01" encoding:NSUTF8StringEncoding]];
    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
    size_t bytesPerRow = image.size.width * 4;
    data = (ui8 *)malloc(image.size.height * bytesPerRow);
    CGContextRef context = CGBitmapContextCreate(data,
                                                 image.size.width,
                                                 image.size.height,
                                                 8,
                                                 bytesPerRow,
                                                 colorSpace,
                                                 kCGImageAlphaNoneSkipFirst);
    UIGraphicsPushContext(context);
    CGContextTranslateCTM(context, 0.0, image.size.height);
    CGContextScaleCTM(context, 1.0, -1.0);
    [image drawInRect:CGRectMake(0.0, 0.0, image.size.width, image.size.height)];
    UIGraphicsPopContext();
    
    m_sizeX = image.size.width;
    m_sizeZ = image.size.height;

#elif defined(__OSX__)
    
    NSImage* image = [NSImage imageNamed:[NSString stringWithCString:"map_01" encoding:NSUTF8StringEncoding]];
    CGImageSourceRef source = CGImageSourceCreateWithData((__bridge CFDataRef)[image TIFFRepresentation], NULL);
    CGImageRef mask =  CGImageSourceCreateImageAtIndex(source, 0, NULL);
    NSBitmapImageRep *bitmap = [[NSBitmapImageRep alloc] initWithCGImage:mask];
    data = [bitmap bitmapData];
    
    m_sizeX = image.size.width;
    m_sizeZ = image.size.height;
    
#endif
    
    m_uncopressedVertexes.resize(m_sizeX * m_sizeZ);
    m_faces.resize((m_sizeX - 1) * (m_sizeZ - 1) * 2);
    
    for(ui32 i = 0; i < m_sizeX; ++i)
    {
        for(ui32 j = 0; j < m_sizeZ; ++j)
        {
            m_uncopressedVertexes[i + j * m_sizeZ].m_position = glm::vec3(static_cast<f32>(i),
                                                                          (static_cast<f32>(data[(i + j * m_sizeZ) * 4 + 1] - 64) / 255) * 32.0,
                                                                          static_cast<f32>(j));
            m_uncopressedVertexes[i + j * m_sizeZ].m_texcoord = CVertexBuffer::compressVec2(glm::vec2(static_cast<ui32>(i) /
                                                                                                      static_cast<f32>(m_sizeX),
                                                                                                      static_cast<ui32>(j) /
                                                                                                      static_cast<f32>(m_sizeZ)));
            m_maxHeight = m_maxHeight < data[i + j * m_sizeZ] ? data[i + j * m_sizeZ] : m_maxHeight;
            m_minHeight = m_minHeight > data[i + j * m_sizeZ] ? data[i + j * m_sizeZ] : m_minHeight;
        }
    }

    ui32 index = 0;
    for(ui32 i = 0; i < (m_sizeX - 1); ++i)
    {
        for(ui32 j = 0; j < (m_sizeZ - 1); ++j)
        {
            SFace face;
            face.m_indexes[0] = i + j * m_sizeZ;
            m_uncopressedVertexes[face.m_indexes[0]].m_containInFace.push_back(index);
            glm::vec3 point_01 = m_uncopressedVertexes[face.m_indexes[0]].m_position;
            face.m_indexes[1] = i + (j + 1) * m_sizeZ;
            m_uncopressedVertexes[face.m_indexes[1]].m_containInFace.push_back(index);
            glm::vec3 point_02 = m_uncopressedVertexes[face.m_indexes[1]].m_position;
            face.m_indexes[2] = i + 1 + j * m_sizeZ;
            m_uncopressedVertexes[face.m_indexes[2]].m_containInFace.push_back(index);
            glm::vec3 point_03 = m_uncopressedVertexes[face.m_indexes[2]].m_position;
            
            glm::vec3 edge_01 = point_02 - point_01;
            glm::vec3 edge_02 = point_03 - point_01;
            glm::vec3 normal = glm::cross(edge_01, edge_02);
            f32 sin = glm::length(normal) / (glm::length(edge_01) * glm::length(edge_02));
            normal = glm::normalize(normal) * asinf(sin);
            glm::u8vec4 compressedNormal = CVertexBuffer::compressVec3(normal);
            face.m_normal = compressedNormal;
            
            m_faces[index] = face;
            index++;
            
            face.m_indexes[0] = i + (j + 1) * m_sizeZ;
            m_uncopressedVertexes[face.m_indexes[0]].m_containInFace.push_back(index);
            point_01 = m_uncopressedVertexes[face.m_indexes[0]].m_position;
            face.m_indexes[1] = i + 1 + (j + 1) * m_sizeZ;
            m_uncopressedVertexes[face.m_indexes[1]].m_containInFace.push_back(index);
            point_02 = m_uncopressedVertexes[face.m_indexes[1]].m_position;
            face.m_indexes[2] = i + 1 + j * m_sizeZ;
            m_uncopressedVertexes[face.m_indexes[2]].m_containInFace.push_back(index);
            point_03 = m_uncopressedVertexes[face.m_indexes[2]].m_position;
            
            edge_01 = point_02 - point_01;
            edge_02 = point_03 - point_01;
            normal = glm::cross(edge_01, edge_02);
            sin = glm::length(normal) / (glm::length(edge_01) * glm::length(edge_02));
            normal = glm::normalize(normal) * asinf(sin);
            compressedNormal = CVertexBuffer::compressVec3(normal);
            face.m_normal = compressedNormal;
            
            m_faces[index] = face;
            index++;
        }
    }
    
    for(ui32 i = 0; i < m_uncopressedVertexes.size(); ++i)
    {
        SUncomressedVertex vertex = m_uncopressedVertexes.at(i);
        assert(vertex.m_containInFace.size() != 0);
        glm::vec3 normal = CVertexBuffer::uncompressU8Vec4(m_faces.at(vertex.m_containInFace.at(0)).m_normal);
        for(ui32 j = 1; j < vertex.m_containInFace.size(); ++j)
        {
            normal += CVertexBuffer::uncompressU8Vec4(m_faces.at(vertex.m_containInFace.at(j)).m_normal);
        }
        normal = glm::normalize(normal);
        m_uncopressedVertexes.at(i).m_normal = CVertexBuffer::compressVec3(normal);
    }
    
#if !defined(__EDITOR__)
    std::vector<SFace> facesDeleter;
    m_faces.swap(facesDeleter);
#endif
    
    for(ui32 i = 0; i < m_uncopressedVertexes.size(); ++i)
    {
        SCompressedVertex vertex;
        vertex.m_position = m_uncopressedVertexes.at(i).m_position;
        vertex.m_normal = m_uncopressedVertexes.at(i).m_normal;
        vertex.m_texcoord = m_uncopressedVertexes.at(i).m_texcoord;
        m_compressedVertexes.push_back(vertex);
    }
    
#if !defined(__EDITOR__)
    std::vector<SUncomressedVertex> uncompressedVertexesDeleter;
    m_uncopressedVertexes.swap(uncompressedVertexesDeleter);
#endif
}

glm::vec3 CHeightmapData::getVertexPosition(ui32 i, ui32 j) const
{
    return m_compressedVertexes[i + j * m_sizeX].m_position;
}

glm::u16vec2 CHeightmapData::getVertexTexcoord(ui32 i, ui32 j) const
{
    return m_compressedVertexes[i + j * m_sizeX].m_texcoord;
}

glm::u8vec4 CHeightmapData::getVertexNormal(ui32 i, ui32 j) const
{
    return m_compressedVertexes[i + j * m_sizeX].m_normal;
}

void CHeightmapData::updateVertexesData(const std::vector<std::tuple<ui32, ui32, f32>>& modifiedVertexes)
{
    for(ui32 i = 0; i < modifiedVertexes.size(); ++i)
    {
        ui32 indexX = std::get<0>(modifiedVertexes.at(i));
        ui32 indexZ = std::get<1>(modifiedVertexes.at(i));
        glm::vec3 position = glm::vec3(static_cast<f32>(indexX),
                                       std::get<2>(modifiedVertexes.at(i)),
                                       static_cast<f32>(indexZ));
        
        m_uncopressedVertexes[indexX + indexZ * m_sizeX].m_position = position;
    }
    
    for(ui32 i = 0; i < modifiedVertexes.size(); ++i)
    {
        ui32 indexX = std::get<0>(modifiedVertexes.at(i));
        ui32 indexZ = std::get<1>(modifiedVertexes.at(i));
        for(ui32 j = 0; j < m_uncopressedVertexes[indexX + indexZ * m_sizeX].m_containInFace.size(); ++j)
        {
            ui32 index = m_uncopressedVertexes[indexX + indexZ * m_sizeX].m_containInFace.at(j);
            SFace face = m_faces.at(index);
            
            glm::vec3 point_01 = m_uncopressedVertexes[face.m_indexes[0]].m_position;
            glm::vec3 point_02 = m_uncopressedVertexes[face.m_indexes[1]].m_position;
            glm::vec3 point_03 = m_uncopressedVertexes[face.m_indexes[2]].m_position;
            
            glm::vec3 edge_01 = point_02 - point_01;
            glm::vec3 edge_02 = point_03 - point_01;
            glm::vec3 normal = glm::cross(edge_01, edge_02);
            f32 sin = glm::length(normal) / (glm::length(edge_01) * glm::length(edge_02));
            normal = glm::normalize(normal) * asinf(sin);
            glm::u8vec4 compressedNormal = CVertexBuffer::compressVec3(normal);
            m_faces.at(index).m_normal = compressedNormal;
        }
    }
    
    for(ui32 i = 0; i < modifiedVertexes.size(); ++i)
    {
        ui32 indexX = std::get<0>(modifiedVertexes.at(i));
        ui32 indexZ = std::get<1>(modifiedVertexes.at(i));
        SUncomressedVertex vertex = m_uncopressedVertexes.at(indexX + indexZ * m_sizeX);
        assert(vertex.m_containInFace.size() != 0);
        glm::vec3 normal = CVertexBuffer::uncompressU8Vec4(m_faces.at(vertex.m_containInFace.at(0)).m_normal);
        for(ui32 j = 1; j < vertex.m_containInFace.size(); ++j)
        {
            normal += CVertexBuffer::uncompressU8Vec4(m_faces.at(vertex.m_containInFace.at(j)).m_normal);
        }
        normal = glm::normalize(normal);
        m_uncopressedVertexes.at(indexX + indexZ * m_sizeX).m_normal = CVertexBuffer::compressVec3(normal);
        
        m_compressedVertexes.at(indexX + indexZ * m_sizeX).m_position = m_uncopressedVertexes.at(indexX + indexZ * m_sizeX).m_position;
        m_compressedVertexes.at(indexX + indexZ * m_sizeX).m_normal = m_uncopressedVertexes.at(indexX + indexZ * m_sizeX).m_normal;
    }
}

i32 CHeightmapData::getSizeX(void) const
{
    return m_sizeX;
}

i32 CHeightmapData::getSizeZ(void) const
{
    return m_sizeZ;
}

f32 CHeightmapData::getMaxHeight(void) const
{
    decltype(m_compressedVertexes)::iterator minHeight, maxHeight;
    auto values = std::minmax_element(begin(m_compressedVertexes), end(m_compressedVertexes), [] (SCompressedVertex const& value_01, SCompressedVertex const& value_02) {
        return value_01.m_position.y < value_02.m_position.y;
    });
    return values.second->m_position.y;
}

f32 CHeightmapData::getMinHeight(void) const
{
    decltype(m_compressedVertexes)::iterator minHeight, maxHeight;
    auto values = std::minmax_element(begin(m_compressedVertexes), end(m_compressedVertexes), [] (SCompressedVertex const& value_01, SCompressedVertex const& value_02) {
        return value_01.m_position.y < value_02.m_position.y;
    });
    return values.first->m_position.y;
}

f32 CHeightmapDataAccessor::getAngleOnHeightmapSurface(const glm::vec3& point_01,
                                                      const glm::vec3& point_02,
                                                      const glm::vec3& point_03)
{
    f32 vectorLength_01 = sqrt(pow(point_02.x - point_01.x, 2) + pow(point_02.y - point_01.y, 2) + pow(point_02.z - point_01.z, 2));
    f32 vectorLength_02 = sqrt(pow(point_03.x - point_01.x, 2) + pow(point_03.y - point_01.y, 2) + pow(point_03.z - point_01.z, 2));
    
    f32 scalar = (point_02.x - point_01.x) * (point_03.x - point_01.x) +
    (point_02.y - point_01.y) * (point_03.y - point_01.y) +
    (point_02.z - point_01.z) * (point_03.z - point_01.z);
    return scalar / (vectorLength_01 * vectorLength_02);
}

f32 CHeightmapDataAccessor::getHeight(CSharedHeightmapDataRef data, const glm::vec3& position)
{
    f32 _x = position.x / 1.0;
    f32 _z = position.z / 1.0;
    i32 x = static_cast<i32>(floor(_x));
    i32 z = static_cast<i32>(floor(_z));
    f32 dx = _x - x;
    f32 dy = _z - z;
    
    if((x < 0) || (z < 0) || (x > (data->getSizeX() - 1)) || (z > (data->getSizeZ() - 1)))
    {
        return -0.0;
    }
    
    f32 height_00 = data->getVertexPosition(x, z).y;
    f32 height_01 = data->getVertexPosition(x, z).y;
    if(z < (data->getSizeZ() - 1) && z >= 0)
    {
        height_01 = data->getVertexPosition(x, z + 1).y;
    }
    
    f32 height_10 = data->getVertexPosition(x, z).y;
    if(x < (data->getSizeX() - 1) && x >= 0)
    {
        height_10 = data->getVertexPosition(x + 1, z).y;
    }
    
    f32 height_11 = data->getVertexPosition(x, z).y;
    if(z < (data->getSizeZ() - 1) && z >= 0 && x < (data->getSizeX() - 1) && x >= 0)
    {
        height_11 =  data->getVertexPosition(x + 1, z + 1).y;
    }
    
    f32 height_0 = height_00 * (1.0f - dy) + height_01 * dy;
    f32 height_1 = height_10 * (1.0f - dy) + height_11 * dy;
    return height_0 * (1.0f - dx) + height_1 * dx;
}

glm::vec2 CHeightmapDataAccessor::getAngleOnHeightmapSurface(CSharedHeightmapDataRef data, const glm::vec3& position)
{
    f32 offset = 0.25;
    glm::vec3 point_01 = position;
    glm::vec3 point_02 = glm::vec3(position.x, position.y + offset, position.z);
    f32 height =  CHeightmapDataAccessor::getHeight(data, glm::vec3(position.x + offset, 0.0f, position.z));
    glm::vec3 point_03 = glm::vec3(position.x + offset, height, position.z);
    height = CHeightmapDataAccessor::getHeight(data, glm::vec3(position.x, 0.0f, position.z + offset));
    glm::vec3 point_04 = glm::vec3(position.x, height, position.z + offset);
    
    f32 angle_01 = CHeightmapDataAccessor::getAngleOnHeightmapSurface(point_01, point_02, point_03);
    f32 angle_02 = CHeightmapDataAccessor::getAngleOnHeightmapSurface(point_01, point_02, point_04);
    
    return glm::vec2(glm::degrees(acos(angle_02) - M_PI_2), glm::degrees(asin(angle_01)));
}

CHeightmapProcessor::CHeightmapProcessor(ISharedRenderTechniqueAccessorRef renderTechniqueAccessor, ISharedConfigurationRef configuration) :
m_heightmapData(nullptr),
m_renderTechniqueAccessor(renderTechniqueAccessor),
m_heightmapTexture(nullptr),
m_splattingTexture(nullptr),
m_diffuseTexture(nullptr),
m_normalTexture(nullptr),
m_edgesMaskTexture(nullptr),
m_edgesMaskTextureWidth(2048),
m_edgesMaskTextureHeight(2048)
{
    assert(m_renderTechniqueAccessor != nullptr);
    assert(configuration != nullptr);
    
    std::shared_ptr<CConfigurationLandscape> landscapeConfiguration = std::static_pointer_cast<CConfigurationLandscape>(configuration);
    m_heightmapData = std::make_shared<CHeightmapData>("");
    
    m_chunkSizeX = 64;
    m_chunkSizeZ = 64;
    
    m_chunkLODSizeX = 65;
    m_chunkLODSizeZ = 65;
    
    m_numChunksX = m_heightmapData->getSizeX() / m_chunkSizeX;
    m_numChunksZ = m_heightmapData->getSizeZ() / m_chunkSizeZ;
    
    m_chunkSizeX++;
    m_chunkSizeZ++;
    
    m_chunksUsed.resize(m_numChunksX * m_numChunksZ, std::make_tuple(nullptr, nullptr, nullptr, nullptr));
    m_executedOperations.resize(m_numChunksX * m_numChunksZ, nullptr);
    m_canceledOperations.resize(m_numChunksX * m_numChunksZ, nullptr);
    
    m_chunksBounds.resize(m_numChunksX * m_numChunksZ);
    for(ui32 i = 0; i < m_numChunksX; ++i)
    {
        for(ui32 j = 0; j < m_numChunksZ; ++j)
        {
            ui32 index = i + j * m_numChunksX;
            glm::vec3 maxBound = glm::vec3( -4096.0f, -4096.0f, -4096.0f );
            glm::vec3 minBound = glm::vec3(  4096.0f,  4096.0f,  4096.0f );
            CHeightmapProcessor::createChunkBound(m_chunkSizeX, m_chunkSizeZ,
                                                  i, j,
                                                  &maxBound, &minBound);
            
            m_chunksBounds[index] = std::make_tuple(maxBound, minBound);
        }
    }
}

CHeightmapProcessor::~CHeightmapProcessor(void)
{
    
}

ui32 CHeightmapProcessor::getSizeX(void) const
{
    assert(m_heightmapData != nullptr);
    return m_heightmapData->getSizeX();
}

ui32 CHeightmapProcessor::getSizeZ(void) const
{
    assert(m_heightmapData != nullptr);
    return m_heightmapData->getSizeZ();
}

ui32 CHeightmapProcessor::getNumChunksX(void) const
{
    return m_numChunksX;
}

ui32 CHeightmapProcessor::getNumChunksZ(void) const
{
    return m_numChunksZ;
}

ui32 CHeightmapProcessor::getChunkSizeX(ui32 i, ui32 j) const
{
    return m_chunkLODSizeX;
}

ui32 CHeightmapProcessor::getChunkSizeZ(ui32 i, ui32 j) const
{
    return m_chunkLODSizeZ;
}

f32 CHeightmapProcessor::getHeight(const glm::vec3& position) const
{
    assert(m_heightmapData != nullptr);
    assert(position.x < CHeightmapProcessor::getSizeX());
    assert(position.z < CHeightmapProcessor::getSizeZ());
    assert(position.x >= 0.0);
    assert(position.z >= 0.0);
    if(position.x < CHeightmapProcessor::getSizeX() &&
       position.z < CHeightmapProcessor::getSizeZ() &&
       position.x >= 0.0 &&
       position.z >= 0.0)
    {
        return CHeightmapDataAccessor::getHeight(m_heightmapData, glm::vec3(position.x , 0.0, position.z));
    }
    return 0.0;
}

glm::vec2 CHeightmapProcessor::getAngleOnHeightmapSurface(const glm::vec3& position) const
{
    assert(m_heightmapData != nullptr);
    assert(position.x < CHeightmapProcessor::getSizeX());
    assert(position.z < CHeightmapProcessor::getSizeZ());
    assert(position.x >= 0.0);
    assert(position.z >= 0.0);
    if(position.x < CHeightmapProcessor::getSizeX() &&
       position.z < CHeightmapProcessor::getSizeZ() &&
       position.x >= 0.0 &&
       position.z >= 0.0)
    {
        return CHeightmapDataAccessor::getAngleOnHeightmapSurface(m_heightmapData, position);
    }
    return glm::vec2(0.0, 0.0);
}

void CHeightmapProcessor::updateHeightmapData(const std::vector<std::tuple<ui32, ui32, f32>>& modifiedHeights)
{
    assert(m_heightmapData != nullptr);
    m_heightmapData->updateVertexesData(modifiedHeights);
}

void CHeightmapProcessor::updateHeightmap(ui32 offsetX, ui32 offsetZ,
                                          ui32 subWidth, ui32 subHeight)
{
    for(ui32 i = 0; i < CHeightmapProcessor::getNumChunksX(); ++i)
    {
        for(ui32 j = 0; j < CHeightmapProcessor::getNumChunksZ(); ++j)
        {
            ui32 index = i + j * CHeightmapProcessor::getNumChunksX();
            if(std::get<0>(m_chunksUsed.at(index)) != nullptr)
            {
                CHeightmapProcessor::writeToVertexBuffer(i, j);
                CHeightmapProcessor::commitVertexBufferToVRAM(i, j);
            }
        }
    }
    CHeightmapProcessor::updateSplattingTexture(m_splattingTexture, false,
                                                offsetX, offsetZ,
                                                subWidth, subHeight);
    
    CHeightmapProcessor::updateHeightmapTexture(m_heightmapTexture, false,
                                                offsetX, offsetZ,
                                                subWidth, subHeight);
    
    //CHeightmapProcessor::updateEdgesMaskTexture(m_edgesMaskTexture);
}

ui32 CHeightmapProcessor::createTextureId(void)
{
    ui32 textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    return textureId;
}

CSharedTexture CHeightmapProcessor::createHeightmapTexture(void)
{
    assert(m_heightmapTexture == nullptr);
    assert(m_heightmapData != nullptr);
    ui32 textureId = CHeightmapProcessor::createTextureId();
    m_heightmapTexture = CTexture::constructCustomTexture("landscape.heightmap",
                                                          textureId,
                                                          m_heightmapData->getSizeX(),
                                                          m_heightmapData->getSizeZ());
    m_heightmapTexture->setWrapMode(GL_CLAMP_TO_EDGE);
    CHeightmapProcessor::updateHeightmapTexture(m_heightmapTexture);
    return m_heightmapTexture;
}

void CHeightmapProcessor::updateHeightmapTexture(CSharedTextureRef texture, bool isCreation,
                                                 ui32 offsetX, ui32 offsetY,
                                                 ui32 subWidth, ui32 subHeight)
{
    assert(texture != nullptr);
    texture->bind();
    
    ui8* data = nullptr;
    
    if(isCreation)
    {
        data = new ui8[m_heightmapData->getSizeX() * m_heightmapData->getSizeZ()];
        f32 maxHeight = MAX_VALUE(m_heightmapData->getMaxHeight(), abs(m_heightmapData->getMinHeight()));
        for(int i = 0; i < m_heightmapData->getSizeX(); i++)
        {
            for(int j = 0; j < m_heightmapData->getSizeZ(); j++)
            {
                f32 height = CHeightmapDataAccessor::getHeight(m_heightmapData, glm::vec3(i , 0.0f, j));
                height /= maxHeight;
                ui8 color = static_cast<ui8>((height + 1.0) / 2.0 * 255);
                data[i + j * m_heightmapData->getSizeZ()] = color;
            }
        }
        glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA,
                     m_heightmapData->getSizeX(),
                     m_heightmapData->getSizeZ(),
                     0, GL_ALPHA, GL_UNSIGNED_BYTE, data);
        
    }
    else
    {
        assert(offsetX >= 0);
        assert(offsetX + subWidth < texture->getWidth());
        assert(offsetY >= 0);
        assert(offsetY + subHeight < texture->getHeight());
        
        f32 maxHeight = MAX_VALUE(m_heightmapData->getMaxHeight(), abs(m_heightmapData->getMinHeight()));
        
        data = new ui8[subWidth * subHeight];
        for(int i = 0; i < subWidth; i++)
        {
            for(int j = 0; j < subHeight; j++)
            {
                f32 height = CHeightmapDataAccessor::getHeight(m_heightmapData,
                                                               glm::vec3(i + offsetX , 0.0, j + offsetY));
                height /= maxHeight;
                ui8 color = static_cast<ui8>((height + 1.0) / 2.0 * 255);
                data[i + j * subWidth] = color;
            }
        }
        glTexSubImage2D(GL_TEXTURE_2D, 0,
                        offsetX, offsetY,
                        subWidth, subHeight,
                        GL_ALPHA, GL_UNSIGNED_BYTE, data);
    }
    
    delete[] data;
}

std::shared_ptr<CTexture> CHeightmapProcessor::createSplattingTexture(void)
{
    assert(m_splattingTexture == nullptr);
    ui32 textureId = CHeightmapProcessor::createTextureId();
    m_splattingTexture = CTexture::constructCustomTexture("landscape.splatting",
                                                          textureId,
                                                          m_heightmapData->getSizeX(),
                                                          m_heightmapData->getSizeZ());
    m_splattingTexture->setWrapMode(GL_CLAMP_TO_EDGE);
    CHeightmapProcessor::updateSplattingTexture(m_splattingTexture);
    return m_splattingTexture;
}

void CHeightmapProcessor::updateSplattingTexture(CSharedTextureRef texture, bool isCreation,
                                                 ui32 offsetX, ui32 offsetY,
                                                 ui32 subWidth, ui32 subHeight)
{
    assert(texture != nullptr);
    texture->bind();
    
    ui16* data = nullptr;
    
    if(isCreation)
    {
        data = new ui16[m_heightmapData->getSizeX() * m_heightmapData->getSizeZ()];
        for(int i = 0; i < m_heightmapData->getSizeX(); i++)
        {
            for(int j = 0; j < m_heightmapData->getSizeZ(); j++)
            {
                data[i + j * m_heightmapData->getSizeZ()] = TO_RGB565(255, 0, 0);
                f32 height = CHeightmapDataAccessor::getHeight(m_heightmapData, glm::vec3(i , 0.0, j));
                f32 value = glm::dot(glm::vec3(0.0, 1.0, 0.0), CVertexBuffer::uncompressU8Vec4(m_heightmapData->getVertexNormal(i, j)));
                value = glm::degrees(acosf(value));
                assert(value >= 0.0);
                if(height >= 0.25 && value > 45.0)
                {
                    data[i + j * m_heightmapData->getSizeX()] = TO_RGB565(0, 255, 0);
                }
                if(height < 0.25)
                {
                    data[i + j * m_heightmapData->getSizeX()] = TO_RGB565(0, 0, 255);
                }
                
                if(i == 0 || j == 0 ||
                   i == (m_heightmapData->getSizeX() - 1) ||
                   j == (m_heightmapData->getSizeZ() - 1))
                {
                    data[i + j * m_heightmapData->getSizeX()] = TO_RGB565(255, 0, 0);
                }
            }
        }
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                     m_heightmapData->getSizeX(),
                     m_heightmapData->getSizeZ(),
                     0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, data);
    }
    else
    {
        assert(offsetX >= 0);
        assert(offsetX + subWidth < texture->getWidth());
        assert(offsetY >= 0);
        assert(offsetY + subHeight < texture->getHeight());
        
        data = new ui16[subWidth * subHeight];
        for(int i = 0; i < subWidth; i++)
        {
            for(int j = 0; j < subHeight; j++)
            {
                data[i + j * subWidth] = TO_RGB565(255, 0, 0);
                f32 height = CHeightmapDataAccessor::getHeight(m_heightmapData, glm::vec3(i + offsetX , 0.0, j + offsetY));
                f32 value = glm::dot(glm::vec3(0.0, 1.0, 0.0), CVertexBuffer::uncompressU8Vec4(m_heightmapData->getVertexNormal(i + offsetX, j + offsetY)));
                value = glm::degrees(acosf(value));
                assert(value >= 0.0);
                if(height >= 0.25 && value > 45.0)
                {
                    data[i + j * subWidth] = TO_RGB565(0, 255, 0);
                }
                if(height < 0.25)
                {
                    data[i + j * subWidth] = TO_RGB565(0, 0, 255);
                }
            }
        }
        glTexSubImage2D(GL_TEXTURE_2D, 0,
                        offsetX, offsetY,
                        subWidth, subHeight,
                        GL_RGB, GL_UNSIGNED_SHORT_5_6_5, data);
    }

    delete[] data;
}

void CHeightmapProcessor::updateEdgeChunkMaskTexture(ui16* data, ui32 index,
                                                     ui32 edgesMaskWidth,
                                                     ui32 edgesMaskHeight,
                                                     ui32 textureEdgeSize,
                                                     const glm::vec3& point, bool isReverse)
{
    for(ui32 j = 0; j < edgesMaskHeight / 4; ++j)
    {
        f32 currentEdgeHeight = (static_cast<f32>(j) - static_cast<f32>(edgesMaskHeight / 8.0)) / 4.0;
        f32 height = CHeightmapDataAccessor::getHeight(m_heightmapData, point);
        
        ui32 indexOffset = isReverse == true ? (edgesMaskWidth - 1) - index + j * edgesMaskWidth + textureEdgeSize : index + j * edgesMaskWidth + textureEdgeSize;
        data[indexOffset] = TO_RGBA4444(0, 0, 0, 0);
        if(currentEdgeHeight < height && currentEdgeHeight > 0.0)
        {
            data[indexOffset] = TO_RGBA4444(255, 0, 0, 0);
        }
        else
        {
            data[indexOffset] = TO_RGBA4444(0, 255, 0, 0);
        }
    }
}

void CHeightmapProcessor::updateEdgesMaskTexture(CSharedTextureRef texture)
{
    assert(texture != nullptr);
    texture->bind();
    
    ui16* data = new ui16[m_edgesMaskTextureWidth * m_edgesMaskTextureHeight];
    for(ui32 i = 0; i < m_edgesMaskTextureWidth; ++i)
    {
        CHeightmapProcessor::updateEdgeChunkMaskTexture(data,
                                                        i,
                                                        m_edgesMaskTextureWidth,
                                                        m_edgesMaskTextureHeight,
                                                        0,
                                                        glm::vec3(static_cast<f32>(i) / static_cast<f32>(m_edgesMaskTextureWidth) * m_heightmapData->getSizeX(),
                                                                  0.0f, 0.0f),
                                                        true);
        
        CHeightmapProcessor::updateEdgeChunkMaskTexture(data,
                                                        i,
                                                        m_edgesMaskTextureWidth,
                                                        m_edgesMaskTextureHeight,
                                                        m_edgesMaskTextureWidth * (m_edgesMaskTextureHeight / 4),
                                                        glm::vec3(static_cast<f32>(i) / static_cast<f32>(m_edgesMaskTextureWidth) * m_heightmapData->getSizeX(),
                                                                  0.0f, (m_heightmapData->getSizeX() - 1)),
                                                        false);
        
        
        CHeightmapProcessor::updateEdgeChunkMaskTexture(data,
                                                        i,
                                                        m_edgesMaskTextureWidth,
                                                        m_edgesMaskTextureHeight,
                                                        m_edgesMaskTextureWidth * (m_edgesMaskTextureHeight / 4) * 2,
                                                        glm::vec3(0.0f, 0.0f, static_cast<float>(i) / static_cast<float>(m_edgesMaskTextureWidth) * m_heightmapData->getSizeX()),
                                                        false);
        
        CHeightmapProcessor::updateEdgeChunkMaskTexture(data,
                                                        i,
                                                        m_edgesMaskTextureWidth,
                                                        m_edgesMaskTextureHeight,
                                                        m_edgesMaskTextureWidth * (m_edgesMaskTextureHeight / 4) * 3,
                                                        glm::vec3((m_heightmapData->getSizeX() - 1), 0.0f, static_cast<float>(i) / static_cast<float>(m_edgesMaskTextureWidth) * m_heightmapData->getSizeX()),
                                                        true);
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 m_edgesMaskTextureWidth,
                 m_edgesMaskTextureHeight,
                 0, GL_RGBA, GL_UNSIGNED_SHORT_4_4_4_4, data);
    delete[] data;
}

std::shared_ptr<CTexture> CHeightmapProcessor::createEdgesMaskTexture(void)
{
    ui32 textureId = CHeightmapProcessor::createTextureId();
    
    
    m_edgesMaskTexture = CTexture::constructCustomTexture("landscape.edges",
                                                          textureId,
                                                          m_edgesMaskTextureWidth,
                                                          m_edgesMaskTextureHeight);
    m_edgesMaskTexture->setWrapMode(GL_CLAMP_TO_EDGE);
    CHeightmapProcessor::updateEdgesMaskTexture(m_edgesMaskTexture);
    return m_edgesMaskTexture;
}


CSharedTexture CHeightmapProcessor::createSplattingDiffuseTexture(CSharedMaterialRef material)
{
    assert(m_renderTechniqueAccessor != nullptr);
    assert(m_diffuseTexture == nullptr);
    assert(m_splattingTexture != nullptr);
    m_diffuseTexture = m_renderTechniqueAccessor->preprocessTexture(material, 2048, 2048);
    return m_diffuseTexture;
}

CSharedTexture CHeightmapProcessor::createSplattingNormalTexture(CSharedMaterialRef material)
{
    assert(m_renderTechniqueAccessor != nullptr);
    assert(m_normalTexture == nullptr);
    assert(m_splattingTexture != nullptr);
    m_normalTexture = m_renderTechniqueAccessor->preprocessTexture(material, 2048, 2048);
    return m_normalTexture;
}

const std::tuple<glm::vec3, glm::vec3> CHeightmapProcessor::getChunkBounds(ui32 i, ui32 j) const
{
    assert(m_chunksBounds.size() != 0);
    return m_chunksBounds[i + j * m_numChunksZ];
}

void CHeightmapProcessor::writeToVertexBuffer(ui32 chunkOffsetX, ui32 chunkOffsetZ)
{
#if defined(__PERFORMANCE_TIMER__)
    std::chrono::steady_clock::time_point startTimestamp = std::chrono::steady_clock::now();
#endif
    ui32 index = chunkOffsetX + chunkOffsetZ * m_numChunksX;
    assert(std::get<0>(m_chunksUsed[index]) != nullptr);
    assert(std::get<0>(m_chunksUsed[index])->getVertexBuffer() != nullptr);
    assert((m_chunkSizeX - 1) % (m_chunkLODSizeX - 1) == 0.0);
    assert((m_chunkSizeZ - 1) % (m_chunkLODSizeZ - 1) == 0.0);
    
    ui32 chunkLODOffsetX = (m_chunkSizeX - 1) / (m_chunkLODSizeX - 1);
    ui32 chunkLODOffsetZ = (m_chunkSizeZ - 1) / (m_chunkLODSizeZ - 1);
    
    SAttributeVertex* vertexData = std::get<0>(m_chunksUsed[index])->getVertexBuffer()->lock();
    index = 0;
    for(ui32 i = 0; i < m_chunkLODSizeX; ++i)
    {
        for(ui32 j = 0; j < m_chunkLODSizeZ; ++j)
        {
            glm::vec2 position = glm::vec2(i * chunkLODOffsetX + chunkOffsetX * m_chunkSizeX - chunkOffsetX,
                                           j * chunkLODOffsetZ + chunkOffsetZ * m_chunkSizeZ - chunkOffsetZ);
            
            i32 indexXOffset = static_cast<i32>(position.x) < m_heightmapData->getSizeX() ?
            static_cast<i32>(position.x) :
            static_cast<i32>(m_heightmapData->getSizeX() - 1);
            
            i32 indexZOffset = static_cast<i32>(position.y) < m_heightmapData->getSizeZ() ?
            static_cast<i32>(position.y) :
            static_cast<i32>(m_heightmapData->getSizeZ() - 1);
            
            vertexData[index].m_position = m_heightmapData->getVertexPosition(indexXOffset, indexZOffset);
            
            vertexData[index].m_texcoord = CVertexBuffer::compressVec2(glm::vec2(static_cast<i32>(vertexData[index].m_position.x) /
                                                                                 static_cast<f32>(m_heightmapData->getSizeX()),
                                                                                 
                                                                                 static_cast<i32>(vertexData[index].m_position.z) /
                                                                                 static_cast<f32>(m_heightmapData->getSizeZ())));
            
            vertexData[index].m_normal = m_heightmapData->getVertexNormal(indexXOffset, indexZOffset);
            ++index;
        }
    }
#if defined(__PERFORMANCE_TIMER__)
    std::chrono::steady_clock::time_point endTimestamp = std::chrono::steady_clock::now();
    f32 duration = std::chrono::duration_cast<std::chrono::microseconds>(endTimestamp - startTimestamp).count();
    std::cout<<"writeToVertexBufferOperation: "<<duration<<std::endl;
#endif
}

void CHeightmapProcessor::commitVertexBufferToVRAM(ui32 chunkOffsetX, ui32 chunkOffsetZ)
{
#if defined(__PERFORMANCE_TIMER__)
    std::chrono::steady_clock::time_point startTimestamp = std::chrono::steady_clock::now();
#endif
    ui32 index = chunkOffsetX + chunkOffsetZ * m_numChunksX;
    assert(std::get<0>(m_chunksUsed[index]) != nullptr);
    assert(std::get<0>(m_chunksUsed[index])->getVertexBuffer() != nullptr);
    
    std::get<0>(m_chunksUsed[index])->getVertexBuffer()->unlock();
    
#if defined(__PERFORMANCE_TIMER__)
    std::chrono::steady_clock::time_point endTimestamp = std::chrono::steady_clock::now();
    f32 duration = std::chrono::duration_cast<std::chrono::microseconds>(endTimestamp - startTimestamp).count();
    std::cout<<"commitVertexBufferOperation: "<<duration<<std::endl;
#endif
}

void CHeightmapProcessor::writeToIndexBuffer(ui32 chunkOffsetX, ui32 chunkOffsetZ)
{
#if defined(__PERFORMANCE_TIMER__)
    std::chrono::steady_clock::time_point startTimestamp = std::chrono::steady_clock::now();
#endif
    ui32 index = chunkOffsetX + chunkOffsetZ * m_numChunksX;
    assert(std::get<0>(m_chunksUsed[index]) != nullptr);
    assert(std::get<0>(m_chunksUsed[index])->getIndexBuffer() != nullptr);
    assert((m_chunkSizeX - 1) % (m_chunkLODSizeX - 1) == 0.0);
    assert((m_chunkSizeZ - 1) % (m_chunkLODSizeZ - 1) == 0.0);
    
    ui16* indexData = std::get<0>(m_chunksUsed[index])->getIndexBuffer()->lock();
    
    index = 0;
    for(ui32 i = 0; i < (m_chunkLODSizeX - 1); ++i)
    {
        for(ui32 j = 0; j < (m_chunkLODSizeZ - 1); ++j)
        {
            indexData[index] = i + j * m_chunkLODSizeX;
            index++;
            indexData[index] = i + (j + 1) * m_chunkLODSizeX;
            index++;
            indexData[index] = i + 1 + j * m_chunkLODSizeX;
            index++;
            
            indexData[index] = i + (j + 1) * m_chunkLODSizeX;
            index++;
            indexData[index] = i + 1 + (j + 1) * m_chunkLODSizeX;
            index++;
            indexData[index] = i + 1 + j * m_chunkLODSizeX;
            index++;
        }
    }
    
#if defined(__PERFORMANCE_TIMER__)
    std::chrono::steady_clock::time_point endTimestamp = std::chrono::steady_clock::now();
    f32 duration = std::chrono::duration_cast<std::chrono::microseconds>(endTimestamp - startTimestamp).count();
    std::cout<<"writeToIndexBufferOperation: "<<duration<<std::endl;
#endif
}

void CHeightmapProcessor::commitIndexBufferToVRAM(ui32 chunkOffsetX, ui32 chunkOffsetZ)
{
#if defined(__PERFORMANCE_TIMER__)
    std::chrono::steady_clock::time_point startTimestamp = std::chrono::steady_clock::now();
#endif
    ui32 index = chunkOffsetX + chunkOffsetZ * m_numChunksX;
    assert(std::get<0>(m_chunksUsed[index]) != nullptr);
    assert(std::get<0>(m_chunksUsed[index])->getIndexBuffer() != nullptr);
    
    std::get<0>(m_chunksUsed[index])->getIndexBuffer()->unlock();
    
#if defined(__PERFORMANCE_TIMER__)
    std::chrono::steady_clock::time_point endTimestamp = std::chrono::steady_clock::now();
    f32 duration = std::chrono::duration_cast<std::chrono::microseconds>(endTimestamp - startTimestamp).count();
    std::cout<<"commitIndexBufferOperation: "<<duration<<std::endl;
#endif
}

void CHeightmapProcessor::generateQuadTree(ui32 chunkOffsetX, ui32 chunkOffsetZ)
{
    ui32 index = chunkOffsetX + chunkOffsetZ * m_numChunksX;
    CSharedMesh mesh = std::get<0>(m_chunksUsed[index]);
    CSharedQuadTree quadTree = std::get<1>(m_chunksUsed[index]);
#if defined(__PERFORMANCE_TIMER__)
    std::chrono::steady_clock::time_point startTimestamp = std::chrono::steady_clock::now();
#endif
    
    mesh->updateBounds();
    quadTree->generate(mesh->getVertexBuffer(),
                       mesh->getIndexBuffer(),
                       mesh->getMaxBound(),
                       mesh->getMinBound(),
                       4,
                       m_chunkLODSizeX);
    
#if defined(__PERFORMANCE_TIMER__)
    std::chrono::steady_clock::time_point endTimestamp = std::chrono::steady_clock::now();
    f32 duration = std::chrono::duration_cast<std::chrono::microseconds>(endTimestamp - startTimestamp).count();
    std::cout<<"createQuadTreeOperation: "<<duration<<std::endl;
#endif
}

void CHeightmapProcessor::captureChunk(ui32 i, ui32 j,
                                       const std::function<void(CSharedMeshRef)>& meshCreatedCallback,
                                       const std::function<void(CSharedQuadTreeRef)>& quadTreeGeneratedCallback)
{
#if defined(__PERFORMANCE_TIMER__)
    std::chrono::steady_clock::time_point startTimestamp = std::chrono::steady_clock::now();
#endif
    
    ui32 index = i + j * m_numChunksX;
    if(m_chunksUnused.size() != 0)
    {
        assert(std::get<0>(m_chunksUsed[index]) == nullptr);
        assert(std::get<1>(m_chunksUsed[index]) == nullptr);
        assert(std::get<2>(m_chunksUsed[index]) == nullptr);
        assert(std::get<3>(m_chunksUsed[index]) == nullptr);
        
        std::get<0>(m_chunksUsed[index]) = m_chunksUnused.at(m_chunksUnused.size() - 1);
        std::get<1>(m_chunksUsed[index]) = std::make_shared<CQuadTree>();
        std::get<2>(m_chunksUsed[index]) = meshCreatedCallback;
        std::get<3>(m_chunksUsed[index]) = quadTreeGeneratedCallback;
        
        assert(std::get<0>(m_chunksUsed[index]) != nullptr);
        assert(std::get<1>(m_chunksUsed[index]) != nullptr);
        assert(std::get<2>(m_chunksUsed[index]) != nullptr);
        assert(std::get<3>(m_chunksUsed[index]) != nullptr);
        
        m_chunksUnused.pop_back();
    }
    else
    {
        glm::vec3 maxBound = glm::vec3(-4096.0, -4096.0, -4096.0);
        glm::vec3 minBound = glm::vec3(4096.0, 4096.0, 4096.0);
        
        CSharedVertexBuffer vertexBuffer = std::make_shared<CVertexBuffer>(m_chunkLODSizeX * m_chunkLODSizeZ,
                                                                           GL_STATIC_DRAW);
        
        CSharedIndexBuffer indexBuffer = std::make_shared<CIndexBuffer>((m_chunkLODSizeX - 1) * (m_chunkLODSizeZ - 1) * 6,
                                                                        GL_STREAM_DRAW);
        
        CHeightmapProcessor::createChunkBound(m_chunkSizeX, m_chunkSizeZ,
                                              i, j,
                                              &maxBound, &minBound);
        
        assert(std::get<0>(m_chunksUsed[index]) == nullptr);
        assert(std::get<1>(m_chunksUsed[index]) == nullptr);
        assert(std::get<2>(m_chunksUsed[index]) == nullptr);
        
        std::get<0>(m_chunksUsed[index]) = CMesh::constructCustomMesh("landscape.chunk", vertexBuffer, indexBuffer,
                                                                                     maxBound, minBound);
        std::get<1>(m_chunksUsed[index]) = std::make_shared<CQuadTree>();
        std::get<2>(m_chunksUsed[index]) = meshCreatedCallback;
        std::get<3>(m_chunksUsed[index]) = quadTreeGeneratedCallback;
    }
    
    CSharedThreadOperation writeToVertexBufferOperation = std::make_shared<CThreadOperation>(E_THREAD_OPERATION_QUEUE_BACKGROUND);
    writeToVertexBufferOperation->setExecutionBlock([this, i, j](void) {
        CHeightmapProcessor::writeToVertexBuffer(i, j);
    });
    
    CSharedThreadOperation commitVertexBufferOperation = std::make_shared<CThreadOperation>(E_THREAD_OPERATION_QUEUE_MAIN);
    commitVertexBufferOperation->setExecutionBlock([this, i, j](void) {
        CHeightmapProcessor::commitVertexBufferToVRAM(i, j);
    });
    
    CSharedThreadOperation writeToIndexBufferOperation = std::make_shared<CThreadOperation>(E_THREAD_OPERATION_QUEUE_BACKGROUND);
    writeToIndexBufferOperation->setExecutionBlock([this, i, j](void) {
        CHeightmapProcessor::writeToIndexBuffer(i, j);
    });
    
    CSharedThreadOperation commitIndexBufferOperation = std::make_shared<CThreadOperation>(E_THREAD_OPERATION_QUEUE_MAIN);
    commitIndexBufferOperation->setExecutionBlock([this, i, j, index](void) {
        CHeightmapProcessor::commitIndexBufferToVRAM(i, j);
        assert(std::get<2>(m_chunksUsed[index]) != nullptr);
        std::get<2>(m_chunksUsed[index])(std::get<0>(m_chunksUsed[index]));
    });
    
    CSharedThreadOperation createQuadTreeOperation = std::make_shared<CThreadOperation>(E_THREAD_OPERATION_QUEUE_BACKGROUND);
    createQuadTreeOperation->setExecutionBlock([this, i, j](void) {
        CHeightmapProcessor::generateQuadTree(i, j);
    });
    
    CSharedThreadOperation completionOperation = std::make_shared<CThreadOperation>(E_THREAD_OPERATION_QUEUE_MAIN);
    completionOperation->setExecutionBlock([this,
#if defined(__PERFORMANCE_TIMER__)
                                            startTimestamp,
#endif
                                            i, j, index](void){
        
        assert(std::get<2>(m_chunksUsed[index]) != nullptr);
        std::get<3>(m_chunksUsed[index])(std::get<1>(m_chunksUsed[index]));
        
#if defined(__PERFORMANCE_TIMER__)
        std::chrono::steady_clock::time_point endTimestamp = std::chrono::steady_clock::now();
        f32 duration = std::chrono::duration_cast<std::chrono::microseconds>(endTimestamp - startTimestamp).count();
        std::cout<<"getChunk: "<<duration<<std::endl;
#endif
    });
    
    completionOperation->addDependency(writeToVertexBufferOperation);
    completionOperation->addDependency(commitVertexBufferOperation);
    completionOperation->addDependency(writeToIndexBufferOperation);
    completionOperation->addDependency(commitIndexBufferOperation);
    completionOperation->addDependency(createQuadTreeOperation);
    
    assert(m_executedOperations[index] == nullptr);
    m_executedOperations[index] = completionOperation;
    completionOperation->addToExecutionQueue();
}

void CHeightmapProcessor::releaseChunk(ui32 i, ui32 j)
{
    ui32 index = i + j * m_numChunksX;
    m_executedOperations[index]->cancel();
    m_canceledOperations[index] = m_executedOperations[index];
    m_executedOperations[index] = nullptr;
}

void CHeightmapProcessor::update(void)
{
    for(ui32 index = 0; index < m_canceledOperations.size(); ++index)
    {
        if(m_canceledOperations.at(index) != nullptr &&
           m_canceledOperations.at(index)->isCompleted())
        {
            m_canceledOperations.at(index) = nullptr;
            assert(std::get<0>(m_chunksUsed[index]) != nullptr);
            m_chunksUnused.push_back(std::get<0>(m_chunksUsed[index]));
            std::get<0>(m_chunksUsed[index]) = nullptr;
            std::get<1>(m_chunksUsed[index]) = nullptr;
            std::get<2>(m_chunksUsed[index]) = nullptr;
            std::get<3>(m_chunksUsed[index]) = nullptr;
        }
    }
}

void CHeightmapProcessor::createChunkBound(ui32 chunkLODSizeX, ui32 chunkLODSizeZ,
                                           ui32 chunkOffsetX, ui32 chunkOffsetZ,
                                           glm::vec3* maxBound, glm::vec3* minBound)
{
    assert(m_heightmapData != nullptr);
    assert(chunkLODSizeX != 0);
    assert(chunkLODSizeZ != 0);
    assert((m_chunkSizeX - 1) % (chunkLODSizeX - 1) == 0.0);
    assert((m_chunkSizeZ - 1) % (chunkLODSizeZ - 1) == 0.0);
    
    ui32 chunkLODOffsetX = (m_chunkSizeX - 1) / (chunkLODSizeX - 1);
    ui32 chunkLODOffsetZ = (m_chunkSizeZ - 1) / (chunkLODSizeZ - 1);
    
    for(ui32 i = 0; i < chunkLODSizeX; ++i)
    {
        for(ui32 j = 0; j < chunkLODSizeZ; ++j)
        {
            glm::vec2 position = glm::vec2(i * chunkLODOffsetX + chunkOffsetX * m_chunkSizeX - chunkOffsetX,
                                           j * chunkLODOffsetZ + chunkOffsetZ * m_chunkSizeZ - chunkOffsetZ);
            
            ui32 indexXOffset = static_cast<ui32>(position.x) < m_heightmapData->getSizeX() ?
            static_cast<ui32>(position.x) :
            static_cast<ui32>(m_heightmapData->getSizeX() - 1);
            
            ui32 indexZOffset = static_cast<ui32>(position.y) < m_heightmapData->getSizeZ() ?
            static_cast<ui32>(position.y) :
            static_cast<ui32>(m_heightmapData->getSizeZ() - 1);
            
            glm::vec3 point = m_heightmapData->getVertexPosition(indexXOffset,
                                                                 indexZOffset);
            
            *maxBound = CMeshData::calculateMaxBound(point, *maxBound);
            *minBound = CMeshData::calculateMinBound(point, *minBound);
        }
    }
}

void CHeightmapProcessor::generateTangentSpace(CSharedHeightmapDataRef heightmapData,
                                               CSharedVertexBufferRef vertexBuffer,
                                               CSharedIndexBufferRef indexBuffer)
{
    std::vector<glm::vec3> tangents, binormals;
    
    SAttributeVertex* vertexData = vertexBuffer->lock();
    ui32 numVertexes = vertexBuffer->getSize();
    
    ui16* indexData = indexBuffer->lock();
    ui32 numIndexes = indexBuffer->getSize();
    
    for (ui32 i = 0; i < numIndexes; i += 3 )
    {
        glm::vec3 v1 = vertexData[indexData[i + 0]].m_position;
        glm::vec3 v2 = vertexData[indexData[i + 1]].m_position;
        glm::vec3 v3 = vertexData[indexData[i + 2]].m_position;
        f32 s1 = CVertexBuffer::uncompressU16Vec2(vertexData[indexData[i + 0]].m_texcoord).x;
        f32 t1 = CVertexBuffer::uncompressU16Vec2(vertexData[indexData[i + 0]].m_texcoord).y;
        f32 s2 = CVertexBuffer::uncompressU16Vec2(vertexData[indexData[i + 1]].m_texcoord).x;
        f32 t2 = CVertexBuffer::uncompressU16Vec2(vertexData[indexData[i + 1]].m_texcoord).y;
        f32 s3 = CVertexBuffer::uncompressU16Vec2(vertexData[indexData[i + 2]].m_texcoord).x;
        f32 t3 = CVertexBuffer::uncompressU16Vec2(vertexData[indexData[i + 2]].m_texcoord).y;
        
        glm::vec3 t, b;
        CHeightmapProcessor::getTriangleBasis(v1, v2, v3, s1, t1, s2, t2, s3, t3, t, b);
        tangents.push_back(t);
        binormals.push_back(b);
    }
    
    for (ui32 i = 0; i < numVertexes; i++)
    {
        std::vector<glm::vec3> lrt, lrb;
        for (ui32 j = 0; j < numIndexes; j += 3)
        {
            if ((indexData[j + 0]) == i || (indexData[j + 1]) == i || (indexData[j + 2]) == i)
            {
                lrt.push_back(tangents[i]);
                lrb.push_back(binormals[i]);
            }
        }
        
        glm::vec3 tangentRes(0.0f, 0.0f, 0.0f);
        glm::vec3 binormalRes(0.0f, 0.0f, 0.0f);
        for (ui32 j = 0; j < lrt.size(); j++)
        {
            tangentRes += lrt[j];
            binormalRes += lrb[j];
        }
        tangentRes /= static_cast<f32>(lrt.size());
        binormalRes /= static_cast<f32>(lrb.size());
        
        glm::vec3 normal =  CVertexBuffer::uncompressU8Vec4(vertexData[i].m_normal);
        tangentRes = CHeightmapProcessor::ortogonalize(normal, tangentRes);
        binormalRes = CHeightmapProcessor::ortogonalize(normal, binormalRes);
        
        vertexData[i].m_tangent = CVertexBuffer::compressVec3(tangentRes);
    }
}

void CHeightmapProcessor::getTriangleBasis(const glm::vec3& E, const glm::vec3& F, const glm::vec3& G,
                                           f32 sE, f32 tE, f32 sF, f32 tF, f32 sG, f32 tG,
                                           glm::vec3& tangentX, glm::vec3& tangentY)
{
    glm::vec3 P = F - E;
    glm::vec3 Q = G - E;
    f32 s1 = sF - sE;
    f32 t1 = tF - tE;
    f32 s2 = sG - sE;
    f32 t2 = tG - tE;
    f32 pqMatrix[2][3];
    pqMatrix[0][0] = P[0];
    pqMatrix[0][1] = P[1];
    pqMatrix[0][2] = P[2];
    pqMatrix[1][0] = Q[0];
    pqMatrix[1][1] = Q[1];
    pqMatrix[1][2] = Q[2];
    f32 temp = 1.0f / ( s1 * t2 - s2 * t1);
    f32 stMatrix[2][2];
    stMatrix[0][0] = t2 * temp;
    stMatrix[0][1] = -t1 * temp;
    stMatrix[1][0] = -s2 * temp;
    stMatrix[1][1] = s1 * temp;
    f32 tbMatrix[2][3];
    tbMatrix[0][0] = stMatrix[0][0] * pqMatrix[0][0] + stMatrix[0][1] * pqMatrix[1][0];
    tbMatrix[0][1] = stMatrix[0][0] * pqMatrix[0][1] + stMatrix[0][1] * pqMatrix[1][1];
    tbMatrix[0][2] = stMatrix[0][0] * pqMatrix[0][2] + stMatrix[0][1] * pqMatrix[1][2];
    tbMatrix[1][0] = stMatrix[1][0] * pqMatrix[0][0] + stMatrix[1][1] * pqMatrix[1][0];
    tbMatrix[1][1] = stMatrix[1][0] * pqMatrix[0][1] + stMatrix[1][1] * pqMatrix[1][1];
    tbMatrix[1][2] = stMatrix[1][0] * pqMatrix[0][2] + stMatrix[1][1] * pqMatrix[1][2];
    tangentX = glm::vec3( tbMatrix[0][0], tbMatrix[0][1], tbMatrix[0][2] );
    tangentY = glm::vec3( tbMatrix[1][0], tbMatrix[1][1], tbMatrix[1][2] );
    tangentX = glm::normalize(tangentX);
    tangentY = glm::normalize(tangentY);
}

glm::vec3 CHeightmapProcessor::getClosestPointOnLine(const glm::vec3& a, const glm::vec3& b, const glm::vec3& p)
{
    glm::vec3 c = p - a;
    glm::vec3 V = b - a;
    f32 d = V.length();
    V = glm::normalize(V);
    f32 t = glm::dot( V, c );
    
    if ( t < 0.0f )
        return a;
    if ( t > d )
        return b;
    V *= t;
    return ( a + V );
}

glm::vec3 CHeightmapProcessor::ortogonalize(const glm::vec3& v1, const glm::vec3& v2)
{
    glm::vec3 v2ProjV1 = CHeightmapProcessor::getClosestPointOnLine( v1, -v1, v2 );
    glm::vec3 res = v2 - v2ProjV1;
    res = glm::normalize(res);
    return res;
}

