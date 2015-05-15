//
//  CHeightmapLoader.cpp
//  ie3D-Core
//
//  Created by sergey.sergeev on 5/11/15.
//  Copyright (c) 2015 Sergey Sergeev. All rights reserved.
//

#include "CHeightmapLoader.h"
#include "CCommonOS.h"

static const std::string kUncompressedVerticesMetadataFilename = "-uncompressed.vertices.data_";
static const std::string kCompressedVerticesMetadataFilename = "-compressed.vertices.data_";
static const std::string kFacesMetadataFilename = "-faces.data_";
static const std::string kVBOsMetadataFilename = "-vbos.data_";
static const std::string kIBOsMetadataFilename = "-ibos.data_";
static const std::string kSplattingTextureMasksMetadataFilename = "-splatting.texture.masks.data_";

ui32 CHeightmapLoader::g_heightmapGUID = 0;

CHeightmapLoader::CHeightmapLoader(void)
{
    
}

CHeightmapLoader::~CHeightmapLoader(void)
{
    
}

std::tuple<glm::ivec2, std::vector<f32>> CHeightmapLoader::getHeights(const std::string& filename)
{
    std::vector<f32> heights;
    glm::ivec2 size;
    std::ifstream stream(bundlepath().append(filename).c_str());
    if(!stream.is_open())
    {
        assert(false);
    }
    else
    {
        stream.read((char*)&size, sizeof(glm::ivec2));
        heights.resize(size.x * size.y);
        
        for(ui32 i = 0; i < heights.size(); ++i)
        {
            stream.read((char*)&heights[i], sizeof(f32));
        }
        stream.close();
    }
    return std::make_tuple(size, heights);
}

std::string CHeightmapLoader::getUncompressedVerticesMMAPFilename(const std::string &filename)
{
    std::ostringstream stringstream;
    stringstream<<filename<<kUncompressedVerticesMetadataFilename<<g_heightmapGUID;
    
#if defined(__IOS__)
    
    return documentspath() + stringstream.str();
    
#endif
    
    return stringstream.str();
}

std::string CHeightmapLoader::getCompressedVerticesMMAPFilename(const std::string& filename)
{
    std::ostringstream stringstream;
    stringstream<<filename<<kCompressedVerticesMetadataFilename<<g_heightmapGUID;
    
#if defined(__IOS__)
    
    return documentspath() + stringstream.str();
    
#endif
    
    return stringstream.str();
}

std::string CHeightmapLoader::getFacesMMAPFilename(const std::string& filename)
{
    std::ostringstream stringstream;
    stringstream<<filename<<kFacesMetadataFilename<<g_heightmapGUID;
    
#if defined(__IOS__)
    
    return documentspath() + stringstream.str();
    
#endif
    
    return stringstream.str();
}

std::string CHeightmapLoader::getVBOsMMAPFilename(const std::string &filename)
{
    std::ostringstream stringstream;
    stringstream<<filename<<kVBOsMetadataFilename<<g_heightmapGUID;
    
#if defined(__IOS__)
    
    return documentspath() + stringstream.str();
    
#endif
    
    return stringstream.str();
}

std::string CHeightmapLoader::getIBOsMMAPFilename(const std::string &filename)
{
    std::ostringstream stringstream;
    stringstream<<filename<<kIBOsMetadataFilename<<g_heightmapGUID;
    
#if defined(__IOS__)
    
    return documentspath() + stringstream.str();
    
#endif
    
    return stringstream.str();
}

std::string CHeightmapLoader::getSplattingTextureMasksMMAPFilename(const std::string &filename)
{
    std::ostringstream stringstream;
    stringstream<<filename<<kSplattingTextureMasksMetadataFilename<<g_heightmapGUID;
    
#if defined(__IOS__)
    
    return documentspath() + stringstream.str();
    
#endif
    
    return stringstream.str();
}

bool CHeightmapLoader::isUncompressedVerticesMMAPExist(const std::string& filename)
{
    std::ifstream stream(CHeightmapLoader::getUncompressedVerticesMMAPFilename(filename));
    bool isExist = stream.good();
    stream.close();
    return isExist;
}

bool CHeightmapLoader::isCompressedVerticesMMAPExist(const std::string& filename)
{
    std::ifstream stream(CHeightmapLoader::getCompressedVerticesMMAPFilename(filename));
    bool isExist = stream.good();
    stream.close();
    return isExist;
}

bool CHeightmapLoader::isFacesMMAPExist(const std::string& filename)
{
    std::ifstream stream(CHeightmapLoader::getFacesMMAPFilename(filename));
    bool isExist = stream.good();
    stream.close();
    return isExist;
}

bool CHeightmapLoader::isVBOsMMAPExist(const std::string& filename)
{
    std::ifstream stream(CHeightmapLoader::getVBOsMMAPFilename(filename));
    bool isExist = stream.good();
    stream.close();
    return isExist;
}

bool CHeightmapLoader::isIBOsMMAPExist(const std::string& filename)
{
    std::ifstream stream(CHeightmapLoader::getIBOsMMAPFilename(filename));
    bool isExist = stream.good();
    stream.close();
    return isExist;
}

bool CHeightmapLoader::isSplattingTextureMasksMMAPExist(const std::string &filename)
{
    std::ifstream stream(CHeightmapLoader::getSplattingTextureMasksMMAPFilename(filename));
    bool isExist = stream.good();
    stream.close();
    return isExist;
}
