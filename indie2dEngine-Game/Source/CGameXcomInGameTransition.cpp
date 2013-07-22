//
//  CGameXcomInGameTransition.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 6/12/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CGameXcomInGameTransition.h"
#include "CCommonOS.h"
#include "CCamera.h"
#include "CLight.h"
#include "CModel.h"
#include "CParticleEmitter.h"

CGameXcomInGameTransition::CGameXcomInGameTransition(const std::string& _filename, std::shared_ptr<IGraphicsContext> _graphicsContext, std::shared_ptr<CResourceAccessor> _resourceAccessor, std::shared_ptr<CTemplateAccessor> _templateAccessor) :
IGameTransition(_filename, _graphicsContext, _resourceAccessor, _templateAccessor)
{
    
}

CGameXcomInGameTransition::~CGameXcomInGameTransition(void)
{
    
}

void CGameXcomInGameTransition::_OnLoaded(void)
{
    m_camera = CreateCamera(60.0f,
                            0.1f,
                            128.0f,
                            glm::vec4(0.0f, 0.0f, Get_ScreenWidth(), Get_ScreenHeight()));
    m_camera->Set_Position(glm::vec3(0.0f, 0.0f, 0.0f));
    m_camera->Set_LookAt(glm::vec3(16.0f, 4.0f, 16.0f));
    m_camera->Set_Distance(16.0f);
    m_camera->Set_Height(16.0f);
    m_light = CreateLight();
    m_light->Set_Position(glm::vec3(16.0f, 8.0f, 16.0f));
    Set_Camera(m_camera);
    Set_Light(m_light);
    
    m_model = CreateModel("model.xml");
    m_model->Set_Position(glm::vec3(16.0f, 0.0f, 16.0f));
    InsertModel(m_model);
    
    std::shared_ptr<CModel> model_01 = CreateModel("model.xml");
    model_01->Set_Position(glm::vec3(16.0f, 0.0f, 32.0f));
    InsertModel(model_01);
    
    
    std::shared_ptr<CModel> model_02 = CreateModel("model.xml");
    model_02->Set_Position(glm::vec3(32.0f, 0.0f, 16.0f));
    InsertModel(model_02);

    
    std::shared_ptr<CParticleEmitter> particleEmitter = CreateParticleEmitter("particle.emitter.01.xml");
    particleEmitter->Set_Position(glm::vec3(16.0f, 2.0f, 16.0f));
    InsertParticleEmitter(particleEmitter);
    m_isLoaded = true;
}

void CGameXcomInGameTransition::_OnGameLoopUpdate(f32 _deltatime)
{
    if(m_isLoaded)
    {
        static float angle = 0.0f;
        //m_model->Set_Rotation(glm::vec3(0.0f, angle, 0.0f));
        angle += 1.0f;
        
        m_camera->Set_Rotation(-angle / 50.0f);
        
        static glm::vec3 position;
        position.y = 8.0f;
        position.x = 16.0f + cosf(-angle / 10.0f) * -8.0f;
        position.z = 16.0f + sinf(-angle / 10.0f) * -8.0f;
        m_light->Set_Position(position);
    }
}