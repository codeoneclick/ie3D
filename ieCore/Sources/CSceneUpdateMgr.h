//
//  CSceneUpdateMgr.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/23/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CSceneUpdateMgr_h
#define CSceneUpdateMgr_h

#include "HCommon.h"
#include "IGameLoopHandler.h"

class ISceneUpdateHandler;

class CSceneUpdateMgr final : public IGameLoopHandler
{
private:
    
    std::set<std::shared_ptr<ISceneUpdateHandler> > m_handlers;
    
protected:
    
    void _OnGameLoopUpdate(f32 _deltatime);
    
public:
    
    CSceneUpdateMgr(void);
    ~CSceneUpdateMgr(void);
    
    void RegisterSceneUpdateHandler(std::shared_ptr<ISceneUpdateHandler> _handler);
    void UnregisterSceneUpdateHandler(std::shared_ptr<ISceneUpdateHandler> _handler);
};


#endif 
