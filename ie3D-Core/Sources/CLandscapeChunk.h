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
#include "HDeclaration.h"

class CLandscapeChunk : public IGameObject
{
private:
    
    friend class CLandscape;
    
    ui32 m_chunkSizeX;
    ui32 m_chunkSizeZ;
    
    ui32 m_heightmapSizeX;
    ui32 m_heightmapSizeZ;
    
    f32 m_splattingTillingLayer_01;
    f32 m_splattingTillingLayer_02;
    f32 m_splattingTillingLayer_03;
    
    ui32 m_numIndexesToRender;
    CSharedQuadTree m_quadTree;
    
protected:
    
    void setMesh(CSharedMeshRef mesh,
                 ui32 chunkSizeX, ui32 chunkSizeZ,
                 ui32 heightmapSizeX, ui32 heightmapSizeZ);
    
    void setSplattingSettings(f32 splattingTillingLayer_01,
                              f32 splattingTillingLayer_02,
                              f32 splattingTillingLayer_03);
    
    void setSplattingDiffuseTexture(CSharedTextureRef texture);
    void setSplattingNormalTexture(CSharedTextureRef texture);
    void setSplattinMaskTexture(CSharedTextureRef texture);
    
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
    
public:
    
    CLandscapeChunk(CSharedResourceAccessorRef resourceAccessor,
                    ISharedScreenSpaceTextureAccessorRef screenSpaceTextureAccessor);
    ~CLandscapeChunk(void);
    
    CSharedVertexBuffer getCollisionVertexBuffer(void) const;
    CSharedIndexBuffer getCollisionIndexBuffer(void) const;
};

#endif
