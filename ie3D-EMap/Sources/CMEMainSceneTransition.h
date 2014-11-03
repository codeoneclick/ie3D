//
//  CMEMainSceneTransition.h
//  indieEngine-EMap
//
//  Created by Sergey Sergeev on 4/20/14.
//
//

#ifndef CMEMainSceneTransition_h
#define CMEMainSceneTransition_h

#include "IEGameTransition.h"
#include "HMEDeclaration.h"

class CMEMainSceneTransition : public IEGameTransition
{
private:
    
protected:
    
    void _OnLoaded(void);
    void _OnGameLoopUpdate(f32 deltatime);
    
public:
    
    CMEMainSceneTransition(const std::string& filename);
    ~CMEMainSceneTransition(void);
    
    void initScene(void);
    
    CMESharedLandscapeBrush createLandscapeBrush(const std::string& filename);
    CMESharedModelBrush createModelBrush(const std::string& filename);
    
    CSharedMEUIToSceneCommands getUIToSceneCommands(void) const;
    void setSceneToUICommands(CSharedMESceneToUICommandsRef commands);
};

#endif
