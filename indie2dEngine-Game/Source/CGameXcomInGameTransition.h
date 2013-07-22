//
//  CGameXcomInGameTransition.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 6/12/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CGameXcomInGameTransition_h
#define CGameXcomInGameTransition_h

#include "HCommon.h"
#include "IGameTransition.h"

class CMainMenuScene;

class CGameXcomInGameTransition : public IGameTransition
{
private:
    
    std::shared_ptr<CMainMenuScene> m_scene;
    
protected:
    
    void _OnLoaded(void);
    void _OnGameLoopUpdate(f32 _deltatime);
    
public:
    CGameXcomInGameTransition(const std::string& _filename, std::shared_ptr<IGraphicsContext> _graphicsContext, std::shared_ptr<IInputContext> _inputContext, std::shared_ptr<CResourceAccessor> _resourceAccessor, std::shared_ptr<CTemplateAccessor> _templateAccessor);
    ~CGameXcomInGameTransition(void);
};

#endif 
