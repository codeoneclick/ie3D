//
//  CHeightmapProcessor.cpp
//  iGaia
//
//  Created by Sergey Sergeev on 3/3/13.
//
//

#include "CHeightmap.h"
#include "CTexture.h"
#include "CMaterial.h"
#include "CMesh.h"
#include "CVertexBuffer.h"
#include "CIndexBuffer.h"
#include "IRenderTechniqueImporter.h"
#include "IRenderTechniqueAccessor.h"
#include "CQuadTree.h"
#include "CThreadOperation.h"
#include "CConfigurationAccessor.h"
#include "CPerlinNoise.h"

#if defined(__IOS__)

#import <UIKit/UIKit.h>

#elif defined(__OSX__)

#include <Cocoa/Cocoa.h>

#endif

CHeightmap::CHeightmap(const std::string& filename, const glm::ivec2& size) :
m_size(size)
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
    m_size = glm::ivec2(image.size.width, image.size.height);

#elif defined(__OSX__)
    
    NSImage* image = [NSImage imageNamed:[NSString stringWithCString:"map_01" encoding:NSUTF8StringEncoding]];
    CGImageSourceRef source = CGImageSourceCreateWithData((__bridge CFDataRef)[image TIFFRepresentation], NULL);
    CGImageRef mask =  CGImageSourceCreateImageAtIndex(source, 0, NULL);
    NSBitmapImageRep *bitmap = [[NSBitmapImageRep alloc] initWithCGImage:mask];
    data = [bitmap bitmapData];
    m_size = glm::ivec2(image.size.width, image.size.height);
    
    std::vector<f32> heights;
    for(ui32 i = 0; i < m_size.x; ++i)
    {
        for(ui32 j = 0; j < m_size.y; ++j)
        {
            heights.push_back(data[(i + j * m_size.x) * 4 + 1] / 255);
        }
    }
    
#endif
    
    CHeightmap::createVertexesData(heights);
}

CHeightmap::CHeightmap(const glm::ivec2& size, f32 frequency, i32 octaves, ui32 seed) :
m_size(size)
{
    const CPerlinNoise perlin(seed);
    const f32 fx = m_size.x / frequency;
    const f32 fy = m_size.y / frequency;
    
    std::vector<f32> heights;
    for(ui32 i = 0; i < m_size.x; ++i)
    {
        for(ui32 j = 0; j < m_size.y; ++j)
        {
            f32 n = perlin.octaveNoise(i / fx, j / fy, octaves);
            n = glm::clamp(n * 0.5f + 0.5f, 0.0f, 1.0f);
            heights.push_back(n);
        }
    }
    CHeightmap::createVertexesData(heights);
}

CHeightmap::~CHeightmap(void)
{
    std::vector<SUncomressedVertex> uncompressedVertexesDeleter;
    m_uncompressedVertexes.swap(uncompressedVertexesDeleter);
    
    std::vector<SCompressedVertex> compressedVertexesDeleter;
    m_compressedVertexes.swap(compressedVertexesDeleter);
    
    std::vector<SFace> faces;
    m_faces.swap(faces);
}

void CHeightmap::createVertexesData(const std::vector<f32>& data)
{
    m_uncompressedVertexes.resize(m_size.x * m_size.y);
    m_faces.resize((m_size.x - 1) * (m_size.y - 1) * 2);
    
    for(ui32 i = 0; i < m_size.x; ++i)
    {
        for(ui32 j = 0; j < m_size.y; ++j)
        {
            f32 y = i == 0 || j == 0 || i == (m_size.x - 1) || j == (m_size.y - 1) ? -1.0 : data.at(i + j * m_size.x);
            m_uncompressedVertexes[i + j * m_size.x].m_position = glm::vec3(static_cast<f32>(i),
                                                                          y,
                                                                          static_cast<f32>(j));
            m_uncompressedVertexes[i + j * m_size.x].m_texcoord = glm::packUnorm2x16(glm::vec2(static_cast<ui32>(i) /
                                                                                             static_cast<f32>(m_size.x),
                                                                                             static_cast<ui32>(j) /
                                                                                             static_cast<f32>(m_size.y)));
        }
    }
    
    ui32 index = 0;
    for(ui32 i = 0; i < (m_size.x - 1); ++i)
    {
        for(ui32 j = 0; j < (m_size.y - 1); ++j)
        {
            SFace face;
            face.m_indexes[0] = i + j * m_size.x;
            m_uncompressedVertexes[face.m_indexes[0]].m_containInFace.push_back(index);
            glm::vec3 point_01 = m_uncompressedVertexes[face.m_indexes[0]].m_position;
            face.m_indexes[1] = i + (j + 1) * m_size.x;
            m_uncompressedVertexes[face.m_indexes[1]].m_containInFace.push_back(index);
            glm::vec3 point_02 = m_uncompressedVertexes[face.m_indexes[1]].m_position;
            face.m_indexes[2] = i + 1 + j * m_size.x;
            m_uncompressedVertexes[face.m_indexes[2]].m_containInFace.push_back(index);
            glm::vec3 point_03 = m_uncompressedVertexes[face.m_indexes[2]].m_position;
            
            glm::vec3 edge_01 = point_02 - point_01;
            glm::vec3 edge_02 = point_03 - point_01;
            glm::vec3 normal = glm::cross(edge_01, edge_02);
            f32 sin = glm::length(normal) / (glm::length(edge_01) * glm::length(edge_02));
            normal = glm::normalize(normal) * asinf(sin);
            face.m_normal = glm::packSnorm4x8(glm::vec4(normal.x, normal.y, normal.z, 0.0f));
            
            m_faces[index] = face;
            index++;
            
            face.m_indexes[0] = i + (j + 1) * m_size.x;
            m_uncompressedVertexes[face.m_indexes[0]].m_containInFace.push_back(index);
            point_01 = m_uncompressedVertexes[face.m_indexes[0]].m_position;
            face.m_indexes[1] = i + 1 + (j + 1) * m_size.x;
            m_uncompressedVertexes[face.m_indexes[1]].m_containInFace.push_back(index);
            point_02 = m_uncompressedVertexes[face.m_indexes[1]].m_position;
            face.m_indexes[2] = i + 1 + j * m_size.x;
            m_uncompressedVertexes[face.m_indexes[2]].m_containInFace.push_back(index);
            point_03 = m_uncompressedVertexes[face.m_indexes[2]].m_position;
            
            edge_01 = point_02 - point_01;
            edge_02 = point_03 - point_01;
            normal = glm::cross(edge_01, edge_02);
            sin = glm::length(normal) / (glm::length(edge_01) * glm::length(edge_02));
            normal = glm::normalize(normal) * asinf(sin);
            face.m_normal = glm::packSnorm4x8(glm::vec4(normal.x, normal.y, normal.z, 0.0f));;
            
            m_faces[index] = face;
            index++;
        }
    }
    
    for(ui32 i = 0; i < m_uncompressedVertexes.size(); ++i)
    {
        SUncomressedVertex vertex = m_uncompressedVertexes.at(i);
        assert(vertex.m_containInFace.size() != 0);
        glm::vec4 normal = glm::unpackSnorm4x8(m_faces.at(vertex.m_containInFace.at(0)).m_normal);
        for(ui32 j = 1; j < vertex.m_containInFace.size(); ++j)
        {
            normal += glm::unpackSnorm4x8(m_faces.at(vertex.m_containInFace.at(j)).m_normal);
        }
        normal = glm::normalize(normal);
        m_uncompressedVertexes.at(i).m_normal = glm::packSnorm4x8(normal);
    }
    
#if !defined(__EDITOR__)
    std::vector<SFace> facesDeleter;
    m_faces.swap(facesDeleter);
#endif
    
    for(ui32 i = 0; i < m_uncompressedVertexes.size(); ++i)
    {
        SCompressedVertex vertex;
        vertex.m_position = m_uncompressedVertexes.at(i).m_position;
        vertex.m_normal = m_uncompressedVertexes.at(i).m_normal;
        vertex.m_texcoord = m_uncompressedVertexes.at(i).m_texcoord;
        m_compressedVertexes.push_back(vertex);
    }
    
#if !defined(__EDITOR__)
    std::vector<SUncomressedVertex> uncompressedVertexesDeleter;
    m_uncompressedVertexes.swap(uncompressedVertexesDeleter);
#endif
    
}

glm::vec3 CHeightmap::getVertexPosition(ui32 i, ui32 j) const
{
    return m_compressedVertexes[i + j * m_size.x].m_position;
}

glm::uint32 CHeightmap::getVertexTexcoord(ui32 i, ui32 j) const
{
    return m_compressedVertexes[i + j * m_size.x].m_texcoord;
}

glm::uint32 CHeightmap::getVertexNormal(ui32 i, ui32 j) const
{
    return m_compressedVertexes[i + j * m_size.x].m_normal;
}

void CHeightmap::updateVertexesData(const std::vector<std::tuple<ui32, ui32, f32>>& modifiedVertexes)
{
    for(ui32 i = 0; i < modifiedVertexes.size(); ++i)
    {
        ui32 indexX = std::get<0>(modifiedVertexes.at(i));
        ui32 indexZ = std::get<1>(modifiedVertexes.at(i));
        glm::vec3 position = glm::vec3(static_cast<f32>(indexX),
                                       std::get<2>(modifiedVertexes.at(i)),
                                       static_cast<f32>(indexZ));
        
        m_uncompressedVertexes[indexX + indexZ * m_size.x].m_position = position;
    }
    
    for(ui32 i = 0; i < modifiedVertexes.size(); ++i)
    {
        ui32 indexX = std::get<0>(modifiedVertexes.at(i));
        ui32 indexZ = std::get<1>(modifiedVertexes.at(i));
        for(ui32 j = 0; j < m_uncompressedVertexes[indexX + indexZ * m_size.x].m_containInFace.size(); ++j)
        {
            ui32 index = m_uncompressedVertexes[indexX + indexZ * m_size.x].m_containInFace.at(j);
            SFace face = m_faces.at(index);
            
            glm::vec3 point_01 = m_uncompressedVertexes[face.m_indexes[0]].m_position;
            glm::vec3 point_02 = m_uncompressedVertexes[face.m_indexes[1]].m_position;
            glm::vec3 point_03 = m_uncompressedVertexes[face.m_indexes[2]].m_position;
            
            glm::vec3 edge_01 = point_02 - point_01;
            glm::vec3 edge_02 = point_03 - point_01;
            glm::vec3 normal = glm::cross(edge_01, edge_02);
            f32 sin = glm::length(normal) / (glm::length(edge_01) * glm::length(edge_02));
            normal = glm::normalize(normal) * asinf(sin);
            m_faces.at(index).m_normal = glm::packSnorm4x8(glm::vec4(normal.x, normal.y, normal.z, 0.0));
        }
    }
    
    for(ui32 i = 0; i < modifiedVertexes.size(); ++i)
    {
        ui32 indexX = std::get<0>(modifiedVertexes.at(i));
        ui32 indexZ = std::get<1>(modifiedVertexes.at(i));
        SUncomressedVertex vertex = m_uncompressedVertexes.at(indexX + indexZ * m_size.x);
        assert(vertex.m_containInFace.size() != 0);
        glm::vec4 normal = glm::unpackSnorm4x8(m_faces.at(vertex.m_containInFace.at(0)).m_normal);
        for(ui32 j = 1; j < vertex.m_containInFace.size(); ++j)
        {
            normal += glm::unpackSnorm4x8(m_faces.at(vertex.m_containInFace.at(j)).m_normal);
        }
        normal = glm::normalize(normal);
        m_uncompressedVertexes.at(indexX + indexZ * m_size.x).m_normal = glm::packSnorm4x8(normal);
        
        m_compressedVertexes.at(indexX + indexZ * m_size.x).m_position = m_uncompressedVertexes.at(indexX + indexZ * m_size.x).m_position;
        m_compressedVertexes.at(indexX + indexZ * m_size.x).m_normal = m_uncompressedVertexes.at(indexX + indexZ * m_size.x).m_normal;
    }
}

glm::ivec2 CHeightmap::getSize(void) const
{
    return m_size;
}

f32 CHeightmap::getMaxHeight(void) const
{
    decltype(m_compressedVertexes)::iterator minHeight, maxHeight;
    auto values = std::minmax_element(begin(m_compressedVertexes), end(m_compressedVertexes), [] (SCompressedVertex const& value_01, SCompressedVertex const& value_02) {
        return value_01.m_position.y < value_02.m_position.y;
    });
    return values.second->m_position.y;
}

f32 CHeightmap::getMinHeight(void) const
{
    decltype(m_compressedVertexes)::iterator minHeight, maxHeight;
    auto values = std::minmax_element(begin(m_compressedVertexes), end(m_compressedVertexes), [] (SCompressedVertex const& value_01, SCompressedVertex const& value_02) {
        return value_01.m_position.y < value_02.m_position.y;
    });
    return values.first->m_position.y;
}

f32 CHeightmapAccessor::getAngleOnHeightmapSurface(const glm::vec3& point_01,
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

f32 CHeightmapAccessor::getHeight(CSharedHeightmapRef data, const glm::vec3& position)
{
    f32 _x = position.x / 1.0;
    f32 _z = position.z / 1.0;
    i32 x = static_cast<i32>(floor(_x));
    i32 z = static_cast<i32>(floor(_z));
    f32 dx = _x - x;
    f32 dy = _z - z;
    
    if((x < 0) || (z < 0) || (x > (data->getSize().x - 1)) || (z > (data->getSize().y - 1)))
    {
        return 0.0;
    }
    
    f32 height_00 = data->getVertexPosition(x, z).y;
    f32 height_01 = data->getVertexPosition(x, z).y;
    if(z < (data->getSize().y - 1) && z >= 0)
    {
        height_01 = data->getVertexPosition(x, z + 1).y;
    }
    
    f32 height_10 = data->getVertexPosition(x, z).y;
    if(x < (data->getSize().x - 1) && x >= 0)
    {
        height_10 = data->getVertexPosition(x + 1, z).y;
    }
    
    f32 height_11 = data->getVertexPosition(x, z).y;
    if(z < (data->getSize().y - 1) && z >= 0 && x < (data->getSize().x - 1) && x >= 0)
    {
        height_11 =  data->getVertexPosition(x + 1, z + 1).y;
    }
    
    f32 height_0 = height_00 * (1.0f - dy) + height_01 * dy;
    f32 height_1 = height_10 * (1.0f - dy) + height_11 * dy;
    return height_0 * (1.0f - dx) + height_1 * dx;
}

glm::vec2 CHeightmapAccessor::getAngleOnHeightmapSurface(CSharedHeightmapRef data, const glm::vec3& position)
{
    f32 offset = 0.25;
    glm::vec3 point_01 = position;
    glm::vec3 point_02 = glm::vec3(position.x, position.y + offset, position.z);
    f32 height =  CHeightmapAccessor::getHeight(data, glm::vec3(position.x + offset, 0.0f, position.z));
    glm::vec3 point_03 = glm::vec3(position.x + offset, height, position.z);
    height = CHeightmapAccessor::getHeight(data, glm::vec3(position.x, 0.0f, position.z + offset));
    glm::vec3 point_04 = glm::vec3(position.x, height, position.z + offset);
    
    f32 angle_01 = CHeightmapAccessor::getAngleOnHeightmapSurface(point_01, point_02, point_03);
    f32 angle_02 = CHeightmapAccessor::getAngleOnHeightmapSurface(point_01, point_02, point_04);
    
    return glm::vec2(glm::degrees(acos(angle_02) - M_PI_2), glm::degrees(asin(angle_01)));
}

CHeightmapGenerator::CHeightmapGenerator(ISharedRenderTechniqueAccessorRef renderTechniqueAccessor, ISharedConfigurationRef configuration) :
m_heightmap(nullptr),
m_renderTechniqueAccessor(renderTechniqueAccessor),
m_heightmapTexture(nullptr),
m_splattingTexture(nullptr)
{
    assert(m_renderTechniqueAccessor != nullptr);
    assert(configuration != nullptr);
    
    m_heightmap = std::make_shared<CHeightmap>("", glm::ivec2(0));
    
    m_chunkSize = glm::ivec2(65, 65);
    m_chunkLODsSizes.at(0) = glm::ivec2(65, 65);
    m_chunkLODsSizes.at(1) = glm::ivec2(33, 33);
    m_chunkLODsSizes.at(2) = glm::ivec2(17, 17);
    m_chunkLODsSizes.at(3) = glm::ivec2(9, 9);
    
    m_chunksNum = glm::ivec2(m_heightmap->getSize().x / (m_chunkSize.x - 1),
                             m_heightmap->getSize().y / (m_chunkSize.y - 1));
    
    m_chunksUsed.resize(m_chunksNum.x * m_chunksNum.y, std::make_tuple(nullptr, nullptr, nullptr, nullptr,
                                                                       E_LANDSCAPE_CHUNK_LOD_UNKNOWN));
    m_executedOperations.resize(m_chunksNum.x * m_chunksNum.y, nullptr);
    m_canceledOperations.resize(m_chunksNum.x * m_chunksNum.y, nullptr);
    
    m_chunksBounds.resize(m_chunksNum.x * m_chunksNum.y);
    for(ui32 i = 0; i < m_chunksNum.x; ++i)
    {
        for(ui32 j = 0; j < m_chunksNum.y; ++j)
        {
            ui32 index = i + j * m_chunksNum.x;
            glm::vec3 maxBound = glm::vec3(-4096.0f, -4096.0f, -4096.0f);
            glm::vec3 minBound = glm::vec3( 4096.0f,  4096.0f,  4096.0f);
            CHeightmapGenerator::createChunkBound(m_chunkLODsSizes.at(0).x, m_chunkLODsSizes.at(0).y,
                                                  i, j,
                                                  &maxBound, &minBound);
            
            m_chunksBounds[index] = std::make_tuple(maxBound, minBound);
        }
    }
}

CHeightmapGenerator::~CHeightmapGenerator(void)
{
    
}

void CHeightmapGenerator::generateVertecesData(const glm::ivec2& size, f32 frequency, i32 octaves, ui32 seed)
{
    if(m_heightmap != nullptr && (m_heightmap->getSize().x != size.x || m_heightmap->getSize().y != size.y))
    {
        m_heightmap = std::make_shared<CHeightmap>(size, frequency, octaves, seed);
        
        m_chunksNum = glm::ivec2(m_heightmap->getSize().x / (m_chunkSize.x - 1),
                                 m_heightmap->getSize().y / (m_chunkSize.y - 1));
        
        for(ui32 i = 0; i < m_chunksUsed.size(); ++i)
        {
            m_chunksUsed.at(i) = nullptr;
        }
        std::vector<std::tuple<CSharedMesh, CSharedQuadTree, std::function<void(CSharedMeshRef)>, std::function<void(CSharedQuadTreeRef)>, E_LANDSCAPE_CHUNK_LOD>> chunksUsed;
        m_chunksUsed.swap(chunksUsed);
        m_chunksUsed.resize(m_chunksNum.x * m_chunksNum.y, std::make_tuple(nullptr, nullptr, nullptr, nullptr,
                                                                           E_LANDSCAPE_CHUNK_LOD_UNKNOWN));
        
        for(ui32 i = 0; i < m_chunksUnused.size(); ++i)
        {
            m_chunksUnused.at(i) = nullptr;
        }
        std::vector<CSharedMesh> chunksUnused;
        m_chunksUnused.swap(chunksUnused);
        
        
        for (ui32 i = 0; i < m_executedOperations.size(); ++i)
        {
            m_executedOperations.at(i) = nullptr;
        }
        std::vector<CSharedThreadOperation> executedOperations;
        m_executedOperations.swap(executedOperations);
        m_executedOperations.resize(m_chunksNum.x * m_chunksNum.y, nullptr);
        
        for (ui32 i = 0; i < m_canceledOperations.size(); ++i)
        {
            m_canceledOperations.at(i) = nullptr;
        }
        std::vector<CSharedThreadOperation> canceledOperations;
        m_canceledOperations.swap(canceledOperations);
        m_canceledOperations.resize(m_chunksNum.x * m_chunksNum.y, nullptr);

        std::vector<std::tuple<glm::vec3, glm::vec3>> chunksBounds;
        m_chunksBounds.swap(chunksBounds);
        m_chunksBounds.resize(m_chunksNum.x * m_chunksNum.y);
        
        for(ui32 i = 0; i < m_chunksNum.x; ++i)
        {
            for(ui32 j = 0; j < m_chunksNum.y; ++j)
            {
                ui32 index = i + j * m_chunksNum.x;
                glm::vec3 maxBound = glm::vec3(-4096.0f, -4096.0f, -4096.0f);
                glm::vec3 minBound = glm::vec3( 4096.0f,  4096.0f,  4096.0f);
                CHeightmapGenerator::createChunkBound(m_chunkLODsSizes.at(0).x, m_chunkLODsSizes.at(0).y,
                                                      i, j,
                                                      &maxBound, &minBound);
                
                m_chunksBounds[index] = std::make_tuple(maxBound, minBound);
            }
        }
    }
    else
    {
        m_heightmap = std::make_shared<CHeightmap>(size, frequency, octaves, seed);
        for(ui32 i = 0; i < CHeightmapGenerator::getNumChunks().x; ++i)
        {
            for(ui32 j = 0; j < CHeightmapGenerator::getNumChunks().y; ++j)
            {
                ui32 index = i + j * CHeightmapGenerator::getNumChunks().x;
                if(std::get<0>(m_chunksUsed.at(index)) != nullptr)
                {
                    E_LANDSCAPE_CHUNK_LOD LOD = std::get<4>(m_chunksUsed[index]);
                    CHeightmapGenerator::writeToVertexBuffer(i, j, LOD);
                    CHeightmapGenerator::commitVertexBufferToVRAM(i, j, LOD);
                }
            }
        }
    }
    
    CHeightmapGenerator::updateSplattingTexture(m_splattingTexture);
    CHeightmapGenerator::updateHeightmapTexture(m_heightmapTexture);
}

glm::ivec2 CHeightmapGenerator::getSize(void) const
{
    assert(m_heightmap != nullptr);
    return m_heightmap->getSize();
}

glm::ivec2 CHeightmapGenerator::getNumChunks(void) const
{
    return m_chunksNum;
}

f32 CHeightmapGenerator::getHeight(const glm::vec3& position) const
{
    assert(m_heightmap != nullptr);
    assert(position.x < CHeightmapGenerator::getSize().x);
    assert(position.z < CHeightmapGenerator::getSize().y);
    assert(position.x >= 0.0);
    assert(position.z >= 0.0);
    if(position.x < CHeightmapGenerator::getSize().x &&
       position.z < CHeightmapGenerator::getSize().y &&
       position.x >= 0.0 &&
       position.z >= 0.0)
    {
        return CHeightmapAccessor::getHeight(m_heightmap, glm::vec3(position.x , 0.0, position.z));
    }
    return 0.0;
}

glm::vec2 CHeightmapGenerator::getAngleOnHeightmapSurface(const glm::vec3& position) const
{
    assert(m_heightmap != nullptr);
    assert(position.x < CHeightmapGenerator::getSize().x);
    assert(position.z < CHeightmapGenerator::getSize().y);
    assert(position.x >= 0.0);
    assert(position.z >= 0.0);
    if(position.x < CHeightmapGenerator::getSize().x &&
       position.z < CHeightmapGenerator::getSize().y &&
       position.x >= 0.0 &&
       position.z >= 0.0)
    {
        return CHeightmapAccessor::getAngleOnHeightmapSurface(m_heightmap, position);
    }
    return glm::vec2(0.0, 0.0);
}

void CHeightmapGenerator::updateHeightmap(const std::vector<std::tuple<ui32, ui32, f32>>& modifiedHeights)
{
    assert(m_heightmap != nullptr);
    m_heightmap->updateVertexesData(modifiedHeights);
}

void CHeightmapGenerator::updateHeightmap(ui32 offsetX, ui32 offsetZ,
                                          ui32 subWidth, ui32 subHeight)
{
    for(ui32 i = 0; i < CHeightmapGenerator::getNumChunks().x; ++i)
    {
        for(ui32 j = 0; j < CHeightmapGenerator::getNumChunks().y; ++j)
        {
            ui32 index = i + j * CHeightmapGenerator::getNumChunks().x;
            if(std::get<0>(m_chunksUsed.at(index)) != nullptr)
            {
                E_LANDSCAPE_CHUNK_LOD LOD = std::get<4>(m_chunksUsed[index]);
                CHeightmapGenerator::writeToVertexBuffer(i, j, LOD);
                CHeightmapGenerator::commitVertexBufferToVRAM(i, j, LOD);
            }
        }
    }
    CHeightmapGenerator::updateSplattingTexture(m_splattingTexture, false,
                                                offsetX, offsetZ,
                                                subWidth, subHeight);
    
    CHeightmapGenerator::updateHeightmapTexture(m_heightmapTexture, false,
                                                offsetX, offsetZ,
                                                subWidth, subHeight);
}

ui32 CHeightmapGenerator::createTextureId(void)
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

CSharedTexture CHeightmapGenerator::createHeightmapTexture(void)
{
    assert(m_heightmapTexture == nullptr);
    assert(m_heightmap != nullptr);
    ui32 textureId = CHeightmapGenerator::createTextureId();
    m_heightmapTexture = CTexture::constructCustomTexture("landscape.heightmap",
                                                          textureId,
                                                          m_heightmap->getSize().x,
                                                          m_heightmap->getSize().y);
    m_heightmapTexture->setWrapMode(GL_CLAMP_TO_EDGE);
    CHeightmapGenerator::updateHeightmapTexture(m_heightmapTexture);
    return m_heightmapTexture;
}

void CHeightmapGenerator::updateHeightmapTexture(CSharedTextureRef texture, bool isCreation,
                                                 ui32 offsetX, ui32 offsetY,
                                                 ui32 subWidth, ui32 subHeight)
{
    assert(texture != nullptr);
    texture->bind();
    
    ui8* data = nullptr;
    
    if(isCreation)
    {
        data = new ui8[m_heightmap->getSize().x * m_heightmap->getSize().y];
        f32 maxDeep = abs(m_heightmap->getMinHeight());
        for(int i = 0; i < m_heightmap->getSize().x; i++)
        {
            for(int j = 0; j < m_heightmap->getSize().y; j++)
            {
                f32 height = CHeightmapAccessor::getHeight(m_heightmap, glm::vec3(i , 0.0f, j));
                height = height <= 0.0f ? height : 0.0f;
                height /= maxDeep;
                height = std::max(0.0f, std::min((height + 1.0f) / 2.0f, 1.0f));
                ui8 color = static_cast<ui8>(height * 255);
                data[i + j * m_heightmap->getSize().x] = color;
            }
        }
        
        glTexImage2D(GL_TEXTURE_2D, 0,
#if defined(__OPENG_30__)
                     GL_RED,
#else
                     GL_ALPHA,
#endif
                     m_heightmap->getSize().x,
                     m_heightmap->getSize().y,
                     0,
#if defined(__OPENG_30__)
                     GL_RED,
#else
                     GL_ALPHA,
#endif
                     GL_UNSIGNED_BYTE, data);
    }
    else
    {
        assert(offsetX >= 0);
        assert(offsetX + subWidth < texture->getWidth());
        assert(offsetY >= 0);
        assert(offsetY + subHeight < texture->getHeight());
        
        f32 maxDeep = abs(m_heightmap->getMinHeight());
        
        data = new ui8[subWidth * subHeight];
        for(int i = 0; i < subWidth; i++)
        {
            for(int j = 0; j < subHeight; j++)
            {
                f32 height = CHeightmapAccessor::getHeight(m_heightmap,
                                                           glm::vec3(i + offsetX , 0.0, j + offsetY));
                height = height <= 0.0f ? height : 0.0f;
                height /= maxDeep;
                height = std::max(0.0f, std::min((height + 1.0f) / 2.0f, 1.0f));
                ui8 color = static_cast<ui8>(height * 255);
                data[i + j * subWidth] = color;
            }
        }
        glTexSubImage2D(GL_TEXTURE_2D, 0,
                        offsetX, offsetY,
                        subWidth, subHeight,
#if defined(__OPENG_30__)
                        GL_RED,
#else
                        GL_ALPHA,
#endif
                        GL_UNSIGNED_BYTE, data);
    }
    
    delete[] data;
}

std::shared_ptr<CTexture> CHeightmapGenerator::createSplattingTexture(void)
{
    assert(m_splattingTexture == nullptr);
    ui32 textureId = CHeightmapGenerator::createTextureId();
    m_splattingTexture = CTexture::constructCustomTexture("landscape.splatting",
                                                          textureId,
                                                          m_heightmap->getSize().x,
                                                          m_heightmap->getSize().y);
    m_splattingTexture->setWrapMode(GL_CLAMP_TO_EDGE);
    m_splattingTexture->setMagFilter(GL_LINEAR);
    m_splattingTexture->setMinFilter(GL_LINEAR);
    CHeightmapGenerator::updateSplattingTexture(m_splattingTexture);
    return m_splattingTexture;
}

void CHeightmapGenerator::updateSplattingTexture(CSharedTextureRef texture, bool isCreation,
                                                 ui32 offsetX, ui32 offsetY,
                                                 ui32 subWidth, ui32 subHeight)
{
    assert(texture != nullptr);
    texture->bind();
    
    ui16* data = nullptr;
    
    if(isCreation)
    {
        data = new ui16[m_heightmap->getSize().x * m_heightmap->getSize().y];
        for(int i = 0; i < m_heightmap->getSize().x; i++)
        {
            for(int j = 0; j < m_heightmap->getSize().y; j++)
            {
                data[i + j * m_heightmap->getSize().x] = TO_RGB565(255, 0, 0);
                f32 height = CHeightmapAccessor::getHeight(m_heightmap, glm::vec3(i , 0.0, j));
                glm::vec4 normal = glm::unpackSnorm4x8(m_heightmap->getVertexNormal(i, j));
                f32 value = glm::dot(glm::vec3(0.0, 1.0, 0.0), glm::vec3(normal.x, normal.y, normal.z));
                value = glm::degrees(acosf(value));
                assert(value >= 0.0);
                if(height >= 0.25 && value > 45.0)
                {
                    data[i + j * m_heightmap->getSize().x] = TO_RGB565(0, 255, 0);
                }
                if(height < 0.25)
                {
                    data[i + j * m_heightmap->getSize().x] = TO_RGB565(0, 0, 255);
                }
                
                if(i == 0 || j == 0 ||
                   i == (m_heightmap->getSize().x - 1) ||
                   j == (m_heightmap->getSize().y - 1))
                {
                    data[i + j * m_heightmap->getSize().x] = TO_RGB565(255, 0, 0);
                }
            }
        }
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                     m_heightmap->getSize().x,
                     m_heightmap->getSize().y,
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
                f32 height = CHeightmapAccessor::getHeight(m_heightmap, glm::vec3(i + offsetX , 0.0, j + offsetY));
                glm::vec4 normal = glm::unpackSnorm4x8(m_heightmap->getVertexNormal(i + offsetX, j + offsetY));
                f32 value = glm::dot(glm::vec3(0.0, 1.0, 0.0), glm::vec3(normal.x, normal.y, normal.z));
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

const std::tuple<glm::vec3, glm::vec3> CHeightmapGenerator::getChunkBounds(ui32 i, ui32 j) const
{
    assert(m_chunksBounds.size() != 0);
    return m_chunksBounds[i + j * m_chunksNum.x];
}

void CHeightmapGenerator::writeToVertexBuffer(ui32 chunkOffsetX, ui32 chunkOffsetZ, E_LANDSCAPE_CHUNK_LOD LOD)
{
#if defined(__PERFORMANCE_TIMER__)
    std::chrono::steady_clock::time_point startTimestamp = std::chrono::steady_clock::now();
#endif
    ui32 index = chunkOffsetX + chunkOffsetZ * m_chunksNum.x;
    assert(std::get<0>(m_chunksUsed[index]) != nullptr);
    assert(std::get<0>(m_chunksUsed[index])->getVertexBuffer() != nullptr);
    assert((m_chunkSize.x - 1) % (m_chunkLODsSizes.at(LOD).x - 1) == 0.0);
    assert((m_chunkSize.y - 1) % (m_chunkLODsSizes.at(LOD).y - 1) == 0.0);
    
    ui32 chunkLODOffsetX = (m_chunkSize.x - 1) / (m_chunkLODsSizes.at(LOD).x - 1);
    ui32 chunkLODOffsetZ = (m_chunkSize.y - 1) / (m_chunkLODsSizes.at(LOD).y - 1);
    
    SAttributeVertex* vertexData = std::get<0>(m_chunksUsed[index])->getVertexBuffer()->lock();
    index = 0;
    for(ui32 i = 0; i < m_chunkLODsSizes.at(LOD).x; ++i)
    {
        for(ui32 j = 0; j < m_chunkLODsSizes.at(LOD).y; ++j)
        {
            glm::vec2 position = glm::vec2(i * chunkLODOffsetX + chunkOffsetX * m_chunkSize.x - chunkOffsetX,
                                           j * chunkLODOffsetZ + chunkOffsetZ * m_chunkSize.y - chunkOffsetZ);
            
            i32 indexXOffset = static_cast<i32>(position.x) < m_heightmap->getSize().x ?
            static_cast<i32>(position.x) :
            static_cast<i32>(m_heightmap->getSize().x - 1);
            
            i32 indexZOffset = static_cast<i32>(position.y) < m_heightmap->getSize().y ?
            static_cast<i32>(position.y) :
            static_cast<i32>(m_heightmap->getSize().y - 1);
            
            vertexData[index].m_position = m_heightmap->getVertexPosition(indexXOffset, indexZOffset);
            
            vertexData[index].m_texcoord = glm::packUnorm2x16(glm::vec2(static_cast<i32>(vertexData[index].m_position.x) /
                                                                        static_cast<f32>(m_heightmap->getSize().x),
                                                                        static_cast<i32>(vertexData[index].m_position.z) /
                                                                        static_cast<f32>(m_heightmap->getSize().y)));
            
            vertexData[index].m_normal = m_heightmap->getVertexNormal(indexXOffset, indexZOffset);
            ++index;
        }
    }
#if defined(__PERFORMANCE_TIMER__)
    std::chrono::steady_clock::time_point endTimestamp = std::chrono::steady_clock::now();
    f32 duration = std::chrono::duration_cast<std::chrono::microseconds>(endTimestamp - startTimestamp).count();
    std::cout<<"writeToVertexBufferOperation: "<<duration<<std::endl;
#endif
}

void CHeightmapGenerator::commitVertexBufferToVRAM(ui32 chunkOffsetX, ui32 chunkOffsetZ, E_LANDSCAPE_CHUNK_LOD LOD)
{
#if defined(__PERFORMANCE_TIMER__)
    std::chrono::steady_clock::time_point startTimestamp = std::chrono::steady_clock::now();
#endif
    ui32 index = chunkOffsetX + chunkOffsetZ * m_chunksNum.x;
    assert(std::get<0>(m_chunksUsed[index]) != nullptr);
    assert(std::get<0>(m_chunksUsed[index])->getVertexBuffer() != nullptr);
    
    std::get<0>(m_chunksUsed[index])->getVertexBuffer()->unlock(m_chunkLODsSizes.at(LOD).x * m_chunkLODsSizes.at(LOD).y);
    
#if defined(__PERFORMANCE_TIMER__)
    std::chrono::steady_clock::time_point endTimestamp = std::chrono::steady_clock::now();
    f32 duration = std::chrono::duration_cast<std::chrono::microseconds>(endTimestamp - startTimestamp).count();
    std::cout<<"commitVertexBufferOperation: "<<duration<<std::endl;
#endif
}

void CHeightmapGenerator::writeToIndexBuffer(ui32 chunkOffsetX, ui32 chunkOffsetZ, E_LANDSCAPE_CHUNK_LOD LOD)
{
#if defined(__PERFORMANCE_TIMER__)
    std::chrono::steady_clock::time_point startTimestamp = std::chrono::steady_clock::now();
#endif
    ui32 index = chunkOffsetX + chunkOffsetZ * m_chunksNum.x;
    assert(std::get<0>(m_chunksUsed[index]) != nullptr);
    assert(std::get<0>(m_chunksUsed[index])->getIndexBuffer() != nullptr);
    
    ui16* indexData = std::get<0>(m_chunksUsed[index])->getIndexBuffer()->lock();
    
    index = 0;
    for(ui32 i = 0; i < (m_chunkLODsSizes.at(LOD).x - 1); ++i)
    {
        for(ui32 j = 0; j < (m_chunkLODsSizes.at(LOD).y - 1); ++j)
        {
            indexData[index] = i + j * m_chunkLODsSizes.at(LOD).x;
            index++;
            indexData[index] = i + (j + 1) * m_chunkLODsSizes.at(LOD).x;
            index++;
            indexData[index] = i + 1 + j * m_chunkLODsSizes.at(LOD).x;
            index++;
            
            indexData[index] = i + (j + 1) * m_chunkLODsSizes.at(LOD).x;
            index++;
            indexData[index] = i + 1 + (j + 1) * m_chunkLODsSizes.at(LOD).x;
            index++;
            indexData[index] = i + 1 + j * m_chunkLODsSizes.at(LOD).x;
            index++;
        }
    }
    
#if defined(__PERFORMANCE_TIMER__)
    std::chrono::steady_clock::time_point endTimestamp = std::chrono::steady_clock::now();
    f32 duration = std::chrono::duration_cast<std::chrono::microseconds>(endTimestamp - startTimestamp).count();
    std::cout<<"writeToIndexBufferOperation: "<<duration<<std::endl;
#endif
}

void CHeightmapGenerator::commitIndexBufferToVRAM(ui32 chunkOffsetX, ui32 chunkOffsetZ, E_LANDSCAPE_CHUNK_LOD LOD)
{
#if defined(__PERFORMANCE_TIMER__)
    std::chrono::steady_clock::time_point startTimestamp = std::chrono::steady_clock::now();
#endif
    ui32 index = chunkOffsetX + chunkOffsetZ * m_chunksNum.x;
    assert(std::get<0>(m_chunksUsed[index]) != nullptr);
    assert(std::get<0>(m_chunksUsed[index])->getIndexBuffer() != nullptr);
    
    std::get<0>(m_chunksUsed[index])->getIndexBuffer()->unlock((m_chunkLODsSizes.at(LOD).x - 1) * (m_chunkLODsSizes.at(LOD).y - 1) * 6);
    
#if defined(__PERFORMANCE_TIMER__)
    std::chrono::steady_clock::time_point endTimestamp = std::chrono::steady_clock::now();
    f32 duration = std::chrono::duration_cast<std::chrono::microseconds>(endTimestamp - startTimestamp).count();
    std::cout<<"commitIndexBufferOperation: "<<duration<<std::endl;
#endif
}

void CHeightmapGenerator::generateQuadTree(ui32 chunkOffsetX, ui32 chunkOffsetZ)
{
    ui32 index = chunkOffsetX + chunkOffsetZ * m_chunksNum.x;
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
                       m_chunkLODsSizes.at(0).x);
    
#if defined(__PERFORMANCE_TIMER__)
    std::chrono::steady_clock::time_point endTimestamp = std::chrono::steady_clock::now();
    f32 duration = std::chrono::duration_cast<std::chrono::microseconds>(endTimestamp - startTimestamp).count();
    std::cout<<"createQuadTreeOperation: "<<duration<<std::endl;
#endif
}

void CHeightmapGenerator::runChunkLoading(ui32 i, ui32 j, E_LANDSCAPE_CHUNK_LOD LOD,
                                          const std::function<void(CSharedMeshRef)>& meshCreatedCallback,
                                          const std::function<void(CSharedQuadTreeRef)>& quadTreeGeneratedCallback)
{
#if defined(__PERFORMANCE_TIMER__)
    std::chrono::steady_clock::time_point startTimestamp = std::chrono::steady_clock::now();
#endif
    
    ui32 index = i + j * m_chunksNum.x;
    if(m_executedOperations[index] != nullptr)
    {
        return;
    }
    if(m_chunksUnused.size() != 0)
    {
        std::get<0>(m_chunksUsed[index]) = m_chunksUnused.at(m_chunksUnused.size() - 1);
        m_chunksUnused.pop_back();
    }
    else
    {
        glm::vec3 maxBound = glm::vec3(-4096.0, -4096.0, -4096.0);
        glm::vec3 minBound = glm::vec3(4096.0, 4096.0, 4096.0);
        
        CSharedVertexBuffer vertexBuffer = std::make_shared<CVertexBuffer>(m_chunkSize.x * m_chunkSize.y,
                                                                           GL_STATIC_DRAW);
        
        CSharedIndexBuffer indexBuffer = std::make_shared<CIndexBuffer>((m_chunkSize.x - 1) * (m_chunkSize.y - 1) * 6,
                                                                        GL_STREAM_DRAW);
        
        CHeightmapGenerator::createChunkBound(m_chunkLODsSizes.at(LOD).x, m_chunkLODsSizes.at(LOD).y,
                                              i, j,
                                              &maxBound, &minBound);
        
        std::get<0>(m_chunksUsed[index]) = CMesh::constructCustomMesh("landscape.chunk", vertexBuffer, indexBuffer,
                                                                      maxBound, minBound);
    }
    
    std::get<1>(m_chunksUsed[index]) = std::make_shared<CQuadTree>();
    std::get<2>(m_chunksUsed[index]) = meshCreatedCallback;
    std::get<3>(m_chunksUsed[index]) = quadTreeGeneratedCallback;
    std::get<4>(m_chunksUsed[index]) = LOD;
    
    assert(std::get<0>(m_chunksUsed[index]) != nullptr);
    assert(std::get<1>(m_chunksUsed[index]) != nullptr);
    assert(std::get<2>(m_chunksUsed[index]) != nullptr);
    assert(std::get<3>(m_chunksUsed[index]) != nullptr);
    assert(std::get<4>(m_chunksUsed[index]) != E_LANDSCAPE_CHUNK_LOD_UNKNOWN);
    
    CSharedThreadOperation writeToVertexBufferOperation = std::make_shared<CThreadOperation>(E_THREAD_OPERATION_QUEUE_BACKGROUND);
    writeToVertexBufferOperation->setExecutionBlock([this, i, j, LOD](void) {
        CHeightmapGenerator::writeToVertexBuffer(i, j, LOD);
    });
    
    CSharedThreadOperation commitVertexBufferOperation = std::make_shared<CThreadOperation>(E_THREAD_OPERATION_QUEUE_MAIN);
    commitVertexBufferOperation->setExecutionBlock([this, i, j, LOD](void) {
        CHeightmapGenerator::commitVertexBufferToVRAM(i, j, LOD);
    });
    
    CSharedThreadOperation writeToIndexBufferOperation = std::make_shared<CThreadOperation>(E_THREAD_OPERATION_QUEUE_BACKGROUND);
    writeToIndexBufferOperation->setExecutionBlock([this, i, j, LOD](void) {
        CHeightmapGenerator::writeToIndexBuffer(i, j, LOD);
    });
    
    CSharedThreadOperation commitIndexBufferOperation = std::make_shared<CThreadOperation>(E_THREAD_OPERATION_QUEUE_MAIN);
    commitIndexBufferOperation->setExecutionBlock([this, i, j, index, LOD](void) {
        CHeightmapGenerator::commitIndexBufferToVRAM(i, j, LOD);
        assert(std::get<2>(m_chunksUsed[index]) != nullptr);
        std::get<2>(m_chunksUsed[index])(std::get<0>(m_chunksUsed[index]));
    });
    
    CSharedThreadOperation createQuadTreeOperation = std::make_shared<CThreadOperation>(E_THREAD_OPERATION_QUEUE_BACKGROUND);
    createQuadTreeOperation->setExecutionBlock([this, i, j](void) {
        CHeightmapGenerator::generateQuadTree(i, j);
    });
    
    CSharedThreadOperation completionOperation = std::make_shared<CThreadOperation>(E_THREAD_OPERATION_QUEUE_MAIN);
    completionOperation->setExecutionBlock([this,
#if defined(__PERFORMANCE_TIMER__)
                                            startTimestamp,
#endif
                                            i, j, index](void){
        
        assert(std::get<3>(m_chunksUsed[index]) != nullptr);
        std::get<3>(m_chunksUsed[index])(std::get<1>(m_chunksUsed[index]));
        m_executedOperations[index] = nullptr;
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
    
    completionOperation->setCancelBlock([this, index](void) {
        assert(m_executedOperations[index] != nullptr);
        m_canceledOperations[index] = m_executedOperations[index];
        m_executedOperations[index] = nullptr;
    });
    completionOperation->addToExecutionQueue();
}

void CHeightmapGenerator::stopChunkLoading(ui32 i, ui32 j, const std::function<void(void)>& stopLoadingCallback)
{
    ui32 index = i + j * m_chunksNum.x;
    if(m_executedOperations[index] != nullptr)
    {
        m_executedOperations[index]->setCancelBlock([this, stopLoadingCallback, index](void) {
            stopLoadingCallback();
            assert(m_executedOperations[index] != nullptr);
            m_canceledOperations[index] = m_executedOperations[index];
            m_executedOperations[index] = nullptr;
        });
        m_executedOperations[index]->cancel();
    }
    else
    {
        stopLoadingCallback();
    }
}

void CHeightmapGenerator::runChunkUnLoading(ui32 i, ui32 j)
{
    ui32 index = i + j * m_chunksNum.x;
    if(m_executedOperations[index] != nullptr)
    {
        m_executedOperations[index]->cancel();
    }
    else
    {
        std::get<0>(m_chunksUsed[index]) = nullptr;
        std::get<1>(m_chunksUsed[index]) = nullptr;
        std::get<2>(m_chunksUsed[index]) = nullptr;
        std::get<3>(m_chunksUsed[index]) = nullptr;
        std::get<4>(m_chunksUsed[index]) = E_LANDSCAPE_CHUNK_LOD_UNKNOWN;
    }
}

void CHeightmapGenerator::update(void)
{
    for(ui32 index = 0; index < m_canceledOperations.size(); ++index)
    {
        if(m_canceledOperations.at(index) != nullptr &&
           (m_canceledOperations.at(index)->isCompleted() ||
            m_canceledOperations.at(index)->isCanceled()))
        {
            assert(std::get<0>(m_chunksUsed[index]) != nullptr);
            m_chunksUnused.push_back(std::get<0>(m_chunksUsed[index]));
            std::get<0>(m_chunksUsed[index]) = nullptr;
            std::get<1>(m_chunksUsed[index]) = nullptr;
            std::get<2>(m_chunksUsed[index]) = nullptr;
            std::get<3>(m_chunksUsed[index]) = nullptr;
            std::get<4>(m_chunksUsed[index]) = E_LANDSCAPE_CHUNK_LOD_UNKNOWN;
            m_canceledOperations.at(index) = nullptr;
        }
    }
}

void CHeightmapGenerator::createChunkBound(ui32 chunkLODSizeX, ui32 chunkLODSizeZ,
                                           ui32 chunkOffsetX, ui32 chunkOffsetZ,
                                           glm::vec3* maxBound, glm::vec3* minBound)
{
    assert(m_heightmap != nullptr);
    assert(chunkLODSizeX != 0);
    assert(chunkLODSizeZ != 0);
    
    ui32 chunkLODOffsetX = (m_chunkSize.x - 1) / (chunkLODSizeX - 1);
    ui32 chunkLODOffsetZ = (m_chunkSize.y - 1) / (chunkLODSizeZ - 1);
    
    for(ui32 i = 0; i < chunkLODSizeX; ++i)
    {
        for(ui32 j = 0; j < chunkLODSizeZ; ++j)
        {
            glm::vec2 position = glm::vec2(i * chunkLODOffsetX + chunkOffsetX * m_chunkSize.x - chunkOffsetX,
                                           j * chunkLODOffsetZ + chunkOffsetZ * m_chunkSize.y - chunkOffsetZ);
            
            ui32 indexXOffset = static_cast<ui32>(position.x) < m_heightmap->getSize().x ?
            static_cast<ui32>(position.x) :
            static_cast<ui32>(m_heightmap->getSize().x - 1);
            
            ui32 indexZOffset = static_cast<ui32>(position.y) < m_heightmap->getSize().y ?
            static_cast<ui32>(position.y) :
            static_cast<ui32>(m_heightmap->getSize().y - 1);
            
            glm::vec3 point = m_heightmap->getVertexPosition(indexXOffset,
                                                                 indexZOffset);
            
            *maxBound = CMeshData::calculateMaxBound(point, *maxBound);
            *minBound = CMeshData::calculateMinBound(point, *minBound);
        }
    }
}

void CHeightmapGenerator::generateTangentSpace(CSharedVertexBufferRef vertexBuffer,
                                               CSharedIndexBufferRef indexBuffer)
{
    std::vector<glm::vec3> tangents, binormals;
    
    SAttributeVertex* vertexData = vertexBuffer->lock();
    ui32 numVertexes = vertexBuffer->getUsedSize();
    
    ui16* indexData = indexBuffer->lock();
    ui32 numIndexes = indexBuffer->getUsedSize();
    
    for (ui32 i = 0; i < numIndexes; i += 3 )
    {
        glm::vec3 v1 = vertexData[indexData[i + 0]].m_position;
        glm::vec3 v2 = vertexData[indexData[i + 1]].m_position;
        glm::vec3 v3 = vertexData[indexData[i + 2]].m_position;
        f32 s1 = glm::unpackUnorm2x16(vertexData[indexData[i + 0]].m_texcoord).x;
        f32 t1 = glm::unpackUnorm2x16(vertexData[indexData[i + 0]].m_texcoord).y;
        f32 s2 = glm::unpackUnorm2x16(vertexData[indexData[i + 1]].m_texcoord).x;
        f32 t2 = glm::unpackUnorm2x16(vertexData[indexData[i + 1]].m_texcoord).y;
        f32 s3 = glm::unpackUnorm2x16(vertexData[indexData[i + 2]].m_texcoord).x;
        f32 t3 = glm::unpackUnorm2x16(vertexData[indexData[i + 2]].m_texcoord).y;
        
        glm::vec3 t, b;
        CHeightmapGenerator::getTriangleBasis(v1, v2, v3, s1, t1, s2, t2, s3, t3, t, b);
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
        
        glm::vec3 tangent(0.0f);
        glm::vec3 binormal(0.0f);
        for (ui32 j = 0; j < lrt.size(); j++)
        {
            tangent += lrt[j];
        }
        tangent /= static_cast<f32>(lrt.size());
        
        glm::vec4 normal = glm::unpackSnorm4x8(vertexData[i].m_normal);
        tangent = CHeightmapGenerator::ortogonalize(glm::vec3(normal.x, normal.y, normal.z), tangent);
        vertexData[i].m_tangent = glm::packSnorm4x8(glm::vec4(tangent.x, tangent.y, tangent.z, 0.0));
    }
}

void CHeightmapGenerator::getTriangleBasis(const glm::vec3& E, const glm::vec3& F, const glm::vec3& G,
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

glm::vec3 CHeightmapGenerator::getClosestPointOnLine(const glm::vec3& a, const glm::vec3& b, const glm::vec3& p)
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

glm::vec3 CHeightmapGenerator::ortogonalize(const glm::vec3& v1, const glm::vec3& v2)
{
    glm::vec3 v2ProjV1 = CHeightmapGenerator::getClosestPointOnLine( v1, -v1, v2 );
    glm::vec3 res = v2 - v2ProjV1;
    res = glm::normalize(res);
    return res;
}
