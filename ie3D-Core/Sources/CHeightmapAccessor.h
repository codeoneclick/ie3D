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
    std::vector<std::tuple<std::function<void(CSharedMeshRef)>, std::function<void(CSharedQuadTreeRef)>>> m_callbacks;
    std::vector<std::tuple<CSharedMesh, CSharedQuadTree, E_LANDSCAPE_CHUNK_LOD>> m_chunksMetadata;
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
    
protected:
    
public:
    
    CHeightmapAccessor(void);
    ~CHeightmapAccessor(void);
    
    void generate(const std::string& filename, const std::function<void(void)>& callback);
    
    void runLoading(i32 i, i32 j, E_LANDSCAPE_CHUNK_LOD LOD,
                    const std::function<void(CSharedMeshRef)>& meshLoadedCallback,
                    const std::function<void(CSharedQuadTreeRef)>& quadTreeLoadedCallback);
    void runUnLoading(i32 i, i32 j);
};

#endif
