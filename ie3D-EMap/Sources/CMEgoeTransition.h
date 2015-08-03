//
//  CMEgoeTransition.h
//  ie3D-EMap
//
//  Created by sergey.sergeev on 2/6/15.
//
//

#ifndef CMEgoeTransition_h
#define CMEgoeTransition_h

#include "IEGameTransition.h"
#include "HMEDeclaration.h"

class CMEgoeTransition : public IEGameTransition
{
private:
    
protected:
    
    void _OnLoaded(void);
    void _OnGameLoopUpdate(f32 deltatime);
    
public:
    
    CMEgoeTransition(const std::string& filename, bool isOffscreen);
    ~CMEgoeTransition(void);
    
    void initScene(void);
    
    CMESharedSceneStage createSceneStage(const std::string& filename);
};

#endif
