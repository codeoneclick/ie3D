//
//  CDemoGameTransition.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 6/12/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CDemoGameTransition_h
#define CDemoGameTransition_h

#include "HCommon.h"
#include "IGameTransition.h"

class CDemoGameTransition : public IGameTransition
{
private:
    
protected:
    
    void _OnLoaded(void);
    void _OnGameLoopUpdate(f32 _deltatime);
    
public:
    CDemoGameTransition(const std::string& _filename, std::shared_ptr<IGraphicsContext> _graphicsContext, std::shared_ptr<IInputContext> _inputContext, std::shared_ptr<CResourceAccessor> _resourceAccessor, std::shared_ptr<CConfigurationAccessor> _templateAccessor);
    ~CDemoGameTransition(void);
};

#endif 
