//
//  CRenderPipeline.h
//  ie3D-Core
//
//  Created by sergey.sergeev on 7/2/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#ifndef CRenderPipeline_h
#define CRenderPipeline_h

#include "IGameLoopHandler.h"
#include "IRenderTechniqueImporter.h"
#include "IRenderTechniqueAccessor.h"

class CRenderPipeline :
public IGameLoopHandler,
public IRenderTechniqueImporter,
public IRenderTechniqueAccessor
{
private:
    
protected:
    
    CSharedBatchingMgr m_batchingMgr;
    void _OnGameLoopUpdate(f32 deltatime);
    
public:
    
    CRenderPipeline(ISharedGraphicsContextRef graphicContext);
    ~CRenderPipeline(void);
    
    void setBatchingMgr(CSharedBatchingMgrRef batchingMgr);
    
    CSharedTexture preprocessTexture(CSharedMaterialRef material, ui32 width, ui32 height);
    CSharedTexture getTechniqueTexture(const std::string& techniqueName);
    CSharedMaterial getTechniqueMaterial(const std::string& techniqueName);
    
    ui32 getScreenWidth(void);
    ui32 getScreenHeight(void);
    
    ui32 getFrameNumTriagles(void);
};

#endif 
