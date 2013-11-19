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
    
protected:
    
    std::shared_ptr<IGameObject> m_gameObject;
    std::map<std::string, std::shared_ptr<CMaterialExtension>> m_materials;
    std::shared_ptr<CSceneGraph> m_sceneGraph;
    std::shared_ptr<CSceneFabricator> m_sceneFabricator;
    
    void _OnExtensionResourceLoaded(const std::shared_ptr<IResource>& _resource);
    
public:
    
    IGameObjectExtension(const std::shared_ptr<CSceneGraph>& _sceneGraph, const std::shared_ptr<CSceneFabricator>& _sceneFabricator);
    ~IGameObjectExtension(void);
    
    void Load(const std::string& _filename);
    
    inline const std::shared_ptr<IGameObject> Get_GameObject(void)
    {
        return m_gameObject;
    };
};

#endif
