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
    
    std::shared_ptr<CMaterial> Get_Material(const std::string& _mode);
    std::shared_ptr<CShader> Get_Shader(const std::string& _mode);
    
public:
    
    IGameObjectExtension(const std::shared_ptr<IGameObject>& _gameObject);
    ~IGameObjectExtension(void);
    
};

#endif
