//
//  CHeightmapContainer.cpp
//  ie3D-Core
//
//  Created by sergey.sergeev on 5/11/15.
//  Copyright (c) 2015 Sergey Sergeev. All rights reserved.
//

#include "CHeightmapContainer.h"
#include "CHeightmapLoader.h"

#if defined(__IOS__)

#import <UIKit/UIKit.h>

#elif defined(__OSX__)

#include <Cocoa/Cocoa.h>

#endif

CHeightmapContainer::CHeightmapContainer(void) :
m_uncompressedVertices(nullptr),
m_compressedVertices(nullptr),
m_faces(nullptr),
m_uncompressedVerticesFiledescriptor(-1),
m_compressedVerticesFiledescriptor(-1),
m_facesFiledescriptor(-1),
m_size(0)
{
    
}

CHeightmapContainer::~CHeightmapContainer(void)
{
    CHeightmapContainer::erase();
}

void CHeightmapContainer::create(const glm::ivec2& size)
{
    m_size = size;
    
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
    
    m_chunksNum = glm::ivec2(size.x / (m_chunkSize.x - 1),
                             size.y / (m_chunkSize.y - 1));
    
    m_uncompressedVertices = new SUncomressedVertex[m_size.x * m_size.y];
    m_compressedVertices = new SCompressedVertex[m_size.x * m_size.y];
    m_faces = new SFace[(m_size.x - 1) * (m_size.y - 1) * 2];
}

void CHeightmapContainer::erase(void)
{
    if(m_uncompressedVerticesFiledescriptor > 0)
    {
        ::close(m_uncompressedVerticesFiledescriptor);
        m_uncompressedVerticesFiledescriptor = -1;
    }
    else
    {
        delete [] m_uncompressedVertices;
    }
    m_uncompressedVertices = nullptr;
    
    if(m_compressedVerticesFiledescriptor > 0)
    {
        ::close(m_compressedVerticesFiledescriptor);
        m_compressedVerticesFiledescriptor = -1;
    }
    else
    {
        delete [] m_compressedVertices;
    }
    m_compressedVertices = nullptr;
    
    if(m_facesFiledescriptor > 0)
    {
        ::close(m_facesFiledescriptor);
        m_facesFiledescriptor = -1;
    }
    else
    {
        delete [] m_faces;
    }
    m_faces = nullptr;
}

void CHeightmapContainer::mmap(const std::string& filename)
{
    CHeightmapContainer::erase();
    
    ui32 filelength;
    struct stat status;
    
    { // reading compressed vertices metadata
        m_compressedVerticesFiledescriptor = open(CHeightmapLoader::getCompressedVerticesMMAPFilename(filename).c_str(), O_RDWR);
        if (m_compressedVerticesFiledescriptor < 0)
        {
            assert(false);
        }
        
        if (fstat(m_compressedVerticesFiledescriptor, &status) < 0)
        {
            assert(false);
        }
        
        filelength = (ui32)status.st_size;
        m_compressedVertices = (SCompressedVertex* )::mmap(0, filelength, PROT_READ | PROT_WRITE, MAP_FILE | MAP_PRIVATE, m_compressedVerticesFiledescriptor, 0);
        if (!m_compressedVertices)
        {
            assert(false);
        }
    }
    
    { // reading uncompressed vertices metadata
        m_uncompressedVerticesFiledescriptor = open(CHeightmapLoader::getUncompressedVerticesMMAPFilename(filename).c_str(), O_RDWR);
        if (m_uncompressedVerticesFiledescriptor < 0)
        {
            assert(false);
        }
        
        if (fstat(m_uncompressedVerticesFiledescriptor, &status) < 0)
        {
            assert(false);
        }
        
        filelength = (ui32)status.st_size;
        m_uncompressedVertices = (SUncomressedVertex* )::mmap(0, filelength, PROT_READ | PROT_WRITE, MAP_FILE | MAP_PRIVATE, m_uncompressedVerticesFiledescriptor, 0);
        if (!m_uncompressedVertices)
        {
            assert(false);
        }
    }
    
    { // reading faces metadata
        m_facesFiledescriptor = open(CHeightmapLoader::getFacesMMAPFilename(filename).c_str(), O_RDWR);
        if (m_facesFiledescriptor < 0)
        {
            assert(false);
        }
        
        if (fstat(m_facesFiledescriptor, &status) < 0)
        {
            assert(false);
        }
        
        filelength = (ui32)status.st_size;
        m_faces = (SFace* )::mmap(0, filelength, PROT_READ | PROT_WRITE, MAP_FILE | MAP_PRIVATE, m_facesFiledescriptor, 0);
        if (!m_faces)
        {
            assert(false);
        }
    }
}
