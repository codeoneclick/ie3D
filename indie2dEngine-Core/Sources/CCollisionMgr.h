//
//  CCollisionMgr.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 7/19/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CCollisionMgr_h
#define CCollisionMgr_h

#include "HCommon.h"

class CCamera;
class ICollisionHandler;

class CCollisionMgr
{
private:
    
protected:
    
    std::shared_ptr<CCamera> m_camera;
    std::set<std::shared_ptr<ICollisionHandler> > m_handlers;
    
public:
    
    CCollisionMgr(void);
    ~CCollisionMgr(void);
    
    void RegisterRenderHandler(std::shared_ptr<ICollisionHandler> _handler);
    void UnregisterRenderHandler(std::shared_ptr<ICollisionHandler> _handler);
};

#endif 
