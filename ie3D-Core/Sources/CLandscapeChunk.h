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
    std::array<E_LANDSCAPE_CHUNK_LOD, E_LANDSCAPE_SEAM_MAX> m_seamedLOD;
    
    ui32 m_numPassedIndexes;
    CSharedQuadTree m_quadTree;
    
protected:
    
    void setMesh(CSharedMeshRef mesh);
    void setQuadTree(CSharedQuadTreeRef quadTree, E_LANDSCAPE_CHUNK_LOD LOD);
    
    std::function<ui32(ui32)> getEdgeVertexIncrementFunction(E_LANDSCAPE_SEAM seamType) const;
    std::function<f32(const glm::vec3&, const glm::vec3&, const glm::vec3&)> getInterpolationIntensityFunctionToSewSeams(E_LANDSCAPE_SEAM seamType);
    
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
                    ISharedRenderTechniqueAccessorRef renderTechniqueAccessor);
    ~CLandscapeChunk(void);
    
    E_LANDSCAPE_CHUNK_LOD getCurrentLOD(void) const;
    E_LANDSCAPE_CHUNK_LOD getInprogressLOD(void) const;
    E_LANDSCAPE_CHUNK_LOD getSeamedLOD(E_LANDSCAPE_SEAM seamType) const;
    void setInprogressLOD(E_LANDSCAPE_CHUNK_LOD LOD);
    
    CSharedVertexBuffer getCollisionVertexBuffer(void) const;
    CSharedIndexBuffer getCollisionIndexBuffer(void) const;
    
    std::vector<SAttributeVertex> getSeamVerteces(E_LANDSCAPE_SEAM seamType) const;
    void setSeamVerteces(const std::vector<SAttributeVertex>& verteces, E_LANDSCAPE_SEAM seamType);
    void resetSeams(void);
};

#endif
