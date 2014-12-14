//
//  COcclusionQueryManager.cpp
//  ie3D-Core
//
//  Created by sergey.sergeev on 11/24/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#include "COcclusionQueryManager.h"
#include "IOcclusionQueryHandler.h"
#include "CMaterial.h"
#include "CShader.h"

COcclusionQueryManager::COcclusionQueryManager(void) :
m_material(nullptr)
{
    m_material = std::make_shared<CMaterial>();
    CSharedShader shader = CShader::constructCustomShader("OcclusionQuery", ShaderOcclusionQuery_vert, ShaderOcclusionQuery_frag);
    assert(shader != nullptr);
    m_material->setShader(shader);
    
    m_material->setCulling(false);
    m_material->setCullingMode(GL_BACK);
    
    m_material->setBlending(false);
    m_material->setBlendingFunctionSource(GL_SRC_ALPHA);
    m_material->setBlendingFunctionDestination(GL_ONE);
    
    m_material->setDepthTest(true);
    m_material->setDepthMask(false);
    
    m_material->setClipping(false);
    m_material->setClippingPlane(glm::vec4(0.0, 0.0, 0.0, 0.0));
    
    m_material->setReflecting(false);
    m_material->setShadowing(false);
    m_material->setDebugging(false);
}

COcclusionQueryManager::~COcclusionQueryManager(void)
{
    
}

void COcclusionQueryManager::addToOcluddingQuery(const std::string& techniqueName, ISharedOcclusionQueryHandlerRef handler)
{
    auto iterator = m_occludingGeometry.find(techniqueName);
    if(iterator == m_occludingGeometry.end())
    {
        std::set<ISharedOcclusionQueryHandler> handlers;
        m_occludingGeometry.insert(std::make_pair(techniqueName, handlers));
        iterator = m_occludingGeometry.find(techniqueName);
    }
    assert(iterator != m_occludingGeometry.end());
    iterator->second.insert(handler);
}

void COcclusionQueryManager::removeFromOcluddingQuery(const std::string &techniqueName, ISharedOcclusionQueryHandlerRef handler)
{
    auto iterator = m_occludingGeometry.find(techniqueName);
    assert(iterator != m_occludingGeometry.end());
    if(iterator != m_occludingGeometry.end())
    {
        iterator->second.erase(handler);
    }
}

void COcclusionQueryManager::update(const std::string& techniqueName)
{
    const auto& iterator_01 = m_occludingGeometry.find(techniqueName);
    if(iterator_01 != m_occludingGeometry.end())
    {
        assert(m_material != nullptr);
        
        m_material->bind();
        glDepthMask(GL_FALSE);
        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
        
        for(const auto& iterator_02 : iterator_01->second)
        {
            iterator_02->onOcclusionQueryDraw(m_material);
        }
        
        glDepthMask(GL_TRUE);
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        m_material->unbind();
        
        // #codeoneclick: can't combine two loops because need to wait for full render data from first loop.
        for(const auto& iterator_02 : iterator_01->second)
        {
            iterator_02->onOcclusionQueryUpdate();
        }
    }
}