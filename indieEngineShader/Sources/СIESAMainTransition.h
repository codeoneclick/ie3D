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

class CIESAMainTransition : public IGameTransition
{
private:
    
protected:
    
    void _OnLoaded(void);
    void _OnGameLoopUpdate(f32 _deltatime);
    
public:
    CIESAMainTransition(const std::string& _filename, std::shared_ptr<IGraphicsContext> _graphicsContext, std::shared_ptr<IInputContext> _inputContext, std::shared_ptr<CResourceAccessor> _resourceAccessor, std::shared_ptr<CTemplateAccessor> _templateAccessor);
    ~CIESAMainTransition(void);
};

#endif
