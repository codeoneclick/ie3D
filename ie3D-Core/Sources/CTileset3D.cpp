//
//  CTileset3D.cpp
//  ie3D-Core
//
//  Created by sergey.sergeev on 3/4/15.
//  Copyright (c) 2015 Sergey Sergeev. All rights reserved.
//

#include "CTileset3D.h"
#include "CVertexBuffer.h"
#include "CIndexBuffer.h"
#include "CMesh.h"

CTileset3D::CTileset3D(CSharedTextureRef texture, const glm::ivec2& size, const std::vector<f32>& heights) :
m_size(size)
{
    m_tileset2D = std::make_shared<CTileset2D>(texture, 1, glm::ivec2((m_size.x - 1) ,(m_size.y - 1)));
    m_tileset2D->paint(1, 1, 15);
    m_tileset2D->paint(3, 3, 15);
    m_tileset2D->paint(4, 4, 15);
    CTileset3D::generateQuadsVerticesFromRawHeights(heights);
}

CTileset3D::~CTileset3D(void)
{
    
}

void CTileset3D::generateQuadsVerticesFromRawHeights(const std::vector<f32>& heights)
{
    assert(heights.size() == m_size.x * m_size.y);
    std::vector<SUncompressedVertex> uncompressedVertices;
    std::vector<SFace> faces;
    uncompressedVertices.resize(m_size.x * m_size.y);
    faces.resize((m_size.x - 1) * (m_size.y - 1) * 2);
    
    for(ui32 i = 0; i < m_size.x; ++i)
    {
        for(ui32 j = 0; j < m_size.y; ++j)
        {
            uncompressedVertices[i + j * m_size.x].m_position = glm::vec3(static_cast<f32>(i),
                                                                          heights.at(i + j * m_size.x),
                                                                          static_cast<f32>(j));
        }
    }
    
    ui32 index = 0;
    for(ui32 i = 0; i < (m_size.x - 1); ++i)
    {
        for(ui32 j = 0; j < (m_size.y - 1); ++j)
        {
            SFace face;
            face.m_indexes[0] = i + j * m_size.x;
            uncompressedVertices[face.m_indexes[0]].m_containInFace.push_back(index);
            glm::vec3 point_01 = uncompressedVertices[face.m_indexes[0]].m_position;
            face.m_indexes[1] = i + (j + 1) * m_size.x;
            uncompressedVertices[face.m_indexes[1]].m_containInFace.push_back(index);
            glm::vec3 point_02 = uncompressedVertices[face.m_indexes[1]].m_position;
            face.m_indexes[2] = i + 1 + j * m_size.x;
            uncompressedVertices[face.m_indexes[2]].m_containInFace.push_back(index);
            glm::vec3 point_03 = uncompressedVertices[face.m_indexes[2]].m_position;
            
            glm::vec3 edge_01 = point_02 - point_01;
            glm::vec3 edge_02 = point_03 - point_01;
            glm::vec3 normal = glm::cross(edge_01, edge_02);
            f32 sin = glm::length(normal) / (glm::length(edge_01) * glm::length(edge_02));
            face.m_normal = glm::normalize(normal) * asinf(sin);
            
            faces[index] = face;
            index++;
            
            face.m_indexes[0] = i + (j + 1) * m_size.x;
            uncompressedVertices[face.m_indexes[0]].m_containInFace.push_back(index);
            point_01 = uncompressedVertices[face.m_indexes[0]].m_position;
            face.m_indexes[1] = i + 1 + (j + 1) * m_size.x;
            uncompressedVertices[face.m_indexes[1]].m_containInFace.push_back(index);
            point_02 = uncompressedVertices[face.m_indexes[1]].m_position;
            face.m_indexes[2] = i + 1 + j * m_size.x;
            uncompressedVertices[face.m_indexes[2]].m_containInFace.push_back(index);
            point_03 = uncompressedVertices[face.m_indexes[2]].m_position;
            
            edge_01 = point_02 - point_01;
            edge_02 = point_03 - point_01;
            normal = glm::cross(edge_01, edge_02);
            sin = glm::length(normal) / (glm::length(edge_01) * glm::length(edge_02));
            face.m_normal = glm::normalize(normal) * asinf(sin);
            
            faces[index] = face;
            index++;
        }
    }
    
    for(ui32 i = 0; i < uncompressedVertices.size(); ++i)
    {
        SUncompressedVertex vertex = uncompressedVertices.at(i);
        assert(vertex.m_containInFace.size() != 0);
        glm::vec3 normal = faces.at(vertex.m_containInFace.at(0)).m_normal;
        for(ui32 j = 1; j < vertex.m_containInFace.size(); ++j)
        {
            normal += faces.at(vertex.m_containInFace.at(j)).m_normal;
        }
        normal = glm::normalize(normal);
        uncompressedVertices.at(i).m_normal = normal;
    }
    
    CTileset3D::generateQuads(uncompressedVertices);
}

void CTileset3D::generateQuads(const std::vector<SUncompressedVertex>& uncompressedVertices)
{
    std::vector<SCompressedVertex> compressedVertices;
    compressedVertices.resize(uncompressedVertices.size());
    for(ui32 i = 0; i < uncompressedVertices.size(); ++i)
    {
        SCompressedVertex vertex;
        vertex.m_position = uncompressedVertices.at(i).m_position;
        vertex.m_normal = glm::packSnorm4x8(glm::vec4(uncompressedVertices.at(i).m_normal, 0.0));
        compressedVertices[i] = vertex;
    }
    
    for(ui32 i = 0; i < m_size.x; ++i)
    {
        for(ui32 j = 0; j < m_size.y; ++j)
        {
            std::cout<<"["<<compressedVertices[i + j * m_size.x].m_position.x<<" ,"<<compressedVertices[i + j * m_size.x].m_position.z<<"] ";
        }
        std::cout<<std::endl;
    }
    std::cout<<std::endl;
    
    m_quads.resize((m_size.x - 1) * (m_size.y - 1));
    CSharedVertexBuffer vbo = std::make_shared<CVertexBuffer>(m_quads.size() * 4, GL_STATIC_DRAW);
    CSharedIndexBuffer ibo = std::make_shared<CIndexBuffer>(m_quads.size() * 6, GL_STATIC_DRAW);
    
    SAttributeVertex *vbodata = vbo->lock();
    ui16 *ibodata = ibo->lock();
    
    ui32 vboindex = 0;
    ui32 iboindex = 0;
    for(ui32 i = 0; i < (m_size.x - 1); ++i)
    {
        for(ui32 j = 0; j < (m_size.y - 1); ++j)
        {
            std::shared_ptr<CTileset3DQuad> quad = std::make_shared<CTileset3DQuad>();
            glm::vec4 texcoords = std::get<2>(m_tileset2D->m_map[i + j * (m_size.y - 1)]);
            
            quad->m_vertices[0].m_position = compressedVertices[i + j * m_size.x].m_position;
            vbodata[vboindex++].m_position = quad->m_vertices[0].m_position;
            vbodata[vboindex - 1].m_normal = compressedVertices[i + j * m_size.x].m_normal;
            vbodata[vboindex - 1].m_texcoord = glm::packUnorm2x16(glm::vec2(texcoords.x, texcoords.y));
            
            quad->m_vertices[1].m_position = compressedVertices[i + (j + 1) * m_size.x].m_position;
            vbodata[vboindex++].m_position = quad->m_vertices[1].m_position;
            vbodata[vboindex - 1].m_normal = compressedVertices[i + (j + 1) * m_size.x].m_normal;
            vbodata[vboindex - 1].m_texcoord = glm::packUnorm2x16(glm::vec2(texcoords.x, texcoords.w));
            
            quad->m_vertices[2].m_position = compressedVertices[(i + 1) + j * m_size.x].m_position;
            vbodata[vboindex++].m_position = quad->m_vertices[2].m_position;
            vbodata[vboindex - 1].m_normal = compressedVertices[(i + 1) + j * m_size.x].m_normal;
            vbodata[vboindex - 1].m_texcoord = glm::packUnorm2x16(glm::vec2(texcoords.z, texcoords.y));
            
            quad->m_vertices[3].m_position = compressedVertices[(i + 1) + (j + 1) * m_size.x].m_position;
            vbodata[vboindex++].m_position = quad->m_vertices[3].m_position;
            vbodata[vboindex - 1].m_normal = compressedVertices[(i + 1) + (j + 1) * m_size.x].m_normal;
            vbodata[vboindex - 1].m_texcoord = glm::packUnorm2x16(glm::vec2(texcoords.z, texcoords.w));
            
            ibodata[iboindex++] = vboindex - 2;
            ibodata[iboindex++] = vboindex - 3;
            ibodata[iboindex++] = vboindex - 4;
            
            ibodata[iboindex++] = vboindex - 3;
            ibodata[iboindex++] = vboindex - 2;
            ibodata[iboindex++] = vboindex - 1;
            
            m_quads[i + j * (m_size.x - 1)] = quad;
            
            std::cout<<"(["<<quad->m_vertices[0].m_position.x<<" ,"<<quad->m_vertices[0].m_position.z<<"] ["<<quad->m_vertices[1].m_position.x<<" ,"<<quad->m_vertices[1].m_position.z<<"] ["<<quad->m_vertices[2].m_position.x<<" ,"<<quad->m_vertices[2].m_position.z<<"] ["<<quad->m_vertices[3].m_position.x<<" ,"<<quad->m_vertices[3].m_position.z<<"]) "<<std::endl;
        }
        std::cout<<std::endl;
    }
    
    vbo->unlock();
    ibo->unlock();
    
    m_mesh = CMesh::constructCustomMesh("tileset.3d", vbo, ibo, glm::vec3(0.0), glm::vec3(0.0));
}