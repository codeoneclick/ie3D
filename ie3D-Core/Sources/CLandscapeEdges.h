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

    void setEdgeTexture(CSharedTextureRef texture);
    
    void onSceneUpdate(f32 deltatime);
    
    void onResourceLoaded(ISharedResourceRef resource, bool success);
    void onConfigurationLoaded(ISharedConfigurationRef configuration, bool success);
    
    i32  zOrder(void);
    bool checkOcclusion(void);
    ui32 numTriangles(void);
    void onBind(const std::string& mode);
    void onDraw(const std::string& mode);
    void onUnbind(const std::string& mode);
    void onBatch(const std::string& mode);
    
    void onOcclusionQueryDraw(CSharedMaterialRef material);
    void onOcclusionQueryUpdate(void);
    
public:
    
    CLandscapeEdges(CSharedResourceAccessorRef resourceAccessor,
                    ISharedRenderTechniqueAccessorRef renderTechniqueAccessor);
    ~CLandscapeEdges(void);
};

#endif 
