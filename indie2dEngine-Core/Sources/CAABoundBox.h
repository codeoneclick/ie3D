//
//  CAABoundBox.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 7/23/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CAABoundBox_h
#define CAABoundBox_h

#include "HCommon.h"

#define k_vsBoundBoxFilename "CommonBoundBox.vert"
#define k_fsBoundBoxFilename "CommonBoundBox.frag"

class CCamera;
class CMaterial;
class CVertexBuffer;
class CIndexBuffer;
class CResourceAccessor;

class CAABoundBox final
{
private:
    
    friend class CMesh;
    
protected:
    
    std::shared_ptr<CVertexBuffer> m_vertexBuffer;
    std::shared_ptr<CIndexBuffer> m_indexBuffer;
    
    glm::vec3 m_maxBound;
    glm::vec3 m_minBound;
    
    inline void _Set_MaxBound(const glm::vec3& _maxBound)
    {
        m_maxBound = _maxBound;
    };
    
    inline void _Set_MinBound(const glm::vec3& _minBound)
    {
        m_minBound = _minBound;
    };
    
public:
    
    CAABoundBox(const glm::vec3& _maxBound, const glm::vec3& _minBound);
    ~CAABoundBox(void);
    
    void Update(const glm::mat4x4& _worldMatrix);
    
    void Bind(const i32* _attributes);
    void Draw(void);
    void Unbind(const i32* _attributes);
    
    inline glm::vec3& Get_MaxBound(void)
    {
        return m_maxBound;
    };
    
    inline glm::vec3& Get_MinBound(void)
    {
        return m_minBound;
    };
    
    inline std::shared_ptr<CVertexBuffer> Get_VertexBuffer(void)
    {
        return m_vertexBuffer;
    };
    
    inline std::shared_ptr<CIndexBuffer> Get_IndexBuffer(void)
    {
        return m_indexBuffer;
    };
};


#endif 
