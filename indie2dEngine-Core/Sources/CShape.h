//
//  CShape.h
//  gEngine-Core
//
//  Created by Sergey Sergeev on 5/7/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CShape_h
#define CShape_h

#include "CVertexBuffer.h"
#include "CIndexBuffer.h"

class CShape
{
private:
    
protected:
    
    CVertexBuffer* m_vertexBuffer;
    CIndexBuffer* m_indexBuffer;
    
    glm::vec3 m_maxBound;
    glm::vec3 m_minBound;
    
public:
    
    CShape(void);
    ~CShape(void);
    
    void Link(CVertexBuffer* _vertexBuffer, CIndexBuffer* _indexBuffer);
    void Link(CVertexBuffer* _vertexBuffer, CIndexBuffer* _indexBuffer, const glm::vec3& _maxBound, const glm::vec3& _minBound);
    
    inline CVertexBuffer* Get_VertexBuffer(void)
    {
        assert(m_vertexBuffer != nullptr);
        return m_vertexBuffer;
    };
    
    inline CIndexBuffer* Get_IndexBuffer(void)
    {
        assert(m_indexBuffer != nullptr);
        return m_indexBuffer;
    };
    
    inline const ui32 Get_NumVertexes(void)
    {
        assert(m_vertexBuffer != nullptr);
        return m_vertexBuffer->Get_NumVertexes();
    };
    
    inline const ui32 Get_NumIndexes(void)
    {
        assert(m_indexBuffer != nullptr);
        return m_indexBuffer->Get_NumIndexes();
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
