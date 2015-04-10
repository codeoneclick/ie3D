//
//  CMeshExtension.cpp
//  ie3D-Core
//
//  Created by sergey.sergeev on 4/10/15.
//  Copyright (c) 2015 Sergey Sergeev. All rights reserved.
//

#include "CMeshExtension.h"
#include "CVertexBuffer.h"
#include "CIndexBuffer.h"

CSharedMesh CMeshExtension::createBox(const glm::vec3 &minBound, const glm::vec3 &maxBound, const glm::u8vec4& color)
{
    CSharedVertexBuffer vbo = std::make_shared<CVertexBuffer>(8, GL_STATIC_DRAW);
    SAttributeVertex* vertices = vbo->lock();
    
    vertices[0].m_position = glm::vec3(minBound.x, minBound.y, maxBound.z);
    vertices[1].m_position = glm::vec3(maxBound.x, minBound.y, maxBound.z);
    vertices[2].m_position = glm::vec3(maxBound.x, maxBound.y, maxBound.z);
    vertices[3].m_position = glm::vec3(minBound.x, maxBound.y, maxBound.z);
    
    vertices[4].m_position = glm::vec3(minBound.x, minBound.y, minBound.z);
    vertices[5].m_position = glm::vec3(maxBound.x, minBound.y, minBound.z);
    vertices[6].m_position = glm::vec3(maxBound.x, maxBound.y, minBound.z);
    vertices[7].m_position = glm::vec3(minBound.x, maxBound.y, minBound.z);
    
    for(ui32 i = 0; i < 8; ++i)
    {
        vertices[i].m_color = color;
    }
    
    vbo->unlock();
    
    CSharedIndexBuffer ibo = std::make_shared<CIndexBuffer>(36, GL_STATIC_DRAW);
    ui16* indices = ibo->lock();
    
    indices[0] = 0;
    indices[1] = 1;
    indices[2] = 2;
    indices[3] = 2;
    indices[4] = 3;
    indices[5] = 0;
    
    indices[6] = 3;
    indices[7] = 2;
    indices[8] = 6;
    indices[9] = 6;
    indices[10] = 7;
    indices[11] = 3;
    
    indices[12] = 7;
    indices[13] = 6;
    indices[14] = 5;
    indices[15] = 5;
    indices[16] = 4;
    indices[17] = 7;
    
    indices[18] = 4;
    indices[19] = 5;
    indices[20] = 1;
    indices[21] = 1;
    indices[22] = 0;
    indices[23] = 4;
    
    indices[24] = 4;
    indices[25] = 0;
    indices[26] = 3;
    indices[27] = 3;
    indices[28] = 7;
    indices[29] = 4;
    
    indices[30] = 1;
    indices[31] = 5;
    indices[32] = 6;
    indices[33] = 6;
    indices[34] = 2;
    indices[35] = 1;
    
    ibo->unlock();
    
    return CMesh::construct("box", vbo, ibo);
}


CSharedMesh CMeshExtension::createSphere(f32 radius, i32 rings, i32 sectors, const glm::u8vec4& color)
{
    const f32 frings = 1.0 / (rings - 1);
    const f32 fsectors = 1.0 / (sectors - 1);
    i32 irings, isectors;
    
    CSharedVertexBuffer vbo = std::make_shared<CVertexBuffer>(rings * sectors * 3, GL_STATIC_DRAW);
    SAttributeVertex* vertices = vbo->lock();
    
    ui32 index = 0;
    for(irings = 0; irings < rings; irings++)
    {
        for(isectors = 0; isectors < sectors; isectors++)
        {
            glm::vec3 position;
            position.y = sin( -M_PI_2 + M_PI * irings * frings);
            position.x = cos(2 * M_PI * isectors * fsectors) * sin( M_PI * irings * frings);
            position.z = sin(2 * M_PI * isectors * fsectors) * sin( M_PI * irings * frings);
            
            glm::vec4 normal = glm::vec4(position, 0.0);
            
            position *= radius;
            
            glm::vec2 texcoord;
            texcoord.x = isectors * fsectors;
            texcoord.y = irings * frings;
            
            vertices[index].m_position = position;
            vertices[index].m_texcoord = glm::packUnorm2x16(texcoord);
            vertices[index].m_normal = glm::packSnorm4x8(normal);
            vertices[index].m_color = color;
            
            index++;
        }
    }
    vbo->unlock();
    
    CSharedIndexBuffer ibo = std::make_shared<CIndexBuffer>((rings - 1) * (sectors - 1) * 6, GL_STATIC_DRAW);
    ui16* indices = ibo->lock();
    index = 0;
    for(irings = 0; irings < rings - 1; irings++)
    {
        for(isectors = 0; isectors < sectors - 1; isectors++)
        {
            indices[index++] = irings * sectors + isectors;
            indices[index++] = (irings + 1) * sectors + isectors;
            indices[index++] = (irings + 1) * sectors + (isectors + 1);
            
            indices[index++] = irings * sectors + isectors;
            indices[index++] = (irings + 1) * sectors + (isectors + 1);
            indices[index++] = irings * sectors + (isectors + 1);
        }
    }
    ibo->unlock();
    
    return CMesh::construct("sphere", vbo, ibo);
}