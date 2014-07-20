//
//  CMeshSerializer_MDL_01.cpp
//  ie3D-Core
//
//  Created by Sergey Sergeev on 7/19/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#include "CMeshSerializer_MDL_01.h"
#include "CMesh.h"
#include "CVertexBuffer.h"
#include "CIndexBuffer.h"

CMeshSerializer_MDL_01::CMeshSerializer_MDL_01(const std::string& filename,
                                               ISharedResourceRef resource) :
IResourceSerializer(filename, resource),
m_filename(filename)
{
    
}

CMeshSerializer_MDL_01::~CMeshSerializer_MDL_01(void)
{
    
}

void CMeshSerializer_MDL_01::serialize(void)
{
    assert(m_resource != nullptr);
    m_status = E_SERIALIZER_STATUS_INPROGRESS;
    
    std::shared_ptr<std::istream> filestream = IResourceSerializer::openStream(m_filename);
    
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
    IResourceSerializer::closeStream(filestream);
    CSharedMeshData meshData = std::make_shared<CMeshData>(vertexData,
                                                           indexData,
                                                           numVertices,
                                                           numIndices,
                                                           maxBound,
                                                           minBound);
    m_status = E_SERIALIZER_STATUS_SUCCESS;
    IResourceSerializer::onResourceDataSerializationFinished(meshData);
}