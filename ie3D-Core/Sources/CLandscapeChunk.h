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
#include "HEnums.h"

class CLandscapeChunk : public IGameObject
{
private:
    
    friend class CLandscape;
    glm::ivec2 m_heightmapSize;
    
    E_LANDSCAPE_CHUNK_LOD m_currentLOD;
    E_LANDSCAPE_CHUNK_LOD m_inprogressLOD;
    
    f32 m_fogLinearStart;
    f32 m_fogLinearEnd;
    
    f32 m_tillingTexcoord[E_SHADER_SAMPLER_MAX];
    
    ui32 m_numIndexesToRender;
    CSharedQuadTree m_quadTree;
    
    CSharedTexture m_prerenderedSplattingDiffuseTexture;
    CSharedTexture m_prerenderedSplattingNormalTexture;
    
    std::array<CSharedLandscapeSeam, E_LANDSCAPE_SEAM_MAX> m_seams;
    
protected:
    
    void setMesh(CSharedMeshRef mesh);
    void setQuadTree(CSharedQuadTreeRef quadTree, E_LANDSCAPE_CHUNK_LOD LOD);
    
    void setTillingTexcoord(f32 value, E_SHADER_SAMPLER sampler);
    
    void setPrerenderedSplattingDiffuseTexture(CSharedTextureRef texture);
    void setPrerenderedSplattingNormalTexture(CSharedTextureRef texture);
    
    void setDiffuseTextureLayer_01(CSharedTextureRef texture);
    void setDiffuseTextureLayer_02(CSharedTextureRef texture);
    void setDiffuseTextureLayer_03(CSharedTextureRef texture);
    
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
    
    void bindCustomShaderUniforms(CSharedMaterialRef material);
    
public:
    
    CLandscapeChunk(CSharedResourceAccessorRef resourceAccessor,
                    ISharedRenderTechniqueAccessorRef renderTechniqueAccessor);
    ~CLandscapeChunk(void);
    
    void setHeightmapSize(const glm::ivec2& size);
    
    E_LANDSCAPE_CHUNK_LOD getCurrentLOD(void) const;
    E_LANDSCAPE_CHUNK_LOD getInprogressLOD(void) const;
    void setInprogressLOD(E_LANDSCAPE_CHUNK_LOD LOD);
    
    CSharedVertexBuffer getCollisionVertexBuffer(void) const;
    CSharedIndexBuffer getCollisionIndexBuffer(void) const;
    
    std::vector<SAttributeVertex> getSeamVertexes(E_LANDSCAPE_SEAM type) const;
    
    void setSeam(CSharedLandscapeSeamRef seam, E_LANDSCAPE_SEAM type);
    CSharedLandscapeSeam getSeam(E_LANDSCAPE_SEAM type) const;
    
    bool isMeshExist(void) const;
};

#endif
