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
#include "CCommonOS.h"
#include "CBoundingBox.h"

#include "CHeightmapLoader.h"

#if defined(__IOS__)

#import <UIKit/UIKit.h>

#elif defined(__OSX__)

#include <Cocoa/Cocoa.h>

#endif

#define __PERFORMANCE_TIMER__ 1

extern ui32 g_heightmapGUID = 0;

static const std::string kUncompressedVerticesMetadataFilename = "uncompressed.vertices.data_";
static const std::string kCompressedVerticesMetadataFilename = "compressed.vertices.data_";
static const std::string kFacesMetadataFilename = "faces.data_";

CHeightmap::CHeightmap(void) :
m_uncompressedVertices(nullptr),
m_faces(nullptr),
m_compressedVertices(nullptr),
m_uncompressedVerticesFiledescriptor(-1),
m_facesFiledescriptor(-1),
m_compressedVerticesFiledescriptor(-1),
m_size(0)
{

}

CHeightmap::~CHeightmap(void)
{
    if(m_uncompressedVerticesFiledescriptor > 0)
    {
        ::close(m_uncompressedVerticesFiledescriptor);
    }
    m_uncompressedVertices = nullptr;
    
    if(m_facesFiledescriptor > 0)
    {
        ::close(m_facesFiledescriptor);
    }
    m_faces = nullptr;
    
    if(m_compressedVerticesFiledescriptor > 0)
    {
        ::close(m_compressedVerticesFiledescriptor);
    }
    m_compressedVertices = nullptr;
}

std::string CHeightmap::getUncompressedVerticesMMAPFilename(const std::string& filename) const
{
    std::ostringstream stringstream;
    stringstream<<filename<<kUncompressedVerticesMetadataFilename<<g_heightmapGUID<<std::endl;
    
#if defined(__IOS__)
    
    return documentspath() + stringstream.str();
    
#endif
    
    return stringstream.str();
}

std::string CHeightmap::getCompressedVerticesMMAPFilename(const std::string& filename) const
{
    std::ostringstream stringstream;
    stringstream<<filename<<kCompressedVerticesMetadataFilename<<g_heightmapGUID<<std::endl;
    
#if defined(__IOS__)
    
    return documentspath() + stringstream.str();
    
#endif
    
    return stringstream.str();
}

std::string CHeightmap::getFacesMMAPFilename(const std::string& filename) const
{
    std::ostringstream stringstream;
    stringstream<<filename<<kFacesMetadataFilename<<g_heightmapGUID<<std::endl;
    
#if defined(__IOS__)
    
    return documentspath() + stringstream.str();
    
#endif
    
    return stringstream.str();
}

void CHeightmap::readHeightmapMetadata(const std::string& filename)
{
    std::ifstream stream(bundlepath().append(filename).c_str());
    if(!stream.is_open())
    {
        assert(false);
    }
    else
    {
        stream.read((char*)&m_size, sizeof(glm::ivec2));
        m_heights = new f32[m_size.x * m_size.y];
        
        ui32 index = 0;
        for(ui32 i = 0; i < m_size.x; ++i)
        {
            for(ui32 j = 0; j < m_size.y; ++j)
            {
                f32 height = 0.0f;
                stream.read((char*)&height, sizeof(f32));
                m_heights[index++] = height;
            }
        }
        stream.close();
    }
}

void CHeightmap::generateHeightmapMetadata(const glm::ivec2& size, f32 frequency, i32 octaves, ui32 seed)
{
    m_size = size;
    m_heights = new f32[m_size.x * m_size.y];
    
    const CPerlinNoise perlin(seed);
    const f32 fx = m_size.x / frequency;
    const f32 fy = m_size.y / frequency;
    
    ui32 index = 0;
    for(ui32 i = 0; i < m_size.x; ++i)
    {
        for(ui32 j = 0; j < m_size.y; ++j)
        {
            f32 n = perlin.octaveNoise(i / fx, j / fy, octaves);
            n = glm::clamp(n * 0.5f + 0.5f, 0.0f, 1.0f);
            m_heights[index++] = n * 64.0f - 32.0f;
        }
    }
}

void CHeightmap::createVerticesMetadata(void)
{
    m_uncompressedVertices = new SUncomressedVertex[m_size.x * m_size.y];
    m_faces = new SFace[(m_size.x - 1) * (m_size.y - 1) * 2];
    
    for(ui32 i = 0; i < m_size.x; ++i)
    {
        for(ui32 j = 0; j < m_size.y; ++j)
        {
            m_uncompressedVertices[i + j * m_size.x].m_position = glm::vec3(i, m_heights[i + j * m_size.x], j);
            
            m_uncompressedVertices[i + j * m_size.x].m_texcoord = glm::vec2(static_cast<f32>(i) / static_cast<f32>(m_size.x),
                                                                            static_cast<f32>(j) / static_cast<f32>(m_size.y));
        }
    }
    
    ui32 index = 0;
    for(ui32 i = 0; i < (m_size.x - 1); ++i)
    {
        for(ui32 j = 0; j < (m_size.y - 1); ++j)
        {
            m_faces[index].m_indexes[0] = i + j * m_size.x;
            m_uncompressedVertices[m_faces[index].m_indexes[0]].m_containsInFace[m_uncompressedVertices[m_faces[index].m_indexes[0]].m_containsInFaceSize++] = index;
            glm::vec3 point_01 = m_uncompressedVertices[m_faces[index].m_indexes[0]].m_position;
            m_faces[index].m_indexes[1] = i + (j + 1) * m_size.x;
            m_uncompressedVertices[m_faces[index].m_indexes[1]].m_containsInFace[m_uncompressedVertices[m_faces[index].m_indexes[1]].m_containsInFaceSize++] = index;
            glm::vec3 point_02 = m_uncompressedVertices[m_faces[index].m_indexes[1]].m_position;
            m_faces[index].m_indexes[2] = i + 1 + j * m_size.x;
            m_uncompressedVertices[m_faces[index].m_indexes[2]].m_containsInFace[m_uncompressedVertices[m_faces[index].m_indexes[2]].m_containsInFaceSize++] = index;
            glm::vec3 point_03 = m_uncompressedVertices[m_faces[index].m_indexes[2]].m_position;
            
            glm::vec3 edge_01 = point_02 - point_01;
            glm::vec3 edge_02 = point_03 - point_01;
            glm::vec3 normal = glm::cross(edge_01, edge_02);
            f32 sin = glm::length(normal) / (glm::length(edge_01) * glm::length(edge_02));
            m_faces[index].m_normal = glm::normalize(normal) * asinf(sin);
            index++;
            
            m_faces[index].m_indexes[0] = i + (j + 1) * m_size.x;
            m_uncompressedVertices[m_faces[index].m_indexes[0]].m_containsInFace[ m_uncompressedVertices[m_faces[index].m_indexes[0]].m_containsInFaceSize++] = index;
            point_01 = m_uncompressedVertices[m_faces[index].m_indexes[0]].m_position;
            m_faces[index].m_indexes[1] = i + 1 + (j + 1) * m_size.x;
            m_uncompressedVertices[m_faces[index].m_indexes[1]].m_containsInFace[m_uncompressedVertices[m_faces[index].m_indexes[1]].m_containsInFaceSize++] = index;
            point_02 = m_uncompressedVertices[m_faces[index].m_indexes[1]].m_position;
            m_faces[index].m_indexes[2] = i + 1 + j * m_size.x;
            m_uncompressedVertices[m_faces[index].m_indexes[2]].m_containsInFace[m_uncompressedVertices[m_faces[index].m_indexes[2]].m_containsInFaceSize++] = index;
            point_03 = m_uncompressedVertices[m_faces[index].m_indexes[2]].m_position;
            
            edge_01 = point_02 - point_01;
            edge_02 = point_03 - point_01;
            normal = glm::cross(edge_01, edge_02);
            sin = glm::length(normal) / (glm::length(edge_01) * glm::length(edge_02));
            m_faces[index].m_normal = glm::normalize(normal) * asinf(sin);
            index++;
        }
    }
    
    for(ui32 i = 0; i < m_size.x * m_size.y; ++i)
    {
        assert(m_uncompressedVertices[i].m_containsInFaceSize != 0 && m_uncompressedVertices[i].m_containsInFaceSize <= kMaxContainsInFace);
        glm::vec3 normal = m_faces[m_uncompressedVertices[i].m_containsInFace[0]].m_normal;
        for(ui32 j = 1; j < m_uncompressedVertices[i].m_containsInFaceSize; ++j)
        {
            normal += m_faces[m_uncompressedVertices[i].m_containsInFace[j]].m_normal;
        }
        normal = glm::normalize(normal);
        m_uncompressedVertices[i].m_normal = normal;
    }
}

void CHeightmap::writeVerticesMetadata(const std::string& filename)
{
    { // writing compressed vertices metadata
        std::ofstream stream;
        stream.open(CHeightmap::getCompressedVerticesMMAPFilename(filename), std::ios::binary | std::ios::out | std::ios::trunc);
        if(!stream.is_open())
        {
            assert(false);
        }
        
        for(ui32 i = 0; i < m_size.x * m_size.y; ++i)
        {
            SCompressedVertex vertex;
            vertex.m_position = m_uncompressedVertices[i].m_position;
            vertex.m_normal = glm::packSnorm4x8(glm::vec4(m_uncompressedVertices[i].m_normal, 0.0f));
            vertex.m_texcoord = glm::packUnorm2x16(m_uncompressedVertices[i].m_texcoord);
            stream.write((char*)&vertex, sizeof(SCompressedVertex));
        }
        stream.close();
    }
    
#if !defined(__IOS__)
    
    { // writing uncompressed vertices metadata
        std::ofstream stream;
        stream.open(CHeightmap::getUncompressedVerticesMMAPFilename(filename), std::ios::binary | std::ios::out | std::ios::trunc);
        if(!stream.is_open())
        {
            assert(false);
        }
        
        for(ui32 i = 0; i < m_size.x * m_size.y; ++i)
        {
            stream.write((char*)&m_uncompressedVertices[i], sizeof(SUncomressedVertex));
        }
        stream.close();
        
        delete [] m_uncompressedVertices;
        m_uncompressedVertices = nullptr;
    }
    
#else
    
    delete [] m_uncompressedVertices;
    m_uncompressedVertices = nullptr;
    
#endif
    
#if !defined(__IOS__)
    
    { // writing faces metadata
        std::ofstream stream;
        stream.open(CHeightmap::getFacesMMAPFilename(filename), std::ios::binary | std::ios::out | std::ios::trunc);
        if(!stream.is_open())
        {
            assert(false);
        }
        
        for(ui32 i = 0; i < (m_size.x - 1) * (m_size.y - 1) * 2; ++i)
        {
            stream.write((char*)&m_faces[i], sizeof(SFace));
        }
        stream.close();
        
        delete [] m_faces;
        m_faces = nullptr;
    }
    
#else
    
    delete [] m_faces;
    m_faces = nullptr;
    
#endif
    
}

void CHeightmap::mmapVerticesMetadata(const std::string& filename)
{
    ui32 filelength;
    struct stat status;
    
    { // reading compressed vertices metadata
        m_compressedVerticesFiledescriptor = open(CHeightmap::getCompressedVerticesMMAPFilename(filename).c_str(), O_RDWR);
        if (m_compressedVerticesFiledescriptor < 0)
        {
            assert(false);
        }
        
        if (fstat(m_compressedVerticesFiledescriptor, &status) < 0)
        {
            assert(false);
        }
        
        filelength = (ui32)status.st_size;
        m_compressedVertices = (SCompressedVertex* )mmap(0, filelength, PROT_READ | PROT_WRITE, MAP_FILE | MAP_PRIVATE, m_compressedVerticesFiledescriptor, 0);
        if (!m_compressedVertices)
        {
            assert(false);
        }
    }
    
#if !defined(__IOS__)
    
    { // reading uncompressed vertices metadata
        m_uncompressedVerticesFiledescriptor = open(CHeightmap::getUncompressedVerticesMMAPFilename(filename).c_str(), O_RDWR);
        if (m_uncompressedVerticesFiledescriptor < 0)
        {
            assert(false);
        }
        
        if (fstat(m_uncompressedVerticesFiledescriptor, &status) < 0)
        {
            assert(false);
        }
        
        filelength = (ui32)status.st_size;
        m_uncompressedVertices = (SUncomressedVertex* )mmap(0, filelength, PROT_READ | PROT_WRITE, MAP_FILE | MAP_PRIVATE, m_uncompressedVerticesFiledescriptor, 0);
        if (!m_uncompressedVertices)
        {
            assert(false);
        }
    }
    
    { // reading faces metadata
        m_facesFiledescriptor = open(CHeightmap::getFacesMMAPFilename(filename).c_str(), O_RDWR);
        if (m_facesFiledescriptor < 0)
        {
            assert(false);
        }
        
        if (fstat(m_facesFiledescriptor, &status) < 0)
        {
            assert(false);
        }
        
        filelength = (ui32)status.st_size;
        m_faces = (SFace* )mmap(0, filelength, PROT_READ | PROT_WRITE, MAP_FILE | MAP_PRIVATE, m_facesFiledescriptor, 0);
        if (!m_faces)
        {
            assert(false);
        }
    }
    
#endif
}

void CHeightmap::create(const std::string& filename, const std::function<void(void)>& callback)
{
    CSharedThreadOperation completionOperation = std::make_shared<CThreadOperation>(E_THREAD_OPERATION_QUEUE_MAIN);
    completionOperation->setExecutionBlock([this, callback](void) {
        delete [] m_heights;
        callback();
    });
    
    CSharedThreadOperation readHeightmapMetadataOperation = std::make_shared<CThreadOperation>(E_THREAD_OPERATION_QUEUE_BACKGROUND);
    readHeightmapMetadataOperation->setExecutionBlock([this, filename](void) {
       CHeightmap::readHeightmapMetadata(filename);
    });
    completionOperation->addDependency(readHeightmapMetadataOperation);

    bool isCompressedVerticesMetadataExist = false;
    
#if defined(__IOS__)
    
    std::ifstream stream;
    stream.open(CHeightmap::getCompressedVerticesMMAPFilename(filename), std::ios::binary | std::ios::in);
    if(stream.is_open())
    {
        isCompressedVerticesMetadataExist = true;
    }
    stream.close();
    
#endif
    
    if(!isCompressedVerticesMetadataExist)
    {
        CSharedThreadOperation createVerticesOperation = std::make_shared<CThreadOperation>(E_THREAD_OPERATION_QUEUE_BACKGROUND);
        createVerticesOperation->setExecutionBlock([this](void) {
            CHeightmap::createVerticesMetadata();
        });
        completionOperation->addDependency(createVerticesOperation);
        
        CSharedThreadOperation writeVerticesMetadataOperation = std::make_shared<CThreadOperation>(E_THREAD_OPERATION_QUEUE_BACKGROUND);
        writeVerticesMetadataOperation->setExecutionBlock([this, filename](void) {
            CHeightmap::writeVerticesMetadata(filename);
        });
        completionOperation->addDependency(writeVerticesMetadataOperation);
    }

    CSharedThreadOperation mmapVerticesMetadataOperation = std::make_shared<CThreadOperation>(E_THREAD_OPERATION_QUEUE_BACKGROUND);
    mmapVerticesMetadataOperation->setExecutionBlock([this, filename](void) {
        CHeightmap::mmapVerticesMetadata(filename);
    });
    completionOperation->addDependency(mmapVerticesMetadataOperation);
    
    completionOperation->addToExecutionQueue();
}

void CHeightmap::create(const glm::ivec2 &size, f32 frequency, i32 octaves, ui32 seed, const std::function<void ()> &callback)
{
    CHeightmap::generateHeightmapMetadata(size, frequency, octaves, seed);
    CHeightmap::createVerticesMetadata();
    CHeightmap::writeVerticesMetadata("");
    CHeightmap::mmapVerticesMetadata("");
}

glm::vec3 CHeightmap::getVertexPosition(ui32 i, ui32 j) const
{
    return m_compressedVertices[i + j * m_size.x].m_position;
}

glm::uint32 CHeightmap::getVertexTexcoord(ui32 i, ui32 j) const
{
    return m_compressedVertices[i + j * m_size.x].m_texcoord;
}

glm::uint32 CHeightmap::getVertexNormal(ui32 i, ui32 j) const
{
    return m_compressedVertices[i + j * m_size.x].m_normal;
}

void CHeightmap::updateVertices(const std::vector<glm::vec3>& vertices)
{
    for(ui32 i = 0; i < vertices.size(); ++i)
    {
        i32 x = static_cast<i32>(vertices.at(i).x);
        i32 z = static_cast<i32>(vertices.at(i).z);
        i32 index = x + z * m_size.x;
        m_uncompressedVertices[index].m_position = vertices.at(i);
    }
    
    for(ui32 i = 0; i < vertices.size(); ++i)
    {
        i32 x = static_cast<i32>(vertices.at(i).x);
        i32 z = static_cast<i32>(vertices.at(i).z);
        i32 index = x + z * m_size.x;
        for(ui32 j = 0; j < m_uncompressedVertices[index].m_containsInFaceSize; ++j)
        {
            ui32 face = m_uncompressedVertices[index].m_containsInFace[j];
            
            glm::vec3 point_01 = m_uncompressedVertices[m_faces[face].m_indexes[0]].m_position;
            glm::vec3 point_02 = m_uncompressedVertices[m_faces[face].m_indexes[1]].m_position;
            glm::vec3 point_03 = m_uncompressedVertices[m_faces[face].m_indexes[2]].m_position;
            
            glm::vec3 edge_01 = point_02 - point_01;
            glm::vec3 edge_02 = point_03 - point_01;
            glm::vec3 normal = glm::cross(edge_01, edge_02);
            f32 sin = glm::length(normal) / (glm::length(edge_01) * glm::length(edge_02));
            m_faces[face].m_normal = glm::normalize(normal) * asinf(sin);
        }
    }
    
    for(ui32 i = 0; i < vertices.size(); ++i)
    {
        i32 x = static_cast<i32>(vertices.at(i).x);
        i32 z = static_cast<i32>(vertices.at(i).z);
        i32 index = x + z * m_size.x;
        assert(m_uncompressedVertices[index].m_containsInFaceSize != 0 && m_uncompressedVertices[index].m_containsInFaceSize <= kMaxContainsInFace);
        glm::vec3 normal = m_faces[m_uncompressedVertices[index].m_containsInFace[0]].m_normal;
        for(ui32 j = 1; j < m_uncompressedVertices[index].m_containsInFaceSize; ++j)
        {
            normal += m_faces[m_uncompressedVertices[index].m_containsInFace[j]].m_normal;
        }
        m_uncompressedVertices[index].m_normal = glm::normalize(normal);
        m_compressedVertices[index].m_position = m_uncompressedVertices[index].m_position;
        m_compressedVertices[index].m_normal = glm::packSnorm4x8(glm::vec4(m_uncompressedVertices[index].m_normal, 0.0));
    }
}

void CHeightmap::attachUncompressedVertexToVBO(ui32 x, ui32 y, ui32 vboIndex, ui32 vboVertexIndex)
{
    m_uncompressedVertices[x + y * m_size.x].m_containsInVBO[m_uncompressedVertices[x + y * m_size.x].m_containsInVBOSize++] = glm::ivec2(vboIndex, vboVertexIndex);
    assert(m_uncompressedVertices[x + y * m_size.x].m_containsInVBOSize <= kMaxContainsInVBO);
}

glm::ivec2* CHeightmap::isVertexAttachedToVBO(ui32 x, ui32 y, ui8* containsInCount)
{
    i32 index = x + y * m_size.x;
    assert(m_uncompressedVertices[index].m_containsInVBOSize != 0 && m_uncompressedVertices[index].m_containsInVBOSize <= kMaxContainsInVBO);
    if(m_uncompressedVertices[index].m_containsInVBOSize != 0)
    {
        *containsInCount = m_uncompressedVertices[index].m_containsInVBOSize;
        return m_uncompressedVertices[index].m_containsInVBO;
    }
    return nullptr;
}

glm::ivec2 CHeightmap::getSize(void) const
{
    return m_size;
}

f32 CHeightmap::getMaxHeight(void) const
{
    auto max = std::max_element(m_compressedVertices, m_compressedVertices + m_size.x * m_size.y, [](SCompressedVertex const& value_01, SCompressedVertex const& value_02) {
        return value_01.m_position.y > value_02.m_position.y;
    });
    return max->m_position.y;
}

f32 CHeightmap::getMinHeight(void) const
{
    auto min = std::max_element(m_compressedVertices, m_compressedVertices + m_size.x * m_size.y, [] (SCompressedVertex const& value_01, SCompressedVertex const& value_02) {
        return value_01.m_position.y < value_02.m_position.y;
    });
    return min->m_position.y;
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

glm::vec3 CHeightmapAccessor::getNormal(CSharedHeightmapRef data, const glm::vec3& position)
{
    f32 _x = position.x / 1.0;
    f32 _z = position.z / 1.0;
    i32 x = static_cast<i32>(floor(_x));
    i32 z = static_cast<i32>(floor(_z));
    
    if((x < 0) || (z < 0) || (x > (data->getSize().x - 1)) || (z > (data->getSize().y - 1)))
    {
        return glm::vec3(0.0f, 1.0f, 0.0f);
    }
    
    glm::vec4 normal_00 = glm::unpackSnorm4x8(data->getVertexNormal(x, z));
    
    glm::vec4 normal_01 = glm::unpackSnorm4x8(data->getVertexNormal(x, z));
    if(z < (data->getSize().y - 1) && z >= 0)
    {
        normal_01 = glm::unpackSnorm4x8(data->getVertexNormal(x, z + 1));
    }
    
    glm::vec4 normal_10 = glm::unpackSnorm4x8(data->getVertexNormal(x, z));
    if(x < (data->getSize().x - 1) && x >= 0)
    {
        normal_10 = glm::unpackSnorm4x8(data->getVertexNormal(x + 1, z));
    }
    
    glm::vec4 normal_11 = glm::unpackSnorm4x8(data->getVertexNormal(x, z));
    if(z < (data->getSize().y - 1) && z >= 0 && x < (data->getSize().x - 1) && x >= 0)
    {
        normal_11 =  glm::unpackSnorm4x8(data->getVertexNormal(x + 1, z + 1));
    }
    
    glm::vec4 normal = (normal_00 + normal_01 + normal_10 + normal_11) / 4.0f;
    return glm::normalize(glm::vec3(normal.x, normal.y, normal.z));
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

namespace ie
{
    i32 mmap_memory::g_filedescriptors = 0;
    mmap_memory::mmap_memory(void) :
    m_filedescriptor(-1),
    m_pointer(nullptr)
    {
        
    }
    
    mmap_memory::~mmap_memory(void)
    {
        mmap_memory::deallocate();
    }
        
    void* mmap_memory::allocate(const std::string& filename)
    {
        if(m_filedescriptor != -1)
        {
            assert(m_pointer != nullptr);
            return m_pointer;
        }
        
        ui32 filelength;
        struct stat status;
        
        m_filedescriptor = ::open(filename.c_str(), O_RDWR);
        if (m_filedescriptor < 0)
        {
            std::cout<<"can't open filedescriptor for filename: "<<filename<<"filedescriptors count: "<<g_filedescriptors<<std::endl;
            assert(false);
        }
        g_filedescriptors++;
        
        if (fstat(m_filedescriptor, &status) < 0)
        {
            std::cout<<"can't retrive filedescriptor status for filename: "<<filename<<"filedescriptors count: "<<g_filedescriptors<<std::endl;
            mmap_memory::deallocate();
            assert(false);
        }
        
        filelength = (ui32)status.st_size;
        m_pointer = (void* )mmap(0, filelength, PROT_READ | PROT_WRITE, MAP_FILE | MAP_PRIVATE, m_filedescriptor, 0);
        if (m_pointer == nullptr)
        {
            std::cout<<"can't mmap filedescriptor for filename: "<<filename<<"filedescriptors count: "<<g_filedescriptors<<std::endl;
            mmap_memory::deallocate();
            assert(false);
        }
        f32 size = static_cast<f32>(filelength) / (1024.0 * 1024);
        std::cout<<"filedescriptor was allocated. filedescriptors count: "<<g_filedescriptors<<" size: "<<size<<"mb"<<std::endl;
        m_filename = filename;
        return m_pointer;
    }
    
    void mmap_memory::deallocate(void)
    {
        if(m_filedescriptor >= 0)
        {
            ::close(m_filedescriptor);
            g_filedescriptors--;
            std::cout<<"filedescriptor was deallocated. filedescriptors count: "<<g_filedescriptors<<std::endl;
        }
        m_filedescriptor = -1;
        m_pointer = nullptr;
    }
    
    void mmap_memory::reallocate(void)
    {
        mmap_memory::deallocate();
        mmap_memory::allocate(m_filename);
    }
};

CHeightmapMMAP::CHeightmapMMAP(const std::shared_ptr<ie::mmap_memory>& descriptor) :
m_descriptor(descriptor),
m_size(0),
m_offset(0)
{
    
}

CHeightmapGenerator::CHeightmapGenerator(ISharedRenderTechniqueAccessorRef renderTechniqueAccessor) :
m_heightmapGUID(g_heightmapGUID++),
m_isGenerated(false),
m_heightmap(std::make_shared<CHeightmap>()),
m_renderTechniqueAccessor(renderTechniqueAccessor),
m_heightmapTexture(nullptr),
m_splattingTexture(nullptr),
m_vbosMMAPDescriptor(nullptr),
m_ibosMMAPDescriptor(nullptr),
m_texturesMMAPDescriptor(nullptr)
{
    assert(m_renderTechniqueAccessor != nullptr);
}

CHeightmapGenerator::~CHeightmapGenerator(void)
{
    
}

void CHeightmapGenerator::createContainers(const std::shared_ptr<CHeightmap>& heightmap)
{
    m_chunkSize = glm::ivec2(MIN_VALUE(m_heightmap->getSize().x, kMaxChunkSize),
                             MIN_VALUE(m_heightmap->getSize().y, kMaxChunkSize));
    
    m_chunkLODsSizes.at(E_LANDSCAPE_CHUNK_LOD_01) = glm::ivec2(MIN_VALUE(m_heightmap->getSize().x, m_chunkSize.x),
                                                               MIN_VALUE(m_heightmap->getSize().y, m_chunkSize.y));
    
    m_chunkLODsSizes.at(E_LANDSCAPE_CHUNK_LOD_02) = glm::ivec2(MIN_VALUE(m_heightmap->getSize().x, floor(static_cast<f32>(kMaxChunkSize) / 2.0f)),
                                                               MIN_VALUE(m_heightmap->getSize().y, floor(static_cast<f32>(kMaxChunkSize) / 2.0f)));
    
    m_chunkLODsSizes.at(E_LANDSCAPE_CHUNK_LOD_03) = glm::ivec2(MIN_VALUE(m_heightmap->getSize().x, ceil(static_cast<f32>(kMaxChunkSize) / 4.0f)),
                                                               MIN_VALUE(m_heightmap->getSize().y, ceil(static_cast<f32>(kMaxChunkSize) / 4.0f)));
    
    m_chunkLODsSizes.at(E_LANDSCAPE_CHUNK_LOD_04) = glm::ivec2(MIN_VALUE(m_heightmap->getSize().x, floor(static_cast<f32>(kMaxChunkSize) / 8.0f)),
                                                               MIN_VALUE(m_heightmap->getSize().y, floor(static_cast<f32>(kMaxChunkSize) / 8.0f)));
    
    m_chunksNum = glm::ivec2(m_heightmap->getSize().x / (m_chunkSize.x - 1),
                             m_heightmap->getSize().y / (m_chunkSize.y - 1));
    
    m_callbacks.clear();
    m_callbacks.resize(m_chunksNum.x * m_chunksNum.y, std::make_tuple(nullptr, nullptr));
    
    m_chunksMetadata.clear();
    m_chunksMetadata.resize(m_chunksNum.x * m_chunksNum.y, std::make_tuple(nullptr, nullptr, E_LANDSCAPE_CHUNK_LOD_UNKNOWN));
    
    m_executedOperations.clear();
    m_executedOperations.resize(m_chunksNum.x * m_chunksNum.y, nullptr);
    
    m_chunksBounds.clear();
    m_chunksBounds.resize(m_chunksNum.x * m_chunksNum.y, std::make_tuple(glm::vec3( 4096.0f,  4096.0f,  4096.0f),
                                                                         glm::vec3(-4096.0f, -4096.0f, -4096.0f)));
    
    for(ui32 i = 0; i < m_chunksNum.x; ++i)
    {
        for(ui32 j = 0; j < m_chunksNum.y; ++j)
        {
            ui32 index = i + j * m_chunksNum.x;
            CHeightmapGenerator::createChunkBound(glm::ivec2(i, j), &std::get<0>(m_chunksBounds[index]), &std::get<1>(m_chunksBounds[index]));
            m_chunksBounds[index] = std::make_tuple(std::get<0>(m_chunksBounds[index]), std::get<1>(m_chunksBounds[index]));
        }
    }
}

void CHeightmapGenerator::generate(const std::string& filename, const std::function<void(void)>& callback)
{
    m_isGenerated = false;
    m_heightmap = std::make_shared<CHeightmap>();
    m_heightmap->create(filename, [this, callback]() {
        CHeightmapGenerator::createContainers(m_heightmap);
        
        CSharedThreadOperation completionOperation = std::make_shared<CThreadOperation>(E_THREAD_OPERATION_QUEUE_MAIN);
        completionOperation->setExecutionBlock([this, callback](void) {
            m_isGenerated = true;
            callback();
        });
        
        CSharedThreadOperation createVBOsOperation = std::make_shared<CThreadOperation>(E_THREAD_OPERATION_QUEUE_BACKGROUND);
        createVBOsOperation->setExecutionBlock([this](void) {
            if(m_vbosMMAPDescriptor)
            {
                m_vbosMMAPDescriptor->deallocate();
                m_vbosMMAPDescriptor = nullptr;
            }
            m_vbosMMAPDescriptor = std::make_shared<ie::mmap_memory>();
            m_vbosMMAP.clear();
            m_vbosMMAPDescriptor->allocate(CHeightmapGenerator::createVBOs());
        });
        completionOperation->addDependency(createVBOsOperation);
        
        CSharedThreadOperation createIBOsOperation = std::make_shared<CThreadOperation>(E_THREAD_OPERATION_QUEUE_BACKGROUND);
        createIBOsOperation->setExecutionBlock([this](void) {
            if(m_ibosMMAPDescriptor)
            {
                m_ibosMMAPDescriptor->deallocate();
                m_ibosMMAPDescriptor = nullptr;
            }
            m_ibosMMAPDescriptor = std::make_shared<ie::mmap_memory>();
            m_ibosMMAP.clear();
            m_ibosMMAPDescriptor->allocate(CHeightmapGenerator::createIBOs());
        });
        completionOperation->addDependency(createIBOsOperation);
        
        CSharedThreadOperation createTexturesOperation = std::make_shared<CThreadOperation>(E_THREAD_OPERATION_QUEUE_BACKGROUND);
        createTexturesOperation->setExecutionBlock([this](void) {
            if(m_texturesMMAPDescriptor)
            {
                m_texturesMMAPDescriptor->deallocate();
                m_texturesMMAPDescriptor = nullptr;
            }
            m_texturesMMAPDescriptor = std::make_shared<ie::mmap_memory>();
            m_texturesMMAP.clear();
            m_texturesMMAPDescriptor->allocate(CHeightmapGenerator::createTextures());
        });
        completionOperation->addDependency(createTexturesOperation);
        
        completionOperation->addToExecutionQueue();
    });
}

void CHeightmapGenerator::generate(const glm::ivec2& size, f32 frequency, i32 octaves, ui32 seed, const std::function<void(void)>& callback)
{
    glm::ivec2 sizeOffset = glm::ivec2(ceil(static_cast<f32>(size.x) / static_cast<f32>(kMaxChunkSize)),
                                       ceil(static_cast<f32>(size.y) / static_cast<f32>(kMaxChunkSize)));
    m_heightmap = std::make_shared<CHeightmap>();
    m_heightmap->create(glm::ivec2(size.x + sizeOffset.x, size.y + sizeOffset.y),
                        frequency, octaves,
                        seed, nullptr);
    
    CHeightmapGenerator::createContainers(m_heightmap);
    
    CHeightmapGenerator::updateSplattingTexture(m_splattingTexture);
    CHeightmapGenerator::updateHeightmapTexture(m_heightmapTexture);
}

bool CHeightmapGenerator::isGenerated(void)
{
    return m_isGenerated;
}

std::string CHeightmapGenerator::createVBOs(void)
{
    
#if defined(__PERFORMANCE_TIMER__)
    
    std::chrono::steady_clock::time_point startTimestamp = std::chrono::steady_clock::now();
    
#endif
    
    m_vbosMMAP.resize(m_chunksNum.x * m_chunksNum.y);
    
    std::string filename;
    std::ostringstream stringstream;
    stringstream<<"vbo_"<<m_heightmapGUID<<std::endl;
    filename = stringstream.str();
    
#if defined(__IOS__)
    
    filename = documentspath() + filename;
    
#endif
    
    std::ofstream stream;
    stream.open(filename.c_str(), std::ios::binary | std::ios::out | std::ios::trunc);
    if(!stream.is_open())
    {
        assert(false);
    }
    
    glm::ivec2 verticesOffset(0);
    ui32 verticesWroteToMMAP = 0;
    
    SAttributeVertex vertex;
    for(ui32 i = 0; i < m_chunksNum.x; ++i)
    {
        verticesOffset.y = 0;
        for(ui32 j = 0; j < m_chunksNum.y; ++j)
        {
            for(ui32 x = 0; x < m_chunkSize.x; ++x)
            {
                for(ui32 y = 0; y < m_chunkSize.y; ++y)
                {
                    vertex.m_position = m_heightmap->getVertexPosition(x + verticesOffset.x, y + verticesOffset.y);
                    vertex.m_normal = m_heightmap->getVertexNormal(x + verticesOffset.x, y + verticesOffset.y);
                    vertex.m_texcoord = glm::packUnorm2x16(glm::vec2(static_cast<f32>(x) / static_cast<f32>(m_chunkSize.x),
                                                                     static_cast<f32>(y) / static_cast<f32>(m_chunkSize.y)));
                    
                    stream.write((char*)&vertex, sizeof(SAttributeVertex));
                    
#if !defined(__IOS__)
                    
                    ui32 index = y + x * m_chunkSize.y;
                    m_heightmap->attachUncompressedVertexToVBO(x + verticesOffset.x, y + verticesOffset.y,
                                                               i + j * m_chunksNum.x, index);
                    
#endif
                }
            }
            verticesOffset.y += m_chunkSize.y - 1;
            
            m_vbosMMAP[i + j * m_chunksNum.x] = std::make_shared<CHeightmapVBOMMAP>(m_vbosMMAPDescriptor);
            m_vbosMMAP[i + j * m_chunksNum.x]->setSize(m_chunkSize.x * m_chunkSize.y);
            m_vbosMMAP[i + j * m_chunksNum.x]->setOffset(verticesWroteToMMAP);
            verticesWroteToMMAP += m_chunkSize.x * m_chunkSize.y;
        }
        verticesOffset.x += m_chunkSize.x - 1;
    }
    
    stream.close();
    
#if defined(__PERFORMANCE_TIMER__)
    
    std::chrono::steady_clock::time_point endTimestamp = std::chrono::steady_clock::now();
    f32 duration = std::chrono::duration_cast<std::chrono::microseconds>(endTimestamp - startTimestamp).count();
    std::cout<<"createVBOs: "<<duration<<" microseconds"<<std::endl;
    
#endif
    
    return filename;
}

std::string CHeightmapGenerator::createIBOs(void)
{
    
#if defined(__PERFORMANCE_TIMER__)
    
    std::chrono::steady_clock::time_point startTimestamp = std::chrono::steady_clock::now();
    
#endif
    
    m_ibosMMAP.resize(m_chunksNum.x * m_chunksNum.y);
    
    std::string filename;
    std::ostringstream stringstream;
    stringstream<<"ibo_"<<m_heightmapGUID<<std::endl;
    filename = stringstream.str();
    
#if defined(__IOS__)
    
    filename = documentspath() + filename;
    
#endif
    
    std::ofstream stream;
    stream.open(filename.c_str(), std::ios::binary | std::ios::out | std::ios::trunc);
    if(!stream.is_open())
    {
        assert(false);
    }
    
    ui32 indicesWroteToMMAP = 0;
    
    for(ui32 i = 0; i < m_chunksNum.x; ++i)
    {
        for(ui32 j = 0; j < m_chunksNum.y; ++j)
        {
            for(ui32 k = 0; k < E_LANDSCAPE_CHUNK_LOD_MAX; ++k)
            {
                glm::ivec2 currentChunkSize = glm::ivec2(m_chunkLODsSizes[k].x % 2 == 0 ? m_chunkLODsSizes[k].x : m_chunkLODsSizes[k].x - 1,
                                                         m_chunkLODsSizes[k].y % 2 == 0 ? m_chunkLODsSizes[k].y : m_chunkLODsSizes[k].y - 1);
                
                glm::ivec2 verticesLODOffset = glm::ivec2(MAX_VALUE((m_chunkSize.x - 1.0f) / currentChunkSize.x, 1),
                                                          MAX_VALUE((m_chunkSize.y - 1.0f) / currentChunkSize.y, 1));
                
                
                i32 verticesLineOffset = m_chunkSize.x;
                glm::ivec2 currentChunkLODStartIndex = glm::ivec2(0);
                currentChunkLODStartIndex.x += k != E_LANDSCAPE_CHUNK_LOD_01 ? 1 : 0;
                currentChunkLODStartIndex.y += k != E_LANDSCAPE_CHUNK_LOD_01 ? 1 : 0;
                
                std::vector<ui16> additionIndices;
                if(k != E_LANDSCAPE_CHUNK_LOD_01)
                {
                    std::vector<ui16> currentLODEdgeIndices;
                    for(ui32 x = 0; x <= currentChunkSize.x; ++x)
                    {
                        ui32 index = x * verticesLODOffset.x + verticesLineOffset * verticesLODOffset.y;
                        currentLODEdgeIndices.push_back(index);
                    }
                    
                    ui32 currentLODIndex = 0;
                    for(ui32 x = 0; x < m_chunkSize.x; ++x)
                    {
                        ui32 mainLODIndex = x;
                        if(currentLODEdgeIndices.size() > currentLODIndex + 1)
                        {
                            if(x != 0 && x % verticesLODOffset.x == 0)
                            {
                                additionIndices.push_back(currentLODEdgeIndices[currentLODIndex]);
                                additionIndices.push_back(currentLODEdgeIndices[currentLODIndex + 1]);
                                additionIndices.push_back(mainLODIndex);
                                currentLODIndex++;
                            }
                            
                            if(currentLODEdgeIndices.size() - 1 == currentLODIndex)
                            {
                                break;
                            }
                            else
                            {
                                additionIndices.push_back(mainLODIndex);
                                additionIndices.push_back(currentLODEdgeIndices[currentLODIndex]);
                                additionIndices.push_back(mainLODIndex + 1);
                            }
                        }
                    }
                    currentLODEdgeIndices.clear();

                    for(ui32 x = 0; x <= currentChunkSize.x; ++x)
                    {
                        ui32 index = x * verticesLODOffset.x + verticesLineOffset * verticesLODOffset.y * (currentChunkSize.y - 1);
                        currentLODEdgeIndices.push_back(index);
                    }
                    
                    currentLODIndex = 0;
                    for(ui32 x = 0; x < m_chunkSize.x; ++x)
                    {
                        ui32 mainLODIndex = x + verticesLineOffset * (m_chunkSize.x - 1);
                        if(currentLODEdgeIndices.size() > currentLODIndex + 1)
                        {
                            if(x != 0 && x % verticesLODOffset.x == 0)
                            {
                                additionIndices.push_back(currentLODEdgeIndices[currentLODIndex]);
                                additionIndices.push_back(mainLODIndex);
                                additionIndices.push_back(currentLODEdgeIndices[currentLODIndex + 1]);
                                currentLODIndex++;
                            }
                            
                            if(currentLODEdgeIndices.size() - 1 == currentLODIndex)
                            {
                                break;
                            }
                            else
                            {
                                additionIndices.push_back(currentLODEdgeIndices[currentLODIndex]);
                                additionIndices.push_back(mainLODIndex);
                                additionIndices.push_back(mainLODIndex + 1);

                            }
                        }
                    }
                    currentLODEdgeIndices.clear();

                    for(ui32 y = 0; y <= currentChunkSize.y; ++y)
                    {
                        ui32 index = verticesLODOffset.x + y * verticesLineOffset * verticesLODOffset.y;
                        currentLODEdgeIndices.push_back(index);
                    }
                    
                    currentLODIndex = 0;
                    for(ui32 y = 0; y < m_chunkSize.y; ++y)
                    {
                        ui32 mainLODIndex = y * m_chunkSize.y;
                        if(currentLODEdgeIndices.size() > currentLODIndex + 1)
                        {
                            if(y != 0 && y % verticesLODOffset.y == 0)
                            {
                                additionIndices.push_back(currentLODEdgeIndices[currentLODIndex]);
                                additionIndices.push_back(mainLODIndex);
                                additionIndices.push_back(currentLODEdgeIndices[currentLODIndex + 1]);
                                currentLODIndex++;
                            }
                            
                            if(currentLODEdgeIndices.size() - 1 == currentLODIndex)
                            {
                                break;
                            }
                            else
                            {
                                additionIndices.push_back(currentLODEdgeIndices[currentLODIndex]);
                                additionIndices.push_back(mainLODIndex);
                                additionIndices.push_back(mainLODIndex + m_chunkSize.y);
                            }
                        }
                    }
                    currentLODEdgeIndices.clear();
                    
                    for(ui32 y = 0; y <= currentChunkSize.y; ++y)
                    {
                        ui32 index = verticesLODOffset.x * (currentChunkSize.x - 1) + y * verticesLineOffset * verticesLODOffset.y;
                        currentLODEdgeIndices.push_back(index);
                    }
                    
                    currentLODIndex = 0;
                    for(ui32 y = 0; y < m_chunkSize.y; ++y)
                    {
                        ui32 mainLODIndex = (m_chunkSize.x - 1) + y * m_chunkSize.y;
                        if(currentLODEdgeIndices.size() > currentLODIndex + 1)
                        {
                            if(y != 0 && y % verticesLODOffset.y == 0)
                            {
                                additionIndices.push_back(currentLODEdgeIndices[currentLODIndex]);
                                additionIndices.push_back(currentLODEdgeIndices[currentLODIndex + 1]);
                                additionIndices.push_back(mainLODIndex);
                                currentLODIndex++;
                            }
                            
                            if(currentLODEdgeIndices.size() - 1 == currentLODIndex)
                            {
                                break;
                            }
                            else
                            {
                                additionIndices.push_back(mainLODIndex);
                                additionIndices.push_back(currentLODEdgeIndices[currentLODIndex]);
                                additionIndices.push_back(mainLODIndex + m_chunkSize.y);
                            }
                        }
                    }
                    currentLODEdgeIndices.clear();
                }
                
                currentChunkSize.x -= k != E_LANDSCAPE_CHUNK_LOD_01 ? 2 : 0;
                currentChunkSize.y -= k != E_LANDSCAPE_CHUNK_LOD_01 ? 2 : 0;
                
                std::cout<<"addition indices: "<<additionIndices.size()<<std::endl;
                ui32 indicesCount = currentChunkSize.x * currentChunkSize.y * 6 + static_cast<ui32>(additionIndices.size());
                
                currentChunkSize.x += k != E_LANDSCAPE_CHUNK_LOD_01 ? 1 : 0;
                currentChunkSize.y += k != E_LANDSCAPE_CHUNK_LOD_01 ? 1 : 0;
                
                ui16* indices = new ui16[indicesCount];
                
                ui32 index = 0;
                
                for(ui32 x = 0; x < additionIndices.size(); ++x)
                {
                    indices[index] = additionIndices[x];
                    index++;
                }
 
                for(ui32 x = currentChunkLODStartIndex.x; x < currentChunkSize.x; ++x)
                {
                    for(ui32 y = currentChunkLODStartIndex.y; y < currentChunkSize.y; ++y)
                    {
                        indices[index] = x * verticesLODOffset.x + y * verticesLODOffset.y * verticesLineOffset;
                        index++;
                        indices[index] = x * verticesLODOffset.x + (y * verticesLODOffset.y + verticesLODOffset.y) * verticesLineOffset;
                        index++;
                        indices[index] = x * verticesLODOffset.x + verticesLODOffset.x + y * verticesLODOffset.y * verticesLineOffset;
                        index++;
                        
                        indices[index] = x * verticesLODOffset.x + (y * verticesLODOffset.y + verticesLODOffset.y) * verticesLineOffset;
                        index++;
                        indices[index] = x * verticesLODOffset.x + verticesLODOffset.x + (y * verticesLODOffset.y + verticesLODOffset.y) * verticesLineOffset;
                        index++;
                        indices[index] = x * verticesLODOffset.x + verticesLODOffset.x + y * verticesLODOffset.y * verticesLineOffset;
                        index++;
                    }
                }
                
                for(ui32 index_01 = 0; index_01 < 2; ++index_01)
                {
                    for(ui32 index_02 = 0; index_02 < indicesCount; ++index_02)
                    {
                        stream.write((char*)&indices[index_02], sizeof(ui16));
                    }
                }

                delete [] indices;
                
                m_ibosMMAP[i + j * m_chunksNum.x][k] = std::make_shared<CHeightmapIBOMMAP>(m_ibosMMAPDescriptor);
                m_ibosMMAP[i + j * m_chunksNum.x][k]->setSize(indicesCount);
                m_ibosMMAP[i + j * m_chunksNum.x][k]->setOffset(indicesWroteToMMAP);
                indicesWroteToMMAP += indicesCount * 2;
            }
        }
    }
    stream.close();
    
#if defined(__PERFORMANCE_TIMER__)
    
    std::chrono::steady_clock::time_point endTimestamp = std::chrono::steady_clock::now();
    f32 duration = std::chrono::duration_cast<std::chrono::microseconds>(endTimestamp - startTimestamp).count();
    std::cout<<"createIBOs: "<<duration<<" microseconds"<<std::endl;
    
#endif
    
    return filename;
}

std::string CHeightmapGenerator::createTextures(void)
{
    
#if defined(__PERFORMANCE_TIMER__)
    
    std::chrono::steady_clock::time_point startTimestamp = std::chrono::steady_clock::now();
    
#endif
    m_texturesMMAP.resize(m_chunksNum.x * m_chunksNum.y);
    
    std::string filename;
    std::ostringstream stringstream;
    stringstream<<"textures_"<<m_heightmapGUID<<std::endl;
    filename = stringstream.str();
    
#if defined(__IOS__)
    
    filename = documentspath() + filename;
    
#endif
    
    std::ofstream stream;
    stream.open(filename.c_str(), std::ios::binary | std::ios::out | std::ios::trunc);
    if(!stream.is_open())
    {
        assert(false);
    }
    
    glm::ivec2 verticesOffset(0);
    ui32 pixelsWroteToMMAP = 0;
    
    glm::ivec2 size = glm::ivec2(32, 32);
    glm::vec2 step = glm::vec2(static_cast<f32>(m_chunkSize.x) / static_cast<f32>(size.x) ,
                               static_cast<f32>(m_chunkSize.y) / static_cast<f32>(size.y));

    glm::vec3 offset = glm::vec3(0.0f);
    
    ui16* pixels = new ui16[size.x * size.y];
    
    for(ui32 i = 0; i < m_chunksNum.x; ++i)
    {
        verticesOffset.y = 0;
        for(ui32 j = 0; j < m_chunksNum.y; ++j)
        {
            offset = glm::vec3(0.0f);
            for(ui32 x = 0; x < size.x; ++x)
            {
                offset.z = 0.0f;
                for(ui32 y = 0; y < size.y; ++y)
                {
                    ui32 index = x + y * size.x;
                    
                    pixels[index] = TO_RGB565(255, 0, 0);
                    f32 height = CHeightmapAccessor::getHeight(m_heightmap, glm::vec3(offset.x + verticesOffset.x, 0.0f, offset.z + verticesOffset.y));
                    glm::vec3 normal = CHeightmapAccessor::getNormal(m_heightmap, glm::vec3(offset.x + verticesOffset.x, 0.0f, offset.z + verticesOffset.y));
                    
                    f32 angle = glm::dot(glm::vec3(0.0f, 1.0f, 0.0f), normal);
                    angle = glm::degrees(acosf(angle));
                    assert(angle >= 0.0);

                    if(height > 1.0)
                    {
                        pixels[index] = TO_RGB565(glm::mix(0, 255, 1.0f - MIN(angle / 45.0f, 1.0f)), glm::mix(0, 255, MIN(angle / 45.0f, 1.0f)), 0);
                    }
                    
                    if(height <= 1.0f && height > 0.0f)
                    {
                        pixels[index] = TO_RGB565(glm::mix(0, 255, height), 0, glm::mix(0, 255, 1.0f - height));
                    }
                    
                    if(height <= 0.0f)
                    {
                        pixels[index] = TO_RGB565(0, 0, 255);
                    }
                    
                    offset.z += step.y;
                }
                offset.x += step.x;
            }
            verticesOffset.y += m_chunkSize.y - 1;
            
            i32 index = 0;
            for(ui32 x = 0; x < size.x; ++x)
            {
                for(ui32 y = 0; y < size.y; ++y)
                {
                    stream.write((char*)&pixels[index], sizeof(ui16));
                    index++;
                }
            }
            
            m_texturesMMAP[i + j * m_chunksNum.x] = std::make_shared<CHeightmapTextureMMAP>(m_texturesMMAPDescriptor);
            m_texturesMMAP[i + j * m_chunksNum.x]->setSize(size.x * size.y);
            m_texturesMMAP[i + j * m_chunksNum.x]->setOffset(pixelsWroteToMMAP);
            pixelsWroteToMMAP += size.x * size.y;
        }
        verticesOffset.x += m_chunkSize.x - 1;
    }
    
    stream.close();
    
    delete[] pixels;
    pixels = nullptr;
    
#if defined(__PERFORMANCE_TIMER__)
    
    std::chrono::steady_clock::time_point endTimestamp = std::chrono::steady_clock::now();
    f32 duration = std::chrono::duration_cast<std::chrono::microseconds>(endTimestamp - startTimestamp).count();
    std::cout<<"createTextures: "<<duration<<" microseconds"<<std::endl;
    
#endif
    
    return filename;
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

void CHeightmapGenerator::updateHeightmap(const glm::ivec2& minBound, const glm::ivec2& maxBound,
                                          const std::vector<glm::vec3>& vertices)
{
    assert(m_heightmap != nullptr);
    m_heightmap->updateVertices(vertices);
    
    std::set<std::shared_ptr<CVertexBuffer>> changedVBOs;
    for(ui32 i = 0; i < vertices.size(); ++i)
    {
        i32 x = static_cast<i32>(vertices.at(i).x);
        i32 z = static_cast<i32>(vertices.at(i).z);
        
        ui8 containsInCount = 0;
        glm::ivec2 *containsIn = m_heightmap->isVertexAttachedToVBO(x, z, &containsInCount);
        if(containsIn)
        {
            for(ui32 j = 0; j < containsInCount; ++j)
            {
                if(std::get<0>(m_chunksMetadata[containsIn[j].x]) != nullptr)
                {
                    std::get<0>(m_chunksMetadata[containsIn[j].x])->getVertexBuffer()->lock()[containsIn[j].y].m_position =
                    m_heightmap->getVertexPosition(x, z);
                    
                    std::get<0>(m_chunksMetadata[containsIn[j].x])->getVertexBuffer()->lock()[containsIn[j].y].m_normal =
                    m_heightmap->getVertexNormal(x, z);
                    std::get<0>(m_chunksMetadata[containsIn[j].x])->getVertexBuffer()->unlock();
                }
            }
        }
    }
    
    for(ui32 i = 0; i < m_chunksNum.x; ++i)
    {
        for(ui32 j = 0; j < m_chunksNum.y; ++j)
        {
            ui32 index = i + j * m_chunksNum.x;
            if(CBoundingBox::isPointInXZ(glm::vec2(minBound.x, minBound.y), std::get<0>(m_chunksBounds[index]), std::get<1>(m_chunksBounds[index])) ||
               CBoundingBox::isPointInXZ(glm::vec2(maxBound.x, minBound.y), std::get<0>(m_chunksBounds[index]), std::get<1>(m_chunksBounds[index])) ||
               CBoundingBox::isPointInXZ(glm::vec2(minBound.x, maxBound.y), std::get<0>(m_chunksBounds[index]), std::get<1>(m_chunksBounds[index])) ||
               CBoundingBox::isPointInXZ(glm::vec2(maxBound.x, maxBound.y), std::get<0>(m_chunksBounds[index]), std::get<1>(m_chunksBounds[index])))
            {
                CHeightmapGenerator::createChunkBound(glm::ivec2(i, j), &std::get<0>(m_chunksBounds[index]), &std::get<1>(m_chunksBounds[index]));
            }
        }
    }
    CHeightmapGenerator::updateSplattingTexture(m_splattingTexture, false,
                                                minBound.x, minBound.y,
                                                maxBound.x - minBound.x, maxBound.y - minBound.y);
    
    CHeightmapGenerator::updateHeightmapTexture(m_heightmapTexture, false,
                                                minBound.x, minBound.y,
                                                maxBound.x - minBound.x, maxBound.y - minBound.y);
}

ui32 CHeightmapGenerator::createTextureId(void)
{
    ui32 textureId;
    ieGenTextures(1, &textureId);
    ieBindTexture(GL_TEXTURE_2D, textureId);
    ieTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    ieTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    ieTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    ieTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
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
        f32 maxDeep = fabsf(m_heightmap->getMinHeight());
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
        
        ieTexImage2D(GL_TEXTURE_2D, 0,
#if defined(__OPENGL_30__)
                     GL_RED,
#else
                     GL_ALPHA,
#endif
                     m_heightmap->getSize().x,
                     m_heightmap->getSize().y,
                     0,
#if defined(__OPENGL_30__)
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
        
        f32 maxDeep = fabsf(m_heightmap->getMinHeight());
        
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
#if defined(__OPENGL_30__)
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
        ieTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
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


void CHeightmapGenerator::createMesh(ui32 index, E_LANDSCAPE_CHUNK_LOD LOD)
{
    m_ibosMMAP[index][LOD]->updateSourcePointer();
    
    std::shared_ptr<CIndexBuffer> ibo = std::make_shared<CIndexBuffer>(m_ibosMMAP[index][LOD]->getSize(),
                                                                       GL_DYNAMIC_DRAW,
                                                                       m_ibosMMAP[index][LOD]->getSourcePointer());
    ibo->unlock();
    
    std::shared_ptr<CVertexBuffer> vbo = std::make_shared<CVertexBuffer>(m_vbosMMAP[index]->getSize(),
                                                                         GL_STATIC_DRAW,
                                                                         m_vbosMMAP[index]->getPointer());
    vbo->unlock();
    
    std::ostringstream stringstream;
    stringstream<<"chunk_"<<index<<"_"<<LOD<<"_"<<m_heightmapGUID<<std::endl;
    std::shared_ptr<CMesh> mesh = CMesh::construct(stringstream.str(), vbo, ibo,
                                                   std::get<0>(m_chunksBounds[index]), std::get<1>(m_chunksBounds[index]));
    std::get<0>(m_chunksMetadata[index]) = mesh;
}

void CHeightmapGenerator::generateQuadTree(ui32 index)
{
    assert(std::get<0>(m_chunksMetadata[index]) != nullptr);
    
    CSharedQuadTree quadTree = std::make_shared<CQuadTree>();
    
    quadTree->generate(std::get<0>(m_chunksMetadata[index])->getVertexBuffer(),
                       std::get<0>(m_chunksMetadata[index])->getIndexBuffer(),
                       std::get<0>(m_chunksMetadata[index])->getMaxBound(),
                       std::get<0>(m_chunksMetadata[index])->getMinBound(),
                       4, m_chunkLODsSizes.at(0).x);
    
    std::get<1>(m_chunksMetadata[index]) = quadTree;
}

CSharedTexture CHeightmapGenerator::generateDiffuseTexture(ui32 index, CSharedMaterialRef material)
{
    assert(m_texturesMMAP[index]->getPointer() != nullptr);
    
    std::ostringstream stringstream;
    stringstream<<"texture_"<<index<<"_"<<m_heightmapGUID<<std::endl;
    
    ui32 textureId = CHeightmapGenerator::createTextureId();
    glm::ivec2 size = glm::ivec2(sqrt(m_texturesMMAP[index]->getSize()));
    CSharedTexture texture = CTexture::constructCustomTexture(stringstream.str(), textureId,
                                                              size.x, size.y);
    texture->setWrapMode(GL_CLAMP_TO_EDGE);
    texture->setMagFilter(GL_LINEAR);
    texture->setMinFilter(GL_LINEAR);
    
    texture->bind();
    
    ieTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 size.x, size.y,
                 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, m_texturesMMAP[index]->getPointer());
    
    material->setTexture(texture, E_SHADER_SAMPLER_04);
    
    texture = m_renderTechniqueAccessor->preprocessTexture(material, 256, 256, true);
    material->setTexture(nullptr, E_SHADER_SAMPLER_04);
    
    texture->setWrapMode(GL_CLAMP_TO_EDGE);
    texture->setMagFilter(GL_LINEAR);
    texture->setMinFilter(GL_LINEAR);
    
    return texture;
}

void CHeightmapGenerator::runChunkLoading(ui32 i, ui32 j, E_LANDSCAPE_CHUNK_LOD LOD,
                                          CSharedMaterialRef preprocessSplattingTextureMaterial,
                                          const std::function<void(CSharedMeshRef)>& meshCreatedCallback,
                                          const std::function<void(CSharedQuadTreeRef)>& quadTreeGeneratedCallback,
                                          const std::function<void(CSharedTextureRef)>& textureGeneratedCallback)
{
    ui32 index = i + j * m_chunksNum.x;
    if(m_executedOperations[index] != nullptr)
    {
        return;
    }

    std::get<0>(m_callbacks[index]) = meshCreatedCallback;
    std::get<1>(m_callbacks[index]) = quadTreeGeneratedCallback;
    
    if(std::get<0>(m_chunksMetadata[index]) != nullptr)
    {
        std::get<0>(m_chunksMetadata[index]) = nullptr;
    }
    if(std::get<1>(m_chunksMetadata[index]) != nullptr)
    {
        std::get<1>(m_chunksMetadata[index]) = nullptr;
    }
    std::get<2>(m_chunksMetadata[index]) = LOD;
    
    assert(std::get<0>(m_callbacks[index]) != nullptr);
    assert(std::get<1>(m_callbacks[index]) != nullptr);
    
    assert(std::get<0>(m_chunksMetadata[index]) == nullptr);
    assert(std::get<1>(m_chunksMetadata[index]) == nullptr);
    assert(std::get<2>(m_chunksMetadata[index]) != E_LANDSCAPE_CHUNK_LOD_UNKNOWN);
    
    CSharedThreadOperation completionOperation = std::make_shared<CThreadOperation>(E_THREAD_OPERATION_QUEUE_MAIN);
    completionOperation->setExecutionBlock([this, index](void) {
        assert(std::get<1>(m_callbacks[index]) != nullptr);
        std::get<1>(m_callbacks[index])(std::get<1>(m_chunksMetadata[index]));
        m_executedOperations[index] = nullptr;
    });
    
    if(preprocessSplattingTextureMaterial)
    {
        CSharedThreadOperation createDiffuseTexture = std::make_shared<CThreadOperation>(E_THREAD_OPERATION_QUEUE_MAIN);
        createDiffuseTexture->setExecutionBlock([this, index, preprocessSplattingTextureMaterial, textureGeneratedCallback](void) {
            CSharedTexture texture = CHeightmapGenerator::generateDiffuseTexture(index, preprocessSplattingTextureMaterial);
            textureGeneratedCallback(texture);
        });
        completionOperation->addDependency(createDiffuseTexture);
    }
    
    CSharedThreadOperation createMeshOperation = std::make_shared<CThreadOperation>(E_THREAD_OPERATION_QUEUE_MAIN);
    createMeshOperation->setExecutionBlock([this, index, LOD](void) {
        CHeightmapGenerator::createMesh(index, LOD);
        assert(std::get<0>(m_callbacks[index]) != nullptr);
        std::get<0>(m_callbacks[index])(std::get<0>(m_chunksMetadata[index]));
    });
    completionOperation->addDependency(createMeshOperation);
    
    CSharedThreadOperation generateQuadTreeOperation = std::make_shared<CThreadOperation>(E_THREAD_OPERATION_QUEUE_BACKGROUND);
    generateQuadTreeOperation->setExecutionBlock([this, index, LOD](void) {
        CHeightmapGenerator::generateQuadTree(index);
    });
    completionOperation->addDependency(generateQuadTreeOperation);
    
    assert(m_executedOperations[index] == nullptr);
    m_executedOperations[index] = completionOperation;
    
    std::thread::id main_thread_id = std::this_thread::get_id();
    
    completionOperation->setCancelBlock([this, index, main_thread_id](void) {
        assert(m_executedOperations[index] != nullptr);
        
        if(main_thread_id == std::this_thread::get_id())
        {
            CHeightmapGenerator::eraseChunkMetadata(index);
        }
        else
        {
            CSharedThreadOperation eraseOperation = std::make_shared<CThreadOperation>(E_THREAD_OPERATION_QUEUE_MAIN);
            eraseOperation->setExecutionBlock([this, index](void) {
                CHeightmapGenerator::eraseChunkMetadata(index);
            });
            eraseOperation->addToExecutionQueue();
        }
    });
    completionOperation->addToExecutionQueue();
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
        CHeightmapGenerator::eraseChunkMetadata(index);
    }
}

void CHeightmapGenerator::eraseChunkMetadata(ui32 index)
{
    std::get<0>(m_chunksMetadata[index]) = nullptr;
    std::get<1>(m_chunksMetadata[index]) = nullptr;
    std::get<2>(m_chunksMetadata[index]) = E_LANDSCAPE_CHUNK_LOD_UNKNOWN;
    
    std::get<0>(m_callbacks[index]) = nullptr;
    std::get<1>(m_callbacks[index]) = nullptr;
    
    m_executedOperations[index] = nullptr;
}

void CHeightmapGenerator::createChunkBound(const glm::ivec2& index,
                                           glm::vec3* minBound, glm::vec3* maxBound)
{
    assert(m_heightmap != nullptr);
    
    for(ui32 x = 0; x < m_chunkSize.x; ++x)
    {
        for(ui32 y = 0; y < m_chunkSize.y; ++y)
        {
            glm::ivec2 position = glm::ivec2(x + index.x * m_chunkSize.x - index.x,
                                             y + index.y * m_chunkSize.y - index.y);
            
            position.x = position.x < m_heightmap->getSize().x ? position.x : m_heightmap->getSize().x - 1;
            position.y = position.y < m_heightmap->getSize().y ? position.y : m_heightmap->getSize().y - 1;
            
            glm::vec3 point = m_heightmap->getVertexPosition(position.x, position.y);
            *minBound = glm::min(point, *minBound);
            *maxBound = glm::max(point, *maxBound);
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

