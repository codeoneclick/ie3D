//
//  CHeightmapProcessor.cpp
//  iGaia
//
//  Created by Sergey Sergeev on 3/3/13.
//
//

#include "CHeightmapProcessor.h"
#include "CTexture.h"
#include "CMaterial.h"
#include "CMesh.h"
#include "CVertexBuffer.h"
#include "CIndexBuffer.h"
#include "IScreenSpaceTextureAccessor.h"
#include "CConfigurationGameObjects.h"
#include "CHeightmapHelper.h"

CHeightmapProcessor::CHeightmapProcessor(const std::shared_ptr<IScreenSpaceTextureAccessor>& _screenSpaceTextureAccessor, ISharedConfigurationRef _template) :
m_heightmapData(nullptr),
m_screenSpaceTextureAccessor(_screenSpaceTextureAccessor),
m_heightmapTexture(nullptr),
m_splattingTexture(nullptr),
m_diffuseTexture(nullptr),
m_normalTexture(nullptr),
m_edgesMaskTexture(nullptr)
{
    assert(m_screenSpaceTextureAccessor != nullptr);
    assert(_template != nullptr);
    
    m_maxHeight = -FLT_MAX;
    m_minHeight = FLT_MAX;
    
    std::shared_ptr<CConfigurationLandscape> landscapeTemplate = std::static_pointer_cast<CConfigurationLandscape>(_template);
    
    m_width = landscapeTemplate->getSize().x;
    m_height = landscapeTemplate->getSize().y;
    
    m_heightmapData = new f32[m_width * m_height];
    m_maxAltitude = 0.0f;
    
    for(ui32 i = 0; i < m_width; ++i)
    {
        for(ui32 j = 0; j < m_height; ++j)
        {
			m_heightmapData[i + j * m_height] = sin(i * 0.1) * 10.0 + cos(j * 0.1) * 10.0;
            
            if(fabsf(m_heightmapData[i +j * m_height]) > m_maxAltitude)
            {
                m_maxAltitude = fabsf(m_heightmapData[i +j * m_height]);
            }
            m_maxHeight = m_maxHeight < m_heightmapData[i + j * m_height] ? m_heightmapData[i + j * m_height] : m_maxHeight;
            m_minHeight = m_minHeight > m_heightmapData[i + j * m_height] ? m_heightmapData[i + j * m_height] : m_minHeight;
        }
    }
    
    m_chunkWidth = 64;
    m_chunkHeight = 64;
    
    m_numChunkRows = m_width / m_chunkWidth;
    m_numChunkCells = m_height / m_chunkHeight;
    
    m_chunkWidth++;
    m_chunkHeight++;
    
    m_chunks.resize(m_numChunkRows * m_numChunkCells);
    
    for(ui32 i = 0; i < m_numChunkRows; ++i)
    {
        for(ui32 j = 0; j < m_numChunkCells; ++j)
        {
            glm::vec3 maxBound = glm::vec3( -4096.0f, -4096.0f, -4096.0f );
            glm::vec3 minBound = glm::vec3(  4096.0f,  4096.0f,  4096.0f );
            
            std::shared_ptr<CVertexBuffer> vertexBuffer = CHeightmapProcessor::_CreateVertexBuffer(i, j, m_chunkWidth * m_chunkHeight, GL_STATIC_DRAW, &maxBound, &minBound);
            std::shared_ptr<CIndexBuffer> indexBuffer =CHeightmapProcessor::_CreateIndexBuffer();
            std::shared_ptr<CMesh> mesh = CMesh::constructCustomMesh("landscape", vertexBuffer, indexBuffer,
                                                                  maxBound, minBound);
            m_chunks[i + j * m_numChunkRows] = mesh;
            
        }
    }
}

CHeightmapProcessor::~CHeightmapProcessor(void)
{
    
}

ui32 CHeightmapProcessor::createTextureId(void)
{
    ui32 textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    return textureId;
}

std::shared_ptr<CTexture> CHeightmapProcessor::PreprocessHeightmapTexture(void)
{
    assert(m_heightmapTexture == nullptr);
    ui32 textureId = CHeightmapProcessor::createTextureId();
    ui8* data = new ui8[m_width * m_height];
    for(int i = 0; i < m_width; i++)
    {
        for(int j = 0; j < m_height; j++)
        {
            f32 height = CHeightmapHelper::Get_HeightValue(m_heightmapData, m_width, m_height, glm::vec3(i , 0.0f, j));
            height /= m_maxAltitude;
            ui8 color = static_cast<ui8>((height + 1.0) / 2.0 * 255);
            data[i + j * m_height] = color;//TO_RGB565(color, color, color);
        }
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, m_width, m_height, 0, GL_ALPHA, GL_UNSIGNED_BYTE, data);
    
    m_heightmapTexture = CTexture::constructCustomTexture("landscape.heightmap",
                                                          textureId,
                                                          m_width,
                                                          m_height);
    m_heightmapTexture->setWrapMode(GL_CLAMP_TO_EDGE);
    return m_heightmapTexture;
}

std::shared_ptr<CTexture> CHeightmapProcessor::PreprocessSplattingTexture(void)
{
    assert(m_splattingTexture == nullptr);
    ui32 textureId = CHeightmapProcessor::createTextureId();
    ui16* data = new ui16[m_width * m_height];
    for(int i = 0; i < m_width; i++)
    {
        for(int j = 0; j < m_height; j++)
        {
            data[i + j * m_height] = TO_RGB565(255, 0, 0);

            f32 height = CHeightmapHelper::Get_HeightValue(m_heightmapData, m_width, m_height, glm::vec3(i , 0.0f, j));
            if(height > 5.0f)
            {
                data[i + j * m_width] = TO_RGB565(0, 255, 0);
            }
            if(height < 0.5f)
            {
                data[i + j * m_width] = TO_RGB565(0, 0, 255);
            }
            
            if( i == 0 || j == 0 || i == (m_width - 1) || j == (m_height - 1))
            {
                data[i + j * m_width] = TO_RGB565(255, 0, 0);
            }
        }
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, data);
    
    m_splattingTexture = CTexture::constructCustomTexture("landscape.splatting",
                                                          textureId,
                                                          m_width,
                                                          m_height);
    m_splattingTexture->setWrapMode(GL_CLAMP_TO_EDGE);
    
    return m_splattingTexture;
}

void CHeightmapProcessor::_FillEdgesMaskTextureBlock(ui16* _data, ui32 _index, ui32 _edgesMaskWidth, ui32 _edgesMaskHeight, ui32 _textureBlockSize, const glm::vec3& _point, bool _reverse)
{
    for(ui32 j = 0; j < _edgesMaskHeight / 4; ++j)
    {
        f32 currentEdgeHeight = (static_cast<f32>(j) - static_cast<f32>(_edgesMaskHeight / 8.0)) / 16.0;
        f32 height = CHeightmapHelper::Get_HeightValue(m_heightmapData, m_width, m_height, _point);

        ui32 indexOffset = _reverse == true ? (_edgesMaskWidth - 1) - _index + j * _edgesMaskWidth + _textureBlockSize : _index + j * _edgesMaskWidth + _textureBlockSize;
        _data[indexOffset] = TO_RGBA4444(0, 0, 0, 0);
        if(currentEdgeHeight < height && currentEdgeHeight > 0.0)
        {
            _data[indexOffset] = TO_RGBA4444(255, 0, 0, 255);
        }
        else
        {
            _data[indexOffset] = TO_RGBA4444(0, 255, 0, 0);
        }
    }
}

std::shared_ptr<CTexture> CHeightmapProcessor::PreprocessEdgesMaskTexture(void)
{
    ui32 edgesMaskWidth = 2048;
    ui32 edgesMaskHeight = 2048;

    ui32 textureId = CHeightmapProcessor::createTextureId();
    ui16* data = new ui16[edgesMaskWidth * edgesMaskHeight];
    for(ui32 i = 0; i < edgesMaskWidth; ++i)
    {
        CHeightmapProcessor::_FillEdgesMaskTextureBlock(data,
                                                        i,
                                                        edgesMaskWidth,
                                                        edgesMaskHeight,
                                                        0,
                                                        glm::vec3(static_cast<f32>(i) / static_cast<f32>(edgesMaskWidth) * m_width, 0.0f, 0.0f),
                                                        true);
        
        CHeightmapProcessor::_FillEdgesMaskTextureBlock(data,
                                                        i,
                                                        edgesMaskWidth,
                                                        edgesMaskHeight,
                                                        edgesMaskWidth * (edgesMaskHeight / 4),
                                                        glm::vec3(static_cast<f32>(i) / static_cast<f32>(edgesMaskWidth) * m_width, 0.0f, (m_width - 1)),
                                                        false);
        
        
        CHeightmapProcessor::_FillEdgesMaskTextureBlock(data,
                                                        i,
                                                        edgesMaskWidth,
                                                        edgesMaskHeight,
                                                        edgesMaskWidth * (edgesMaskHeight / 4) * 2,
                                                        glm::vec3(0.0f, 0.0f, static_cast<float>(i) / static_cast<float>(edgesMaskWidth) * m_width),
                                                        false);
        
        CHeightmapProcessor::_FillEdgesMaskTextureBlock(data,
                                                        i,
                                                        edgesMaskWidth,
                                                        edgesMaskHeight,
                                                        edgesMaskWidth * (edgesMaskHeight / 4) * 3,
                                                        glm::vec3((m_width - 1), 0.0f, static_cast<float>(i) / static_cast<float>(edgesMaskWidth) * m_width),
                                                        true);
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, edgesMaskWidth, edgesMaskHeight, 0, GL_RGBA, GL_UNSIGNED_SHORT_4_4_4_4, data);
    
    m_edgesMaskTexture = CTexture::constructCustomTexture("landscape.edges",
                                                          textureId,
                                                          edgesMaskWidth,
                                                          edgesMaskHeight);
    m_edgesMaskTexture->setWrapMode(GL_CLAMP_TO_EDGE);
    
    return m_edgesMaskTexture;
}


std::shared_ptr<CTexture> CHeightmapProcessor::PreprocessSplattingDiffuseTexture(const std::shared_ptr<CMaterial> &_material)
{
    assert(m_screenSpaceTextureAccessor != nullptr);
    assert(m_diffuseTexture == nullptr);
    assert(m_splattingTexture != nullptr);
    m_diffuseTexture = m_screenSpaceTextureAccessor->Get_CustomScreenSpaceOperationTexture(_material, 2048, 2048);
    return m_diffuseTexture;
}

std::shared_ptr<CTexture> CHeightmapProcessor::PreprocessSplattingNormalTexture(const std::shared_ptr<CMaterial> &_material)
{
    assert(m_screenSpaceTextureAccessor != nullptr);
    assert(m_normalTexture == nullptr);
    assert(m_splattingTexture != nullptr);
    m_normalTexture = m_screenSpaceTextureAccessor->Get_CustomScreenSpaceOperationTexture(_material, 2048, 2048);
    return m_normalTexture;
}

std::shared_ptr<CIndexBuffer> CHeightmapProcessor::_CreateIndexBuffer(void)
{
    assert(m_chunkWidth != 0);
    assert(m_chunkHeight != 0);
    
    std::shared_ptr<CIndexBuffer> indexBuffer = std::make_shared<CIndexBuffer>((m_chunkWidth - 1) * (m_chunkHeight - 1) * 6, GL_DYNAMIC_DRAW);
    ui16* indexData = indexBuffer->lock();
    
    ui32 index = 0;
    for(ui32 i = 0; i < (m_chunkWidth - 1); ++i)
    {
        for(ui32 j = 0; j < (m_chunkHeight - 1); ++j)
        {
            indexData[index] = i + j * m_chunkWidth;
            index++;
            indexData[index] = i + (j + 1) * m_chunkWidth;
            index++;
            indexData[index] = i + 1 + j * m_chunkWidth;
            index++;
            
            indexData[index] = i + (j + 1) * m_chunkWidth;
            index++;
            indexData[index] = i + 1 + (j + 1) * m_chunkWidth;
            index++;
            indexData[index] = i + 1 + j * m_chunkWidth;
            index++;
        }
    }
    indexBuffer->unlock();
    return indexBuffer;
}

std::shared_ptr<CVertexBuffer> CHeightmapProcessor::_CreateVertexBuffer(ui32 _widthOffset, ui32 _heightOffset, ui32 _numVertexes, GLenum _mode, glm::vec3 *_maxBound, glm::vec3 *_minBound)
{
    assert(m_heightmapData != nullptr);
    assert(m_chunkWidth != 0);
    assert(m_chunkHeight != 0);
    
    std::shared_ptr<CVertexBuffer> vertexBuffer = std::make_shared<CVertexBuffer>(_numVertexes, _mode);
    SAttributeVertex* vertexData = vertexBuffer->lock();
    
    ui32 index = 0;
    for(ui32 i = 0; i < m_chunkWidth;++i)
    {
        for(ui32 j = 0; j < m_chunkHeight;++j)
        {
            glm::vec2 position = glm::vec2(i + _widthOffset * m_chunkWidth - _widthOffset, j + _heightOffset * m_chunkHeight - _heightOffset);

            vertexData[index].m_position.x = position.x;
            vertexData[index].m_position.z = position.y;

            ui32 indexOffset_x = static_cast<ui32>(position.x) < m_width ? static_cast<ui32>(position.x) : static_cast<ui32>(m_width - 1);
            ui32 indexOffset_z = static_cast<ui32>(position.y) < m_height ? static_cast<ui32>(position.y) : static_cast<ui32>(m_height - 1);
            ui32 indexOffset = indexOffset_x + indexOffset_z * m_height;

            vertexData[index].m_position.y = m_heightmapData[indexOffset];
            vertexData[index].m_texcoord = CVertexBuffer::compressVec2(glm::vec2(static_cast<ui32>(position.x) / static_cast<f32>(m_width), static_cast<ui32>(position.y) / static_cast<f32>(m_height)));
            
            if(vertexData[index].m_position.x > _maxBound->x)
            {
                _maxBound->x = vertexData[index].m_position.x;
            }
            if(vertexData[index].m_position.y > _maxBound->y)
            {
                _maxBound->y = vertexData[index].m_position.y;
            }
            if(vertexData[index].m_position.z > _maxBound->z)
            {
                _maxBound->z = vertexData[index].m_position.z;
            }
            if(vertexData[index].m_position.x < _minBound->x)
            {
                _minBound->x = vertexData[index].m_position.x;
            }
            if(vertexData[index].m_position.y < _minBound->y)
            {
                _minBound->y = vertexData[index].m_position.y;
            }
            if(vertexData[index].m_position.z < _minBound->z)
            {
                _minBound->z = vertexData[index].m_position.z;
            }
            ++index;
        }
    }
    vertexBuffer->unlock();
    return vertexBuffer;
}
