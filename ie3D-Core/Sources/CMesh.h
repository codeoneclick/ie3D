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

extern const ui32 kMaxBones;

class CMeshMetadata final : public IResourceData
{
private:
    
protected:
    
    SVertexData* m_vertices;
    ui16* m_indices;
    
    ui32 m_numVertices;
    ui32 m_numIndices;
    
    glm::vec3 m_minBound;
    glm::vec3 m_maxBound;
    
public:
    
    CMeshMetadata(SVertexData* vertices, ui16* indices,
                  ui32 numVertices, ui32 numIndices,
                  const glm::vec3& minBound, const glm::vec3& maxBound);
    
    ~CMeshMetadata(void);
    
    const SVertexData* getVertices(void) const;
    const ui16* getIndices(void) const;
    
    ui32 getNumVertices(void) const;
    ui32 getNumIndices(void) const;
    
    const glm::vec3 getMinBound(void) const;
    const glm::vec3 getMaxBound(void) const;
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
    
    CSharedMeshMetadata m_meshMetadata;
    CSharedSkeletonData m_skeletonData;
    CSharedSequenceData m_bindposeData;
    
    CSharedVertexBuffer m_vbo;
    CSharedIndexBuffer m_ibo;
    
    std::unordered_map<std::string, CSharedVertexArrayBuffer> m_VAOstates;
    
    glm::vec3 m_maxBound;
    glm::vec3 m_minBound;
    
    void onResourceDataSerializationFinished(ISharedResourceDataRef resourceData);
    
    void onResourceDataCommitFinished(ISharedResourceDataRef resourceData);
    
public:
    
    CMesh(const std::string& guid);
    
    static CSharedMesh construct(const std::string& guid,
                                 CSharedVertexBufferRef vbo,
                                 CSharedIndexBufferRef ibo,
                                 const glm::vec3& maxBound,
                                 const glm::vec3& minBound);
    
    static CSharedMesh construct(const std::string& guid,
                                 CSharedVertexBufferRef vbo,
                                 CSharedIndexBufferRef ibo);
    ~CMesh(void);
    
    CSharedVertexBuffer getVertexBuffer(void) const;
    CSharedIndexBuffer getIndexBuffer(void) const;
    
    const SVertexData* getRawVertices(void) const;
    const ui16* getRawIndices(void) const;
    
    ui32 getNumRawVertices(void) const;
    ui32 getNumRawIndices(void) const;
    
    const glm::vec3 getMinBound(void) const;
    const glm::vec3 getMaxBound(void) const;
    const std::tuple<glm::vec3, glm::vec3> getBounds(void) const;
    
    const glm::vec3 getMinBound(const glm::mat4& matrix) const;
    const glm::vec3 getMaxBound(const glm::mat4& matrix) const;
    const std::tuple<glm::vec3, glm::vec3> getBounds(const glm::mat4& matrix) const;
    
    const CSharedSkeletonData getSkeletonData(void) const;
    const CSharedSequenceData getBindposeData(void) const;

    void bind(const std::string& attributesGUID, const std::array<i32, E_SHADER_ATTRIBUTE_MAX>& attributes);
    void draw(void) const;
    void draw(ui32 indices) const;
    void unbind(const std::string& attributesGUID, const std::array<i32, E_SHADER_ATTRIBUTE_MAX>& attributes);
};

#endif
