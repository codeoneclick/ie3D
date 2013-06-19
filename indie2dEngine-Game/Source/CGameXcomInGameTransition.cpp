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

CGameXcomInGameTransition::CGameXcomInGameTransition(const std::string& _filename, std::shared_ptr<IGraphicsContext> _graphicsContext, std::shared_ptr<CResourceAccessor> _resourceAccessor, std::shared_ptr<CTemplateAccessor> _templateAccessor) :
IGameTransition(_filename, _graphicsContext, _resourceAccessor, _templateAccessor)
{

}

CGameXcomInGameTransition::~CGameXcomInGameTransition(void)
{
    
}

void CGameXcomInGameTransition::_OnLoaded(void)
{
    std::shared_ptr<CCamera> camera = CreateCamera(60.0f,
                                                   0.1f,
                                                   1024.0f,
                                                   glm::vec4(0.0f, 0.0f, Get_ScreenWidth(), Get_ScreenHeight()));
    camera->Set_Position(glm::vec3(0.0f, 0.0f, 0.0f));
    camera->Set_LookAt(glm::vec3(32.0f, 0.0f, 32.0f));
    camera->Set_Distance(32.0f);
    camera->Set_Height(8.0f);
    std::shared_ptr<CLight> light = CreateLight();
    light->Set_Position(glm::vec3(32.0f, 32.0f, 32.0f));
    Set_Camera(camera);
    Set_Light(light);
    m_model = CreateModel("model.xml");
    m_model->Set_Position(glm::vec3(32.0f, 0.0f, 32.0f));
    InsertModel(m_model);
}

void CGameXcomInGameTransition::_OnGameLoopUpdate(f32 _deltatime)
{
    static float angle = 0.0f;
    m_model->Set_Rotation(glm::vec3(0.0f, angle, 0.0f));
    angle += 1.0f;
}