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

class CRenderMgr;
class CSceneUpdateMgr;

class IGraph
{
private:

protected:

	CSharedRenderMgr m_renderMgr;
    CSharedSceneUpdateMgr m_sceneUpdateMgr;

public:
    
    IGraph(CSharedRenderMgrRef renderMgr,
           CSharedSceneUpdateMgrRef sceneUpdateMgr);
    virtual ~IGraph(void);
    
};

#endif
