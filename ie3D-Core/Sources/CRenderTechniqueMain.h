//
//  CRenderTechniqueMain.h
//  ie3D-Core
//
//  Created by sergey.sergeev on 7/2/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#ifndef CRenderTechniqueMain_h
#define CRenderTechniqueMain_h

#include "IRenderTechniqueBase.h"

class CRenderTechniqueMain final : IRenderTechniqueBase
{
private:
    
protected:
    
    CSharedMaterial m_material;
    ui32 m_renderBuffer;
    CSharedQuad m_quad;
    
public:
    
    CRenderTechniqueMain(ui32 frameWidth, ui32 frameHeight, CSharedMaterialRef material, ui32 frameBuffer, ui32 renderBuffer);
    ~CRenderTechniqueMain(void);
    
    void bind(void);
    void unbind(void);
    void draw(void);
};

#endif
