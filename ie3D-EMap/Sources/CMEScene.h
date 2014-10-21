//
//  CMEScene.h
//  indieEngine-EMap
//
//  Created by Sergey Sergeev on 4/20/14.
//
//

#ifndef CMEScene_h
#define CMEScene_h

#include "IScene.h"
#include "HMEDeclaration.h"

class CMapDragController;

class CMEScene final :
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
    CSharedGlobalLightSource m_lightSource;
    CSharedSkyBox m_skyBox;
    CSharedAtmosphericScattering m_atmosphericScattering;
    CSharedLandscape m_landscape;
    CSharedMaterial m_landscapeMaterial;
    
    CSharedMELandscapeBrush m_landscapeBrush;
    
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
    
    void onConfigurationLoaded(ISharedConfigurationRef configuration);
    void onResourceLoaded(ISharedResourceRef resource);
    
    std::vector<ISharedGameObject> colliders(void);
    void onCollision(const glm::vec3& position, ISharedGameObjectRef gameObject, E_INPUT_BUTTON inputButton);
    
    void onGestureRecognizerPressed(const glm::ivec2& point, E_INPUT_BUTTON inputButton);
    void onGestureRecognizerDragged(const glm::ivec2& point, E_INPUT_BUTTON inputButton);
    void onGestureRecognizerMoved(const glm::ivec2& point);
    void onGestureRecognizerReleased(const glm::ivec2& point, E_INPUT_BUTTON inputButton);
    void onGestureRecognizerWheelScroll(E_SCROLL_WHEEL_DIRECTION direction);
    
    void onKeyUp(i32 key);
    void onKeyDown(i32 key);
    
public:
    
    CMEScene(IGameTransition* root);
    ~CMEScene(void);
    
    void load(void);
    void update(f32 deltatime);
    
    CSharedMEUIToSceneCommands getUIToSceneCommands(void) const;
    void setSceneToUICommands(CSharedMESceneToUICommandsRef commands);
};

#endif
