//
//  CLandscapeEdges.h
//  iGaia
//
//  Created by sergey sergeev on 7/11/12.
//
//

#ifndef CLandscapeEdges_h
#define CLandscapeEdges_h

#include "IGameObject.h"

class CLandscapeEdges : public IGameObject
{
private:

    ui32 m_width;
    ui32 m_height;
    glm::vec2 m_heightBounds;
    
    friend class CLandscape;

protected:

    void _OnSceneUpdate(f32 _deltatime);
    void _Set_EdgeTexture(const std::shared_ptr<CTexture>& _texture);
    
    i32 _OnQueuePosition(void);
    void _OnBind(const std::string& _mode);
    void _OnDraw(const std::string& _mode);
    void _OnUnbind(const std::string& _mode);
    void _OnBatch(const std::string& _mode);
    
    void _OnTemplateLoaded(std::shared_ptr<I_RO_TemplateCommon> _template);
    void _OnResourceLoaded(std::shared_ptr<IResource> _resource, bool _success);
    
public:
    
    CLandscapeEdges(const std::shared_ptr<CResourceAccessor>& _resourceAccessor, const std::shared_ptr<IScreenSpaceTextureAccessor>& _screenSpaceTextureAccessor);
    ~CLandscapeEdges(void);
};

#endif 
