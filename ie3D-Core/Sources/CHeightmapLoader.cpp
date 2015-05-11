//
//  CHeightmapLoader.cpp
//  ie3D-Core
//
//  Created by sergey.sergeev on 5/11/15.
//  Copyright (c) 2015 Sergey Sergeev. All rights reserved.
//

#include "CHeightmapLoader.h"
#include "CCommonOS.h"

extern ui32 g_heightmapGUID;

static const std::string kUncompressedVerticesMetadataFilename = "uncompressed.vertices.data_";
static const std::string kCompressedVerticesMetadataFilename = "compressed.vertices.data_";
static const std::string kFacesMetadataFilename = "faces.data_";

CHeightmapLoader::CHeightmapLoader(void)
{
    
}

CHeightmapLoader::~CHeightmapLoader(void)
{
    
}

std::string CHeightmapLoader::getUncompressedVerticesMMAPFilename(const std::string &filename)
{
    std::ostringstream stringstream;
    stringstream<<filename<<kUncompressedVerticesMetadataFilename<<g_heightmapGUID<<std::endl;
    
#if defined(__IOS__)
    
    return documentspath() + stringstream.str();
    
#endif
    
    return stringstream.str();
}

std::string CHeightmapLoader::getCompressedVerticesMMAPFilename(const std::string& filename)
{
    std::ostringstream stringstream;
    stringstream<<filename<<kCompressedVerticesMetadataFilename<<g_heightmapGUID<<std::endl;
    
#if defined(__IOS__)
    
    return documentspath() + stringstream.str();
    
#endif
    
    return stringstream.str();
}

std::string CHeightmapLoader::getFacesMMAPFilename(const std::string& filename)
{
    std::ostringstream stringstream;
    stringstream<<filename<<kFacesMetadataFilename<<g_heightmapGUID<<std::endl;
    
#if defined(__IOS__)
    
    return documentspath() + stringstream.str();
    
#endif
    
    return stringstream.str();
}

bool CHeightmapLoader::isUncompressedVerticesMMAPExist(const std::string& filename)
{
    std::ofstream stream;
    stream.open(CHeightmapLoader::getUncompressedVerticesMMAPFilename(filename), std::ios::binary | std::ios::out | std::ios::trunc);
    bool isExist = stream.is_open();
    stream.close();
    return isExist;
}

bool CHeightmapLoader::isCompressedVerticesMMAPExist(const std::string& filename)
{
    std::ofstream stream;
    stream.open(CHeightmapLoader::getCompressedVerticesMMAPFilename(filename), std::ios::binary | std::ios::out | std::ios::trunc);
    bool isExist = stream.is_open();
    stream.close();
    return isExist;
}

bool CHeightmapLoader::isFacesMMAPExist(const std::string& filename)
{
    std::ofstream stream;
    stream.open(CHeightmapLoader::getFacesMMAPFilename(filename), std::ios::binary | std::ios::out | std::ios::trunc);
    bool isExist = stream.is_open();
    stream.close();
    return isExist;
}
