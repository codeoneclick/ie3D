//
//  CComponentRender.cpp
//  ie3D-Core
//
//  Created by sergey.sergeev on 1/22/15.
//  Copyright (c) 2015 Sergey Sergeev. All rights reserved.
//

#include "CComponentRendering.h"
#include "CConfigurationAccessor.h"
#include "CMaterial.h"
#include "CTexture.h"
#include "CShader.h"

CComponentRendering::CComponentRendering(ISharedConfigurationGameObjectRef configuration,
                                         CSharedResourceAccessorRef resourceAccessor,
                                         ISharedRenderTechniqueAccessorRef renderTechniqueAccessor,
                                         ISharedResourceLoadingHandlerRef handler,
                                         CSharedFrustumRef cameraFrustum) :
m_cameraFrustum(cameraFrustum),
m_drawCommand(nullptr),
m_checkInCameraFrustumCommand(nullptr)
{
    for(const auto& iterator : configuration->getMaterialsConfigurations())
    {
        CSharedConfigurationMaterial configurationMaterial = std::static_pointer_cast<CConfigurationMaterial>(iterator);
        CSharedMaterial material =  CMaterial::constructCustomMaterial(configurationMaterial,
                                                                       resourceAccessor,
                                                                       renderTechniqueAccessor,
                                                                       handler);
        m_materials.insert(std::make_pair(configurationMaterial->getRenderTechniqueName(), material));
    }
    m_zOrder = configuration->getZOrder();
}

CComponentRendering::~CComponentRendering(void)
{
    std::unordered_map<std::string, CSharedMaterial> eraser;
    m_materials.swap(eraser);
}

E_COMPONENT_CLASS CComponentRendering::getClass(void) const
{
    return E_COMPONENT_CLASS_RENDERING;
}

CSharedMaterial CComponentRendering::getMaterial(const std::string& techniqueName) const
{
    const auto& iterator = m_materials.find(techniqueName);
    CSharedMaterial material = iterator != m_materials.end() ? iterator->second : nullptr;
    return material;
}

i32 CComponentRendering::getZOrder(void) const
{
    return m_zOrder;
}

void CComponentRendering::setDrawCommand(const T_DRAW_COMMAND& command)
{
    m_drawCommand = command;
}

void CComponentRendering::draw(const std::string& techniqueName)
{
    if(m_drawCommand)
    {
        m_drawCommand(CComponentRendering::getMaterial(techniqueName));
    }
}

void CComponentRendering::setCheckInCameraFrustumCommand(const T_CHECK_IN_CAMERA_FRUSTUM_COMMAND& command)
{
    m_checkInCameraFrustumCommand = command;
}

bool CComponentRendering::isInCameraFrustum(void) const
{
    return m_checkInCameraFrustumCommand ? m_checkInCameraFrustumCommand(m_cameraFrustum) : false;
}

void CComponentRendering::setTexture(CSharedTextureRef texture, E_SHADER_SAMPLER sampler,
                                     ISharedResourceLoadingHandlerRef handler, const std::string& techniqueName)
{
    if(techniqueName.length() != 0)
    {
        CSharedMaterial material = CComponentRendering::getMaterial(techniqueName);
        assert(material);
        material->setTexture(texture, sampler);
    }
    else
    {
        std::for_each(m_materials.cbegin(), m_materials.cend(), [texture, sampler](std::pair<std::string, CSharedMaterial> material) {
            material.second->setTexture(texture, sampler);
        });
    }
    texture->addLoadingHandler(handler);
}

void CComponentRendering::removeLoadingDependencies(ISharedResourceLoadingHandlerRef handler)
{
    std::for_each(m_materials.cbegin(), m_materials.cend(), [handler](std::pair<std::string, CSharedMaterial> material) {
        CSharedShader shader = material.second->getShader();
        if(shader)
        {
            shader->removeLoadingHandler(handler);
        }
        
        for(ui32 i = E_SHADER_SAMPLER_01; i < E_SHADER_SAMPLER_MAX; ++i)
        {
            CSharedTexture texture = material.second->getTexture(static_cast<E_SHADER_SAMPLER>(i));
            if(texture != nullptr)
            {
                texture->removeLoadingHandler(handler);
            }
        }
    });
}
