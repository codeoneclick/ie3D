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
#include "HDeclaration.h"
#include "CVertexBuffer.h"
#include "CIndexBuffer.h"

class CMeshData final
{
private:
    
protected:
    
    std::vector<SVertex> m_vertexData;
    std::vector<ui16> m_indexData;
    
    glm::vec3 m_maxBound;
    glm::vec3 m_minBound;
    
public:
    
    CMeshData(const std::vector<SVertex>& vertexData,
              const std::vector<ui16>& indexData,
              const glm::vec3& maxBound,
              const glm::vec3& minBound);
    
    ~CMeshData(void);
    
    const std::vector<SVertex>& getVertexData(void) const;
    const std::vector<ui16>& getIndexData(void) const;
    
    const ui32 getNumVertices(void) const;
    const ui32 getNumIndices(void) const;
    
    const glm::vec3& getMaxBound(void) const;
    const glm::vec3& getMinBound(void) const;
};

class CFrameData final
{
private:
    
protected:
    
    std::vector<glm::quat> m_rotations;
	std::vector<glm::vec3> m_positions;
    std::vector<glm::vec3> m_scales;
    
public:
    
    CFrameData(const std::vector<glm::quat>& rotations,
               const std::vector<glm::vec3>& positions,
               const std::vector<glm::vec3>& scales);
    
    ~CFrameData(void);
    
    const glm::quat& getRotation(ui32 index) const;
    const glm::vec3& getPosition(ui32 index) const;
    const glm::vec3& getScale(ui32 index) const;
};

class CSequenceData final
{
private:
    
protected:
    
    std::vector<CSharedFrameData> m_frames;
    ui32 m_fps;
    std::string m_name;
    
public:
    
    CSequenceData(const std::string& name,
                  ui32 fps,
                  const std::vector<CSharedFrameData>& frames);
    
    ~CSequenceData(void);
    
    const ui32 getNumFrames(void) const;
    
    const ui32 getFPS(void) const;
    
    const std::string getName(void) const;
    
    CSharedFrameData getFrame(ui32 index) const;
};

class CSkeletonData final
{
private:
    
protected:
    
    ui32 m_numBones;
    
    std::set<CSharedBone> m_roots;
    std::unordered_map<std::string, glm::mat4> m_transformations;
    
public:
    
    CSkeletonData(ui32 numBones);
    ~CSkeletonData(void);
    
    void addBone(CSharedBoneRef bone);
    CSharedBone getBone(ui32 index) const;
    
    ui32 getNumBones(void) const;
};


class CMesh : public IResource
{
private:
    
protected:
    
    CSharedMeshData m_meshData;
    CSharedSkeletonData m_skeletonData;
    CSharedSequenceData m_sequenceData;
    
    CSharedVertexBuffer m_vertexBuffer;
    CSharedIndexBuffer m_indexBuffer;
    
public:
    
    CMesh(const std::string& guid);
    CMesh(const std::string& guid,
          CSharedVertexBufferRef vertexBuffer,
          CSharedIndexBufferRef indexBuffer);
    
    ~CMesh(void);
    
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
