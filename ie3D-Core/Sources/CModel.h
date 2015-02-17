//
//  CModel.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/28/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CModel_h
#define CModel_h

#include "IGameObject.h"
#include "HDeclaration.h"

class CModel final : public IGameObject
{
private:
    
protected:
    
    CSharedAnimationMixer m_animationMixer;
    std::map<std::string, std::string> m_animationNamesLinkage;
    
    void onSceneUpdate(f32 deltatime);
    
    void onResourceLoaded(ISharedResourceRef resource, bool success);
    void onConfigurationLoaded(ISharedConfigurationRef configuration, bool success);
    
    void onDraw(CSharedMaterialRef material);
    
    void bindCustomShaderUniforms(CSharedMaterialRef material);
    
public:
    
    CModel(CSharedResourceAccessorRef resourceAccessor,
           ISharedRenderTechniqueAccessorRef renderTechniqueAccessor);
    ~CModel(void);
    
    void setAnimation(const std::string& name);
};



#endif 
