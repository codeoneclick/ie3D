//
//  CHeightmapAccessor.h
//  ie3D-Core
//
//  Created by sergey.sergeev on 5/11/15.
//  Copyright (c) 2015 Sergey Sergeev. All rights reserved.
//

#ifndef CHeightmapAccessor_h
#define CHeightmapAccessor_h

#include "HCommon.h"
#include "HEnums.h"
#include "HDeclaration.h"

class CHeightmapContainer;

class CHeightmapAccessor
{
public:
    
    struct SHeightmapCustomParameters
    {
        glm::ivec2 m_size;
        f32 m_frequency;
        i32 m_octaves;
        ui32 m_seed;
    };
    
private:
    
    std::shared_ptr<CHeightmapContainer> m_container;
    bool m_isGenerated;
    ISharedRenderTechniqueAccessor m_renderTechniqueAccessor;
    CSharedHeightmapGeneratorStatistic m_generatorStatistic;
    
    std::vector<CSharedThreadOperation> m_executedOperations;
    std::vector<bool> m_canceledOperationsStatus;
    std::vector<std::tuple<std::function<void(CSharedMeshRef)>, std::function<void(CSharedQuadTreeRef)>, std::function<void(CSharedTextureRef, CSharedTextureRef)>>> m_callbacks;
    std::vector<std::tuple<CSharedMesh, CSharedQuadTree, CSharedTexture, CSharedTexture, E_LANDSCAPE_CHUNK_LOD>> m_chunksMetadata;
    std::vector<std::tuple<glm::vec3, glm::vec3>> m_chunksBounds;
    std::array<CSharedTexture, E_SPLATTING_TEXTURE_MAX> m_splattingDTextures;
    std::array<CSharedTexture, E_SPLATTING_TEXTURE_MAX> m_splattingNTextures;
    std::queue<CSharedThreadOperation> m_updateHeightmapOperations;
    
    void createLoadingOperations(void);
    void eraseLoadingOperations(void);
    
    void createBoundingBoxes(void);
    void createBoundingBox(ui32 i, ui32 j);
    void eraseBoundingBoxes(void);
    
    void createMetadataContainers(void);
    void eraseMetadataContainers(void);
    
    void eraseChunkMetadata(i32 index);
    
    void generateMesh(i32 index, E_LANDSCAPE_CHUNK_LOD LOD);
    void generateQuadTree(i32 index);
    void generateSplattingTextures(i32 index, E_LANDSCAPE_CHUNK_LOD LOD);
    void updateSplattingTextures(i32 index);
    
    static f32 getAngle(const glm::vec3& point_01,
                        const glm::vec3& point_02,
                        const glm::vec3& point_03);
    
protected:
    
public:
    
    CHeightmapAccessor(void);
    ~CHeightmapAccessor(void);
    
    inline glm::ivec2 getMainSize(void) const;
    inline glm::ivec2 getChunksNum(void) const;
    inline glm::ivec2 getChunkSize(void) const;
    inline const std::tuple<glm::vec3, glm::vec3> getChunkBounds(ui32 i, ui32 j) const;
    inline CSharedHeightmapGeneratorStatistic getGeneratorStatistic(void) const;
    
    f32 getHeight(const glm::vec3& position) const;
    glm::vec3 getNormal(const glm::vec3& position) const;
    glm::vec2 getAngles(const glm::vec3& position) const;
    
    static f32 getHeight(std::shared_ptr<CHeightmapContainer> container, const glm::vec3& position);
    static glm::vec3 getNormal(std::shared_ptr<CHeightmapContainer> container, const glm::vec3& position);
    static glm::vec2 getAngles(std::shared_ptr<CHeightmapContainer> container, const glm::vec3& position);
    
    void generate(const std::string& filename, ISharedRenderTechniqueAccessorRef renderTechniqueAccessor,
                  const std::array<CSharedTexture, 3>& splattingDTextures,
                  const std::array<CSharedTexture, 3>& splattingNTextures,
                  const std::function<void(void)>& callback,
                  const std::shared_ptr<SHeightmapCustomParameters>& customParameters = nullptr);
    
    void runLoading(i32 i, i32 j, E_LANDSCAPE_CHUNK_LOD LOD,
                    const std::function<void(CSharedMeshRef)>& callbackMeshLoaded,
                    const std::function<void(CSharedQuadTreeRef)>& callbackQuadTreeLoaded,
                    const std::function<void(CSharedTextureRef, CSharedTextureRef)>& callbackTexturesLoaded);
    void runUnLoading(i32 i, i32 j);
    
    void update(void);
    void updateVertices(const std::vector<glm::vec3>& vertices,
                        const glm::ivec2& minBound, const glm::ivec2& maxBound);
};

#include "CHeightmapAccessor.hpp"

#endif
