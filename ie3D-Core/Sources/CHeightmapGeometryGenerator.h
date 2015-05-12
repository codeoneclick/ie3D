//
//  CHeightmapGeometryGenerator.h
//  ie3D-Core
//
//  Created by sergey.sergeev on 5/11/15.
//  Copyright (c) 2015 Sergey Sergeev. All rights reserved.
//

#ifndef CHeightmapGeometryGenerator_h
#define CHeightmapGeometryGenerator_h

#include "HCommon.h"

class CHeightmapContainer;
class CHeightmapGeometryGenerator
{
private:
    
    static void createVerticesMetadata(const std::shared_ptr<CHeightmapContainer>& container, const glm::ivec2& size, const std::vector<f32>& heights,
                                       const std::string& filename);
    
    static void createVBOsMetadata(const std::shared_ptr<CHeightmapContainer>& container, const std::string& filename);
    static void createIBOsMetadata(const std::shared_ptr<CHeightmapContainer>& container, const std::string& filename);
    
protected:
    
public:
    
    CHeightmapGeometryGenerator(void);
    ~CHeightmapGeometryGenerator(void);
    
    static void generate(const std::shared_ptr<CHeightmapContainer>& container, const std::string& filename,
                         const glm::ivec2& size, const std::vector<f32>& heights, const std::function<void(void)>& callback);
};

#endif
