//
//  CMEmseScene.h
//  indieEngine-EMap
//
//  Created by Sergey Sergeev on 4/20/14.
//
//

#ifndef CMEmseScene_h
#define CMEmseScene_h

#include "IScene.h"
#include "HMEDeclaration.h"

class CMapDragController;

class CMEmseScene final :
public IScene,
public IGestureRecognizerHandler
{
private:
    
    struct SEditableSettings
    {
        ui32 m_brushSize;
        ui32 m_brushStrength;
        ui32 m_falloffCoefficient;
        ui32 m_smoothCoefficient;
    };
    
protected:
    
    std::shared_ptr<CMapDragController> m_mapDragController;
    CSharedGlobalLightSource m_globalLightSource;
    CSharedSkyBox m_skybox;
    CSharedLandscape m_landscape;
    CSharedMaterial m_landscapeMaterial;
    
    CMESharedLandscapeBrush m_landscapeBrush;
    CMESharedModelBrush m_modelBrush;
    
    glm::ivec2 m_previousDraggedPoint;
    
    SEditableSettings m_editableSettings;
    
    CSharedMEUIToSceneCommands m_uiToSceneCommands;
    CSharedMESceneToUICommands m_sceneToUICommands;
    
    void setBrushSize(ui32 value);
    void setBrushStrength(ui32 value);
    void setFalloffCoefficient(ui32 value);
    void setSmoothCoefficient(ui32 value);
    void setTextureSampler(const std::string& filename, E_SHADER_SAMPLER sampler);
    void setTillingTexcoord(f32 value, E_SHADER_SAMPLER sampler);
    void generateVertecesDataCommand(const glm::ivec2& size, f32 frequency, i32 octaves, ui32 seed);
    
    void onConfigurationLoaded(ISharedConfigurationRef configuration);
    void onResourceLoaded(ISharedResourceRef resource);
    
    std::vector<ISharedGameObject> colliders(void);
    
    void onGestureRecognizerPressed(const glm::ivec2& point, E_INPUT_BUTTON inputButton);
    void onGestureRecognizerDragged(const glm::ivec2& point, E_INPUT_BUTTON inputButton);
    void onGestureRecognizerMoved(const glm::ivec2& point);
    void onGestureRecognizerReleased(const glm::ivec2& point, E_INPUT_BUTTON inputButton);
    void onGestureRecognizerWheelScroll(E_SCROLL_WHEEL_DIRECTION direction);
    
    void onKeyUp(i32 key);
    void onKeyDown(i32 key);
    
public:
    
    CMEmseScene(IGameTransition* root);
    ~CMEmseScene(void);
    
    void load(void);
    void update(f32 deltatime);
    
    CSharedMEUIToSceneCommands getUIToSceneCommands(void) const;
    void setSceneToUICommands(CSharedMESceneToUICommandsRef commands);
};

#endif
