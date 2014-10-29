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
#include "CConfigurationGameObjects.h"
#include "CMesh.h"
#include "CVertexBuffer.h"
#include "CIndexBuffer.h"
#include "CCubemapTexture.h"

CSkyBox::CSkyBox(CSharedResourceAccessorRef resourceAccessor,
                 ISharedRenderTechniqueAccessorRef renderTechniqueAccessor) :
IGameObject(resourceAccessor, renderTechniqueAccessor)
{
    m_zOrder = E_GAME_OBJECT_Z_ORDER_SKYBOX;
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
    
    std::shared_ptr<CConfigurationSkyBox> skyBoxConfiguration = std::static_pointer_cast<CConfigurationSkyBox>(configuration);
    assert(m_resourceAccessor != nullptr);
    
    CSharedTexture xpositiveTexture = m_resourceAccessor->getTexture(skyBoxConfiguration->getXPositiveTextureFilename());
    CSharedTexture xnegativeTexture = m_resourceAccessor->getTexture(skyBoxConfiguration->getXNegativeTextureFilename());
    CSharedTexture ypositiveTexture = m_resourceAccessor->getTexture(skyBoxConfiguration->getYPositiveTextureFilename());
    CSharedTexture ynegativeTexture = m_resourceAccessor->getTexture(skyBoxConfiguration->getYNegativeTextureFilename());
    CSharedTexture zpositiveTexture = m_resourceAccessor->getTexture(skyBoxConfiguration->getZPositiveTextureFilename());
    CSharedTexture znegativeTexture = m_resourceAccessor->getTexture(skyBoxConfiguration->getZNegativeTextureFilename());
    
    CSharedCubemapTexture texture = CCubemapTexture::constructCustomCubemapTexture("skybox.cubemap.texture",
                                                                                   xpositiveTexture,
                                                                                   xnegativeTexture,
                                                                                   ypositiveTexture,
                                                                                   ynegativeTexture,
                                                                                   zpositiveTexture,
                                                                                   znegativeTexture);
    
    m_resourceAccessor->addCustomTexture("skybox.cubemap.texture", texture);
    
    for(const auto& iterator : m_materials)
    {
        CSharedMaterial material = iterator.second;
        material->setTexture(texture, E_SHADER_SAMPLER_01);
    }
    
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
    
	IGameObject::enableRender(m_isNeedToRender);
    IGameObject::enableUpdate(m_isNeedToUpdate);
    
    m_status |= E_LOADING_STATUS_TEMPLATE_LOADED;
}

i32 CSkyBox::zOrder(void)
{
    return m_zOrder;
}

bool CSkyBox::checkOcclusion(void)
{
    return false;
}

ui32 CSkyBox::numTriangles(void)
{
    return IGameObject::numTriangles();
}

void CSkyBox::onBind(const std::string& mode)
{
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        assert(m_materials.find(mode) != m_materials.end());
        IGameObject::onBind(mode);
    }
}

void CSkyBox::onDraw(const std::string& mode)
{
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        assert(m_camera != nullptr);
        assert(m_materials.find(mode) != m_materials.end());
        
        CSharedMaterial material = m_materials.find(mode)->second;
        assert(material->getShader() != nullptr);
        
        IGameObject::setPosition(m_camera->Get_Position());
        
        glm::vec3 currentRotation = m_rotation;
        if(material->isReflecting())
        {
            IGameObject::setRotation(glm::vec3(m_rotation.x + glm::degrees(M_PI),
                                               -glm::degrees(m_camera->Get_Rotation()) * 2.0 + m_rotation.y,
                                               m_rotation.z));
        }
        IGameObject::onSceneUpdate(0);

        material->getShader()->setMatrix4x4(m_matrixWorld, E_SHADER_UNIFORM_MATRIX_WORLD);
        material->getShader()->setMatrix4x4(m_camera->Get_ProjectionMatrix(), E_SHADER_UNIFORM_MATRIX_PROJECTION);
        material->getShader()->setMatrix4x4(m_camera->Get_ViewMatrix(), E_SHADER_UNIFORM_MATRIX_VIEW);
        material->getShader()->setMatrix4x4(m_camera->Get_MatrixNormal(), E_SHADER_UNIFORM_MATRIX_NORMAL);
        
        material->getShader()->setVector3(m_camera->Get_Position(), E_SHADER_UNIFORM_VECTOR_CAMERA_POSITION);
        material->getShader()->setFloat(m_camera->Get_Near(), E_SHADER_UNIFORM_FLOAT_CAMERA_NEAR);
        material->getShader()->setFloat(m_camera->Get_Far(), E_SHADER_UNIFORM_FLOAT_CAMERA_FAR);
        
        IGameObject::onDraw(mode);
        
        if(material->isReflecting())
        {
            IGameObject::setRotation(currentRotation);
            IGameObject::onSceneUpdate(0);
        }
    }
}

void CSkyBox::onUnbind(const std::string& mode)
{
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        assert(m_materials.find(mode) != m_materials.end());
        IGameObject::onUnbind(mode);
    }
}

void CSkyBox::onBatch(const std::string& mode)
{
    IGameObject::onBatch(mode);
}
