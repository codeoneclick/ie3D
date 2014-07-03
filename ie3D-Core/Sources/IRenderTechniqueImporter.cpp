//
//  IRenderTechniqueImporter.cpp
//  ie3D-Core
//
//  Created by sergey.sergeev on 7/2/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#include "IRenderTechniqueImporter.h"
#include "CRenderTechniqueMain.h"
#include "CRenderTechniqueWorldSpace.h"
#include "CRenderTechniqueScreenSpace.h"
#include "IGraphicsContext.h"

IRenderTechniqueImporter::IRenderTechniqueImporter(ISharedGraphicsContextRef graphicsContext) :
m_graphicsContext(graphicsContext),
m_mainRenderTechnique(nullptr)
{
    assert(m_graphicsContext != nullptr);
}

IRenderTechniqueImporter::~IRenderTechniqueImporter(void)
{
    
}

void IRenderTechniqueImporter::setMainRenderTechnique(CSharedMaterialRef material)
{
    assert(m_graphicsContext != nullptr);
    assert(material != nullptr);
    m_mainRenderTechnique = std::make_shared<CRenderTechniqueMain>(m_graphicsContext->getWidth(),
                                                                   m_graphicsContext->getHeight(),
                                                                   material,
                                                                   m_graphicsContext->getFrameBuffer(),
                                                                   m_graphicsContext->getRenderBuffer());
#if defined(__IOS__)
    const auto& platform = g_platforms.find(getPlatform());
    assert(platform != g_platforms.end());
    std::cout<<"[Device] : "<<platform->second<<std::endl;
#endif
    std::cout<<"[Output resolution] : "<<m_graphicsContext->getWidth()<<"x"<<m_graphicsContext->getHeight()<<std::endl;
}

void IRenderTechniqueImporter::addWorldSpaceRenderTechnique(const std::string& techniqueName, CSharedRenderTechniqueWorldSpaceRef technique)
{
    assert(m_worldSpaceRenderTechniques.find(techniqueName) == m_worldSpaceRenderTechniques.end());
    m_worldSpaceRenderTechniques.insert(std::make_pair(techniqueName, technique));
}

void IRenderTechniqueImporter::removeWorldSpaceRenderTechnique(const std::string& techniqueName)
{
    const auto& iterator = m_worldSpaceRenderTechniques.find(techniqueName);
    assert(iterator != m_worldSpaceRenderTechniques.end());
    m_worldSpaceRenderTechniques.erase(iterator);
}

void IRenderTechniqueImporter::addScreenSpaceRenderTechnique(const std::string& techniqueName, CSharedRenderTechniqueScreenSpaceRef technique)
{
    assert(m_screenSpaceRenderTechniques.find(techniqueName) == m_screenSpaceRenderTechniques.end());
    m_screenSpaceRenderTechniques.insert(std::make_pair(techniqueName, technique));
}

void IRenderTechniqueImporter::removeScreenSpaceRenderTechnique(const std::string& techniqueName)
{
    const auto& iterator = m_screenSpaceRenderTechniques.find(techniqueName);
    assert(iterator != m_screenSpaceRenderTechniques.end());
    m_screenSpaceRenderTechniques.erase(iterator);
}

void IRenderTechniqueImporter::addRenderTechniqueHandler(const std::string& techniqueName, ISharedRenderTechniqueHandlerRef handler)
{
    const auto& iterator = m_worldSpaceRenderTechniques.find(techniqueName);
    assert(iterator != m_worldSpaceRenderTechniques.end());
    iterator->second->addRenderTechniqueHandler(handler);
}

void IRenderTechniqueImporter::removeRenderTechniqueHandler(const std::string& techniqueName, ISharedRenderTechniqueHandlerRef handler)
{
    const auto& iterator = m_worldSpaceRenderTechniques.find(techniqueName);
    assert(iterator != m_worldSpaceRenderTechniques.end());
    iterator->second->removeRenderTechniqueHandler(handler);
}