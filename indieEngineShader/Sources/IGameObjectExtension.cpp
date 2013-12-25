//
//  IGameObjectExtension.cpp
//  indieEngineShader
//
//  Created by Sergey Sergeev on 11/6/13.
//
//

#include "IGameObjectExtension.h"
#include "CMaterial.h"
#include "CTexture.h"
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
    m_templateLoadingHandler = std::make_shared<std::function<void(const std::shared_ptr<I_RO_TemplateCommon>&)>>(std::bind(&IGameObjectExtension::_OnExtensionTemplateLoaded, this, std::placeholders::_1));
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

void IGameObjectExtension::_OnExtensionTemplateLoaded(const std::shared_ptr<I_RO_TemplateCommon> _template)
{
    m_template = _template;
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

void IGameObjectExtension::Get_Template(const TEMPLATE_LOADING_HANDLER &_handler)
{
    assert(m_gameObject != nullptr);
    if(m_template != nullptr)
    {
        (*_handler)(m_template);
    }
    m_gameObject->RegisterTemplateLoadingHandler(_handler);
}

void IGameObjectExtension::Get_Material(const RESOURCE_LOADING_HANDLER& _handler, const std::string& _mode)
{
    assert(m_gameObject != nullptr);
    auto materialExtension = m_materialsExtension.find(_mode);
    if(materialExtension != m_materialsExtension.end() && materialExtension->second->Get_Material()->IsCommited())
    {
        (*_handler)(materialExtension->second);
    }
    
    RESOURCE_LOADING_HANDLER handler;
    std::function<void(const std::shared_ptr<IResource>&)> function = [_mode, _handler, handler, this](const std::shared_ptr<IResource>& _resource)
    {
        auto materialExtension = m_materialsExtension.find(_mode);
        if(materialExtension != m_materialsExtension.end() && materialExtension->second->Get_Material()->IsCommited())
        {
            (*_handler)(materialExtension->second);
        }
        else
        {
            assert(false);
        }
        m_gameObject->UnregisterResourceLoadingHandler(handler, _resource->Get_Class());
    };
    handler = std::make_shared<std::function<void(const std::shared_ptr<IResource>&)>>(function);
    m_gameObject->RegisterResourceLoadingHandler(handler, E_RESOURCE_CLASS_MATERIAL);
}

void IGameObjectExtension::Get_Shader(const RESOURCE_LOADING_HANDLER& _handler, const std::string& _mode)
{
    assert(m_gameObject != nullptr);
    auto materialExtension = m_materialsExtension.find(_mode);
    if(materialExtension != m_materialsExtension.end() && materialExtension->second->Get_Material()->Get_Shader()->IsCommited())
    {
        (*_handler)(materialExtension->second->Get_ShaderExtension());
    }
    
    RESOURCE_LOADING_HANDLER handler;
    std::function<void(const std::shared_ptr<IResource>&)> function = [_mode, _handler, handler, this](const std::shared_ptr<IResource>& _resource)
    {
        auto materialExtension = m_materialsExtension.find(_mode);
        if(materialExtension != m_materialsExtension.end() && materialExtension->second->Get_Material()->Get_Shader()->IsCommited())
        {
            (*_handler)(materialExtension->second->Get_ShaderExtension());
        }
        else
        {
            assert(false);
        }
        m_gameObject->UnregisterResourceLoadingHandler(handler, _resource->Get_Class());
    };
    handler = std::make_shared<std::function<void(const std::shared_ptr<IResource>&)>>(function);
    m_gameObject->RegisterResourceLoadingHandler(handler, E_RESOURCE_CLASS_SHADER);
}

void IGameObjectExtension::Get_Texture(const RESOURCE_LOADING_HANDLER& _handler, const std::string& _mode, E_SHADER_SAMPLER _sampler)
{
    assert(m_gameObject != nullptr);
    auto materialExtension = m_materialsExtension.find(_mode);
    if(materialExtension != m_materialsExtension.end() && materialExtension->second->Get_Material()->Get_Texture(_sampler) != nullptr && materialExtension->second->Get_Material()->Get_Texture(_sampler)->IsCommited())
    {
        (*_handler)(materialExtension->second->Get_Material()->Get_Texture(_sampler));
    }
    
    RESOURCE_LOADING_HANDLER handler;
    std::function<void(const std::shared_ptr<IResource>&)> function = [_mode, _handler, handler, _sampler, this](const std::shared_ptr<IResource>& _resource)
    {
        auto materialExtension = m_materialsExtension.find(_mode);
        if(materialExtension != m_materialsExtension.end() && materialExtension->second->Get_Material()->Get_Texture(_sampler) != nullptr && materialExtension->second->Get_Material()->Get_Texture(_sampler)->IsCommited())
        {
            (*_handler)(materialExtension->second->Get_Material()->Get_Texture(_sampler));
        }
        else
        {
            assert(false);
        }
        m_gameObject->UnregisterResourceLoadingHandler(handler, _resource->Get_Class());
    };
    handler = std::make_shared<std::function<void(const std::shared_ptr<IResource>&)>>(function);
    m_gameObject->RegisterResourceLoadingHandler(handler, E_RESOURCE_CLASS_SHADER);
}
