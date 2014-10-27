//
//  CMEGameTransition.h
//  indieEngine-EMap
//
//  Created by Sergey Sergeev on 4/20/14.
//
//

#ifndef CMEGameTransition_h
#define CMEGameTransition_h

#include "IEGameTransition.h"
#include "HMEDeclaration.h"

class CMEGameTransition : public IEGameTransition
{
private:
    
protected:
    
    void _OnLoaded(void);
    void _OnGameLoopUpdate(f32 deltatime);
    
public:
    
    CMEGameTransition(const std::string& filename,
                      std::shared_ptr<IGraphicsContext> graphicsContext,
                      std::shared_ptr<IInputContext> gestureRecognizerContext,
                      std::shared_ptr<CResourceAccessor> resourceAccessor,
                      std::shared_ptr<CConfigurationAccessor> configurationAccessor);
    
    ~CMEGameTransition(void);
    
    void initScene(void);
    
    CMESharedLandscapeBrush createLandscapeBrush(const std::string& filename);
    CMESharedPlacementModel createPlacementModel(const std::string& filename);
    
    CSharedMEUIToSceneCommands getUIToSceneCommands(void) const;
    void setSceneToUICommands(CSharedMESceneToUICommandsRef commands);
};

#endif
