//
//  CSceneUpdateMgr.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/23/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CSceneUpdateMgr.h"
#include "ISceneUpdateHandler.h"

CSceneUpdateMgr::CSceneUpdateMgr(void)
{

}

CSceneUpdateMgr::~CSceneUpdateMgr(void)
{
    m_handlers.clear();
}

void CSceneUpdateMgr::RegisterSceneUpdateHandler(std::shared_ptr<ISceneUpdateHandler> _handler)
{
    m_handlers.insert(_handler);
}

void CSceneUpdateMgr::UnregisterSceneUpdateHandler(std::shared_ptr<ISceneUpdateHandler> _handler)
{
    auto handler = std::find(m_handlers.begin(), m_handlers.end(), _handler);
    if(handler != m_handlers.end())
    {
        m_handlers.erase(_handler);
    }
}

void CSceneUpdateMgr::_OnGameLoopUpdate(f32 _deltatime)
{
    for(auto iterator = m_handlers.begin(); iterator != m_handlers.end(); ++iterator)
    {
        std::shared_ptr<ISceneUpdateHandler> handler = (*iterator);
        handler->_Get_Commands()._ExecuteSceneUpdateCommand(_deltatime);
    }
}