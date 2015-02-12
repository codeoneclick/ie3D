//
//  CMEmseTransition.h
//  indieEngine-EMap
//
//  Created by Sergey Sergeev on 4/20/14.
//
//

#ifndef CMEmseTransition_h
#define CMEmseTransition_h

#include "IEGameTransition.h"
#include "HMEDeclaration.h"

class CMEmseTransition : public IEGameTransition
{
private:
    
protected:
    
    void _OnLoaded(void);
    void _OnGameLoopUpdate(f32 deltatime);
    
public:
    
    CMEmseTransition(const std::string& filename, bool isOffscreen);
    ~CMEmseTransition(void);
    
    void initScene(void);
    
    CMESharedLandscapeBrush createLandscapeBrush(const std::string& filename);
    CMESharedModelBrush createModelBrush(const std::string& filename);
};

#endif
