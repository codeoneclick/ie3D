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

class CHeightmapData
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
    
    i32 m_size;
    
    f32 m_maxHeight;
    f32 m_minHeight;
    
    void createVertexesData(ui8* data);
    
public:
    
    CHeightmapData(const std::string& filename, i32 size);
    CHeightmapData(i32 size, f32 frequency, i32 octaves, ui32 seed);
    ~CHeightmapData(void);
    
    glm::vec3 getVertexPosition(ui32 i, ui32 j) const;
    glm::uint32 getVertexTexcoord(ui32 i, ui32 j) const;
    glm::uint32 getVertexNormal(ui32 i, ui32 j) const;
    
    void updateVertexesData(const std::vector<std::tuple<ui32, ui32, f32>>& modifiedVertexes);
    
    i32 getSizeX(void) const;
    i32 getSizeZ(void) const;
    
    f32 getMaxHeight(void) const;
    f32 getMinHeight(void) const;
};

class CHeightmapDataAccessor
{
private:
    
    static f32 getAngleOnHeightmapSurface(const glm::vec3& point_01,
                                         const glm::vec3& point_02,
                                         const glm::vec3& point_03);
    
protected:
    
public:
    
    CHeightmapDataAccessor(void) = default;
    ~CHeightmapDataAccessor(void) = default;
    
    static f32 getHeight(CSharedHeightmapDataRef data, const glm::vec3& position);
    static glm::vec2 getAngleOnHeightmapSurface(CSharedHeightmapDataRef data, const glm::vec3& position);
};

class CHeightmapProcessor
{
private:
    
protected:
    
    std::shared_ptr<CHeightmapData> m_heightmapData;
    
    CSharedTexture m_heightmapTexture;
    CSharedTexture m_splattingTexture;
    
    glm::ivec2 m_chunkSize;
    glm::ivec2 m_chunksNum;
    std::array<glm::ivec2, E_LANDSCAPE_CHUNK_LOD_MAX> m_chunkLODsSizes;
    
    std::vector<std::tuple<CSharedMesh, CSharedQuadTree, std::function<void(CSharedMeshRef)>, std::function<void(CSharedQuadTreeRef)>, E_LANDSCAPE_CHUNK_LOD>> m_chunksUsed;
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
    
    CHeightmapProcessor(ISharedRenderTechniqueAccessorRef renderTechniqueAccessor, ISharedConfigurationRef configuration);
    ~CHeightmapProcessor(void);
    
    CSharedTexture createHeightmapTexture(void);
    CSharedTexture createSplattingTexture(void);
    
    void generateVertecesData(i32 size, f32 frequency, i32 octaves, ui32 seed);
    
    static void generateTangentSpace(CSharedHeightmapDataRef heightmapData,
                                     CSharedVertexBufferRef vertexBuffer,
                                     CSharedIndexBufferRef indexBuffer);
    
    ui32 getSizeX(void) const;
    ui32 getSizeZ(void) const;
    
    void update(void);
    
    void runChunkLoading(ui32 i, ui32 j, E_LANDSCAPE_CHUNK_LOD LOD,
                         const std::function<void(CSharedMeshRef)>& meshCreatedCallback,
                         const std::function<void(CSharedQuadTreeRef)>& quadTreeGeneratedCallback);
    void stopChunkLoading(ui32 i, ui32 j, const std::function<void(void)>& stopLoadingCallback);
    void runChunkUnLoading(ui32 i, ui32 j);
    
    const std::tuple<glm::vec3, glm::vec3> getChunkBounds(ui32 i, ui32 j) const;
    
    ui32 getNumChunksX(void) const;
    ui32 getNumChunksZ(void) const;
    
    f32 getHeight(const glm::vec3& position) const;
    glm::vec2 getAngleOnHeightmapSurface(const glm::vec3& position) const;
    void updateHeightmapData(const std::vector<std::tuple<ui32, ui32, f32>>& modifiedHeights);
    void updateHeightmap(ui32 offsetX, ui32 offsetZ,
                         ui32 subWidth, ui32 subHeight);
    
    inline std::shared_ptr<CTexture> Get_HeightmapTexture(void)
    {
        assert(m_heightmapTexture != nullptr);
        return m_heightmapTexture;
    };
    
    inline std::shared_ptr<CTexture> Get_SplattingTexture(void)
    {
        assert(m_splattingTexture != nullptr);
        return m_splattingTexture;
    };
};

#endif 
