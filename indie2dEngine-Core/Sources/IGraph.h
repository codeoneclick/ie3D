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

class CRenderMgr;
class CSceneUpdateMgr;

class IGraph
{
private:

protected:

	std::shared_ptr<CRenderMgr> m_renderMgr;
    std::shared_ptr<CSceneUpdateMgr> m_sceneUpdateMgr;

public:
    
    IGraph(void);
    virtual ~IGraph(void);
    
};

#endif
