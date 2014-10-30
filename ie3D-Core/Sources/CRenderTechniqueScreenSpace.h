//
//  CRenderTechniqueScreenSpace.h
//  ie3D-Core
//
//  Created by sergey.sergeev on 7/2/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#ifndef CRenderTechniqueScreenSpace_h
#define CRenderTechniqueScreenSpace_h

#include "IRenderTechniqueBase.h"

class CRenderTechniqueScreenSpace final : public IRenderTechniqueBase
{
private:
    
protected:
    
    CSharedTexture m_operatingTexture;
    CSharedMaterial m_material;
    CSharedQuad m_quad;
    
public:
    
    CRenderTechniqueScreenSpace(ui32 frameWidth, ui32 frameHeight, const std::string& name, CSharedMaterialRef material);
    ~CRenderTechniqueScreenSpace(void);
    
    CSharedTexture getOperatingTexture(void) const;
    CSharedMaterial getMaterial(void) const;
    
    void bind(void);
    void unbind(void);
    void draw(void);
    void batch(void);
};

#endif
