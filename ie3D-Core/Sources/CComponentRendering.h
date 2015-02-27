//
//  CComponentRender.h
//  ie3D-Core
//
//  Created by sergey.sergeev on 1/22/15.
//  Copyright (c) 2015 Sergey Sergeev. All rights reserved.
//

#ifndef CComponentRendering_h
#define CComponentRendering_h

#include "IComponent.h"
#include "CMaterial.h"

class CComponentRendering : public IComponent
{
private:
    
    std::unordered_map<std::string, CSharedMaterial> m_materials;
    
    CSharedFrustum m_cameraFrustum;
    i32 m_zOrder;
    
    typedef std::function<void(CSharedMaterialRef)> T_DRAW_COMMAND;
    T_DRAW_COMMAND m_drawCommand;
    
    typedef std::function<bool(CSharedFrustumRef)> T_CHECK_IN_CAMERA_FRUSTUM_COMMAND;
    T_CHECK_IN_CAMERA_FRUSTUM_COMMAND m_checkInCameraFrustumCommand;
    
protected:
    
public:
    
    CComponentRendering(ISharedConfigurationGameObjectRef configuration,
                        CSharedResourceAccessorRef resourceAccessor,
                        ISharedRenderTechniqueAccessorRef renderTechniqueAccessor,
                        ISharedResourceLoadingHandlerRef handler,
                        CSharedFrustumRef cameraFrustum);
    ~CComponentRendering(void);
    
    E_COMPONENT_CLASS getClass(void) const;
    
    CSharedMaterial getMaterial(const std::string& techniqueName) const;
    i32 getZOrder(void) const;
    
    void setDrawCommand(const T_DRAW_COMMAND& command);
    void draw(const std::string& techniqueName);
    
    void setCheckInCameraFrustumCommand(const T_CHECK_IN_CAMERA_FRUSTUM_COMMAND& command);
    bool isInCameraFrustum(void) const;
    
    template<typename T_VALUE>
    void setCustomShaderUniform(T_VALUE value, const std::string& uniform, const std::string& techniqueName)
    {
        if(techniqueName.length() != 0)
        {
            CSharedMaterial material = CComponentRendering::getMaterial(techniqueName);
            assert(material);
            material->setCustomShaderUniform(value, uniform);
        }
        else
        {
            std::for_each(m_materials.cbegin(), m_materials.cend(), [value, uniform](std::pair<std::string, CSharedMaterial> material) {
                material.second->setCustomShaderUniform(value, uniform);
            });
        }
    };
    
    void setTexture(CSharedTextureRef texture, E_SHADER_SAMPLER sampler,
                    ISharedResourceLoadingHandlerRef handler, const std::string& techniqueName = "");
};

#endif
