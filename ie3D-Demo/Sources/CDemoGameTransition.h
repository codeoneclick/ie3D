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
#include "IEGameTransition.h"
#include "HDEDeclaration.h"

class CDemoGameTransition : public IEGameTransition
{
private:
    
protected:
    
    void _OnLoaded(void);
    void _OnGameLoopUpdate(f32 deltatime);
    
public:
    
    CDemoGameTransition(const std::string& filename);
    ~CDemoGameTransition(void);
    
    void initScene(void);
    
    CDESharedUIToSceneCommands getUIToSceneCommands(void) const;
    void setSceneToUICommands(CDESharedSceneToUICommandsRef commands);
};

#endif
