//
//  CQuad.h
//  gEngine-Core
//
//  Created by Sergey Sergeev on 5/7/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CQuad_h
#define CQuad_h

#include "CVertexBuffer.h"
#include "CIndexBuffer.h"

class CQuad
{
private:
    
protected:
    
    std::shared_ptr<CVertexBuffer> m_vertexBuffer;
    std::shared_ptr<CIndexBuffer> m_indexBuffer;
    
    glm::vec3 m_maxBound;
    glm::vec3 m_minBound;
    
public:
    
	CQuad(void);
    ~CQuad(void);
    
    inline std::shared_ptr<CVertexBuffer> Get_VertexBuffer(void)
    {
        assert(m_vertexBuffer != nullptr);
        return m_vertexBuffer;
    };
    
    inline std::shared_ptr<CIndexBuffer> Get_IndexBuffer(void)
    {
        assert(m_indexBuffer != nullptr);
        return m_indexBuffer;
    };
    
    inline const ui32 Get_NumVertexes(void)
    {
        assert(m_vertexBuffer != nullptr);
        return m_vertexBuffer->Get_Size();
    };
    
    inline const ui32 Get_NumIndexes(void)
    {
        assert(m_indexBuffer != nullptr);
        return m_indexBuffer->Get_Size();
    };
    
    inline const glm::vec3 Get_MaxBound(void)
    {
        return m_maxBound;
    }
    
    inline const glm::vec3 Get_MinBound(void)
    {
        return m_minBound;
    }
    
    void Bind(const i32* _attributes);
    void Draw(void);
    void Unbind(const i32* _attributes);
};
#endif 
