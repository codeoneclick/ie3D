//
//  IGameObjectExtension.cpp
//  indieEngineShader
//
//  Created by Sergey Sergeev on 11/6/13.
//
//

#include "IGameObjectExtension.h"
#include "CMaterial.h"
#include "CSceneGraph.h"
#include "CSceneFabricator.h"
#include "CModel.h"
#include "CMaterialExtension.h"
#include "CShaderExtension.h"

IGameObjectExtension::IGameObjectExtension(const std::shared_ptr<CSceneGraph>& _sceneGraph, const std::shared_ptr<CSceneFabricator>& _sceneFabricator) :
IGameObject(nullptr, nullptr),
m_gameObject(nullptr),
m_sceneGraph(_sceneGraph),
m_sceneFabricator(_sceneFabricator)
{
    m_resourceLoadingHandler = std::make_shared<std::function<void(const std::shared_ptr<IResource>&)>>(std::bind(&IGameObjectExtension::_OnExtensionResourceLoaded, this, std::placeholders::_1));
    m_templateLoadingHandler = std::make_shared<std::function<void(const std::set<std::string>&)>>(std::bind(&IGameObjectExtension::_OnExtensionTemplateLoaded, this, std::placeholders::_1));
}

IGameObjectExtension::~IGameObjectExtension(void)
{
    m_gameObject->UnregisterResourceLoadingHandler(m_resourceLoadingHandler, E_RESOURCE_CLASS_MATERIAL);
    m_gameObject->UnregisterResourceLoadingHandler(m_resourceLoadingHandler, E_RESOURCE_CLASS_SHADER);
    m_gameObject->UnregisterResourceLoadingHandler(m_resourceLoadingHandler, E_RESOURCE_CLASS_TEXTURE);
    m_gameObject->UnregisterTemplateLoadingHandler(m_templateLoadingHandler);
    
    assert(m_sceneGraph != nullptr);
    assert(m_sceneFabricator != nullptr);
    
    if(m_gameObject != nullptr)
    {
        m_sceneGraph->RemoveModel(std::static_pointer_cast<CModel>(m_gameObject));
        m_sceneFabricator->DeleteModel(std::static_pointer_cast<CModel>(m_gameObject));
    }
}

void IGameObjectExtension::_OnExtensionResourceLoaded(const std::shared_ptr<IResource>& _resource)
{
    switch (_resource->Get_Class())
    {
        case E_RESOURCE_CLASS_SHADER:
        {
            
        }
            break;
        case E_RESOURCE_CLASS_TEXTURE:
        {

        }
            break;
        case E_RESOURCE_CLASS_MATERIAL:
        {
            std::shared_ptr<CMaterial> material = std::static_pointer_cast<CMaterial>(_resource);
            std::shared_ptr<CMaterialExtension> materialExtension = std::make_shared<CMaterialExtension>(material);
            m_materialsExtension.insert(std::make_pair(material->Get_Guid(), materialExtension));
        }
            break;
            
        default:
            break;
    }
}

void IGameObjectExtension::_OnExtensionTemplateLoaded(const std::set<std::string> _modes)
{
    m_modes = _modes;
}

void IGameObjectExtension::Load(const std::string &_filename)
{
    assert(m_sceneGraph != nullptr);
    assert(m_sceneFabricator != nullptr);
    assert(m_gameObject == nullptr);
    
    m_gameObject = m_sceneFabricator->CreateModel(_filename);
    m_gameObject->Set_Position(glm::vec3(0.0f, 0.0f, 0.0f));
    m_gameObject->Set_Scale(glm::vec3(10.0f, 10.0f, 10.0f));

    m_gameObject->RegisterResourceLoadingHandler(m_resourceLoadingHandler, E_RESOURCE_CLASS_MATERIAL);
    m_gameObject->RegisterResourceLoadingHandler(m_resourceLoadingHandler, E_RESOURCE_CLASS_SHADER);
    m_gameObject->RegisterResourceLoadingHandler(m_resourceLoadingHandler, E_RESOURCE_CLASS_TEXTURE);
    m_gameObject->RegisterTemplateLoadingHandler(m_templateLoadingHandler);
    
    m_sceneGraph->InsertModel(std::static_pointer_cast<CModel>(m_gameObject));
}

void IGameObjectExtension::Get_Modes(const TEMPLATE_LOADING_HANDLER &_handler)
{
    assert(m_gameObject != nullptr);
    if(m_modes.size() != 0)
    {
        (*_handler)(m_modes);
    }
    m_gameObject->RegisterTemplateLoadingHandler(_handler);
}

void IGameObjectExtension::Get_Material(const RESOURCE_LOADING_HANDLER& _handler, const std::string& _mode)
{
    assert(m_gameObject != nullptr);
    auto materialExtension = m_materialsExtension.find(_mode);
    if(materialExtension != m_materialsExtension.end() && materialExtension->second->Get_Material()->IsCommited())
    {
        (*_handler)(materialExtension->second->Get_Material());
    }
    
    RESOURCE_LOADING_HANDLER handler;
    std::function<void(const std::shared_ptr<IResource>&)> function = [_mode, _handler, handler, this](const std::shared_ptr<IResource>& _resource)
    {
        if(_mode == _resource->Get_Guid())
        {
            (*_handler)(_resource);
        }
        m_gameObject->UnregisterResourceLoadingHandler(handler, _resource->Get_Class());
    };
    handler = std::make_shared<std::function<void(const std::shared_ptr<IResource>&)>>(function);
    m_gameObject->RegisterResourceLoadingHandler(handler, E_RESOURCE_CLASS_MATERIAL);
}

void IGameObjectExtension::Get_Shader(const RESOURCE_LOADING_HANDLER& _handler, const std::string& _mode)
{
    
}

void IGameObjectExtension::Get_Texture(const RESOURCE_LOADING_HANDLER& _handler, const std::string& _mode, E_SHADER_SAMPLER _sampler)
{
    
}
