//
//  CShape.h
//  gEngine-Core
//
//  Created by Sergey Sergeev on 5/7/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CShape_h
#define CShape_h

#include "CHVertexBuffer.h"
#include "CHIndexBuffer.h"

class CShape
{
private:
    
protected:
    
    std::shared_ptr<CHVertexBuffer> m_vertexBuffer;
    std::shared_ptr<CHIndexBuffer> m_indexBuffer;
    
    glm::vec3 m_maxBound;
    glm::vec3 m_minBound;
    
public:
    
    CShape(void);
    ~CShape(void);
    
    inline std::shared_ptr<CHVertexBuffer> Get_VertexBuffer(void)
    {
        assert(m_vertexBuffer != nullptr);
        return m_vertexBuffer;
    };
    
    inline std::shared_ptr<CHIndexBuffer> Get_IndexBuffer(void)
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
