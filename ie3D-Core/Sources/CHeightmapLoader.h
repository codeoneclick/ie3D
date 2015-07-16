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
    
    CHeightmapLoader(void);
    ~CHeightmapLoader(void);
    
    static std::tuple<glm::ivec2, std::vector<f32>> getHeights(const std::string& filename);
    
    static std::string getUncompressedVerticesMMAPFilename(const std::string& filename);
    static std::string getCompressedVerticesMMAPFilename(const std::string& filename);
    static std::string getFacesMMAPFilename(const std::string& filename);
    
    static std::string getVBOsMMAPFilename(const std::string& filename);
    static std::string getIBOsMMAPFilename(const std::string& filename);
    
    static std::string getSplattingTexturesM_MMapFilename(const std::string& filename);
    static std::string getSplattingTexturesN_MMapFilename(const std::string& filename);
    static std::string getSplattingTexturesH_MMapFilename(const std::string& filename);
    static std::string getSplattingTexturesD_MMapFilename(const std::string& filename);
    
    static std::string getTangentSpace_MMapFilename(const std::string& filename);
    static std::string getAttachesToVBO_MMapFilename(const std::string& filename);
    
    static bool isUncompressedVerticesMMAPExist(const std::string& filename);
    static bool isCompressedVerticesMMAPExist(const std::string& filename);
    static bool isFacesMMAPExist(const std::string& filename);
    
    static bool isVBOsMMAPExist(const std::string& filename);
    static bool isIBOsMMAPExist(const std::string& filename);
    
    static bool isSplattingTexturesM_MMapExist(const std::string& filename);
    static bool isSplattingTexturesD_MMapExist(const std::string& filename);
    static bool isSplattingTexturesN_MMapExist(const std::string& filename);
    static bool isSplattingTexturesH_MMapExist(const std::string& filename);
    
    static bool isTangentSpace_MMapExist(const std::string& filename);
    static bool isAttachesToVBO_MMapExist(const std::string& filename);
};

#endif
