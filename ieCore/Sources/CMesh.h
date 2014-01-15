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
#include "CVertexBuffer.h"
#include "CIndexBuffer.h"

class CMeshHeader final
{
private:
    
protected:
    
    friend class CMesh;
    friend class CMeshSerializer_MDL;
    friend class CMeshCommiter_MDL;
    
    SVertex* m_vertexData;
    ui16* m_indexData;
    
    ui32 m_numIndices;
    ui32 m_numVerticies;
    
    glm::vec3 m_maxBound;
    glm::vec3 m_minBound;
    
    inline void _Set_VertexData(SVertex* _vertexData, ui32 _numVerticies)
    {
        m_vertexData = _vertexData;
        m_numVerticies = _numVerticies;
    };
    
    inline void _Set_IndexData(ui16* _indexData, ui32 _numIndices)
    {
        m_indexData = _indexData;
        m_numIndices = _numIndices;
    };
    
    inline void _Set_MaxBound(const glm::vec3& _maxBound)
    {
        m_maxBound = _maxBound;
    };
    
    inline void _Set_MinBound(const glm::vec3& _minBound)
    {
        m_minBound = _minBound;
    };
    
    inline ui32 _Get_NumVerticies(void) const
    {
        return m_numVerticies;
    };
    
    inline ui32 _Get_NumIndices(void) const
    {
        return m_numIndices;
    };
    
    inline SVertex* _Get_VertexData(void) const
    {
        return m_vertexData;
    };
    
    inline ui16* _Get_IndexData(void) const
    {
        return m_indexData;
    };
    
public:
    
    CMeshHeader(void);
    ~CMeshHeader(void);
};

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
        m_status |= E_RESOURCE_STATUS_COMMITED;
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
        return m_header != nullptr ? m_header->m_maxBound : glm::vec3(0.0f);
    };
    
    inline const glm::vec3 Get_MinBound(void)
    {
        return m_header != nullptr ? m_header->m_minBound : glm::vec3(0.0f);
    };
    
    void Bind(const i32* _attributes) const;
    void Draw(void) const;
    void Draw(ui32 _indices) const;
    void Unbind(const i32* _attributes) const;

};

#endif
