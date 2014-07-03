//
//  IGraph.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/28/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "IGraph.h"

IGraph::IGraph(CSharedRenderPipelineRef renderPipeline,
               CSharedSceneUpdateMgrRef sceneUpdateMgr) :
m_renderPipeline(renderPipeline),
m_sceneUpdateMgr(sceneUpdateMgr)
{

}

IGraph::~IGraph(void)
{

}