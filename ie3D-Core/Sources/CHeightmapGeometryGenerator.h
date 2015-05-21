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
    static void createTangentSpace(const std::shared_ptr<CHeightmapContainer>& container, const std::string& filename);
    
    static void getTriangleBasis(const glm::vec3& E, const glm::vec3& F, const glm::vec3& G,
                                 f32 sE, f32 tE, f32 sF, f32 tF, f32 sG, f32 tG,
                                 glm::vec3& tangentX, glm::vec3& tangentY);
    static glm::vec3 getClosestPointOnLine(const glm::vec3& a, const glm::vec3& b, const glm::vec3& p);
    static glm::vec3 ortogonalize(const glm::vec3& v1, const glm::vec3& v2);
    
protected:
    
public:
    
    CHeightmapGeometryGenerator(void);
    ~CHeightmapGeometryGenerator(void);
    
    static void generate(const std::shared_ptr<CHeightmapContainer>& container, const std::string& filename,
                         const glm::ivec2& size, const std::vector<f32>& heights);
    
    static void generateTangentSpace(const std::shared_ptr<CHeightmapContainer>& container, const std::string& filename);
};

#endif
