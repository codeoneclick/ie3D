//
//  CQuad.h
//  gEngine-Core
//
//  Created by Sergey Sergeev on 5/7/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CQuad_h
#define CQuad_h

#include "HCommon.h"
#include "HDeclaration.h"
#include "HEnums.h"

class CQuad
{
private:
    
protected:
    
    CSharedVertexBuffer m_vertexBuffer;
    CSharedIndexBuffer m_indexBuffer;
    
    std::unordered_map<std::string, CSharedVertexArrayBuffer> m_VAOstates;
    
    glm::vec3 m_maxBound;
    glm::vec3 m_minBound;
    
public:
    
	CQuad(void);
    ~CQuad(void);
    
    void bind(const std::string& attributesGUID, const std::array<i32, E_SHADER_ATTRIBUTE_MAX>& attributes);
    void draw(void);
    void unbind(const std::string& attributesGUID, const std::array<i32, E_SHADER_ATTRIBUTE_MAX>& attributes);
};

#endif 
