//
//  CLandscape.h
//  iGaia
//
//  Created by sergey.sergeev on 1/16/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef CLandscape_h
#define CLandscape_h

#include "IGameObject.h"
#include "HDeclaration.h"

class CLandscapeChunk;
class CLandscapeEdges;

class CLandscape : public IGameObject
{
private:

    CSharedHeightmapProcessor m_heightmapProcessor;
    std::vector<CSharedLandscapeChunk> m_chunks;
    CSharedLandscapeEdges m_edges;
    ISharedConfiguration m_configuration;
    
    CSharedTexture m_splattingDiffuseTexture;
    CSharedTexture m_splattingNormalTexture;
    
    CSharedMaterial m_splattingDiffuseMaterial;
    CSharedMaterial m_splattingNormalMaterial;
    
    bool m_isSplattingDiffuseTextureProcessed;
    bool m_isSplattingNormalTextureProcessed;
    
    void processSplattingDiffuseTexture(void);
    void processSplattingNormalTexture(void);
    
protected:

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

    CLandscape(CSharedResourceAccessorRef resourceAccessor,
               ISharedScreenSpaceTextureAccessorRef screenSpaceTextureAccessor);
    ~CLandscape(void);
    
    void setCamera(CSharedCameraRef camera);
    void setLightSource(CSharedLightSourceRef lightSource,
                        E_LIGHT_SOURCE index);
    
    void setRenderMgr(CSharedRenderMgrRef renderMgr);
    void setSceneUpdateMgr(CSharedSceneUpdateMgrRef sceneUpdateMgr);
    
    void listenRenderMgr(bool value);
    void listenSceneUpdateMgr(bool value);
    
    std::vector<ISharedGameObject> getChunks(void) const;

    CSharedTexture getHeightmapTexture(void) const;
    
    ui32 getHeightmapSizeX(void) const;
    ui32 getHeightmapSizeZ(void) const;
};

#endif
