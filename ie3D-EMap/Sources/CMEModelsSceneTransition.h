//
//  CMEModelsSceneTransition.h
//  ie3D-EMap
//
//  Created by sergey.sergeev on 11/3/14.
//
//

#ifndef CMEModelsSceneTransition_h
#define CMEModelsSceneTransition_h

#include "IEGameTransition.h"
#include "HMEDeclaration.h"

class CMEModelsSceneTransition : public IEGameTransition
{
private:
    
protected:
    
    void _OnLoaded(void);
    void _OnGameLoopUpdate(f32 deltatime);
    
public:
    
    CMEModelsSceneTransition(const std::string& filename);
    ~CMEModelsSceneTransition(void);
    
    void initScene(void);
    
    CMESharedLandscapeBrush createLandscapeBrush(const std::string& filename);
    CMESharedModelBrush createModelBrush(const std::string& filename);
};


#endif
