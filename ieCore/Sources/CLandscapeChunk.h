//
//  CLandscapeChunk.h
//  iGaia
//
//  Created by Sergey Sergeev on 3/14/13.
//
//

#ifndef CLandscapeChunk_h
#define CLandscapeChunk_h

#include "IGameObject.h"

class CMesh;
class CQuadTree;

class CLandscapeChunk : public IGameObject
{
private:
    
    friend class CLandscape;
    
    ui32 m_width;
    ui32 m_height;
    ui32 m_numIndexesToRender;
    std::shared_ptr<CQuadTree> m_quadTree;
    
protected:
    
    void _Set_Mesh(const std::shared_ptr<CMesh>& _mesh, ui32 _width, ui32 _height);
    void _Set_SplattingDiffuseTexture(const std::shared_ptr<CTexture>& _texture);
    void _Set_SplattingNormalTexture(const std::shared_ptr<CTexture>& _texture);
    
    void _OnSceneUpdate(f32 _deltatime);
    
    i32 _OnQueuePosition(void);
    void _OnBind(const std::string& _mode);
    void _OnDraw(const std::string& _mode);
    void _OnUnbind(const std::string& _mode);
    void _OnBatch(const std::string& _mode);
    
    void _OnTemplateLoaded(std::shared_ptr<I_RO_TemplateCommon> _template);
    void _OnResourceLoaded(std::shared_ptr<IResource> _resource, bool _success);
    
public:
    
    CLandscapeChunk(const std::shared_ptr<CResourceAccessor>& _resourceAccessor, const std::shared_ptr<IScreenSpaceTextureAccessor>& _screenSpaceTextureAccessor);
    ~CLandscapeChunk(void);
};

#endif 
