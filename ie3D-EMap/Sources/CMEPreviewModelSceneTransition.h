//
//  CMEPreviewModelSceneTransition.h
//  ie3D-EMap
//
//  Created by sergey.sergeev on 11/3/14.
//
//

#ifndef CMEPreviewModelSceneTransition_h
#define CMEPreviewModelSceneTransition_h

#include "IEGameTransition.h"
#include "HMEDeclaration.h"

class CMEPreviewModelSceneTransition : public IEGameTransition
{
private:
    
protected:
    
    void _OnLoaded(void);
    void _OnGameLoopUpdate(f32 deltatime);
    
public:
    
    CMEPreviewModelSceneTransition(const std::string& filename, bool isOffscreen);
    ~CMEPreviewModelSceneTransition(void);
    
    void initScene(void);
    
    CMESharedLandscapeBrush createLandscapeBrush(const std::string& filename);
    CMESharedModelBrush createModelBrush(const std::string& filename);
};


#endif
