//
//  IGameObjectExtension.h
//  indieEngineShader
//
//  Created by Sergey Sergeev on 11/6/13.
//
//

#ifndef IGameObjectExtension_h
#define IGameObjectExtension_h

#include "IGameObject.h"

class CMaterialExtension;

class CSceneGraph;
class CSceneFabricator;

class IGameObjectExtension : public IGameObject
{
private:
    
    typedef std::function<void(const std::set<std::string> _modes)> __MODES_LOADED_COMMAND;
    
protected:
    
    std::shared_ptr<IGameObject> m_gameObject;
    std::map<std::string, std::shared_ptr<CMaterialExtension>> m_materialsExtension;
    std::shared_ptr<CSceneGraph> m_sceneGraph;
    std::shared_ptr<CSceneFabricator> m_sceneFabricator;
    
    RESOURCE_LOADING_HANDLER m_resourceLoadingHandler;
    TEMPLATE_LOADING_HANDLER m_templateLoadingHandler;
    
    void _OnExtensionResourceLoaded(const std::shared_ptr<IResource>& _resource);
    void _OnExtensionTemplateLoaded(const std::shared_ptr<I_RO_TemplateCommon> _template);
    
public:
    
    IGameObjectExtension(const std::shared_ptr<CSceneGraph>& _sceneGraph, const std::shared_ptr<CSceneFabricator>& _sceneFabricator);
    ~IGameObjectExtension(void);
    
    void Load(const std::string& _filename);
    
    inline const std::shared_ptr<IGameObject> Get_GameObject(void)
    {
        return m_gameObject;
    };
    
    void Get_Template(const TEMPLATE_LOADING_HANDLER& _handler);
    
    void Get_Material(const RESOURCE_LOADING_HANDLER& _handler, const std::string& _mode);
    void Get_Shader(const RESOURCE_LOADING_HANDLER& _handler, const std::string& _mode);
    void Get_Texture(const RESOURCE_LOADING_HANDLER& _handler, const std::string& _mode, E_SHADER_SAMPLER _sampler);
};

#endif
