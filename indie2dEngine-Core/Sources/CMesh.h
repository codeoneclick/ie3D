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

class CMeshData final : public IResourceData
{
private:
    
protected:
    
    SVertexData* m_vertexData;
    ui16* m_indexData;
    
    ui32 m_numVertices;
    ui32 m_numIndices;
    
    glm::vec3 m_maxBound;
    glm::vec3 m_minBound;
    
public:
    
    CMeshData(SVertexData* vertexData,
              ui16* indexData,
              ui32 numVertices,
              ui32 numIndices,
              const glm::vec3& maxBound,
              const glm::vec3& minBound);
    
    ~CMeshData(void);
    
    const SVertexData* getVertexData(void) const;
    const ui16* getIndexData(void) const;
    
    ui32 getNumVertices(void) const;
    ui32 getNumIndices(void) const;
    
    const glm::vec3& getMaxBound(void) const;
    const glm::vec3& getMinBound(void) const;
    
    void removeData(void);
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

class CSequenceData final : public IResourceData
{
private:
    
protected:
    
    std::vector<CSharedFrameData> m_frames;
    ui32 m_animationFPS;
    std::string m_animationName;
    
public:
    
    CSequenceData(const std::string& animationName,
                  ui32 animationFPS,
                  const std::vector<CSharedFrameData>& frames);
    
    ~CSequenceData(void);
    
    const ui32 getNumFrames(void) const;
    
    const ui32 getAnimationFPS(void) const;
    
    const std::string getAnimationName(void) const;
    
    CSharedFrameData getFrame(ui32 index) const;
};

class CSkeletonData final : public IResourceData
{
private:
    
protected:
    
    ui32 m_numBones;
    
    std::set<CSharedBone> m_roots;
    
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
    
    void onResourceDataSerializationFinished(ISharedResourceDataRef resourceData);
    
    void onResourceDataCommitFinished(ISharedResourceDataRef resourceData);
    
public:
    
    CMesh(const std::string& guid);
    
    static CSharedMesh constructCustomMesh(const std::string& guid,
                                           CSharedVertexBufferRef vertexBuffer,
                                           CSharedIndexBufferRef indexBuffer,
                                           const glm::vec3& maxBound,
                                           const glm::vec3& minBound);
    ~CMesh(void);
    
    CSharedVertexBuffer getVertexBuffer(void) const;
    CSharedIndexBuffer getIndexBuffer(void) const;
    
    const SVertexData* getVertexData(void) const;
    const ui16* getIndexData(void) const;
    
    const ui32 getNumVertices(void) const;
    const ui32 getNumIndices(void) const;
    
    const glm::vec3 getMaxBound(void) const;
    const glm::vec3 getMinBound(void) const;
    
    const ui32 getNumFrames(void) const;
    const ui32 getAnimationFPS(void) const;
    const std::string getAnimationName(void) const;
    CSharedFrameData getFrame(ui32 index) const;
    
    CSharedBone getBone(ui32 index) const;
    ui32 getNumBones(void) const;

    void bind(const i32* attributes) const;
    void draw(void) const;
    void draw(ui32 indices) const;
    void unbind(const i32* attributes) const;
};

#endif
