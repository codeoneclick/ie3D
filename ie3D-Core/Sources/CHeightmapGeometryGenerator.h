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
    static void createSmoothTexcoord(const std::shared_ptr<CHeightmapContainer>& container, const std::string& filename);
    static void createAttachesToVBO(const std::shared_ptr<CHeightmapContainer>& container, const std::string& filename);
    
    static glm::vec3 generateTangent(const glm::vec3& point_01, const glm::vec3& point_02, const glm::vec3& point_03,
                                     const glm::vec2& texcoord_01, const glm::vec2& texcoord_02, const glm::vec2& texcoord_03);
    
    static glm::vec3 getClosestPointOnLine(const glm::vec3& a, const glm::vec3& b, const glm::vec3& p);
    static glm::vec3 ortogonalize(const glm::vec3& v1, const glm::vec3& v2);
    
protected:
    
public:
    
    CHeightmapGeometryGenerator(void);
    ~CHeightmapGeometryGenerator(void);
    
    static void generate(const std::shared_ptr<CHeightmapContainer>& container, const std::string& filename,
                         const glm::ivec2& size, const std::vector<f32>& heights);
    
    static void generateSmoothTexcoord(const std::shared_ptr<CHeightmapContainer>& container, const std::string& filename);
    static void generateSmoothTexcoord(const std::shared_ptr<CHeightmapContainer>& container, ui32 index);
    
    static void generateTangentSpace(const std::shared_ptr<CHeightmapContainer>& container, const std::string& filename);
    static void generateTangentSpace(const std::shared_ptr<CHeightmapContainer>& container, ui32 index);
    
    static void generateAttachesToVBO(const std::shared_ptr<CHeightmapContainer>& container, const std::string& filename);
    
    static void updateVertices(const std::shared_ptr<CHeightmapContainer>& container, const std::vector<glm::vec3>& vertices);
};

#endif
