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
#include "CHVertexBuffer.h"
#include "CSVertexBuffer.h"
#include "CHIndexBuffer.h"
#include "CSIndexBuffer.h"

class CMeshHeader final
{
private:
    
protected:
    
    friend class CMesh;
    friend class CMeshSerializer_MDL;
    friend class CMeshCommiter_MDL;
    
    CSVertexBuffer::SVertex* m_vertexData;
    ui16* m_indexData;
    
    ui32 m_numIndexes;
    ui32 m_numVertexes;
    
    glm::vec3 m_maxBound;
    glm::vec3 m_minBound;
    
    inline void _Set_VertexData(CSVertexBuffer::SVertex* _vertexData, ui32 _numVertexes)
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
};

#include "CHVertexBuffer.h"
#include "CSVertexBuffer.h"
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
    
    std::shared_ptr<CHVertexBuffer> m_hardwareVertexBuffer;
    std::shared_ptr<CSVertexBuffer> m_softwareVertexBuffer;
    
    std::shared_ptr<CHIndexBuffer> m_hardwareIndexBuffer;
    std::shared_ptr<CSIndexBuffer> m_softwareIndexBuffer;
    
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
    
    inline void _Set_Handlers(std::shared_ptr<CSVertexBuffer> _softwareVertexBuffer, std::shared_ptr<CSIndexBuffer> _softwareIndexBuffer)
    {
        assert(_softwareVertexBuffer != nullptr);
        assert(_softwareIndexBuffer != nullptr);
        m_softwareVertexBuffer = _softwareVertexBuffer;
        m_softwareIndexBuffer = _softwareIndexBuffer;
    };
    
public:
    
    CMesh(const std::string& _guid);
    CMesh(const std::string& _guid, std::shared_ptr<CSVertexBuffer> _softwareVertexBuffer, std::shared_ptr<CSIndexBuffer> _softwareIndexBuffer);
    ~CMesh(void);
    
    std::shared_ptr<CAABoundBox> CreateBoundBox(void);
    
    void CreateHardwareBuffers(GLenum _vertexBufferMode, GLenum _indexBufferMode);
    
    inline std::shared_ptr<CHVertexBuffer> Get_HardwareVertexBuffer(void)
    {
        return m_hardwareVertexBuffer;
    };
    
    inline std::shared_ptr<CSVertexBuffer> Get_SoftwareVertexBuffer(void)
    {
        return m_softwareVertexBuffer;
    };
    
    inline std::shared_ptr<CHIndexBuffer> Get_HardwareIndexBuffer(void)
    {
        return m_hardwareIndexBuffer;
    };
    
    inline std::shared_ptr<CSIndexBuffer> Get_SoftwareIndexBuffer(void)
    {
        return m_softwareIndexBuffer;
    };
    
    inline const ui32 Get_NumVertexes(void)
    {
        assert(m_softwareVertexBuffer != nullptr);
        return m_softwareVertexBuffer->Get_Size();
    };
    
    inline const ui32 Get_NumIndexes(void)
    {
        assert(m_softwareIndexBuffer != nullptr);
        return m_softwareIndexBuffer->Get_Size();
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
