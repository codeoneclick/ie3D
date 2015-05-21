//
//  CHeightmapLoader.h
//  ie3D-Core
//
//  Created by sergey.sergeev on 5/11/15.
//  Copyright (c) 2015 Sergey Sergeev. All rights reserved.
//

#ifndef CHeightmapLoader_h
#define CHeightmapLoader_h

#include "HCommon.h"

class CHeightmapLoader
{
private:
    
protected:
    
public:
    
    static ui32 g_heightmapGUID;
    
    CHeightmapLoader(void);
    ~CHeightmapLoader(void);
    
    static std::tuple<glm::ivec2, std::vector<f32>> getHeights(const std::string& filename);
    
    static std::string getUncompressedVerticesMMAPFilename(const std::string& filename);
    static std::string getCompressedVerticesMMAPFilename(const std::string& filename);
    static std::string getFacesMMAPFilename(const std::string& filename);
    
    static std::string getVBOsMMAPFilename(const std::string& filename);
    static std::string getIBOsMMAPFilename(const std::string& filename);
    
    static std::string getSplattingTextureMasksMMAPFilename(const std::string& filename);
    static std::string getSplattingTexturesMMAPFilename(const std::string& filename);
    
    static std::string getTangentSpaceMMAPFilename(const std::string& filename);
    
    static bool isUncompressedVerticesMMAPExist(const std::string& filename);
    static bool isCompressedVerticesMMAPExist(const std::string& filename);
    static bool isFacesMMAPExist(const std::string& filename);
    
    static bool isVBOsMMAPExist(const std::string& filename);
    static bool isIBOsMMAPExist(const std::string& filename);
    
    static bool isSplattingTextureMasksMMAPExist(const std::string& filename);
    static bool isSplattingTexturesMMAPExist(const std::string& filename);
    
    static bool isTangentSpaceMMAPExist(const std::string& filename);
};

#endif
