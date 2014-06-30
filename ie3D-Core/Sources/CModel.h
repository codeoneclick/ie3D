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
    
    void onSceneUpdate(f32 deltatime);
    
    void onResourceLoaded(ISharedResourceRef resource, bool success);
    void onConfigurationLoaded(ISharedConfigurationRef configuration, bool success);
    
    i32  zOrder(void);
    bool checkOcclusion(void);
    ui32 numTriangles(void);
    void onBind(const std::string& mode);
    void onDraw(const std::string& mode);
    void onUnbind(const std::string& mode);
    void onBatch(const std::string& mode);
    
public:
    
    CModel(CSharedResourceAccessorRef resourceAccessor,
           ISharedScreenSpaceRenderAccessorRef screenSpaceTextureAccessor);
    ~CModel(void);
    
    void setAnimation(const std::string& name);
};



#endif 
