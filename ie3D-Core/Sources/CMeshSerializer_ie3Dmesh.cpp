//
//  CMeshSerializer_ie3Dmesh.cpp
//  ie3D-Core
//
//  Created by Sergey Sergeev on 2/15/15.
//  Copyright (c) 2015 Sergey Sergeev. All rights reserved.
//

#include "CMeshSerializer_ie3Dmesh.h"
#include "CMesh.h"
#include "CBone.h"
#include "CAnimationSequence.h"
#include "CVertexBuffer.h"
#include "CIndexBuffer.h"

CMeshSerializer_ie3Dmesh::CMeshSerializer_ie3Dmesh(const std::string& filename,
                                         ISharedResourceRef resource) :
IResourceSerializer(filename, resource),
m_filename(filename)
{
    
}

CMeshSerializer_ie3Dmesh::~CMeshSerializer_ie3Dmesh(void)
{
    
}

void CMeshSerializer_ie3Dmesh::serialize(void)
{
    assert(m_resource != nullptr);
    m_status = E_SERIALIZER_STATUS_INPROGRESS;
    
    std::shared_ptr<std::istream> filestream = IResourceSerializer::openStream(m_filename, &m_status);
    
    CSharedMesh mesh = std::static_pointer_cast<CMesh>(m_resource);
    
    ui32 numVertices = 0;
    ui32 numIndices = 0;
    glm::vec3 maxBound(-4096.0, -4096.0, -4096.0);
    glm::vec3 minBound(4096.0, 4096.0, 4096.0);
    
    filestream->read((char*)&numVertices, sizeof(ui32));
    filestream->read((char*)&numIndices, sizeof(ui32));
    
    SVertexData* vertexData = new SVertexData[numVertices];
    
    for(ui32 i = 0; i < numVertices; ++i)
    {
        glm::vec3 position;
        filestream->read((char*)&position, sizeof(glm::vec3));
        glm::vec3 normal;
        filestream->read((char*)&normal, sizeof(glm::vec3));
        glm::vec3 tangent;
        filestream->read((char*)&tangent, sizeof(glm::vec3));
        glm::vec2 texcoord;
        filestream->read((char*)&texcoord, sizeof(glm::vec2));
        
        ui32 numWeights = 0;
        filestream->read((char*)&numWeights, sizeof(i32));
        
        for(ui32 j = 0; j < numWeights; ++j)
        {
            SBoneData bone;
            filestream->read((char*)&bone.m_id, sizeof(i32));
            filestream->read((char*)&bone.m_weigth, sizeof(f32));
            vertexData[i].m_bones.push_back(bone);
        }
        
        vertexData[i].m_position = position;
        vertexData[i].m_texcoord = texcoord;
        vertexData[i].m_normal = normal;
        vertexData[i].m_tangent = tangent;
        
        if(vertexData[i].m_position.x > maxBound.x)
        {
            maxBound.x = vertexData[i].m_position.x;
        }
        if(vertexData[i].m_position.y > maxBound.y)
        {
            maxBound.y = vertexData[i].m_position.y;
        }
        if(vertexData[i].m_position.z > maxBound.z)
        {
            maxBound.z = vertexData[i].m_position.z;
        }
        if(vertexData[i].m_position.x < minBound.x)
        {
            minBound.x = vertexData[i].m_position.x;
        }
        if(vertexData[i].m_position.y < minBound.y)
        {
            minBound.y = vertexData[i].m_position.y;
        }
        if(vertexData[i].m_position.z < minBound.z)
        {
            minBound.z = vertexData[i].m_position.z;
        }
    }
    
    ui16* indexData = new ui16[numIndices];
    
    for(ui32 i = 0; i < numIndices; ++i)
    {
        filestream->read((char*)&indexData[i], sizeof(ui16));
    }
    
    for(ui32 i = 0; i < numIndices; i += 3)
    {
        ui16 index = indexData[i + 1];
        indexData[i + 1] = indexData[i + 2];
        indexData[i + 2] = index;
    }
    
    CSharedMeshData meshData = std::make_shared<CMeshData>(vertexData,
                                                           indexData,
                                                           numVertices,
                                                           numIndices,
                                                           maxBound,
                                                           minBound);
    IResourceSerializer::onResourceDataSerializationFinished(meshData);
    
    ui32 numBones; i32 id, parentId;
    filestream->read((char*)&numBones, sizeof(i32));
    assert(numBones <= kMaxBones);
    std::shared_ptr<CSkeletonData> skeletonData = std::make_shared<CSkeletonData>(numBones);
    
    for (ui32 i = 0; i < numBones; ++i)
    {
        filestream->read((char*)&id, sizeof(i32));
        filestream->read((char*)&parentId, sizeof(i32));
        skeletonData->addBone(id, parentId);
    }
    IResourceSerializer::onResourceDataSerializationFinished(skeletonData);
    
    filestream->read((char*)&numBones, sizeof(i32));
    assert(numBones <= kMaxBones);
    i32 numFrames = 0;
    filestream->read((char*)&numFrames, sizeof(i32));
    
    std::vector<CSharedFrameData> frames;
    frames.resize(numFrames);
    
    for (ui32 i = 0; i < numFrames; ++i)
    {
        std::vector<glm::quat> rotations;
        std::vector<glm::vec3> positions;
        std::vector<glm::vec3> scales;
        
        for (i32 i = 0; i < numBones; ++i)
        {
            glm::vec3 position;
            filestream->read((char*)&position, sizeof(glm::vec3));
            positions.push_back(glm::vec3(position));
            
            glm::quat rotation;
            filestream->read((char*)&rotation, sizeof(glm::quat));
            glm::quat _rotation = glm::quat(rotation.w, -rotation.x, -rotation.y, -rotation.z);
            rotations.push_back(_rotation);
            
            glm::vec3 scale;
            filestream->read((char*)&scale, sizeof(glm::vec3));
            scales.push_back(scale);
        }
        frames[i] = std::make_shared<CFrameData>(rotations,
                                                 positions,
                                                 scales);
    }
    CSharedSequenceData sequenceData = std::make_shared<CSequenceData>(kBindposeAnimationName,
                                                                       30,
                                                                       frames);
    IResourceSerializer::onResourceDataSerializationFinished(sequenceData);
    
    IResourceSerializer::closeStream(filestream);
    m_status = E_SERIALIZER_STATUS_SUCCESS;
}