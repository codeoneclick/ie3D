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
private:
    
    std::shared_ptr<CHeightmapContainer> m_container;
    bool m_isGenerated;
    
    std::vector<CSharedThreadOperation> m_executedOperations;
    std::vector<std::tuple<std::function<void(CSharedMeshRef)>, std::function<void(CSharedQuadTreeRef)>, std::function<void(CSharedTextureRef)>>> m_callbacks;
    std::vector<std::tuple<CSharedMesh, CSharedQuadTree, CSharedTexture, E_LANDSCAPE_CHUNK_LOD>> m_chunksMetadata;
    std::vector<std::tuple<glm::vec3, glm::vec3>> m_chunksBounds;
    
    void createLoadingOperations(void);
    void eraseLoadingOperations(void);
    
    void createBoundingBoxes(void);
    void eraseBoundingBoxes(void);
    
    void createMetadataContainers(void);
    void eraseMetadataContainers(void);
    
    void eraseChunkMetadata(i32 index);
    
    void generateMesh(i32 index, E_LANDSCAPE_CHUNK_LOD LOD);
    void generateQuadTree(i32 index);
    void generateSplattingTexture(i32 index, E_LANDSCAPE_CHUNK_LOD LOD);
    
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
    
    f32 getHeight(const glm::vec3& position) const;
    glm::vec3 getNormal(const glm::vec3& position) const;
    glm::vec2 getAngles(const glm::vec3& position) const;
    
    static f32 getHeight(std::shared_ptr<CHeightmapContainer> container, const glm::vec3& position);
    static glm::vec3 getNormal(std::shared_ptr<CHeightmapContainer> container, const glm::vec3& position);
    static glm::vec2 getAngles(std::shared_ptr<CHeightmapContainer> container, const glm::vec3& position);
    
    void generate(const std::string& filename, ISharedRenderTechniqueAccessorRef renderTechniqueAccessor,
                  const std::array<CSharedTexture, 3>& splattingTextures, const std::function<void(void)>& callback);
    
    void runLoading(i32 i, i32 j, E_LANDSCAPE_CHUNK_LOD LOD,
                    const std::function<void(CSharedMeshRef)>& meshLoadedCallback,
                    const std::function<void(CSharedQuadTreeRef)>& quadTreeLoadedCallback,
                    const std::function<void(CSharedTextureRef)>& textureLoadingCallback);
    void runUnLoading(i32 i, i32 j);
};

#include "CHeightmapAccessor.hpp"

#endif
