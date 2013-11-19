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

IGameObjectExtension::IGameObjectExtension(const std::shared_ptr<CSceneGraph>& _sceneGraph, const std::shared_ptr<CSceneFabricator>& _sceneFabricator) :
IGameObject(nullptr, nullptr),
m_gameObject(nullptr),
m_sceneGraph(_sceneGraph),
m_sceneFabricator(_sceneFabricator)
{
    
}

IGameObjectExtension::~IGameObjectExtension(void)
{
    m_gameObject->Listen(nullptr, E_RESOURCE_CLASS_MATERIAL);
    m_gameObject->Listen(nullptr, E_RESOURCE_CLASS_SHADER);
    m_gameObject->Listen(nullptr, E_RESOURCE_CLASS_TEXTURE);
    
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
    RESOURCE_LOADED_HANDLER handler = IResourceLoadingHandler::_Get_Handler(_resource->Get_Class());
    if(handler != nullptr)
    {
        handler(_resource);
    }
}

void IGameObjectExtension::Load(const std::string &_filename)
{
    assert(m_sceneGraph != nullptr);
    assert(m_sceneFabricator != nullptr);
    assert(m_gameObject == nullptr);
    
    m_gameObject = m_sceneFabricator->CreateModel(_filename);
    m_gameObject->Set_Position(glm::vec3(0.0f, 0.0f, 0.0f));
    m_gameObject->Set_Scale(glm::vec3(10.0f, 10.0f, 10.0f));

    RESOURCE_LOADED_HANDLER handler = std::bind(&IGameObjectExtension::_OnExtensionResourceLoaded, this, std::placeholders::_1);
    m_gameObject->Listen(handler, E_RESOURCE_CLASS_MATERIAL);
    m_gameObject->Listen(handler, E_RESOURCE_CLASS_SHADER);
    m_gameObject->Listen(handler, E_RESOURCE_CLASS_TEXTURE);
    
    m_sceneGraph->InsertModel(std::static_pointer_cast<CModel>(m_gameObject));
}
