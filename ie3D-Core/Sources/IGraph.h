//
//  IGraph.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/28/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef IGraph_h
#define IGraph_h

#include "HCommon.h"
#include "HDeclaration.h"

class IGraph
{
private:

protected:

	CSharedRenderPipeline m_renderPipeline;
    CSharedSceneUpdateMgr m_sceneUpdateMgr;

public:
    
    IGraph(CSharedRenderPipelineRef renderPipeline,
           CSharedSceneUpdateMgrRef sceneUpdateMgr);
    virtual ~IGraph(void);
    
};

#endif
