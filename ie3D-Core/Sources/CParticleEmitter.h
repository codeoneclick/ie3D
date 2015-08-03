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
#include "HDeclaration.h"

class CConfigurationParticleEmitter;
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
    
    std::shared_ptr<CConfigurationParticleEmitter> m_settings;
    SParticle* m_particles;
    ui32 m_numParticles;
    f32 m_lastEmittTimestamp;
    f32 m_lastParticleEmittTime;
    
    void emittParticle(ui32 index);
    void createParticlesMesh(void);
    
    void onSceneUpdate(f32 deltatime);
    void onResourceLoaded(ISharedResourceRef resource, bool success);
    void onConfigurationLoaded(ISharedConfigurationRef configuration, bool success);
    
    void onDraw(CSharedMaterialRef material);
    
public:
    
    CParticleEmitter(CSharedResourceAccessorRef resourceAccessor,
                     ISharedRenderTechniqueAccessorRef renderTechniqueAccessor);
    ~CParticleEmitter(void);
    
    void onConfigurationUpdated(CSharedConfigurationParticleEmitterRef configuration);
};


#endif
