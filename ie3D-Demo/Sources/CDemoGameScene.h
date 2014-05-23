//
//  CDemoGameScene.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 7/22/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CDemoGameScene_h
#define CDemoGameScene_h

#include "HCommon.h"
#include "IScene.h"
#include "HDeclaration.h"

class CMapDragController;

class CDemoGameScene final : public IScene
{
private:
    
protected:
    
    std::shared_ptr<CMapDragController> m_mapDragController;
    CSharedSkyBox m_skyBox;
    
    void onCollision(const glm::vec3& position, ISharedGameObjectRef gameObject);
    
public:
    
    CDemoGameScene(IGameTransition* root);
    ~CDemoGameScene(void);
    
    void load(void);
    void update(f32 deltatime);
};

#endif 