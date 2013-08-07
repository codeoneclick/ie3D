//
//  CMeshSerializer_MDL.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/26/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CMeshSerializer_MDL.h"
#include "CCommonOS.h"
#include "CMesh.h"
#include "CSkeleton.h"
#include "CSequence.h"
#include "PVRTTexture.h"

CMeshSerializer_MDL::CMeshSerializer_MDL(const std::string& _filename, std::shared_ptr<IResource> _resource) :
IResourceSerializer(_filename, _resource),
m_filename(_filename)
{
    
}

CMeshSerializer_MDL::~CMeshSerializer_MDL(void)
{
    
}

void CMeshSerializer_MDL::Serialize(void)
{
    assert(m_resource != nullptr);
    m_status = E_SERIALIZER_STATUS_INPROGRESS;
    
    std::string path(Get_BundlePath());
    std::string filename(path);
    filename.append(m_filename);
    
    std::ifstream filestream;
    filestream.open(filename.c_str());
    if (!filestream.is_open())
    {
        m_status = E_SERIALIZER_STATUS_FAILURE;
        return;
    }
    
    std::shared_ptr<CMeshHeader> header = std::make_shared<CMeshHeader>();
    std::shared_ptr<CMesh> mesh = std::static_pointer_cast<CMesh >(m_resource);
    
    filestream.read((char*)&header->m_numVertexes, sizeof(ui32));
    filestream.read((char*)&header->m_numIndexes, sizeof(ui32));
    
    header->m_vertexData = new SVertex[header->m_numVertexes];
    
    for(ui32 i = 0; i < header->m_numVertexes; ++i)
    {
        glm::vec3 position;
        filestream.read((char*)&position, sizeof(glm::vec3));
        glm::vec3 normal;
        filestream.read((char*)&normal, sizeof(glm::vec3));
        glm::vec3 tangent;
        filestream.read((char*)&tangent, sizeof(glm::vec3));
        glm::vec2 texcoord;
        filestream.read((char*)&texcoord, sizeof(glm::vec2));
        
		header->m_vertexData[i].m_position = position;
        header->m_vertexData[i].m_texcoord = texcoord;
        header->m_vertexData[i].m_normal = CVertexBuffer::CompressVec3(normal);
        header->m_vertexData[i].m_tangent = CVertexBuffer::CompressVec3(tangent);
        
        if(header->m_vertexData[i].m_position.x > header->m_maxBound.x)
        {
            header->m_maxBound.x = header->m_vertexData[i].m_position.x;
        }
        if(header->m_vertexData[i].m_position.y > header->m_maxBound.y)
        {
            header->m_maxBound.y = header->m_vertexData[i].m_position.y;
        }
        if(header->m_vertexData[i].m_position.z > header->m_maxBound.z)
        {
            header->m_maxBound.z = header->m_vertexData[i].m_position.z;
        }
        if(header->m_vertexData[i].m_position.x < header->m_minBound.x)
        {
            header->m_minBound.x = header->m_vertexData[i].m_position.x;
        }
        if(header->m_vertexData[i].m_position.y < header->m_minBound.y)
        {
            header->m_minBound.y = header->m_vertexData[i].m_position.y;
        }
        if(header->m_vertexData[i].m_position.z < header->m_minBound.z)
        {
            header->m_minBound.z = header->m_vertexData[i].m_position.z;
        }
    }
    
	header->m_indexData = new ui16[header->m_numIndexes];
    
    for(ui32 i = 0; i < header->m_numIndexes; ++i)
    {
        filestream.read((char*)&header->m_indexData[i], sizeof(ui16));
    }
    
    for(ui32 i = 0; i < header->m_numIndexes; i += 3)
    {
        ui16 index = header->m_indexData[i + 1];
        header->m_indexData[i + 1] = header->m_indexData[i + 2];
        header->m_indexData[i + 2] = index;
    }
    
    int isAnimated = 0;
    filestream.read((char*)&isAnimated, sizeof(i32));
    
    if(isAnimated)
    {
        mesh->Get_Skeleton()->_Serialize(filestream);
        mesh->Get_Sequence()->_Serialize(filestream, mesh->Get_Skeleton()->Get_NumBones());
        
        SSequenceVertex* sequenceData = mesh->_LockSequenceData(header->m_numVertexes);
        for(ui32 i = 0; i < header->m_numVertexes; ++i)
        {
            sequenceData[i].m_position = header->m_vertexData[i].m_position;
            sequenceData[i].m_normal = CVertexBuffer::UncompressU8Vec4(header->m_vertexData[i].m_normal);
            
            i32 numWeights = 0;
            filestream.read((char*)&numWeights, sizeof(i32));
            sequenceData[i].m_numWeights = numWeights;
            for(ui32 j = 0; j < numWeights; ++j)
            {
                i32 boneId;
                filestream.read((char*)&boneId, sizeof(i32));
                f32 weight;
                filestream.read((char*)&weight, sizeof(f32));
                sequenceData[i].m_weights[j].m_boneId = boneId;
                sequenceData[i].m_weights[j].m_weigth = weight;
            }
        }
    }
    
    filestream.close();
    
    mesh->_Set_Header(header);
    
    if(isAnimated)
    {
        mesh->_BindSkeleton();
        mesh->_BindSequence();
    }
    
    m_status = E_SERIALIZER_STATUS_SUCCESS;
}