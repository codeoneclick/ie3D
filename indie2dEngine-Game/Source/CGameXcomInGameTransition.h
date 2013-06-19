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

class CModel;

class CGameXcomInGameTransition : public IGameTransition
{
private:
    
protected:
    
    std::shared_ptr<CModel> m_model;
    
    void _OnLoaded(void);
    void _OnGameLoopUpdate(f32 _deltatime);
    
public:
    CGameXcomInGameTransition(const std::string& _filename, std::shared_ptr<IGraphicsContext> _graphicsContext, std::shared_ptr<CResourceAccessor> _resourceAccessor, std::shared_ptr<CTemplateAccessor> _templateAccessor);
    ~CGameXcomInGameTransition(void);
};

#endif 
