//
//  ieDisplayObjectContainer.h
//  ieCore
//
//  Created by sergey.sergeev on 4/7/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#ifndef __ieCore__ieDisplayObjectContainer__
#define __ieCore__ieDisplayObjectContainer__

#include "ieEventDispatcher.h"

class ieDisplayObjectContainer : public ieEventDispatcher
{
private:
    
    ieSharedDisplayObjectContainer m_parent;
    std::set<ieSharedDisplayObjectContainer> m_childs;
    
protected:
    
    ieEventDispatcherFunctionShared m_functionOnUpdate;
    ieEventDispatcherFunctionShared m_functionOnDraw;
    
    ieEventDispatcherFunctionShared m_functionOnAdded;
    ieEventDispatcherFunctionShared m_functionOnRemoved;
    
    std::string m_name;
    
    ieSharedStage m_stage;
    ieSharedCamera m_camera;
    std::array<ieSharedLightSource, 4> m_lights;
    
    ieSharedRenderManager m_renderManager;
    ieSharedResourceAccessor m_resourceAccessor;
    ieSharedScreenSpaceTextureAccessor m_screenSpaceTextureAccessor;
    
    virtual void onUpdate(ieSharedEventRef event);
    virtual void onDraw(ieSharedEventRef event);
    
    virtual void onAdded(ieSharedEventRef event);
    virtual void onRemoved(ieSharedEventRef event);
    
    ieDisplayObjectContainer(void);
    
public:
    
    virtual ~ieDisplayObjectContainer(void);
    
    void setName(const std::string& name);
    std::string getName(void) const;
    
    void addChild(ieSharedDisplayObjectContainerRef child);
    void removeChild(ieSharedDisplayObjectContainerRef child);
    
    bool contains(ieSharedDisplayObjectContainerRef child) const;
    ieSharedDisplayObjectContainer getChildByName(const std::string& name) const;
};


#endif
