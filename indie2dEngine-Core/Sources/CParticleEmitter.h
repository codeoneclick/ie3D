//
//  CParticleEmitter.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/24/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CParticleEmitter_h
#define CParticleEmitter_h

#include "IGameObject.h"

class CParticleEmitterTemplate;
struct SParticle
{
    glm::vec3 m_position;
    glm::vec3 m_velocity;
    glm::vec2 m_size;
    glm::u8vec4 m_color;
    ui64 m_timestamp;
};

class CParticleEmitter : public IGameObject
{
private:
    
protected:
    
    std::shared_ptr<CParticleEmitterTemplate> m_settings;
    SParticle* m_particles;
    f32 m_lastEmittTimestamp;
    f32 m_lastParticleEmittTime;
    ui8 m_locked;
    
    void _EmittParticle(ui32 _index);
    
    void _OnSceneUpdate(f32 _deltatime);
    
    i32 _OnQueuePosition(void);
    void _OnBind(const std::string& _renderMode);
    void _OnDraw(const std::string& _renderMode);
    void _OnUnbind(const std::string& _renderMode);
    
    void _OnTemplateLoaded(std::shared_ptr<I_RO_TemplateCommon> _template);
    void _OnResourceLoaded(std::shared_ptr<IResource> _resource, bool _success);
    
public:
    
    CParticleEmitter(const std::shared_ptr<CResourceAccessor>& _resourceAccessor, const std::shared_ptr<IScreenSpaceTextureAccessor>& _screenSpaceTextureAccessor);
    ~CParticleEmitter(void);
};


#endif
