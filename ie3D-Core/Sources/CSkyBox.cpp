//
//  CSkyBox.cpp
//  ie3D-Core
//
//  Created by Sergey Sergeev on 5/6/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#include "CSkyBox.h"
#include "CMaterial.h"
#include "CShader.h"
#include "CTexture.h"
#include "CCamera.h"
#include "CResourceAccessor.h"
#include "CMesh.h"
#include "CVertexBuffer.h"
#include "CIndexBuffer.h"
#include "CConfigurationAccessor.h"

CSkyBox::CSkyBox(CSharedResourceAccessorRef resourceAccessor,
                 ISharedRenderTechniqueAccessorRef renderTechniqueAccessor) :
IGameObject(resourceAccessor, renderTechniqueAccessor)
{

}

CSkyBox::~CSkyBox(void)
{
    
}

void CSkyBox::onSceneUpdate(f32 deltatime)
{
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        IGameObject::onSceneUpdate(deltatime);
    }
}

void CSkyBox::onResourceLoaded(ISharedResourceRef resource, bool success)
{
    IGameObject::onResourceLoaded(resource, success);
}

void CSkyBox::onConfigurationLoaded(ISharedConfigurationRef configuration, bool success)
{
    IGameObject::onConfigurationLoaded(configuration, success);
    
    CSharedVertexBuffer vertexBuffer = std::make_shared<CVertexBuffer>(8, GL_STATIC_DRAW);
    SAttributeVertex* vertexData = vertexBuffer->lock();
    
    vertexData[0].m_position = glm::vec3(-1.0, -1.0,  1.0);
    vertexData[1].m_position = glm::vec3( 1.0, -1.0,  1.0);
    vertexData[2].m_position = glm::vec3( 1.0,  1.0,  1.0);
    vertexData[3].m_position = glm::vec3(-1.0,  1.0,  1.0);
    
    vertexData[4].m_position = glm::vec3(-1.0, -1.0, -1.0);
    vertexData[5].m_position = glm::vec3( 1.0, -1.0, -1.0);
    vertexData[6].m_position = glm::vec3( 1.0,  1.0, -1.0);
    vertexData[7].m_position = glm::vec3(-1.0,  1.0, -1.0);
    
    vertexBuffer->unlock();
    
    CSharedIndexBuffer indexBuffer = std::make_shared<CIndexBuffer>(36, GL_STATIC_DRAW);
    ui16* indexData = indexBuffer->lock();
    
    indexData[0] = 0;
    indexData[1] = 1;
    indexData[2] = 2;
    indexData[3] = 2;
    indexData[4] = 3;
    indexData[5] = 0;
    
    indexData[6] = 3;
    indexData[7] = 2;
    indexData[8] = 6;
    indexData[9] = 6;
    indexData[10] = 7;
    indexData[11] = 3;
    
    indexData[12] = 7;
    indexData[13] = 6;
    indexData[14] = 5;
    indexData[15] = 5;
    indexData[16] = 4;
    indexData[17] = 7;
    
    indexData[18] = 4;
    indexData[19] = 5;
    indexData[20] = 1;
    indexData[21] = 1;
    indexData[22] = 0;
    indexData[23] = 4;
    
    indexData[24] = 4;
    indexData[25] = 0;
    indexData[26] = 3;
    indexData[27] = 3;
    indexData[28] = 7;
    indexData[29] = 4;
    
    indexData[30] = 1;
    indexData[31] = 5;
    indexData[32] = 6;
    indexData[33] = 6;
    indexData[34] = 2;
    indexData[35] = 1;

    indexBuffer->unlock();
    
    m_mesh = CMesh::constructCustomMesh("skyBox", vertexBuffer, indexBuffer,
                                        glm::vec3(4096.0), glm::vec3(-4096.0));
    assert(m_mesh != nullptr);
    
    m_status |= E_LOADING_STATUS_TEMPLATE_LOADED;
}

bool CSkyBox::isInCameraFrustum(CSharedFrustumRef cameraFrustum)
{
    return true;
}

void CSkyBox::onBind(CSharedMaterialRef material)
{
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        IGameObject::setPosition(m_camera->getPosition());
        glm::vec3 currentRotation = IGameObject::getRotation();
        bool currentReflectingState = material->isReflecting();
        if(currentReflectingState)
        {
            IGameObject::setRotation(glm::vec3(IGameObject::getRotation().x + glm::degrees(M_PI),
                                               -glm::degrees(m_camera->getRotation()) * 2.0 + IGameObject::getRotation().y,
                                               IGameObject::getRotation().z));
            material->setReflecting(false);
        }
        IGameObject::onBind(material);
        
        if(currentReflectingState)
        {
            IGameObject::setRotation(currentRotation);
            material->setReflecting(true);
        }
    }
}

void CSkyBox::onDraw(CSharedMaterialRef material)
{
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        IGameObject::onDraw(material);
    }
}