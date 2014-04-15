//
//  CKOTHInGameTransition.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 6/12/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CKOTHInGameTransition_h
#define CKOTHInGameTransition_h

#include "HCommon.h"
#include "IGameTransition.h"

class CKOTHInGameTransition : public IGameTransition
{
private:
    
protected:
    
    void _OnLoaded(void);
    void _OnGameLoopUpdate(f32 _deltatime);
    
public:
    CKOTHInGameTransition(const std::string& _filename, std::shared_ptr<IGraphicsContext> _graphicsContext, std::shared_ptr<IInputContext> _inputContext, std::shared_ptr<CResourceAccessor> _resourceAccessor, std::shared_ptr<CConfigurationAccessor> _templateAccessor);
    ~CKOTHInGameTransition(void);
};

#endif 
