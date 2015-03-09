//
//  CHeightmapProcessor.h
//  iGaia
//
//  Created by Sergey Sergeev on 3/3/13.
//
//

#ifndef CHeightmapProcessor_h
#define CHeightmapProcessor_h

#include "HCommon.h"
#include "HDeclaration.h"
#include "HEnums.h"

class CHeightmap
{
private:
    
    struct SUncomressedVertex
    {
        glm::vec3 m_position;
        glm::uint32 m_normal;
        glm::uint32 m_texcoord;
        glm::uint32 m_tangent;
        std::vector<ui32> m_containInFace;
        
        SUncomressedVertex(void) = default;
        ~SUncomressedVertex(void)
        {
            m_containInFace.clear();
        };
    };
    
    struct SCompressedVertex
    {
        glm::vec3 m_position;
        glm::uint32 m_normal;
        glm::uint32 m_texcoord;
        
        SCompressedVertex(void) = default;
        ~SCompressedVertex(void) = default;
    };
    
    struct SFace
    {
        glm::uint32 m_normal;
        std::array<ui32, 3> m_indexes;
        
        SFace(void) = default;
        ~SFace(void) = default;
    };
    
protected:
    
    std::vector<SUncomressedVertex> m_uncompressedVertexes;
    std::vector<SCompressedVertex> m_compressedVertexes;
    std::vector<SFace> m_faces;
    
    glm::ivec2 m_size;
    void createVertexesData(const std::vector<f32>& data);
    
public:
    
    CHeightmap(const std::string& filename, const glm::ivec2& size);
    CHeightmap(const glm::ivec2& size, f32 frequency, i32 octaves, ui32 seed);
    ~CHeightmap(void);
    
    glm::vec3 getVertexPosition(ui32 i, ui32 j) const;
    glm::uint32 getVertexTexcoord(ui32 i, ui32 j) const;
    glm::uint32 getVertexNormal(ui32 i, ui32 j) const;
    
    void updateVertexesData(const std::vector<std::tuple<ui32, ui32, f32>>& modifiedVertexes);
    
    glm::ivec2 getSize(void) const;
    
    f32 getMaxHeight(void) const;
    f32 getMinHeight(void) const;
};

class CHeightmapAccessor
{
private:
    
    static f32 getAngleOnHeightmapSurface(const glm::vec3& point_01,
                                         const glm::vec3& point_02,
                                         const glm::vec3& point_03);
    
protected:
    
public:
    
    CHeightmapAccessor(void) = default;
    ~CHeightmapAccessor(void) = default;
    
    static f32 getHeight(CSharedHeightmapRef data, const glm::vec3& position);
    static glm::vec2 getAngleOnHeightmapSurface(CSharedHeightmapRef data, const glm::vec3& position);
};

class CHeightmapGenerator
{
private:
    
protected:
    
    std::shared_ptr<CHeightmap> m_heightmap;
    
    std::vector<std::shared_ptr<CVertexBuffer>> m_vbos;
    std::vector<std::tuple<std::array<std::shared_ptr<CIndexBuffer>, E_LANDSCAPE_CHUNK_LOD_MAX>, ui32>> m_ibos;
    
    void createVBO(void);
    void createIBOs(void);
    
    CSharedTexture m_heightmapTexture;
    CSharedTexture m_splattingTexture;
    
    glm::ivec2 m_chunkSize;
    glm::ivec2 m_chunksNum;
    std::array<glm::ivec2, E_LANDSCAPE_CHUNK_LOD_MAX> m_chunkLODsSizes;
    
    std::vector<std::tuple<CSharedMesh,
                           CSharedQuadTree,
                           std::function<void(CSharedMeshRef)>,
                           std::function<void(CSharedQuadTreeRef)>,
                           std::function<void(void)>,
                           E_LANDSCAPE_CHUNK_LOD>> m_chunksUsed;
    
    std::vector<CSharedMesh> m_chunksUnused;
    std::vector<CSharedThreadOperation> m_executedOperations;
    std::vector<CSharedThreadOperation> m_canceledOperations;
    std::vector<std::tuple<glm::vec3, glm::vec3>> m_chunksBounds;
    
    ISharedRenderTechniqueAccessor m_renderTechniqueAccessor;
    
    void createChunkBound(ui32 chunkLODSizeX, ui32 chunkLODSizeZ,
                          ui32 chunkOffsetX, ui32 chunkOffsetZ,
                          glm::vec3* maxBound, glm::vec3* minBound);
    
    void writeToVertexBuffer(ui32 chunkOffsetX, ui32 chunkOffsetZ, E_LANDSCAPE_CHUNK_LOD LOD);
    void commitVertexBufferToVRAM(ui32 chunkOffsetX, ui32 chunkOffsetZ, E_LANDSCAPE_CHUNK_LOD LOD);
    
    void writeToIndexBuffer(ui32 chunkOffsetX, ui32 chunkOffsetZ, E_LANDSCAPE_CHUNK_LOD LOD);
    void commitIndexBufferToVRAM(ui32 chunkOffsetX, ui32 chunkOffsetZ, E_LANDSCAPE_CHUNK_LOD LOD);
    
    void generateQuadTree(ui32 chunkOffsetX, ui32 chunkOffsetZ);
    
    void updateSplattingTexture(CSharedTextureRef texture, bool isCreation = true,
                                ui32 offsetX = 0, ui32 offsetY = 0,
                                ui32 subWidth = 0, ui32 subHeight = 0);
    
    void updateHeightmapTexture(CSharedTextureRef texture, bool isCreation = true,
                                ui32 offsetX = 0, ui32 offsetY = 0,
                                ui32 subWidth = 0, ui32 subHeight = 0);
    
    ui32 createTextureId(void);
    
    
    static void getTriangleBasis(const glm::vec3& E, const glm::vec3& F, const glm::vec3& G,
                                 f32 sE, f32 tE, f32 sF, f32 tF, f32 sG, f32 tG,
                                 glm::vec3& tangentX, glm::vec3& tangentY);
    static glm::vec3 getClosestPointOnLine(const glm::vec3& a, const glm::vec3& b, const glm::vec3& p);
    static glm::vec3 ortogonalize(const glm::vec3& v1, const glm::vec3& v2);
    
public:
    
    CHeightmapGenerator(ISharedRenderTechniqueAccessorRef renderTechniqueAccessor, ISharedConfigurationRef configuration);
    ~CHeightmapGenerator(void);
    
    CSharedTexture createHeightmapTexture(void);
    CSharedTexture createSplattingTexture(void);
    
    void generateVertecesData(const glm::ivec2& size, f32 frequency, i32 octaves, ui32 seed);
    
    void generateTangentSpace(CSharedVertexBufferRef vertexBuffer,
                              CSharedIndexBufferRef indexBuffer);
    
    glm::ivec2 getSize(void) const;
    glm::ivec2 getNumChunks(void) const;
    
    void update(void);
    
    void runChunkLoading(ui32 i, ui32 j, E_LANDSCAPE_CHUNK_LOD LOD,
                         const std::function<void(CSharedMeshRef)>& meshCreatedCallback,
                         const std::function<void(CSharedQuadTreeRef)>& quadTreeGeneratedCallback,
                         const std::function<void(void)>& meshUpdatedCallback);
    void stopChunkLoading(ui32 i, ui32 j, const std::function<void(void)>& stopLoadingCallback);
    void runChunkUnLoading(ui32 i, ui32 j);
    
    const std::tuple<glm::vec3, glm::vec3> getChunkBounds(ui32 i, ui32 j) const;
    
    f32 getHeight(const glm::vec3& position) const;
    glm::vec2 getAngleOnHeightmapSurface(const glm::vec3& position) const;
    void updateHeightmap(const std::vector<std::tuple<ui32, ui32, f32>>& modifiedHeights);
    void updateHeightmap(ui32 offsetX, ui32 offsetZ,
                         ui32 subWidth, ui32 subHeight);
};

#endif 
