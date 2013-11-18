//
//  IGameObjectExtension.cpp
//  indieEngineShader
//
//  Created by Sergey Sergeev on 11/6/13.
//
//

#include "IGameObjectExtension.h"
#include "CMaterial.h"

IGameObjectExtension::IGameObjectExtension(const std::shared_ptr<IGameObject>& _gameObject) :
IGameObject(nullptr, nullptr),
m_gameObject(_gameObject),
m_startLoadingCallback(nullptr),
m_endLoadingCallback(nullptr)
{
    
}

IGameObjectExtension::~IGameObjectExtension(void)
{
    
}

std::shared_ptr<CMaterial> IGameObjectExtension::Get_Material(const std::string& _mode)
{
    assert(m_gameObject != nullptr);
    auto material = m_materials.find(_mode);
    assert(material != m_materials.end());
    return material->second;
}

std::shared_ptr<CShader> IGameObjectExtension::Get_Shader(const std::string& _mode)
{
    assert(m_gameObject != nullptr);
    std::shared_ptr<CMaterial> material = IGameObjectExtension::Get_Material(_mode);
    std::shared_ptr<CShader> shader = material->Get_Shader();
    return shader;
}

void IGameObjectExtension::Load(void)
{
    assert(m_startLoadingCallback != nullptr);
    assert(m_endLoadingCallback != nullptr);
    
    std::function<void(void)> startLoadingFunction = [this]()
    {
        std::function<void(void)> endLadingFunction = [this]()
        {
            
        };
    };
    
    gcdpp::impl::DispatchAsync(gcdpp::queue::GetGlobalQueue(gcdpp::queue::GCDPP_DISPATCH_QUEUE_PRIORITY_LOW), startLoadingFunction);
}

void IGameObjectExtension::Set_StartLoadingCallback(const std::function<void(const std::shared_ptr<IGameObjectExtension>&)>& _callback)
{
    
}

void IGameObjectExtension::Set_EndLoadingCallback(const std::function<void(const std::shared_ptr<IGameObjectExtension>&)>& _callaback)
{
    
}