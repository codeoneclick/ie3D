//
//  CMESceneTransition.h
//  indieEngine-EMap
//
//  Created by Sergey Sergeev on 4/20/14.
//
//

#ifndef CMESceneTransition_h
#define CMESceneTransition_h

#include "IGameTransition.h"
#include "HMEDeclaration.h"

class CMESceneTransition : public IGameTransition
{
private:
    
protected:
    
    void _OnLoaded(void);
    void _OnGameLoopUpdate(f32 deltatime);
    
public:
    
    CMESceneTransition(const std::string& filename,
                             std::shared_ptr<IGraphicsContext> graphicsContext,
                             std::shared_ptr<IInputContext> gestureRecognizerContext,
                             std::shared_ptr<CResourceAccessor> resourceAccessor,
                             std::shared_ptr<CConfigurationAccessor> configurationAccessor);
    
    ~CMESceneTransition(void);
    
    void initScene(void);
    
    CSharedMELandscapeBrush createLandscapeBrush(const std::string& filename);
    void deleteLandscapeBrush(CSharedMELandscapeBrushRef gameObject);
    
    CSharedMETankComplex createTankComplex(const std::string& filename);
    void deleteTankComplex(CSharedMETankComplexRef gameObject);
    
    CSharedMEUIToSceneCommands getUIToSceneCommands(void) const;
    void setSceneToUICommands(CSharedMESceneToUICommandsRef commands);
};


#endif
