//
//  CRenderPipeline.cpp
//  ie3D-Core
//
//  Created by sergey.sergeev on 7/2/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#include "CRenderPipeline.h"
#include "CRenderTechniqueMain.h"
#include "CRenderTechniqueWorldSpace.h"
#include "CRenderTechniqueScreenSpace.h"
#include "IGraphicsContext.h"
#include "CTexture.h"
#include "IRenderTechniqueOperationTextureHandler.h"
#include "COcclusionQueryManager.h"

CRenderPipeline::CRenderPipeline(ISharedGraphicsContextRef graphicContext, bool isOffscreen) :
IRenderTechniqueImporter(graphicContext, isOffscreen),
IRenderTechniqueAccessor()
{

}

CRenderPipeline::~CRenderPipeline(void)
{
    
}

void CRenderPipeline::setBatchingMgr(CSharedBatchingMgrRef batchingMgr)
{
    m_batchingMgr = batchingMgr;
}

void CRenderPipeline::_OnGameLoopUpdate(f32 deltatime)
{
    assert(m_graphicsContext != nullptr);
    m_graphicsContext->makeCurrent();
    
    m_numTriangles = 0;
    
    typedef std::pair<std::string, CSharedRenderTechniqueWorldSpace> TechniquePair;
    typedef const TechniquePair& TechniquePairRef;
    
    std::vector<TechniquePair> techniques(m_worldSpaceRenderTechniques.begin(), m_worldSpaceRenderTechniques.end());
    std::sort(techniques.begin(), techniques.end(), [](TechniquePairRef technique_01, TechniquePairRef technique_02){
        return technique_01.second->getIndex() < technique_02.second->getIndex();
    });
    
    for(const auto& iterator : techniques)
    {
        CSharedRenderTechniqueWorldSpace technique = iterator.second;
        
        /*if(m_batchingMgr != nullptr)
        {
            m_batchingMgr->lock(iterator.first);
            technique->batch();
            m_batchingMgr->unlock(iterator.first);
        }*/
        
        technique->bind();
        technique->draw();
        
#if defined(__OCCLUSIOON_QUERY__)
        
        if(technique->isOcclusionQueryEnabled())
        {
            assert(m_occlusionQueryManager != nullptr);
            m_occlusionQueryManager->update(iterator.first);
        }
        
#endif
        
        technique->unbind();
        
        auto techniqueOperationTextureHandlers = m_renderTechniqueOperationTextureHandlers.find(iterator.first);
        if(techniqueOperationTextureHandlers != m_renderTechniqueOperationTextureHandlers.end())
        {
            ui32 rawdataSize = technique->getFrameWidth() * technique->getFrameHeight() * 4;
            ui8 *rawdata = new ui8[rawdataSize];
            glReadPixels(0, 0, technique->getFrameWidth(), technique->getFrameHeight(), GL_RGBA, GL_UNSIGNED_BYTE, rawdata);
            for(const auto& techniqueOperationTextureHandler : techniqueOperationTextureHandlers->second)
            {
                techniqueOperationTextureHandler->onTextureRendered(iterator.first, rawdata, technique->getFrameWidth(), technique->getFrameHeight());
            }
            delete[] rawdata;
        }
        
        //CSharedTexture texture = technique->getOperatingColorTexture();
        //IRenderTechniqueImporter::saveTexture(texture, iterator.first + ".png", texture->getWidth(), texture->getHeight());
        m_numTriangles += technique->getNumTriangles();
    }
    
    for(const auto& iterator : m_screenSpaceRenderTechniques)
    {
        CSharedRenderTechniqueScreenSpace technique = iterator.second;
        
        technique->bind();
        technique->draw();
        technique->unbind();
        
        m_numTriangles += 2;
    }
    
    while(!m_customScreenSpaceRenderTechniquesQueue.empty())
    {
        CSharedRenderTechniqueScreenSpace technique = m_customScreenSpaceRenderTechniquesQueue.front();
        
        technique->bind();
        technique->draw();
        technique->unbind();
        
        m_customScreenSpaceRenderTechniquesQueue.pop();
        
        m_numTriangles += 2;
    }
    
    if(m_mainRenderTechnique != nullptr && !m_isOffscreen)
    {
        m_mainRenderTechnique->bind();
        m_mainRenderTechnique->draw();
        m_mainRenderTechnique->unbind();
        
        m_numTriangles += 2;
    }
    
    if(!m_isOffscreen)
    {
        m_graphicsContext->draw();
    }
}

CSharedTexture CRenderPipeline::preprocessTexture(CSharedMaterialRef material, ui32 width, ui32 height)
{
    CSharedRenderTechniqueScreenSpace technique = std::make_shared<CRenderTechniqueScreenSpace>(width, height, "render.technique.custom", material);
    m_customScreenSpaceRenderTechniquesQueue.push(technique);
    return technique->getOperatingTexture();
}

CSharedTexture CRenderPipeline::getTechniqueTexture(const std::string& techniqueName)
{
    std::string findTechniqueName = techniqueName;
    std::string::size_type location = techniqueName.find(".depth");
    if (std::string::npos != location)
    {
        findTechniqueName = std::string(techniqueName, 0, location);
    }
    CSharedTexture texture = m_worldSpaceRenderTechniques.find(findTechniqueName) != m_worldSpaceRenderTechniques.end() ? std::string::npos == location ? m_worldSpaceRenderTechniques.find(findTechniqueName)->second->getOperatingColorTexture() : m_worldSpaceRenderTechniques.find(findTechniqueName)->second->getOperatingDepthTexture() : m_screenSpaceRenderTechniques.find(findTechniqueName) != m_screenSpaceRenderTechniques.end() ? m_screenSpaceRenderTechniques.find(findTechniqueName)->second->getOperatingTexture() : nullptr;
    assert(texture != nullptr);
    return texture;
}

CSharedMaterial CRenderPipeline::getTechniqueMaterial(const std::string& techniqueName)
{
    CSharedMaterial material = m_screenSpaceRenderTechniques.find(techniqueName) != m_screenSpaceRenderTechniques.end() ? m_screenSpaceRenderTechniques.find(techniqueName)->second->getMaterial(): nullptr;
    assert(material != nullptr);
    return material;
}

ui32 CRenderPipeline::getScreenWidth(void)
{
    assert(m_graphicsContext != nullptr);
    return m_graphicsContext->getWidth();
}

ui32 CRenderPipeline::getScreenHeight(void)
{
    assert(m_graphicsContext != nullptr);
    return m_graphicsContext->getHeight();
}

ui32 CRenderPipeline::getFrameNumTriagles(void)
{
    return m_numTriangles;
}