//
//  IRenderTechniqueAccessor.h
//  ie3D-Core
//
//  Created by sergey.sergeev on 7/2/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#ifndef IRenderTechniqueAccessor_h
#define IRenderTechniqueAccessor_h

#include "HCommon.h"
#include "HDeclaration.h"

class IRenderTechniqueAccessor
{
private:
    
protected:
    
    ui32 m_numTriangles;
    CSharedBatchingMgr m_batchingMgr;
    
public:
    
    IRenderTechniqueAccessor(void);
    virtual ~IRenderTechniqueAccessor(void);
    
    virtual CSharedTexture preprocessTexture(CSharedMaterialRef material, ui32 width, ui32 height, bool force = false) = 0;
    virtual CSharedTexture getTechniqueTexture(const std::string& techniqueName) = 0;
    virtual CSharedMaterial getTechniqueMaterial(const std::string& techniqueName) = 0;
    
    CSharedBatchingMgr getBatchingMgr(void) const;
    virtual ISharedGraphicsContext getGraphicsContext(void) const = 0;
    
    virtual ui32 getScreenWidth(void) = 0;
    virtual ui32 getScreenHeight(void) = 0;
    
    virtual ui32 getFrameNumTriagles(void) = 0;
};

#endif
