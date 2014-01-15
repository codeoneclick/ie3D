//
//  CAABoundBox.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 7/23/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CAABoundBox.h"
#include "CResourceAccessor.h"
#include "CCamera.h"
#include "CMaterial.h"
#include "CVertexBuffer.h"
#include "CIndexBuffer.h"
#include "CShader.h"

CAABoundBox::CAABoundBox(const glm::vec3& _maxBound, const glm::vec3& _minBound) :
m_maxBound(_maxBound),
m_minBound(_minBound)
{
    m_vertexBuffer = std::make_shared<CVertexBuffer>(24, GL_DYNAMIC_DRAW);
    SHardwareVertex* vertexData = m_vertexBuffer->Lock();
    
    vertexData[0].m_position = glm::vec3( _minBound.x,  _minBound.y, _maxBound.z);
    vertexData[1].m_position = glm::vec3( _maxBound.x,  _minBound.y, _maxBound.z);
    vertexData[2].m_position = glm::vec3( _maxBound.x,  _maxBound.y, _maxBound.z);
    vertexData[3].m_position = glm::vec3( _minBound.x,  _maxBound.y, _maxBound.z);

    vertexData[4].m_position = glm::vec3( _minBound.x,  _minBound.y,  _minBound.z);
    vertexData[5].m_position = glm::vec3( _minBound.x,  _maxBound.y,  _minBound.z);
    vertexData[6].m_position = glm::vec3( _maxBound.x,  _maxBound.y,  _minBound.z);
    vertexData[7].m_position = glm::vec3( _maxBound.x,  _minBound.y,  _minBound.z);

    vertexData[8].m_position = glm::vec3( _minBound.x,  _maxBound.y,  _minBound.z);
    vertexData[9].m_position = glm::vec3( _minBound.x,  _maxBound.y,  _maxBound.z);
    vertexData[10].m_position = glm::vec3( _maxBound.x,  _maxBound.y,  _maxBound.z);
    vertexData[11].m_position = glm::vec3( _maxBound.x,  _maxBound.y,  _minBound.z);

    vertexData[12].m_position = glm::vec3( _minBound.x,  _maxBound.y,  _minBound.z);
    vertexData[13].m_position = glm::vec3( _maxBound.x,  _maxBound.y,  _minBound.z);
    vertexData[14].m_position = glm::vec3( _maxBound.x,  _maxBound.y,  _maxBound.z);
    vertexData[15].m_position = glm::vec3( _minBound.x,  _maxBound.y,  _maxBound.z);

    vertexData[16].m_position = glm::vec3( _maxBound.x,  _minBound.y,  _minBound.z);
    vertexData[17].m_position = glm::vec3( _maxBound.x,  _maxBound.y,  _minBound.z);
    vertexData[18].m_position = glm::vec3( _maxBound.x,  _maxBound.y,  _maxBound.z);
    vertexData[19].m_position = glm::vec3( _maxBound.x,  _minBound.y,  _maxBound.z);

    vertexData[20].m_position = glm::vec3( _minBound.x,  _minBound.y,  _minBound.z);
    vertexData[21].m_position = glm::vec3( _minBound.x,  _minBound.y,  _maxBound.z);
    vertexData[22].m_position = glm::vec3( _minBound.x,  _maxBound.y,  _maxBound.z);
    vertexData[23].m_position = glm::vec3( _minBound.x,  _maxBound.y,  _minBound.z);
   
    m_indexBuffer = std::make_shared<CIndexBuffer>(36, GL_STATIC_DRAW);
    ui16* indexData = m_indexBuffer->Lock();
    
    indexData[0] = 0;
    indexData[1] = 1;
    indexData[2] = 2;
    indexData[3] = 0;
    indexData[4] = 2;
    indexData[5] = 3;

    indexData[6] = 4;
    indexData[7] = 5;
    indexData[8] = 6;
    indexData[9] = 4;
    indexData[10] = 6;
    indexData[11] = 7;

    indexData[12] = 8;
    indexData[13] = 9;
    indexData[14] = 10;
    indexData[15] = 8;
    indexData[16] = 10;
    indexData[17] = 11;

    indexData[18] = 12;
    indexData[19] = 13;
    indexData[20] = 14;
    indexData[21] = 12;
    indexData[22] = 14;
    indexData[23] = 15;

    indexData[24] = 16;
    indexData[25] = 17;
    indexData[26] = 18;
    indexData[27] = 16;
    indexData[28] = 18;
    indexData[29] = 19;

    indexData[30] = 20;
    indexData[31] = 21;
    indexData[32] = 22;
    indexData[33] = 20;
    indexData[34] = 22;
    indexData[35] = 23;
}

CAABoundBox::~CAABoundBox(void)
{
    
}

void CAABoundBox::Update(const glm::mat4x4 &_worldMatrix)
{
    SHardwareVertex* vertexData = m_vertexBuffer->Lock();
    
    static f32 offset = 0.1f;
    
    vertexData[0].m_position = glm::transform(glm::vec3( m_minBound.x - offset,  m_minBound.y - offset, m_maxBound.z + offset), _worldMatrix);
    vertexData[1].m_position = glm::transform(glm::vec3( m_maxBound.x + offset,  m_minBound.y - offset, m_maxBound.z + offset), _worldMatrix);
    vertexData[2].m_position = glm::transform(glm::vec3( m_maxBound.x + offset,  m_maxBound.y + offset, m_maxBound.z + offset), _worldMatrix);
    vertexData[3].m_position = glm::transform(glm::vec3( m_minBound.x - offset,  m_maxBound.y + offset, m_maxBound.z + offset), _worldMatrix);
    
    vertexData[4].m_position = glm::transform(glm::vec3( m_minBound.x - offset,  m_minBound.y - offset,  m_minBound.z - offset), _worldMatrix);
    vertexData[5].m_position = glm::transform(glm::vec3( m_minBound.x - offset,  m_maxBound.y + offset,  m_minBound.z - offset), _worldMatrix);
    vertexData[6].m_position = glm::transform(glm::vec3( m_maxBound.x + offset,  m_maxBound.y + offset,  m_minBound.z - offset), _worldMatrix);
    vertexData[7].m_position = glm::transform(glm::vec3( m_maxBound.x + offset,  m_minBound.y - offset,  m_minBound.z - offset), _worldMatrix);
    
    vertexData[8].m_position = glm::transform(glm::vec3( m_minBound.x - offset,  m_maxBound.y + offset,  m_minBound.z - offset), _worldMatrix);
    vertexData[9].m_position = glm::transform(glm::vec3( m_minBound.x - offset,  m_maxBound.y + offset,  m_maxBound.z + offset), _worldMatrix);
    vertexData[10].m_position = glm::transform(glm::vec3( m_maxBound.x + offset,  m_maxBound.y + offset,  m_maxBound.z + offset), _worldMatrix);
    vertexData[11].m_position = glm::transform(glm::vec3( m_maxBound.x + offset,  m_maxBound.y + offset,  m_minBound.z - offset), _worldMatrix);
    
    vertexData[12].m_position = glm::transform(glm::vec3( m_minBound.x - offset,  m_maxBound.y + offset,  m_minBound.z - offset), _worldMatrix);
    vertexData[13].m_position = glm::transform(glm::vec3( m_maxBound.x + offset,  m_maxBound.y + offset,  m_minBound.z - offset), _worldMatrix);
    vertexData[14].m_position = glm::transform(glm::vec3( m_maxBound.x + offset,  m_maxBound.y + offset,  m_maxBound.z + offset), _worldMatrix);
    vertexData[15].m_position = glm::transform(glm::vec3( m_minBound.x - offset,  m_maxBound.y + offset,  m_maxBound.z + offset), _worldMatrix);
    
    vertexData[16].m_position = glm::transform(glm::vec3( m_maxBound.x + offset,  m_minBound.y - offset,  m_minBound.z - offset), _worldMatrix);
    vertexData[17].m_position = glm::transform(glm::vec3( m_maxBound.x + offset,  m_maxBound.y + offset,  m_minBound.z - offset), _worldMatrix);
    vertexData[18].m_position = glm::transform(glm::vec3( m_maxBound.x + offset,  m_maxBound.y + offset,  m_maxBound.z + offset), _worldMatrix);
    vertexData[19].m_position = glm::transform(glm::vec3( m_maxBound.x + offset,  m_minBound.y - offset,  m_maxBound.z + offset), _worldMatrix);
    
    vertexData[20].m_position = glm::transform(glm::vec3( m_minBound.x - offset,  m_minBound.y - offset,  m_minBound.z - offset), _worldMatrix);
    vertexData[21].m_position = glm::transform(glm::vec3( m_minBound.x - offset,  m_minBound.y - offset,  m_maxBound.z + offset), _worldMatrix);
    vertexData[22].m_position = glm::transform(glm::vec3( m_minBound.x - offset,  m_maxBound.y + offset,  m_maxBound.z + offset), _worldMatrix);
    vertexData[23].m_position = glm::transform(glm::vec3( m_minBound.x - offset,  m_maxBound.y + offset,  m_minBound.z - offset), _worldMatrix);
}

void CAABoundBox::Bind(const i32 *_attributes)
{

}

void CAABoundBox::Draw(void)
{

}

void CAABoundBox::Unbind(const i32 *_attributes)
{

}
