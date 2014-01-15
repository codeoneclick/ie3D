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

class CTexture;
class CMaterial;
class CMesh;
class CVertexBuffer;
class CIndexBuffer;
class IScreenSpaceTextureAccessor;
class I_RO_TemplateCommon;

class CHeightmapProcessor
{
private:

protected:

    f32* m_heightmapData;

    std::shared_ptr<CTexture> m_heightmapTexture;
    std::shared_ptr<CTexture> m_splattingTexture;
    std::shared_ptr<CTexture> m_diffuseTexture;
    std::shared_ptr<CTexture> m_normalTexture;
    std::shared_ptr<CTexture> m_edgesMaskTexture;

    ui32 m_width;
    ui32 m_height;
    
    ui32 m_chunkWidth;
    ui32 m_chunkHeight;
    
    ui32 m_numChunkRows;
    ui32 m_numChunkCells;

    f32 m_maxAltitude;
    f32 m_maxHeight;
    f32 m_minHeight;
    
    std::vector<std::shared_ptr<CMesh>> m_chunks;
    
    std::shared_ptr<IScreenSpaceTextureAccessor> m_screenSpaceTextureAccessor;

    
    std::shared_ptr<CIndexBuffer> _CreateIndexBuffer(void);
    std::shared_ptr<CVertexBuffer> _CreateVertexBuffer(ui32 _widthOffset, ui32 _heightOffset, ui32 _numVertexes, GLenum _mode, glm::vec3* _maxBound, glm::vec3* _minBound);

    void _FillEdgesMaskTextureBlock(ui16* _data,ui32 _index, ui32 _edgesMaskWidth, ui32 _edgesMaskHeight, ui32 _textureBlockSize, const glm::vec3& _point, bool _reverse);

public:

    CHeightmapProcessor(const std::shared_ptr<IScreenSpaceTextureAccessor>& _screenSpaceTextureAccessor, const std::shared_ptr<I_RO_TemplateCommon>& _template);
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

    inline std::shared_ptr<CMesh> Get_Chunk(ui32 _i, ui32 _j)
    {
        assert(m_chunks.size() != 0);
        assert(m_chunks[_i + _j * m_numChunkRows] != nullptr);
        return m_chunks[_i + _j * m_numChunkRows];
    };
    
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
