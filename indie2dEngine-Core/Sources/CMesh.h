//
//  CMesh.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/26/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CMesh_h
#define CMesh_h

#include "HCommon.h"

struct SVertex;
class CMeshHeader final
{
private:
    
protected:
    
    friend class CMesh;
    
    SVertex* m_vertexData;
    ui16* m_indexData;
    
    ui32 m_numIndexes;
    ui32 m_numVertexes;
    
    glm::vec3 m_maxBound;
    glm::vec3 m_minBound;
    
    inline void _Set_VertexData(SVertex* _vertexData, ui32 _numVertexes)
    {
        m_vertexData = _vertexData;
        m_numVertexes = _numVertexes;
    };
    
    inline void _Set_IndexData(ui16* _indexData, ui32 _numIndexes)
    {
        m_indexData = _indexData;
        m_numIndexes = _numIndexes;
    };
    
    inline void _Set_MaxBound(const glm::vec3& _maxBound)
    {
        m_maxBound = _maxBound;
    };
    
    inline void _Set_MinBound(const glm::vec3& _minBound)
    {
        m_minBound = _minBound;
    };
    
public:
    
    CMeshHeader(void);
    ~CMeshHeader(void);
    
    inline SVertex* Get_VertexData(void)
    {
        return m_vertexData;
    };

    inline ui16* Get_IndexData(void)
    {
        return m_indexData;
    };

    inline ui32 Get_NumVertexes(void)
    {
        return m_numVertexes;
    };

    inline ui32 Get_NumIndexes(void)
    {
        return m_numIndexes;
    };

    inline glm::vec3 Get_MaxBound(void)
    {
        return m_maxBound;
    };

    inline glm::vec3 Get_MinBound(void)
    {
        return m_minBound;
    };
};

#endif
