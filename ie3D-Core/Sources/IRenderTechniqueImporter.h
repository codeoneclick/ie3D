//
//  IRenderTechniqueImporter.h
//  ie3D-Core
//
//  Created by sergey.sergeev on 7/2/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#ifndef IRenderTechniqueImporter_h
#define IRenderTechniqueImporter_h

#include "HCommon.h"
#include "HDeclaration.h"

class IRenderTechniqueImporter
{
private:
    
protected:
    
    ISharedGraphicsContext m_graphicsContext;
    bool m_isOffscreen;
    std::map<std::string, CSharedRenderTechniqueWorldSpace> m_worldSpaceRenderTechniques;
    std::map<std::string, CSharedRenderTechniqueScreenSpace> m_screenSpaceRenderTechniques;
    CSharedRenderTechniqueMain m_mainRenderTechnique;
    std::queue<CSharedRenderTechniqueScreenSpace> m_customScreenSpaceRenderTechniquesQueue;
    std::map<std::string, std::set<ISharedRenderTechniqueOperationTextureHandler>> m_renderTechniqueOperationTextureHandlers;
    CSharedOcclusionQueryManager m_occlusionQueryManager;
    
public:
    
    IRenderTechniqueImporter(ISharedGraphicsContextRef graphicsContext, bool isOffscreen);
    virtual ~IRenderTechniqueImporter(void);
    
    void setMainRenderTechnique(CSharedMaterialRef material);
    
    void saveTexture(CSharedTextureRef texture, const std::string& filename, ui32 width, ui32 height);
    
    void addWorldSpaceRenderTechnique(const std::string& techniqueName, CSharedRenderTechniqueWorldSpaceRef technique);
    void removeWorldSpaceRenderTechnique(const std::string& techniqueName);
    
    void addScreenSpaceRenderTechnique(const std::string& techniqueName, CSharedRenderTechniqueScreenSpaceRef technique);
    void removeScreenSpaceRenderTechnique(const std::string& techniqueName);
    
    void addRenderTechninqueOperationTextureHandler(const std::string& techniqueName,
                                                    ISharedRenderTechniqueOperationTextureHandlerRef handler);
    
    void removeRenderTechninqueOperationTextureHandler(ISharedRenderTechniqueOperationTextureHandlerRef handler);
    
    void addRenderTechniqueHandler(const std::string& techniqueName, ISharedRenderTechniqueHandlerRef handler);
    void removeRenderTechniqueHandler(const std::string& techniqueName, ISharedRenderTechniqueHandlerRef handler);
    bool isSupportingRenderTechnique(const std::string& techniqueName);
    
    void addToOcluddingQuery(const std::string& techniqueName, ISharedOcclusionQueryHandlerRef handler);
    void removeFromOcluddingQuery(const std::string& techniqueName, ISharedOcclusionQueryHandlerRef handler);
    
};

#endif
