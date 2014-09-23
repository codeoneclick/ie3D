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

class CQuad
{
private:
    
protected:
    
    CSharedVertexBuffer m_vertexBuffer;
    CSharedIndexBuffer m_indexBuffer;
    
    glm::vec3 m_maxBound;
    glm::vec3 m_minBound;
    
public:
    
	CQuad(void);
    ~CQuad(void);
    
    void bind(const i32* attributes);
    void draw(void);
    void unbind(const i32* attributes);
};
#endif 
