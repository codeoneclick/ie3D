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
    
    ui32 m_size;
    E_LANDSCAPE_CHUNK_LOD m_currentLOD;
    E_LANDSCAPE_CHUNK_LOD m_inprogressLOD;
    
    ui32 m_numPassedIndexes;
    CSharedQuadTree m_quadTree;
    CSharedTexture m_preprocessedSplattingTexture;
    CSharedTexture m_preprocessedSplattingNormalTexture;
    CSharedTexture m_preprocessedSplattingDisplaceTexture;
    
protected:
    
    void setMesh(CSharedMeshRef mesh);
    void setQuadTree(CSharedQuadTreeRef quadTree, E_LANDSCAPE_CHUNK_LOD LOD);
    
    std::function<ui32(ui32)> getEdgeVertexIncrementFunction(E_LANDSCAPE_SEAM seamType) const;
    std::function<f32(const glm::vec3&, const glm::vec3&, const glm::vec3&)> getInterpolationIntensityFunctionToSewSeams(E_LANDSCAPE_SEAM seamType);
    
    void onSceneUpdate(f32 deltatime);
    
    void onResourceLoaded(ISharedResourceRef resource, bool success);
    void onConfigurationLoaded(ISharedConfigurationRef configuration, bool success);
    
public:
    
    CLandscapeChunk(CSharedResourceAccessorRef resourceAccessor,
                    ISharedRenderTechniqueAccessorRef renderTechniqueAccessor);
    ~CLandscapeChunk(void);
    
    E_LANDSCAPE_CHUNK_LOD getCurrentLOD(void) const;
    E_LANDSCAPE_CHUNK_LOD getInprogressLOD(void) const;
    void setInprogressLOD(E_LANDSCAPE_CHUNK_LOD LOD);
    
    const CSharedTexture getPreprocessedSplattingTexture(void) const;
    void setPreprocessedSplattingTexture(CSharedTextureRef texture);
    
    const CSharedTexture getPreprocessedSplattingNormalTexture(void) const;
    void setPreprocessedSplattingNormalTexture(CSharedTextureRef texture);
    
    const CSharedTexture getPreprocessedSplattingDisplaceTexture(void) const;
    void setPreprocessedSplattingDisplaceTexture(CSharedTextureRef texture);
    
    CSharedVertexBuffer getCollisionVertexBuffer(void) const;
    CSharedIndexBuffer getCollisionIndexBuffer(void) const;
};

#endif
