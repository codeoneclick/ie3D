//
//  CRenderTechniqueWorldSpace.h
//  ie3D-Core
//
//  Created by sergey.sergeev on 7/2/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#ifndef CRenderTechniqueWorldSpace_h
#define CRenderTechniqueWorldSpace_h

#include "IRenderTechniqueBase.h"

class CRenderTechniqueWorldSpace final : public IRenderTechniqueBase
{
private:
    
protected:
    
    CSharedTexture m_operatingColorTexture;
    CSharedTexture m_operatingDepthTexture;
    std::map<ui32, std::set<ISharedRenderTechniqueHandler>> m_handlers;
    ui32 m_numTriangles;
    bool m_areDrawBoundingBoxes;
    
public:
    
    CRenderTechniqueWorldSpace(ui32 frameWidth, ui32 frameHeight, const std::string& name, ui32 index);
    ~CRenderTechniqueWorldSpace(void);
    
    CSharedTexture getOperatingColorTexture(void) const;
    CSharedTexture getOperatingDepthTexture(void) const;
    
    ui32 getNumTriangles(void) const;
    
    void setAreDrawBoundingBoxes(bool value);
    
    void addRenderTechniqueHandler(ISharedRenderTechniqueHandlerRef handler);
    void removeRenderTechniqueHandler(ISharedRenderTechniqueHandlerRef handler);
    
    void drawBoundingBox(void);
    
    void bind(void);
    void unbind(void);
    void draw(void);
    void batch(void);
};

#endif
