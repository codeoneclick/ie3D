//
//  CComponentDebugRendering.cpp
//  ie3D-Core
//
//  Created by sergey.sergeev on 2/17/15.
//  Copyright (c) 2015 Sergey Sergeev. All rights reserved.
//

#include "CComponentDebugRendering.h"
#include "CConfigurationAccessor.h"
#include "CMaterial.h"
#include "CTexture.h"
#include "CShader.h"
#include "CBoundingBox.h"

CComponentDebugRendering::CComponentDebugRendering(CSharedResourceAccessorRef resourceAccessor,
                                                   ISharedRenderTechniqueAccessorRef renderTechniqueAccessor,
                                                   CSharedFrustumRef cameraFrustum,
                                                   const glm::vec3& minBound,
                                                   const glm::vec3& maxBound,
                                                   const std::function<void(CSharedMaterialRef)>& materialBindImposer) :
m_cameraFrustum(cameraFrustum),
m_checkInCameraFrustumCommand(nullptr),
m_materialBindImposer(materialBindImposer)
{
    m_boundingBox = std::make_shared<CBoundingBox>(minBound,
                                                   maxBound);
    m_material = std::make_shared<CMaterial>();
    CSharedShader shader = CShader::constructCustomShader("boundingBox",
                                                          ShaderBoundingBox_vert,
                                                          ShaderBoundingBox_frag);
    assert(shader != nullptr);
    m_material->setShader(shader);
    m_material->setCulling(false);
    m_material->setCullingMode(GL_BACK);
    m_material->setBlending(false);
    m_material->setBlendingFunctionSource(GL_SRC_ALPHA);
    m_material->setBlendingFunctionDestination(GL_ONE);
    m_material->setDepthTest(true);
    m_material->setDepthMask(true);
    m_material->setClipping(false);
    m_material->setClippingPlane(glm::vec4(0.0, 0.0, 0.0, 0.0));
    m_material->setReflecting(false);
    m_material->setShadowing(false);
    m_material->setDebugging(false);
}

CComponentDebugRendering::~CComponentDebugRendering(void)
{

}

E_COMPONENT_CLASS CComponentDebugRendering::getClass(void) const
{
    return E_COMPONENT_CLASS_DEBUG_RENDERING;
}

void CComponentDebugRendering::draw(const std::string& techniqueName)
{
    if(techniqueName == "ws.base")
    {
        m_material->bind();
        m_boundingBox->bind(m_material->getShader()->getGUID(), m_material->getShader()->getAttributes(), true);
        m_materialBindImposer(m_material);
        m_boundingBox->draw(true);
        m_material->unbind();
        m_boundingBox->unbind(m_material->getShader()->getGUID(), m_material->getShader()->getAttributes(), true);
    }
}

void CComponentDebugRendering::setCheckInCameraFrustumCommand(const T_CHECK_IN_CAMERA_FRUSTUM_COMMAND& command)
{
    m_checkInCameraFrustumCommand = command;
}

bool CComponentDebugRendering::isInCameraFrustum(void) const
{
    return m_checkInCameraFrustumCommand ? m_checkInCameraFrustumCommand(m_cameraFrustum) : false;
}
