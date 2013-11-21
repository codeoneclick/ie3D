//
//  CLandscapeEdges.h
//  iGaia
//
//  Created by sergey sergeev on 7/11/12.
//
//

#ifndef CLandscapeEdges_h
#define CLandscapeEdges_h

#include "CGameObject3d.h"

class CLandscapeEdges : public CGameObject3d
{
private:

    ui32 m_width;
    ui32 m_height;
    glm::vec2 m_heightBound;

protected:

    void OnResourceDidLoad(TSharedPtrResource _resource);

    void OnUpdate(f32 _deltatime);

    ui32 OnDrawIndex(void);
    void OnBind(E_RENDER_MODE_WORLD_SPACE _mode);
    void OnDraw(E_RENDER_MODE_WORLD_SPACE _mode);
    void OnUnbind(E_RENDER_MODE_WORLD_SPACE _mode);
    
public:
    
    CLandscapeEdges(void);
    ~CLandscapeEdges(void);

    void Load(CResourceMgrsFacade* _resourceMgrsFacade, CShaderComposite* _shaderComposite, const std::string& _filename)
    {
        assert(false);
    };

    void Load(CMaterial* _material, ui32 _width, ui32 _height, const glm::vec2 _heightBound);
    
};

#endif 
