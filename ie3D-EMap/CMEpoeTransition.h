//
//  CMEpoeTransition.h
//  ie3D-EMap-OSX
//
//  Created by sergey.sergeev on 7/28/15.
//
//

#ifndef CMEpoeTransition_h
#define CMEpoeTransition_h

#include "IEGameTransition.h"
#include "HMEDeclaration.h"

class CMEpoeTransition : public IEGameTransition
{
private:
    
protected:
    
    void _OnLoaded(void);
    void _OnGameLoopUpdate(f32 deltatime);
    
public:
    
    CMEpoeTransition(const std::string& filename, bool isOffscreen);
    ~CMEpoeTransition(void);
    
    void initScene(void);
};

#endif
