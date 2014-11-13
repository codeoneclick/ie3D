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
    
    static glm::vec3 calculateMaxBound(const glm::vec3& point_01, const glm::vec3& point_02);
    static glm::vec3 calculateMinBound(const glm::vec3& point_01, const glm::vec3& point_02);
    
    const glm::vec3& getMaxBound(void) const;
    const glm::vec3& getMinBound(void) const;
    void updateBounds(SAttributeVertex *data, ui32 numVertices);
    
    void removeData(void);
};

class CSkeletonData final : public IResourceData
{
private:
    
protected:
    
    ui32 m_numBones;
    std::vector<CSharedBoneData> m_bonesRawData;
    
public:
    
    CSkeletonData(ui32 numBones);
    ~CSkeletonData(void);
    
    void addBone(i32 id, i32 parentId);
    
    ui32 getNumBones(void) const;
    
    const std::vector<CSharedBoneData> getBonesRawData(void) const;
};


class CMesh : public IResource
{
private:
    
protected:
    
    CSharedMeshData m_meshData;
    CSharedSkeletonData m_skeletonData;
    
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
    
    ui32 getNumVertices(void) const;
    ui32 getNumIndices(void) const;
    
    const glm::vec3 getMaxBound(void) const;
    const glm::vec3 getMinBound(void) const;
    void updateBounds(void);
    
    const CSharedSkeletonData getSkeletonData(void) const;

    void bind(const std::array<i32, E_SHADER_ATTRIBUTE_MAX>& attributes) const;
    void draw(void) const;
    void draw(ui32 indices) const;
    void unbind(const std::array<i32, E_SHADER_ATTRIBUTE_MAX>& attributes) const;
};

#endif
