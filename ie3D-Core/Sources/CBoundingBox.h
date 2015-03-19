//
//  CBoundingBox.h
//  ie3D-Core
//
//  Created by sergey.sergeev on 11/10/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#ifndef CBoundingBox_h
#define CBoundingBox_h

#include "HCommon.h"
#include "HDeclaration.h"
#include "HEnums.h"

class CBoundingBox
{
private:
    
protected:
    
    CSharedVertexBuffer m_vertexBuffer;
    CSharedIndexBuffer m_indexBuffer;
    CSharedIndexBuffer m_wireframeIndexBuffer;
    
    std::unordered_map<std::string, CSharedVertexArrayBuffer> m_VAOstates;
    
    glm::vec3 m_maxBound;
    glm::vec3 m_minBound;
    
public:
    
    CBoundingBox(const glm::vec3& minBound, const glm::vec3& maxBound);
    ~CBoundingBox(void);
    
    static bool isPointInBoundBox(const glm::vec3& point,
                                  const glm::vec3& minBound,
                                  const glm::vec3& maxBound);
    
    static bool isPointInXZ(const glm::vec2& point,
                            const glm::vec3& minBound,
                            const glm::vec3& maxBound);
    
    void bind(const std::string& attributesGUID, const std::array<i32, E_SHADER_ATTRIBUTE_MAX>& attributes, bool isWireframe);
    void draw(bool isWireframe);
    void unbind(const std::string& attributesGUID, const std::array<i32, E_SHADER_ATTRIBUTE_MAX>& attributes, bool isWireframe);
};

#endif
