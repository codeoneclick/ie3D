//
//  CEditableSceneTransition.h
//  indieEngine-EMap
//
//  Created by Sergey Sergeev on 4/20/14.
//
//

#ifndef CEditableSceneTransition_h
#define CEditableSceneTransition_h

#include "IGameTransition.h"
#include "HMEDeclaration.h"

class CEditableSceneTransition : public IGameTransition
{
private:
    
protected:
    
    void _OnLoaded(void);
    void _OnGameLoopUpdate(f32 deltatime);
    
public:
    
    CEditableSceneTransition(const std::string& filename,
                             std::shared_ptr<IGraphicsContext> graphicsContext,
                             std::shared_ptr<IInputContext> gestureRecognizerContext,
                             std::shared_ptr<CResourceAccessor> resourceAccessor,
                             std::shared_ptr<CConfigurationAccessor> configurationAccessor);
    
    ~CEditableSceneTransition(void);
    
    void initScene(void);
    
    CSharedSelectionArea createSelectionArea(const std::string& filename);
    void deleteSelectionArea(CSharedSelectionAreaRef selectionArea);
};


#endif
