//
//  CMesh.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/26/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CMesh_h
#define CMesh_h

#include "IResource.h"

#define MAX_WEIGHTS 8

struct SVertexWeight
{
	f32 m_weigth;
	i32	m_boneId;
};

struct SVertexBind
{
    glm::vec3 m_bindPosition;
    glm::vec3 m_bindNormal;
    glm::vec3 m_bindTangent;
	i32	m_numWeights;
	SVertexWeight m_weights[MAX_WEIGHTS];
};

struct SVertex;
class CMeshHeader final
{
private:
    
protected:
    
    friend class CMesh;
    friend class CMeshSerializer_MDL;
    friend class CMeshCommiter_MDL;
    
    SVertex* m_vertexData;
    ui16* m_indexData;
    SVertexBind* m_vertexBindData;
    
    ui32 m_numIndexes;
    ui32 m_numVertexes;
    
    glm::vec3 m_maxBound;
    glm::vec3 m_minBound;
    
    inline void _Set_VertexData(SVertex* _vertexData, ui32 _numVertexes)
    {
        m_vertexData = _vertexData;
        m_numVertexes = _numVertexes;
    };
    
    inline void _Set_VertexBindData(SVertexBind* _vertexBindData)
    {
        m_vertexBindData = _vertexBindData;
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

#include "CVertexBuffer.h"
#include "CIndexBuffer.h"

class CAABoundBox;

class CMesh : public IResource
{
private:
    
protected:
    
    friend class CMeshSerializer_MDL;
    friend class CMeshCommiter_MDL;
    friend class CMeshLoadingOperation;
    
    std::shared_ptr<CMeshHeader> m_header;
    std::shared_ptr<CVertexBuffer> m_vertexBuffer;
    std::shared_ptr<CIndexBuffer> m_indexBuffer;
    
    std::vector<std::shared_ptr<CAABoundBox> > m_bounds;
    
    void _Set_Header(std::shared_ptr<CMeshHeader> _header);
    
#ifdef TESTING
public:
#endif
    
    inline std::shared_ptr<CMeshHeader> _Get_Header(void)
    {
        return m_header;
    };
        
#ifdef TESTING
protected:
#endif
    
    inline void _Set_Handlers(std::shared_ptr<CVertexBuffer> _vertexBuffer, std::shared_ptr<CIndexBuffer> _indexBuffer)
    {
        assert(_vertexBuffer != nullptr);
        assert(_indexBuffer != nullptr);
        m_vertexBuffer = _vertexBuffer;
        m_indexBuffer = _indexBuffer;
        m_isLinked = true;
    };
    
public:
    
    CMesh(const std::string& _guid);
    CMesh(const std::string& _guid, std::shared_ptr<CVertexBuffer> _vertexBuffer, std::shared_ptr<CIndexBuffer> _indexBuffer);
    ~CMesh(void);
    
    std::shared_ptr<CAABoundBox> CreateBoundBox(void);
    
    inline std::shared_ptr<CVertexBuffer> Get_VertexBuffer(void)
    {
        return m_vertexBuffer;
    };
    
    inline std::shared_ptr<CIndexBuffer> Get_IndexBuffer(void)
    {
        return m_indexBuffer;
    };
    
    inline SVertexBind* Get_VertexBindData(void)
    {
        assert(m_header != nullptr);
        return m_header->m_vertexBindData;
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
        return m_header != nullptr ? m_header->Get_MaxBound() : glm::vec3(0.0f);
    };
    
    inline const glm::vec3 Get_MinBound(void)
    {
        return m_header != nullptr ? m_header->Get_MinBound() : glm::vec3(0.0f);
    };
    
    void Bind(const i32* _attributes);
    void Draw(void);
    void Unbind(const i32* _attributes);

};

#endif
