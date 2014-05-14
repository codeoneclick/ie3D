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

class CHeightmapProcessor
{
private:

protected:

    f32* m_heightmapData;

    CSharedTexture m_heightmapTexture;
    CSharedTexture m_splattingTexture;
    CSharedTexture m_diffuseTexture;
    CSharedTexture m_normalTexture;
    CSharedTexture m_edgesMaskTexture;

    ui32 m_width;
    ui32 m_height;
    
    ui32 m_chunkWidth;
    ui32 m_chunkHeight;
    
    ui32 m_numChunkRows;
    ui32 m_numChunkCells;

    f32 m_maxAltitude;
    f32 m_maxHeight;
    f32 m_minHeight;
    
    std::vector<CSharedMesh> m_chunksUnused;
    std::vector<std::tuple<glm::vec3, glm::vec3>> m_chunksBounds;
    
    std::shared_ptr<IScreenSpaceTextureAccessor> m_screenSpaceTextureAccessor;
    
    CSharedIndexBuffer createIndexBuffer(void);
    CSharedVertexBuffer createVertexBuffer(ui32 widthOffset,
                                           ui32 heightOffset,
                                           ui32 numVertexes,
                                           GLenum mode,
                                           glm::vec3* maxBound, glm::vec3* minBound);
    
    void _FillEdgesMaskTextureBlock(ui16* _data,ui32 _index, ui32 _edgesMaskWidth, ui32 _edgesMaskHeight, ui32 _textureBlockSize, const glm::vec3& _point, bool _reverse);
    
    ui32 createTextureId(void);
    void createChunkBound(ui32 widthOffset, ui32 heightOffset, glm::vec3* maxBound, glm::vec3* minBound);
    
    void fillVertexBuffer(CSharedVertexBufferRef vertexBuffer, ui32 widthOffset, ui32 heightOffset, ui32 numVertexes);
    void fillIndexBuffer(CSharedIndexBufferRef indexBuffer);
    
    void fillNormals(CSharedVertexBufferRef vertexBuffer,
                     CSharedIndexBufferRef indexBuffer);
    
    void fillTangentsAndBinormals(CSharedVertexBufferRef vertexBuffer,
                                  CSharedIndexBufferRef indexBuffer);
    
    
    void getTriangleBasis(const glm::vec3& E, const glm::vec3& F, const glm::vec3& G,
                          f32 sE, f32 tE, f32 sF, f32 tF, f32 sG, f32 tG,
                          glm::vec3& tangentX, glm::vec3& tangentY);
    glm::vec3 getClosestPointOnLine(const glm::vec3& a, const glm::vec3& b, const glm::vec3& p);
    glm::vec3 ortogonalize(const glm::vec3& v1, const glm::vec3& v2);
    
public:

    CHeightmapProcessor(const std::shared_ptr<IScreenSpaceTextureAccessor>& _screenSpaceTextureAccessor, ISharedConfigurationRef _template);
    ~CHeightmapProcessor(void);

    std::shared_ptr<CTexture> PreprocessHeightmapTexture(void);
    std::shared_ptr<CTexture> PreprocessSplattingTexture(void);
    std::shared_ptr<CTexture> PreprocessEdgesMaskTexture(void);
    std::shared_ptr<CTexture> PreprocessSplattingDiffuseTexture(const std::shared_ptr<CMaterial>& _material);
    std::shared_ptr<CTexture> PreprocessSplattingNormalTexture(const std::shared_ptr<CMaterial>& _material);

    inline ui32 Get_Width(void)
    {
        return m_width;
    };

    inline ui32 Get_Height(void)
    {
        return m_height;
    };

    CSharedMesh getChunk(ui32 i, ui32 j);
    void freeChunk(CSharedMeshRef chunk);
    
    const std::tuple<glm::vec3, glm::vec3> getChunkBounds(ui32 i, ui32 j) const;
    
    inline ui32 Get_NumChunkRows(void)
    {
        return m_numChunkRows;
    };
    
    inline ui32 Get_NumChunkCells(void)
    {
        return m_numChunkCells;
    };
    
    inline ui32 Get_ChunkWidth(void)
    {
        return m_chunkWidth;
    };
    
    inline ui32 Get_ChunkHeight(void)
    {
        return m_chunkHeight;
    };
    
    inline f32* Get_HeightmapData(void)
    {
        assert(m_heightmapData != nullptr);
        return m_heightmapData;
    };

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

    inline f32 Get_MinHeight(void)
    {
        return m_minHeight;
    };

    inline f32 Get_MaxHeight(void)
    {
        return m_maxHeight;
    };
};

#endif 
