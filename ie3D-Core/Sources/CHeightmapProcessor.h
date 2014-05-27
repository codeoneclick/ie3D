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

class CHeightmapProcessingOperation
{
private:
    
protected:
    
    CSharedHeightmapData m_heightmapData;
    
    CSharedVertexBuffer m_vertexBuffer;
    CSharedIndexBuffer m_indexBuffer;
    
    std::future<void> m_currentExecutedFunction;
    ui32 m_indexX;
    ui32 m_indexZ;
    bool m_isCanceled;
    bool m_isRunning;
    bool m_isBackgroundThreadOperationDone;
    bool m_isMainThreadOperationDone;
    
public:
    
    CHeightmapProcessingOperation(CSharedHeightmapDataRef heightmapData,
                                  CSharedVertexBufferRef vertexBuffer,
                                  CSharedIndexBufferRef indexBuffer,
                                  ui32 indexX,
                                  ui32 indexZ);
    ~CHeightmapProcessingOperation(void);
    
    void executeBackgroundThreadOperation(void);
    void executeMainThreadOperation(void);
    void cancel(void);
    bool isRunning(void) const;
    bool isCanceled(void) const;
    bool isBackgroundThreadOperationDone(void) const;
    bool isMainThreadOperationDone(void) const;
    
    ui32 getIndexX(void) const;
    ui32 getIndexZ(void) const;
};

class CHeightmapData
{
private:
    
    struct SUncomressedVertex
    {
        glm::vec3 m_position;
        glm::u8vec4 m_normal;
        glm::u16vec2 m_texcoord;
        glm::u8vec4 m_tangent;
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
        glm::u8vec4 m_normal;
        glm::u16vec2 m_texcoord;
        
        SCompressedVertex(void) = default;
        ~SCompressedVertex(void) = default;
    };
    
    struct SFace
    {
        glm::u8vec4 m_normal;
        std::array<ui32, 3> m_indexes;
        
        SFace(void) = default;
        ~SFace(void) = default;
    };
    
protected:
    
    std::vector<SUncomressedVertex> m_uncopressedVertexes;
    std::vector<SCompressedVertex> m_compressedVertexes;
    std::vector<SFace> m_faces;
    
    ui32 m_sizeX;
    ui32 m_sizeZ;
    
    f32 m_maxAltitude;
    f32 m_maxHeight;
    f32 m_minHeight;
    
public:
    
    CHeightmapData(const std::string& filename);
    CHeightmapData(void);
    
    glm::vec3 getVertexPosition(ui32 i, ui32 j) const;
    glm::u16vec2 getVertexTexcoord(ui32 i, ui32 j) const;
    glm::u8vec4 getVertexNormal(ui32 i, ui32 j) const;
    
    void setVertexPosition(ui32 i, ui32 j, f32 value);
    
    ui32 getSizeX(void) const;
    ui32 getSizeZ(void) const;
    
    f32 getMaxAltitude(void) const;
    f32 getMaxHeight(void) const;
    f32 getMinHeight(void) const;
};

class CHeightmapDataAccessor
{
private:
    
    static f32 getAngleOnHeightmapSuface(const glm::vec3& point_01,
                                         const glm::vec3& point_02,
                                         const glm::vec3& point_03);
    
protected:
    
public:
    
    CHeightmapDataAccessor(void) = default;
    ~CHeightmapDataAccessor(void) = default;
    
    static f32 getHeight(CSharedHeightmapDataRef data, const glm::vec3& position);
    static glm::vec2 getAngleOnHeightmapSuface(CSharedHeightmapDataRef data, const glm::vec3& position);
};

class CHeightmapProcessor
{
private:

protected:

    std::shared_ptr<CHeightmapData> m_heightmapData;

    CSharedTexture m_heightmapTexture;
    CSharedTexture m_splattingTexture;
    CSharedTexture m_diffuseTexture;
    CSharedTexture m_normalTexture;
    CSharedTexture m_edgesMaskTexture;
    
    std::queue<CSharedHeightmapProcessingOperation> m_processingOperationQueue;
    std::map<std::tuple<ui32, ui32>, CSharedHeightmapProcessingOperation> m_uniqueProcessingOperations;

    ui32 m_chunkSizeX;
    ui32 m_chunkSizeZ;
    ui32 m_chunkLODSizeX;
    ui32 m_chunkLODSizeZ;
    
    ui32 m_numChunksX;
    ui32 m_numChunksZ;
    
    std::vector<CSharedMesh> m_chunksUsed;
    std::vector<CSharedMesh> m_chunksUnused;
    std::vector<std::tuple<glm::vec3, glm::vec3>> m_chunksBounds;
    
    std::shared_ptr<IScreenSpaceTextureAccessor> m_screenSpaceTextureAccessor;
    
    CSharedVertexBuffer createVertexBuffer(ui32 chunkLODSizeX, ui32 chunkLODSizeZ,
                                           ui32 chunkOffsetX, ui32 chunkOffsetZ,
                                           glm::vec3* maxBound, glm::vec3* minBound);
    
    CSharedIndexBuffer createIndexBuffer(ui32 chunkLODSizeX, ui32 chunkLODSizeZ);
    
    void createChunkBound(ui32 chunkLODSizeX, ui32 chunkLODSizeZ,
                          ui32 chunkOffsetX, ui32 chunkOffsetZ,
                          glm::vec3* maxBound, glm::vec3* minBound);
    
    void fillVertexBuffer(CSharedVertexBufferRef vertexBuffer,
                          ui32 chunkLODSizeX, ui32 chunkLODSizeZ,
                          ui32 chunkOffsetX, ui32 chunkOffsetZ);
    
    void fillIndexBuffer(CSharedIndexBufferRef indexBuffer,
                         ui32 chunkLODSizeX, ui32 chunkLODSizeZ);
    
    void _FillEdgesMaskTextureBlock(ui16* _data,ui32 _index, ui32 _edgesMaskWidth, ui32 _edgesMaskHeight, ui32 _textureBlockSize, const glm::vec3& _point, bool _reverse);
    
    ui32 createTextureId(void);
    
    
    static void getTriangleBasis(const glm::vec3& E, const glm::vec3& F, const glm::vec3& G,
                                 f32 sE, f32 tE, f32 sF, f32 tF, f32 sG, f32 tG,
                                 glm::vec3& tangentX, glm::vec3& tangentY);
    static glm::vec3 getClosestPointOnLine(const glm::vec3& a, const glm::vec3& b, const glm::vec3& p);
    static glm::vec3 ortogonalize(const glm::vec3& v1, const glm::vec3& v2);
    
public:
    
    CHeightmapProcessor(const std::shared_ptr<IScreenSpaceTextureAccessor>& _screenSpaceTextureAccessor, ISharedConfigurationRef _template);
    ~CHeightmapProcessor(void);

    std::shared_ptr<CTexture> PreprocessHeightmapTexture(void);
    std::shared_ptr<CTexture> PreprocessSplattingTexture(void);
    std::shared_ptr<CTexture> PreprocessEdgesMaskTexture(void);
    std::shared_ptr<CTexture> PreprocessSplattingDiffuseTexture(const std::shared_ptr<CMaterial>& _material);
    std::shared_ptr<CTexture> PreprocessSplattingNormalTexture(const std::shared_ptr<CMaterial>& _material);
    
    static void generateTangentSpace(CSharedHeightmapDataRef heightmapData,
                                     CSharedVertexBufferRef vertexBuffer,
                                     CSharedIndexBufferRef indexBuffer);
    
    ui32 getSizeX(void) const;
    ui32 getSizeZ(void) const;
    
    void update(void);

    CSharedMesh getChunk(ui32 i, ui32 j);
    void freeChunk(CSharedMeshRef chunk, ui32 i, ui32 j);
    
    const std::tuple<glm::vec3, glm::vec3> getChunkBounds(ui32 i, ui32 j) const;
    
    ui32 getNumChunksX(void) const;
    ui32 getNumChunksZ(void) const;
    
    ui32 getChunkSizeX(ui32 i, ui32 j) const;
    ui32 getChunkSizeZ(ui32 i, ui32 j) const;
    
    f32 getHeight(const glm::vec3& position) const;
    void updateHeightmapData(const std::vector<std::tuple<ui32, ui32, f32>>& modifiedHeights);
    
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

    inline std::shared_ptr<CTexture> Get_DiffuseTexture(void)
    {
        assert(m_diffuseTexture != nullptr);
        return m_diffuseTexture;
    };
    
    inline std::shared_ptr<CTexture> Get_NormalTexture(void)
    {
        assert(m_normalTexture != nullptr);
        return m_normalTexture;
    };

    inline std::shared_ptr<CTexture> Get_EdgesMaskTexture(void)
    {
        assert(m_edgesMaskTexture != nullptr);
        return m_edgesMaskTexture;
    };
};

#endif 
