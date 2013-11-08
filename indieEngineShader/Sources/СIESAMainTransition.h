//
//  CIESAMainTransition.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 6/12/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CIESAMainTransition_h
#define CIESAMainTransition_h

#include "HCommon.h"
#include "IGameTransition.h"

class CShaderExtension;
class IGameObjectExtension;

class CCamera;
class CModel;

class CIESAMainTransition : public IGameTransition
{
private:
    
protected:
    
    void _OnLoaded(void);
    void _OnGameLoopUpdate(f32 _deltatime);
    
    std::shared_ptr<CCamera> m_camera;
    std::shared_ptr<CModel> m_model;
    
    std::shared_ptr<CShaderExtension> m_shaderExtension;
    std::shared_ptr<IGameObjectExtension> m_gameObjectExtension;
    
public:
    
    CIESAMainTransition(const std::string& _filename, std::shared_ptr<IGraphicsContext> _graphicsContext, std::shared_ptr<IInputContext> _inputContext, std::shared_ptr<CResourceAccessor> _resourceAccessor, std::shared_ptr<CTemplateAccessor> _templateAccessor);
    ~CIESAMainTransition(void);
    
    
    std::shared_ptr<CShaderExtension> Get_ShaderExtension(void);
    std::shared_ptr<IGameObjectExtension> Get_GameObjectExtension(void);
    
    void CreateGameObject(void);
    void LoadGameObject(const std::string& _filename);
};

#endif
