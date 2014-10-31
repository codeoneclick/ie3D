//
//  IRenderTechniqueAccessor.cpp
//  ie3D-Core
//
//  Created by sergey.sergeev on 7/2/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#include "IRenderTechniqueAccessor.h"

IRenderTechniqueAccessor::IRenderTechniqueAccessor(void) :
m_numTriangles(0),
m_batchingMgr(nullptr)
{
    
}

IRenderTechniqueAccessor::~IRenderTechniqueAccessor(void)
{
    
}

CSharedBatchingMgr IRenderTechniqueAccessor::getBatchingMgr(void) const
{
    return m_batchingMgr;
}