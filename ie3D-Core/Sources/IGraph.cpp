//
//  IGraph.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/28/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "IGraph.h"

IGraph::IGraph(CSharedRenderMgrRef renderMgr,
               CSharedSceneUpdateMgrRef sceneUpdateMgr) :
m_renderMgr(renderMgr),
m_sceneUpdateMgr(sceneUpdateMgr)
{

}

IGraph::~IGraph(void)
{

}