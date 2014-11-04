//
//  IEGameTransition.h
//  ie3D-Engine
//
//  Created by sergey.sergeev on 8/6/14.
//  Copyright (c) 2014 sergey.sergeev. All rights reserved.
//

#ifndef IEGameTransition_h
#define IEGameTransition_h

#include "IGameTransition.h"
#include "HEDeclaration.h"

class IEGameTransition : public IGameTransition
{
private:
    
protected:
    
    void _OnLoaded(void);
    void _OnGameLoopUpdate(f32 deltatime);
    
public:
    
    IEGameTransition(const std::string& filename, bool isOffscreen);
    ~IEGameTransition(void);
    
    void initScene(void);
    
    CESharedComplexModel createComplexModel(const std::string& filename);
    CESharedCustomModel createCustomModel(const std::string& filename);
};

#endif
