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

#import <UIKit/UIKit.h>

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
    
    UIImage* image = [UIImage imageNamed:[NSString stringWithCString:"mesa_heightmap" encoding:NSUTF8StringEncoding]];
    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
    size_t bytesPerRow = image.size.width * 4;
    ui8* data = (ui8 *)malloc(image.size.height * bytesPerRow);
    CGContextRef context = CGBitmapContextCreate(data,
                                                 image.size.width,
                                                 image.size.height,
                                                 8,
                                                 bytesPerRow,
                                                 colorSpace,
                                                 kCGImageAlphaNoneSkipFirst);
    UIGraphicsPushContext(context);
    CGContextTranslateCTM(context, 0.0, image.size.height);
    CGContextScaleCTM(context, 1.0, -1.0);
    [image drawInRect:CGRectMake(0.0, 0.0, image.size.width, image.size.height)];
    UIGraphicsPopContext();
    
    ui32 index = 0;
    for(ui32 i = 0; i < m_width; ++i)
    {
        for(ui32 j = 0; j < m_height; ++j)
        {
            m_heightmapData[i + j * m_height] = (static_cast<f32>(data[(i + j * m_width) * 4 + 1] - 64) / 255) * 32.0;
			//m_heightmapData[i + j * m_height] = bytes[index] / 255 * 10.0;//sin(i * 0.1) * 10.0 + cos(j * 0.1) * 10.0;
            index += 4;
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
    
    m_chunksBounds.resize(m_numChunkRows * m_numChunkCells);
    for(ui32 i = 0; i < m_numChunkRows; ++i)
    {
        for(ui32 j = 0; j < m_numChunkCells; ++j)
        {
            glm::vec3 maxBound = glm::vec3( -4096.0f, -4096.0f, -4096.0f );
            glm::vec3 minBound = glm::vec3(  4096.0f,  4096.0f,  4096.0f );
            CHeightmapProcessor::createChunkBound(i, j, &maxBound, &minBound);
            m_chunksBounds[i + j * m_numChunkRows] = std::make_tuple(maxBound, minBound);
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

            f32 height = CHeightmapHelper::Get_HeightValue(m_heightmapData, m_width, m_height, glm::vec3(i , 0.0, j));
            if(height > 10.0)
            {
                data[i + j * m_width] = TO_RGB565(0, 255, 0);
            }
            if(height < 1.0)
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

CSharedIndexBuffer CHeightmapProcessor::createIndexBuffer(void)
{
    assert(m_chunkWidth != 0);
    assert(m_chunkHeight != 0);
    
    CSharedIndexBuffer indexBuffer = std::make_shared<CIndexBuffer>((m_chunkWidth - 1) * (m_chunkHeight - 1) * 6, GL_DYNAMIC_DRAW);
    CHeightmapProcessor::fillIndexBuffer(indexBuffer);
    return indexBuffer;
}

CSharedVertexBuffer CHeightmapProcessor::createVertexBuffer(ui32 widthOffset,
                                                            ui32 heightOffset,
                                                            ui32 numVertexes,
                                                            GLenum mode,
                                                            glm::vec3 *maxBound, glm::vec3 *minBound)
{
    assert(m_heightmapData != nullptr);
    assert(m_chunkWidth != 0);
    assert(m_chunkHeight != 0);
    
    std::shared_ptr<CVertexBuffer> vertexBuffer = std::make_shared<CVertexBuffer>(numVertexes, mode);
    CHeightmapProcessor::fillVertexBuffer(vertexBuffer, widthOffset, heightOffset, numVertexes);
    CHeightmapProcessor::createChunkBound(widthOffset, heightOffset, maxBound, minBound);
    return vertexBuffer;
}

const std::tuple<glm::vec3, glm::vec3> CHeightmapProcessor::getChunkBounds(ui32 i, ui32 j) const
{
    assert(m_chunksBounds.size() != 0);
    return m_chunksBounds[i + j * m_numChunkRows];
}

CSharedMesh CHeightmapProcessor::getChunk(ui32 i, ui32 j)
{
    CSharedMesh mesh = nullptr;
    if(m_chunksUnused.size() != 0)
    {
        mesh = m_chunksUnused.at(m_chunksUnused.size() - 1);
        m_chunksUnused.pop_back();
        CHeightmapProcessor::fillVertexBuffer(mesh->getVertexBuffer(), i, j,  m_chunkWidth * m_chunkHeight);
        CHeightmapProcessor::fillIndexBuffer(mesh->getIndexBuffer());
        CHeightmapProcessor::fillNormals(mesh->getVertexBuffer(), mesh->getIndexBuffer());
        CHeightmapProcessor::fillTangentsAndBinormals(mesh->getVertexBuffer(), mesh->getIndexBuffer());
        mesh->updateBounds();
    }
    else
    {
        glm::vec3 maxBound = glm::vec3( -4096.0f, -4096.0f, -4096.0f );
        glm::vec3 minBound = glm::vec3( 4096.0f, 4096.0f, 4096.0f );
        
        CSharedVertexBuffer vertexBuffer = CHeightmapProcessor::createVertexBuffer(i, j,
                                                                                   m_chunkWidth * m_chunkHeight,
                                                                                   GL_STATIC_DRAW,
                                                                                   &maxBound, &minBound);
        CSharedIndexBuffer indexBuffer = CHeightmapProcessor::createIndexBuffer();
        mesh = CMesh::constructCustomMesh("landscape.chunk", vertexBuffer, indexBuffer,
                                          maxBound, minBound);
        CHeightmapProcessor::fillNormals(mesh->getVertexBuffer(), mesh->getIndexBuffer());
        CHeightmapProcessor::fillTangentsAndBinormals(mesh->getVertexBuffer(), mesh->getIndexBuffer());
    }
    return mesh;
}

void CHeightmapProcessor::freeChunk(CSharedMeshRef chunk)
{
    m_chunksUnused.push_back(chunk);
}

void CHeightmapProcessor::createChunkBound(ui32 widthOffset, ui32 heightOffset, glm::vec3* maxBound, glm::vec3* minBound)
{
    assert(m_heightmapData != nullptr);
    assert(m_chunkWidth != 0);
    assert(m_chunkHeight != 0);
    
    for(ui32 i = 0; i < m_chunkWidth;++i)
    {
        for(ui32 j = 0; j < m_chunkHeight;++j)
        {
            glm::vec3 point;
            point.x = i + widthOffset * m_chunkWidth - widthOffset;
            point.z = j + heightOffset * m_chunkHeight - heightOffset;

            ui32 indexOffset_x = static_cast<ui32>(point.x) < m_width ? static_cast<ui32>(point.x) : static_cast<ui32>(m_width - 1);
            ui32 indexOffset_z = static_cast<ui32>(point.z) < m_height ? static_cast<ui32>(point.z) : static_cast<ui32>(m_height - 1);
            ui32 indexOffset = indexOffset_x + indexOffset_z * m_height;
            
            point.y = m_heightmapData[indexOffset];
            *maxBound = CMeshData::calculateMaxBound(point, *maxBound);
            *minBound = CMeshData::calculateMinBound(point, *minBound);
        }
    }
}

void CHeightmapProcessor::fillVertexBuffer(CSharedVertexBufferRef vertexBuffer,
                                           ui32 widthOffset,
                                           ui32 heightOffset,
                                           ui32 numVertexes)
{
    assert(vertexBuffer != nullptr);
    SAttributeVertex* vertexData = vertexBuffer->lock();
    ui32 index = 0;
    for(ui32 i = 0; i < m_chunkWidth;++i)
    {
        for(ui32 j = 0; j < m_chunkHeight;++j)
        {
            glm::vec2 position = glm::vec2(i + widthOffset * m_chunkWidth - widthOffset, j + heightOffset * m_chunkHeight - heightOffset);
            
            vertexData[index].m_position.x = position.x;
            vertexData[index].m_position.z = position.y;
            
            ui32 indexOffset_x = static_cast<ui32>(position.x) < m_width ? static_cast<ui32>(position.x) : static_cast<ui32>(m_width - 1);
            ui32 indexOffset_z = static_cast<ui32>(position.y) < m_height ? static_cast<ui32>(position.y) : static_cast<ui32>(m_height - 1);
            ui32 indexOffset = indexOffset_x + indexOffset_z * m_height;
            
            vertexData[index].m_position.y = m_heightmapData[indexOffset];
            vertexData[index].m_texcoord = CVertexBuffer::compressVec2(glm::vec2(static_cast<ui32>(position.x) /
                                                                                 static_cast<f32>(m_width),
                                                                                 static_cast<ui32>(position.y) /
                                                                                 static_cast<f32>(m_height)));
            ++index;
        }
    }
    vertexBuffer->unlock();
}

void CHeightmapProcessor::fillIndexBuffer(CSharedIndexBufferRef indexBuffer)
{
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
}

dispatch_queue_t backgroundQueue(void)
{
    static dispatch_queue_t queue;
    static dispatch_once_t onceToken;
        dispatch_once(&onceToken, ^{
            queue = dispatch_queue_create("update.tangent.space.queue", DISPATCH_QUEUE_SERIAL);
        });
    return queue;
}

void CHeightmapProcessor::fillNormals(CSharedVertexBufferRef _vertexBuffer,
                                      CSharedIndexBufferRef _indexBuffer)
{
    __block CSharedVertexBuffer vertexBuffer = _vertexBuffer;
    __block CSharedIndexBuffer indexBuffer = _indexBuffer;
    dispatch_async(backgroundQueue(), ^{
        SAttributeVertex* vertexData = vertexBuffer->lock();
        ui16* indexData = indexBuffer->lock();
        ui32 numIndexes = indexBuffer->getSize();
        
        for(ui32 index = 0; index < numIndexes; index += 3)
        {
            glm::vec3 point_01 = vertexData[indexData[index + 0]].m_position;
            glm::vec3 point_02 = vertexData[indexData[index + 1]].m_position;
            glm::vec3 point_03 = vertexData[indexData[index + 2]].m_position;
            
            glm::vec3 edge_01 = point_02 - point_01;
            glm::vec3 edge_02 = point_03 - point_01;
            glm::vec3 normal = glm::cross(edge_01, edge_02);
            normal = glm::normalize(normal);
            glm::u8vec4 complessedNormal = CVertexBuffer::compressVec3(normal);
            vertexData[indexData[index + 0]].m_normal = complessedNormal;
            vertexData[indexData[index + 1]].m_normal = complessedNormal;
            vertexData[indexData[index + 2]].m_normal = complessedNormal;
        }
        dispatch_sync(dispatch_get_main_queue(), ^{
            vertexBuffer->unlock();
        });
    });
}

void CHeightmapProcessor::fillTangentsAndBinormals(CSharedVertexBufferRef _vertexBuffer,
                                                   CSharedIndexBufferRef _indexBuffer)
{
    __block CSharedVertexBuffer vertexBuffer = _vertexBuffer;
    __block CSharedIndexBuffer indexBuffer = _indexBuffer;
    dispatch_async(backgroundQueue(), ^{
        std::vector<glm::vec3> tangents, binormals;
        
        SAttributeVertex* vertexData = vertexBuffer->lock();
        ui32 numVertexes = vertexBuffer->getSize();
        
        ui16* indexData = indexBuffer->lock();
        ui32 numIndexes = indexBuffer->getSize();
        
        for (ui32 i = 0; i < numIndexes; i += 3 )
        {
            glm::vec3 v1 = vertexData[indexData[i + 0]].m_position;
            glm::vec3 v2 = vertexData[indexData[i + 1]].m_position;
            glm::vec3 v3 = vertexData[indexData[i + 2]].m_position;
            float s1 = vertexData[indexData[i + 0]].m_texcoord.x;
            float t1 = vertexData[indexData[i + 0]].m_texcoord.y;
            float s2 = vertexData[indexData[i + 1]].m_texcoord.x;
            float t2 = vertexData[indexData[i + 1]].m_texcoord.y;
            float s3 = vertexData[indexData[i + 2]].m_texcoord.x;
            float t3 = vertexData[indexData[i + 2]].m_texcoord.y;
            
            glm::vec3 t, b;
            CHeightmapProcessor::getTriangleBasis(v1, v2, v3, s1, t1, s2, t2, s3, t3, t, b);
            tangents.push_back(t);
            binormals.push_back(b);
        }
        
        for (ui32 i = 0; i < numVertexes; i++)
        {
            std::vector<glm::vec3> lrt, lrb;
            for (ui32 j = 0; j < numIndexes; j += 3)
            {
                if ((indexData[j + 0]) == i || (indexData[j + 1]) == i || (indexData[j + 2]) == i)
                {
                    lrt.push_back(tangents[i]);
                    lrb.push_back(binormals[i]);
                }
            }
            
            glm::vec3 tangentRes(0.0f, 0.0f, 0.0f);
            glm::vec3 binormalRes(0.0f, 0.0f, 0.0f);
            for (ui32 j = 0; j < lrt.size(); j++)
            {
                tangentRes += lrt[j];
                binormalRes += lrb[j];
            }
            tangentRes /= static_cast<f32>(lrt.size());
            binormalRes /= static_cast<f32>(lrb.size());
            
            glm::vec3 normal =  CVertexBuffer::uncompressU8Vec4(vertexData[i].m_normal);
            tangentRes = CHeightmapProcessor::ortogonalize(normal, tangentRes);
            binormalRes = CHeightmapProcessor::ortogonalize(normal, binormalRes);
            
            vertexData[i].m_tangent = CVertexBuffer::compressVec3(tangentRes);
        }
        dispatch_sync(dispatch_get_main_queue(), ^{
            vertexBuffer->unlock();
        });
    });
}

void CHeightmapProcessor::getTriangleBasis(const glm::vec3& E, const glm::vec3& F, const glm::vec3& G,
                                           f32 sE, f32 tE, f32 sF, f32 tF, f32 sG, f32 tG,
                                           glm::vec3& tangentX, glm::vec3& tangentY)
{
    glm::vec3 P = F - E;
    glm::vec3 Q = G - E;
    f32 s1 = sF - sE;
    f32 t1 = tF - tE;
    f32 s2 = sG - sE;
    f32 t2 = tG - tE;
    f32 pqMatrix[2][3];
    pqMatrix[0][0] = P[0];
    pqMatrix[0][1] = P[1];
    pqMatrix[0][2] = P[2];
    pqMatrix[1][0] = Q[0];
    pqMatrix[1][1] = Q[1];
    pqMatrix[1][2] = Q[2];
    f32 temp = 1.0f / ( s1 * t2 - s2 * t1);
    f32 stMatrix[2][2];
    stMatrix[0][0] = t2 * temp;
    stMatrix[0][1] = -t1 * temp;
    stMatrix[1][0] = -s2 * temp;
    stMatrix[1][1] = s1 * temp;
    f32 tbMatrix[2][3];
    tbMatrix[0][0] = stMatrix[0][0] * pqMatrix[0][0] + stMatrix[0][1] * pqMatrix[1][0];
    tbMatrix[0][1] = stMatrix[0][0] * pqMatrix[0][1] + stMatrix[0][1] * pqMatrix[1][1];
    tbMatrix[0][2] = stMatrix[0][0] * pqMatrix[0][2] + stMatrix[0][1] * pqMatrix[1][2];
    tbMatrix[1][0] = stMatrix[1][0] * pqMatrix[0][0] + stMatrix[1][1] * pqMatrix[1][0];
    tbMatrix[1][1] = stMatrix[1][0] * pqMatrix[0][1] + stMatrix[1][1] * pqMatrix[1][1];
    tbMatrix[1][2] = stMatrix[1][0] * pqMatrix[0][2] + stMatrix[1][1] * pqMatrix[1][2];
    tangentX = glm::vec3( tbMatrix[0][0], tbMatrix[0][1], tbMatrix[0][2] );
    tangentY = glm::vec3( tbMatrix[1][0], tbMatrix[1][1], tbMatrix[1][2] );
    tangentX = glm::normalize(tangentX);
    tangentY = glm::normalize(tangentY);
}

glm::vec3 CHeightmapProcessor::getClosestPointOnLine(const glm::vec3& a, const glm::vec3& b, const glm::vec3& p)
{
    glm::vec3 c = p - a;
    glm::vec3 V = b - a;
    f32 d = V.length();
    V = glm::normalize(V);
    f32 t = glm::dot( V, c );
    
    if ( t < 0.0f )
        return a;
    if ( t > d )
        return b;
    V *= t;
    return ( a + V );
}

glm::vec3 CHeightmapProcessor::ortogonalize(const glm::vec3& v1, const glm::vec3& v2)
{
    glm::vec3 v2ProjV1 = CHeightmapProcessor::getClosestPointOnLine( v1, -v1, v2 );
    glm::vec3 res = v2 - v2ProjV1;
    res = glm::normalize(res);
    return res;
}

