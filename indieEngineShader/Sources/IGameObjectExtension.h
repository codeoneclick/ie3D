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

class CShader;

class IGameObjectExtension : public IGameObject
{
private:
    
protected:
    
    std::shared_ptr<IGameObject> m_gameObject;
    std::function<void(const std::shared_ptr<IGameObjectExtension>&)> m_startLoadingCallback;
    std::function<void(const std::shared_ptr<IGameObjectExtension>&)> m_endLoadingCallback;

    
public:
    
    IGameObjectExtension(const std::shared_ptr<IGameObject>& _gameObject);
    ~IGameObjectExtension(void);
    
    void Load(void);
    
    void Set_StartLoadingCallback(const std::function<void(const std::shared_ptr<IGameObjectExtension>&)>& _callback);
    void Set_EndLoadingCallback(const std::function<void(const std::shared_ptr<IGameObjectExtension>&)>& _callaback);
    
    std::shared_ptr<CMaterial> Get_Material(const std::string& _mode);
    std::shared_ptr<CShader> Get_Shader(const std::string& _mode);
    
};

#endif
