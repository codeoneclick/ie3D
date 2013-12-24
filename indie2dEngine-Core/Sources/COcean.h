//
//  COcean.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 7/24/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef COcean_h
#define COcean_h

#include "HCommon.h"
#include "IGameObject.h"

class COcean: public IGameObject
{
private:

    f32 m_width;
    f32 m_height;
    f32 m_altitude;
    f32 m_waveGeneratorTimer;
    f32 m_waveGeneratorInterval;
    f32 m_textureTileFactor;
    
protected:
    
    void _OnSceneUpdate(f32 _deltatime);
    
    i32 _OnQueuePosition(void);
    void _OnBind(const std::string& _renderMode);
    void _OnDraw(const std::string& _renderMode);
    void _OnUnbind(const std::string& _renderMode);
    
    void _OnTemplateLoaded(std::shared_ptr<I_RO_TemplateCommon> _template);
    void _OnResourceLoaded(std::shared_ptr<IResource> _resource, bool _success);
    
public:
    
    COcean(const std::shared_ptr<CResourceAccessor>& _resourceAccessor, const std::shared_ptr<IScreenSpaceTextureAccessor>& _screenSpaceTextureAccessor);
    ~COcean(void);
};

#endif 
