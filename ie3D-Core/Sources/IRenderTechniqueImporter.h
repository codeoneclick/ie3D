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
    std::map<std::string, CSharedRenderTechniqueWorldSpace> m_worldSpaceRenderTechniques;
    std::map<std::string, CSharedRenderTechniqueScreenSpace> m_screenSpaceRenderTechniques;
    CSharedRenderTechniqueMain m_mainRenderTechnique;
    std::queue<CSharedRenderTechniqueScreenSpace> m_customScreenSpaceRenderTechniquesQueue;
    
public:
    
    IRenderTechniqueImporter(ISharedGraphicsContextRef graphicsContext);
    virtual ~IRenderTechniqueImporter(void);
    
    void setMainRenderTechnique(CSharedMaterialRef material);
    
    void addWorldSpaceRenderTechnique(const std::string& techniqueName, CSharedRenderTechniqueWorldSpaceRef technique);
    void removeWorldSpaceRenderTechnique(const std::string& techniqueName);
    
    void addScreenSpaceRenderTechnique(const std::string& techniqueName, CSharedRenderTechniqueScreenSpaceRef technique);
    void removeScreenSpaceRenderTechnique(const std::string& techniqueName);
    
    void addRenderTechniqueHandler(const std::string& techniqueName, ISharedRenderTechniqueHandlerRef handler);
    void removeRenderTechniqueHandler(const std::string& techniqueName, ISharedRenderTechniqueHandlerRef handler);
};

#endif
