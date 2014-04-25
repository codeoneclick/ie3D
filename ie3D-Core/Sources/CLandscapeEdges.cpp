//
//  CLandscapeEdges.cpp
//  iGaia
//
//  Created by sergey sergeev on 7/11/12.
//
//

#include "CLandscapeEdges.h"
#include "CMaterial.h"
#include "CShader.h"
#include "CTexture.h"
#include "CCamera.h"
#include "CLight.h"
#include "CResourceAccessor.h"
#include "CConfigurationGameObjects.h"
#include "CRenderMgr.h"
#include "CBatchingMgr.h"
#include "CMesh.h"
#include "CVertexBuffer.h"
#include "CIndexBuffer.h"

CLandscapeEdges::CLandscapeEdges(CSharedResourceAccessorRef resourceAccessor,
                                 ISharedScreenSpaceTextureAccessorRef screenSpaceTextureAccessor) :
IGameObject(resourceAccessor, screenSpaceTextureAccessor),
m_width(0),
m_height(0),
m_heightBounds(glm::vec2(0.0f, 0.0f))
{
    m_zOrder = E_GAME_OBJECT_Z_ORDER_LANDSCAPE_EDGES;
    
    m_materialBindImposer = [this](CSharedMaterialRef material)
    {
        material->getShader()->setMatrix4x4(m_matrixWorld, E_SHADER_UNIFORM_MATRIX_WORLD);
        material->getShader()->setMatrix4x4(m_camera->Get_ProjectionMatrix(), E_SHADER_UNIFORM_MATRIX_PROJECTION);
        material->getShader()->setMatrix4x4(!material->isReflecting() ? m_camera->Get_ViewMatrix() : m_camera->Get_ViewReflectionMatrix(), E_SHADER_UNIFORM_MATRIX_VIEW);
        material->getShader()->setMatrix4x4(m_camera->Get_MatrixNormal(), E_SHADER_UNIFORM_MATRIX_NORMAL);
        
        material->getShader()->setVector3(m_camera->Get_Position(), E_SHADER_UNIFORM_VECTOR_CAMERA_POSITION);
        material->getShader()->setVector4(material->getClippingPlane(), E_SHADER_UNIFORM_VECTOR_CLIP_PLANE);
        material->getShader()->setFloat(m_camera->Get_Near(), E_SHADER_UNIFORM_FLOAT_CAMERA_NEAR);
        material->getShader()->setFloat(m_camera->Get_Far(), E_SHADER_UNIFORM_FLOAT_CAMERA_FAR);
    };
}

CLandscapeEdges::~CLandscapeEdges(void)
{
    
}

void CLandscapeEdges::setEdgeTexture(CSharedTextureRef texture)
{
    for(const auto& material : m_materials)
    {
        material.second->setTexture(texture, E_SHADER_SAMPLER_01);
    }
}

void CLandscapeEdges::onSceneUpdate(f32 deltatime)
{
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        IGameObject::onSceneUpdate(deltatime);
    }
}

void CLandscapeEdges::onResourceLoaded(ISharedResourceRef resource, bool success)
{
     IGameObject::onResourceLoaded(resource, success);
}

void CLandscapeEdges::onConfigurationLoaded(ISharedConfigurationRef configuration, bool success)
{
    std::shared_ptr<CConfigurationLandscape> landscapeConfiguration = std::static_pointer_cast<CConfigurationLandscape>(configuration);
    assert(m_resourceAccessor != nullptr);
    
    m_width = landscapeConfiguration->getSize().x;
    m_height = landscapeConfiguration->getSize().y;
    m_heightBounds = glm::vec2(landscapeConfiguration->getEdgesSize().x,
                               landscapeConfiguration->getEdgesSize().y);
    
    for(const auto& iterator : landscapeConfiguration->getEdgesMaterialsConfigurations())
    {
        CSharedConfigurationMaterial materialConfiguration = std::static_pointer_cast<CConfigurationMaterial>(iterator);
        CSharedMaterial material = std::make_shared<CMaterial>();
        CMaterial::setupMaterial(material, materialConfiguration, m_resourceAccessor, m_screenSpaceTextureAccessor, shared_from_this());
        m_materials.insert(std::make_pair(materialConfiguration->getRenderOperationName(), material));
    }
    
    CSharedVertexBuffer vertexBuffer =std::make_shared<CVertexBuffer>(16, GL_STATIC_DRAW);
    SAttributeVertex* vertexData = vertexBuffer->lock();
    
    glm::vec3 minBound = glm::vec3(0.0f, m_heightBounds.x, 0.0f);
    glm::vec3 maxBound = glm::vec3(m_width, m_heightBounds.y, m_height);
    
    vertexData[0].m_position = glm::vec3(minBound.x,  minBound.y, maxBound.z);
    vertexData[1].m_position = glm::vec3(maxBound.x,  minBound.y, maxBound.z);
    vertexData[2].m_position = glm::vec3(maxBound.x,  maxBound.y, maxBound.z);
    vertexData[3].m_position = glm::vec3(minBound.x,  maxBound.y, maxBound.z);
    
    vertexData[4].m_position = glm::vec3(minBound.x,  minBound.y,  minBound.z);
    vertexData[5].m_position = glm::vec3(minBound.x,  maxBound.y,  minBound.z);
    vertexData[6].m_position = glm::vec3(maxBound.x,  maxBound.y,  minBound.z);
    vertexData[7].m_position = glm::vec3(maxBound.x,  minBound.y,  minBound.z);
    
    vertexData[8].m_position = glm::vec3(maxBound.x,  minBound.y,   maxBound.z);
    vertexData[9].m_position = glm::vec3(maxBound.x,  minBound.y,   minBound.z);
    vertexData[10].m_position = glm::vec3(maxBound.x,  maxBound.y,  minBound.z);
    vertexData[11].m_position = glm::vec3(maxBound.x,  maxBound.y,  maxBound.z);
    
    vertexData[12].m_position = glm::vec3(minBound.x,  minBound.y,  minBound.z);
    vertexData[13].m_position = glm::vec3(minBound.x,  minBound.y,  maxBound.z);
    vertexData[14].m_position = glm::vec3(minBound.x,  maxBound.y,  maxBound.z);
    vertexData[15].m_position = glm::vec3(minBound.x,  maxBound.y,  minBound.z);
    
    vertexData[0].m_texcoord = CVertexBuffer::compressVec2(glm::vec2(0.0f, 1.0f / 4.0f));
    vertexData[1].m_texcoord = CVertexBuffer::compressVec2(glm::vec2(1.0f - 0.001f, 1.0f / 4.0f));
    vertexData[2].m_texcoord = CVertexBuffer::compressVec2(glm::vec2(1.0f - 0.001f, (1.0f / 4.0f) * 2.0f - 0.001f));
    vertexData[3].m_texcoord = CVertexBuffer::compressVec2(glm::vec2(0.0f, (1.0f / 4.0f) * 2.0f - 0.001f));
    
    vertexData[4].m_texcoord = CVertexBuffer::compressVec2(glm::vec2(1.0f - 0.001f, 0.0f));
    vertexData[5].m_texcoord = CVertexBuffer::compressVec2(glm::vec2(1.0f - 0.001f, 1.0f / 4.0f - 0.001f));
    vertexData[6].m_texcoord = CVertexBuffer::compressVec2(glm::vec2(0.0f, 1.0f / 4.0f - 0.001f));
    vertexData[7].m_texcoord = CVertexBuffer::compressVec2(glm::vec2(0.0f, 0.0f));
    
    vertexData[8].m_texcoord =  CVertexBuffer::compressVec2(glm::vec2(0.0f, (1.0f / 4.0f) * 3.0f));
    vertexData[9].m_texcoord =  CVertexBuffer::compressVec2(glm::vec2(1.0f - 0.001f, (1.0f / 4.0f) * 3.0f));
    vertexData[10].m_texcoord = CVertexBuffer::compressVec2(glm::vec2(1.0f - 0.001f, 1.0f - 0.001f));
    vertexData[11].m_texcoord = CVertexBuffer::compressVec2(glm::vec2(0.0f, 1.0f - 0.001f));
    
    vertexData[12].m_texcoord = CVertexBuffer::compressVec2(glm::vec2(0.0f, (1.0f / 4.0f) * 2.0f));
    vertexData[13].m_texcoord = CVertexBuffer::compressVec2(glm::vec2(1.0f - 0.001f, (1.0f / 4.0f) * 2.0f));
    vertexData[14].m_texcoord = CVertexBuffer::compressVec2(glm::vec2(1.0f - 0.001f, (1.0f / 4.0f) * 3.0f - 0.001f));
    vertexData[15].m_texcoord = CVertexBuffer::compressVec2(glm::vec2(0.0f, (1.0f / 4.0f) * 3.0f - 0.001f));
    
    vertexBuffer->unlock();
    
    CSharedIndexBuffer indexBuffer = std::make_shared<CIndexBuffer>(24, GL_STATIC_DRAW);
    ui16* indexData = indexBuffer->lock();
    
    indexData[0] = 0;
    indexData[1] = 1;
    indexData[2] = 2;
    indexData[3] = 0;
    indexData[4] = 2;
    indexData[5] = 3;
    
    indexData[6] = 4;
    indexData[7] = 5;
    indexData[8] = 6;
    indexData[9] = 4;
    indexData[10] = 6;
    indexData[11] = 7;
    
    indexData[12] = 8;
    indexData[13] = 9;
    indexData[14] = 10;
    indexData[15] = 8;
    indexData[16] = 10;
    indexData[17] = 11;
    
    indexData[18] = 12;
    indexData[19] = 13;
    indexData[20] = 14;
    indexData[21] = 12;
    indexData[22] = 14;
    indexData[23] = 15;
    
    indexBuffer->unlock();
    
    m_mesh = CMesh::constructCustomMesh("landscape.edges", vertexBuffer, indexBuffer,
                                     maxBound, minBound);
    
	IGameObject::listenRenderMgr(m_isNeedToRender);
    m_status |= E_LOADING_STATUS_TEMPLATE_LOADED;
}

i32  CLandscapeEdges::zOrder(void)
{
    return m_zOrder;
}

bool CLandscapeEdges::checkOcclusion(void)
{
    return IGameObject::checkOcclusion();
}

ui32 CLandscapeEdges::numTriangles(void)
{
    return IGameObject::numTriangles();
}

void CLandscapeEdges::onBind(const std::string& mode)
{
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        assert(m_materials.find(mode) != m_materials.end());
        IGameObject::onBind(mode);
    }
}

void CLandscapeEdges::onDraw(const std::string& mode)
{
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        assert(m_camera != nullptr);
        assert(m_materials.find(mode) != m_materials.end());
        
        std::shared_ptr<CMaterial> material = m_materials.find(mode)->second;
        assert(material->getShader() != nullptr);
        
        m_materialBindImposer(material);
        
        assert(m_mesh != nullptr);
        IGameObject::onDraw(mode);
    }
}

void CLandscapeEdges::onUnbind(const std::string& mode)
{
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        assert(m_materials.find(mode) != m_materials.end());
        IGameObject::onUnbind(mode);
    }
}

void CLandscapeEdges::onBatch(const std::string& mode)
{
    IGameObject::onBatch(mode);
}
