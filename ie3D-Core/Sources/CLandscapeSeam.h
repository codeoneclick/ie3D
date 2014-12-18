//
//  CLandscapeSeam.h
//  ie3D-Core
//
//  Created by sergey.sergeev on 10/17/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#ifndef CLandscapeSeam_h
#define CLandscapeSeam_h

#include "IGameObject.h"

class CLandscapeSeam : public IGameObject
{
private:
    

protected:

    void onSceneUpdate(f32 deltatime);
    
    void onResourceLoaded(ISharedResourceRef resource, bool success);
    void onConfigurationLoaded(ISharedConfigurationRef configuration, bool success);
    
    i32  zOrder(void);
    bool checkOcclusion(void);
    ui32 numTriangles(void);
    void onBind(const std::string& renderTechnique);
    void onDraw(const std::string& renderTechnique);
    void onUnbind(const std::string& renderTechnique);
    void onBatch(const std::string& renderTechnique);
    
    void onOcclusionQueryDraw(CSharedMaterialRef material);
    void onOcclusionQueryUpdate(void);
    
    ui32 m_seamMaxLength;
    
    std::vector<SAttributeVertex> m_edge_01;
    std::vector<SAttributeVertex> m_edge_02;
    
public:
    
    CLandscapeSeam(CSharedResourceAccessorRef resourceAccessor,
                    ISharedRenderTechniqueAccessorRef renderTechniqueAccessor);
    ~CLandscapeSeam(void);
    
    void setVertexesToSewTogether(const std::vector<SAttributeVertex>& edge_01,
                                  const std::vector<SAttributeVertex>& edge_02);
};

#endif
