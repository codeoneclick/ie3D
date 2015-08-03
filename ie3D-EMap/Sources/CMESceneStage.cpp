//
//  CMESceneStage.cpp
//  ie3D-EMap-OSX
//
//  Created by sergey.sergeev on 8/3/15.
//
//

#include "CMESceneStage.h"
#include "CVertexBuffer.h"
#include "CIndexBuffer.h"
#include "CLandscape.h"
#include "CMesh.h"
#include "CMaterial.h"
#include "CShader.h"
#include "CCamera.h"
#include "HMEConfigurationDeclarations.h"
#include "CMEConfigurationAccessor.h"
#include "CMeshExtension.h"

CMESceneStage::CMESceneStage(CSharedResourceAccessorRef resourceAccessor,
                             ISharedRenderTechniqueAccessorRef renderTechniqueAccessor) :
IGameObject(resourceAccessor, renderTechniqueAccessor),
m_size(4)
{
    
}

CMESceneStage::~CMESceneStage(void)
{
    
}

void CMESceneStage::onSceneUpdate(f32 deltatime)
{
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        IGameObject::onSceneUpdate(deltatime);
    }
}

void CMESceneStage::onResourceLoaded(ISharedResourceRef resource, bool success)
{
    IGameObject::onResourceLoaded(resource, success);
}

void CMESceneStage::onConfigurationLoaded(ISharedConfigurationRef configuration, bool success)
{
    IGameObject::onConfigurationLoaded(configuration, success);
    
    CSharedMEConfigurationSceneStage configurationSceneStage = std::static_pointer_cast<CMEConfigurationSceneStage>(configuration);
    assert(m_resourceAccessor != nullptr);
    m_size = glm::vec3(configurationSceneStage->getSizeX(),
                       configurationSceneStage->getSizeY(),
                       configurationSceneStage->getSizeZ());
    
    glm::vec3 minBound = glm::vec3(m_size.x * -0.5f, m_size.y * -0.5f, m_size.z * -0.5f);
    glm::vec3 maxBound = glm::vec3(m_size.x * 0.5f, m_size.y * 0.5f, m_size.z * 0.5f);
    m_mesh = CMeshExtension::createBox(minBound, maxBound, glm::u8vec4(0));
    assert(m_mesh != nullptr);
    
    m_status |= E_LOADING_STATUS_TEMPLATE_LOADED;
}
