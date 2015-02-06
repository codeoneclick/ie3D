//
//  CMEgopTransition.h
//  ie3D-EMap
//
//  Created by sergey.sergeev on 11/3/14.
//
//

#ifndef CMEgopTransition_h
#define CMEgopTransition_h

#include "IEGameTransition.h"
#include "HMEDeclaration.h"

class CMEgopTransition : public IEGameTransition
{
private:
    
protected:
    
    void _OnLoaded(void);
    void _OnGameLoopUpdate(f32 deltatime);
    
public:
    
    CMEgopTransition(const std::string& filename, bool isOffscreen);
    ~CMEgopTransition(void);
    
    void initScene(void);
};


#endif
