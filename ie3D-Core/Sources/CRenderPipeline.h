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
virtual public IGameLoopHandler,
virtual public IRenderTechniqueImporter,
virtual public IRenderTechniqueAccessor,
virtual public std::enable_shared_from_this<CRenderPipeline>
{
private:
    
protected:
    
    static CSharedBatchingMgr m_batchingMgr;
    CSharedBatchingMgr getBatchingMgr(void);
    
    void _OnGameLoopUpdate(f32 deltatime);
    
public:
    
    CRenderPipeline(ISharedGraphicsContextRef graphicContext);
    ~CRenderPipeline(void);
    
    CSharedTexture preprocessTexture(CSharedMaterialRef material, ui32 width, ui32 height);
    CSharedTexture getTechniqueTexture(const std::string& techniqueName);
    CSharedMaterial getTechniqueMaterial(const std::string& techniqueName);
    
    ui32 getScreenWidth(void);
    ui32 getScreenHeight(void);
    
    ui32 getFrameNumTriagles(void);
};

#endif 
