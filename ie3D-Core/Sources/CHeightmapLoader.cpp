//
//  CHeightmapLoader.cpp
//  ie3D-Core
//
//  Created by sergey.sergeev on 5/11/15.
//  Copyright (c) 2015 Sergey Sergeev. All rights reserved.
//

#include "CHeightmapLoader.h"
#include "CCommonOS.h"
#include "CHeightmapContainer.h"

#if defined(__IOS__)

#import <UIKit/UIKit.h>

#elif defined(__OSX__)

#include <Cocoa/Cocoa.h>

#endif

static const std::string kUncompressedVerticesMetadataFilename = "-uncompressed.vertices.data";
static const std::string kCompressedVerticesMetadataFilename = "-compressed.vertices.data";
static const std::string kFacesMetadataFilename = "-faces.data";
static const std::string kVBOsMetadataFilename = "-vbos.data";
static const std::string kIBOsMetadataFilename = "-ibos.data";
static const std::string kSplattingTextureM_MetadataFilename = "-splatting.textures.m.data";
static const std::string kSplattingTextureN_MetadataFilename = "-splatting.textures.n.data";
static const std::string kSplattingTextureH_MetadataFilename = "-splatting.textures.h.data";
static const std::string kSplattingTextures_MetadataFilename = "-splatting.textures.data";
static const std::string kTangentSpace_MetadataFilename = "-tangent.space.data";
static const std::string kAttachesToVBO_MetadataFilename = "-attaches.vbo.data";

CHeightmapLoader::CHeightmapLoader(void)
{
    
}

CHeightmapLoader::~CHeightmapLoader(void)
{
    
}

std::tuple<glm::ivec2, std::vector<f32>> CHeightmapLoader::getHeights(const std::string& filename)
{
    ui8* data = nullptr;
    
#if defined(__IOS__)
    
    std::vector<f32> heights;
    glm::ivec2 size;
    
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
    
    size = glm::ivec2(image.size.width, image.size.height);
    
    heights.resize(size.x * size.y);
    
    ui32 index = 0;
    for(ui32 i = 0; i < size.x; ++i)
    {
        for(ui32 j = 0; j < size.y; ++j)
        {
            heights[index++] = static_cast<f32>(data[(i + j * size.x) * 4 + 1]) / 255.f * CHeightmapContainer::kRaise - CHeightmapContainer::kDeep;
        }
    }
    
#elif defined(__OSX__)
    
    std::vector<f32> heights;
    glm::ivec2 size;
    
    NSImage* image = [NSImage imageNamed:[NSString stringWithCString:"map_01" encoding:NSUTF8StringEncoding]];
    CGImageSourceRef source = CGImageSourceCreateWithData((__bridge CFDataRef)[image TIFFRepresentation], NULL);
    CGImageRef mask =  CGImageSourceCreateImageAtIndex(source, 0, NULL);
    NSBitmapImageRep *bitmap = [[NSBitmapImageRep alloc] initWithCGImage:mask];
    data = [bitmap bitmapData];
    size = glm::ivec2(image.size.width, image.size.height);
    
    CFRelease(source);
    CFRelease(mask);
    
    heights.resize(size.x * size.y);
    
    ui32 index = 0;
    for(ui32 i = 0; i < size.x; ++i)
    {
        for(ui32 j = 0; j < size.y; ++j)
        {
            heights[index++] = static_cast<f32>(data[(i + j * size.x) * 4 + 1]) / 255.f * CHeightmapContainer::kRaise - CHeightmapContainer::kDeep;
        }
    }
    
#else
    
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
#endif
    
    return std::make_tuple(size, heights);
}

std::string CHeightmapLoader::getUncompressedVerticesMMAPFilename(const std::string &filename)
{
    std::ostringstream stringstream;
    stringstream<<filename<<kUncompressedVerticesMetadataFilename;
    
#if defined(__IOS__)
    
    return documentspath() + stringstream.str();
    
#endif
    
    return stringstream.str();
}

std::string CHeightmapLoader::getCompressedVerticesMMAPFilename(const std::string& filename)
{
    std::ostringstream stringstream;
    stringstream<<filename<<kCompressedVerticesMetadataFilename;
    
#if defined(__IOS__)
    
    return documentspath() + stringstream.str();
    
#endif
    
    return stringstream.str();
}

std::string CHeightmapLoader::getFacesMMAPFilename(const std::string& filename)
{
    std::ostringstream stringstream;
    stringstream<<filename<<kFacesMetadataFilename;
    
#if defined(__IOS__)
    
    return documentspath() + stringstream.str();
    
#endif
    
    return stringstream.str();
}

std::string CHeightmapLoader::getVBOsMMAPFilename(const std::string &filename)
{
    std::ostringstream stringstream;
    stringstream<<filename<<kVBOsMetadataFilename;
    
#if defined(__IOS__)
    
    return documentspath() + stringstream.str();
    
#endif
    
    return stringstream.str();
}

std::string CHeightmapLoader::getIBOsMMAPFilename(const std::string &filename)
{
    std::ostringstream stringstream;
    stringstream<<filename<<kIBOsMetadataFilename;
    
#if defined(__IOS__)
    
    return documentspath() + stringstream.str();
    
#endif
    
    return stringstream.str();
}

std::string CHeightmapLoader::getSplattingTextureM_MMapFilename(const std::string &filename)
{
    std::ostringstream stringstream;
    stringstream<<filename<<kSplattingTextureM_MetadataFilename;
    
#if defined(__IOS__)
    
    return documentspath() + stringstream.str();
    
#endif
    
    return stringstream.str();
}

std::string CHeightmapLoader::getSplattingTextureN_MMapFilename(const std::string& filename)
{
    std::ostringstream stringstream;
    stringstream<<filename<<kSplattingTextureN_MetadataFilename;
    
#if defined(__IOS__)
    
    return documentspath() + stringstream.str();
    
#endif
    
    return stringstream.str();
}

std::string CHeightmapLoader::getSplattingTextureH_MMapFilename(const std::string& filename)
{
    std::ostringstream stringstream;
    stringstream<<filename<<kSplattingTextureH_MetadataFilename;
    
#if defined(__IOS__)
    
    return documentspath() + stringstream.str();
    
#endif
    
    return stringstream.str();
}

std::string CHeightmapLoader::getSplattingTextures_MMapFilename(const std::string& filename)
{
    std::ostringstream stringstream;
    stringstream<<filename<<kSplattingTextures_MetadataFilename;
    
#if defined(__IOS__)
    
    return documentspath() + stringstream.str();
    
#endif
    
    return stringstream.str();
}

std::string CHeightmapLoader::getTangentSpace_MMapFilename(const std::string& filename)
{
    std::ostringstream stringstream;
    stringstream<<filename<<kTangentSpace_MetadataFilename;
    
#if defined(__IOS__)
    
    return documentspath() + stringstream.str();
    
#endif
    
    return stringstream.str();
}

std::string CHeightmapLoader::getAttachesToVBO_MMapFilename(const std::string &filename)
{
    std::ostringstream stringstream;
    stringstream<<filename<<kAttachesToVBO_MetadataFilename;
    
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

bool CHeightmapLoader::isSplattingTextureM_MMapExist(const std::string &filename)
{
    std::ifstream stream(CHeightmapLoader::getSplattingTextureM_MMapFilename(filename));
    bool isExist = stream.good();
    stream.close();
    return isExist;
}

bool CHeightmapLoader::isSplattingTextureN_MMapExist(const std::string& filename)
{
    std::ifstream stream(CHeightmapLoader::getSplattingTextureN_MMapFilename(filename));
    bool isExist = stream.good();
    stream.close();
    return isExist;
}

bool CHeightmapLoader::isSplattingTextureH_MMapExist(const std::string& filename)
{
    std::ifstream stream(CHeightmapLoader::getSplattingTextureH_MMapFilename(filename));
    bool isExist = stream.good();
    stream.close();
    return isExist;
}

bool CHeightmapLoader::isSplattingTextures_MMapExist(const std::string& filename)
{
    std::ifstream stream(CHeightmapLoader::getSplattingTextures_MMapFilename(filename));
    bool isExist = stream.good();
    stream.close();
    return isExist;
}

bool CHeightmapLoader::isTangentSpace_MMapExist(const std::string &filename)
{
    std::ifstream stream(CHeightmapLoader::getTangentSpace_MMapFilename(filename));
    bool isExist = stream.good();
    stream.close();
    return isExist;
}

bool CHeightmapLoader::isAttachesToVBO_MMapExist(const std::string &filename)
{
    std::ifstream stream(CHeightmapLoader::getAttachesToVBO_MMapFilename(filename));
    bool isExist = stream.good();
    stream.close();
    return isExist;
}

