//
//  CIESAMainTransition.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 6/12/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "СIESAMainTransition.h"
#include "CCommonOS.h"
#include "CCamera.h"
#include "CModel.h"
#include "IGameObjectExtension.h"
#include "CShaderExtension.h"

CIESAMainTransition::CIESAMainTransition(const std::string& _filename, std::shared_ptr<IGraphicsContext> _graphicsContext, std::shared_ptr<IInputContext> _inputContext, std::shared_ptr<CResourceAccessor> _resourceAccessor, std::shared_ptr<CTemplateAccessor> _templateAccessor) :
IFabricator(_templateAccessor, _resourceAccessor),
IGameTransition(_filename, _graphicsContext, _inputContext, _resourceAccessor, _templateAccessor),
m_camera(nullptr),
m_model(nullptr),
m_shaderExtension(nullptr),
m_gameObjectExtension(nullptr)
{
    
}

CIESAMainTransition::~CIESAMainTransition(void)
{
    
}

void CIESAMainTransition::_OnLoaded(void)
{
    m_camera = CreateCamera(45.0f,
                            0.1f,
                            1024.0f,
                            glm::ivec4(0, 0, static_cast<i32>(Get_ScreenWidth()), static_cast<i32>(Get_ScreenHeight())));
    m_camera->Set_Position(glm::vec3(0.0f, 0.0f, 0.0f));
    m_camera->Set_LookAt(glm::vec3(0.0f, 0.0f, 0.0f));
    m_camera->Set_Distance(64.0f);
    m_camera->Set_Height(64.0f);
    Set_Camera(m_camera);
    
    m_isLoaded = true;
}

void CIESAMainTransition::_OnGameLoopUpdate(f32 _deltatime)
{
    if(m_isLoaded)
    {
        static f32 value = 0.0f;
        value += _deltatime;
        if(m_model != nullptr)
        {
            m_model->Set_Rotation(glm::vec3(0.0f, value, 0.0f));
            m_model->Set_Animation("model_02.MDL_anim");
        }
    }
}

void CIESAMainTransition::CreateGameObject(void)
{
    
}

void CIESAMainTransition::LoadGameObject(const std::string& _filename)
{
    if(m_model != nullptr)
    {
        RemoveModel(m_model);
        DeleteModel(m_model);
        m_model = nullptr;
        m_shaderExtension = nullptr;
        m_gameObjectExtension = nullptr;
    }
    
    m_model = CreateModel(_filename);
    m_model->Set_Position(glm::vec3(0.0f, 0.0f, 0.0f));
    m_model->Set_Scale(glm::vec3(10.0f, 10.0f, 10.0f));
    InsertModel(m_model);
    
    //m_gameObjectExtension = std::make_shared<IGameObjectExtension>(m_model);
    //m_shaderExtension = std::make_shared<CShaderExtension>(m_gameObjectExtension->Get_Shader("world.space.operation"));
}

std::shared_ptr<IGameObjectExtension> CIESAMainTransition::Get_GameObjectExtension(void)
{
    assert(m_gameObjectExtension != nullptr);
    return m_gameObjectExtension;
}

std::shared_ptr<CShaderExtension> CIESAMainTransition::Get_ShaderExtension(void)
{
    assert(m_shaderExtension != nullptr);
    return m_shaderExtension;
}

