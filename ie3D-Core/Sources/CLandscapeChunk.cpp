//
//  CLandscapeChunk.cpp
//  iGaia
//
//  Created by Sergey Sergeev on 3/14/13.
//
//

#include "CLandscapeChunk.h"
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
#include "CQuadTree.h"

CLandscapeChunk::CLandscapeChunk(CSharedResourceAccessorRef resourceAccessor,
                                 ISharedScreenSpaceTextureAccessorRef screenSpaceTextureAccessor) :
IGameObject(resourceAccessor, screenSpaceTextureAccessor),
m_width(0),
m_height(0),
m_numIndexesToRender(0)
{
    m_zOrder = E_GAME_OBJECT_Z_ORDER_LANDSCAPE;
    
    m_materialBindImposer = [this](CSharedMaterialRef material)
    {
        material->getShader()->setMatrix4x4(m_isBatching ? glm::mat4x4(1.0f) : m_matrixWorld, E_SHADER_UNIFORM_MATRIX_WORLD);
        material->getShader()->setMatrix4x4(m_camera->Get_ProjectionMatrix(), E_SHADER_UNIFORM_MATRIX_PROJECTION);
        material->getShader()->setMatrix4x4(!material->isReflecting() ? m_camera->Get_ViewMatrix() : m_camera->Get_ViewReflectionMatrix(), E_SHADER_UNIFORM_MATRIX_VIEW);
        material->getShader()->setMatrix4x4(m_camera->Get_MatrixNormal(), E_SHADER_UNIFORM_MATRIX_NORMAL);
        
        material->getShader()->setVector3(m_camera->Get_Position(), E_SHADER_UNIFORM_VECTOR_CAMERA_POSITION);
        material->getShader()->setVector4(material->getClippingPlane(), E_SHADER_UNIFORM_VECTOR_CLIP_PLANE);
        material->getShader()->setFloat(m_camera->Get_Near(), E_SHADER_UNIFORM_FLOAT_CAMERA_NEAR);
        material->getShader()->setFloat(m_camera->Get_Far(), E_SHADER_UNIFORM_FLOAT_CAMERA_FAR);
    };
}

CLandscapeChunk::~CLandscapeChunk(void)
{
    m_quadTree->destroy();
    std::cout<<"[CLandscapeChunk::~CLandscapeChunk]"<<std::endl;
}

void CLandscapeChunk::setMesh(CSharedMeshRef mesh,
             ui32 width,
             ui32 height)
{
    assert(m_mesh == nullptr);
    assert(mesh != nullptr);
    m_mesh = mesh;
    m_width = width;
    m_height = height;
}

void CLandscapeChunk::setSplattingDiffuseTexture(CSharedTextureRef texture)
{
    for(const auto& material : m_materials)
    {
        material.second->setTexture(texture, E_SHADER_SAMPLER_01);
    }
}

void CLandscapeChunk::setSplattingNormalTexture(CSharedTextureRef texture)
{
    for(const auto& material : m_materials)
    {
        material.second->setTexture(texture, E_SHADER_SAMPLER_02);
    }
}

void CLandscapeChunk::onSceneUpdate(f32 deltatime)
{
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        IGameObject::onSceneUpdate(deltatime);
        m_numIndexesToRender = m_quadTree->update(m_camera->Get_Frustum());
    }
}

void CLandscapeChunk::onResourceLoaded(ISharedResourceRef resource, bool success)
{
    IGameObject::onResourceLoaded(resource, success);
}

void CLandscapeChunk::onConfigurationLoaded(ISharedConfigurationRef configuration, bool success)
{
    IGameObject::onConfigurationLoaded(configuration, success);
    
    std::shared_ptr<CConfigurationLandscape> landscapeConfiguration = std::static_pointer_cast<CConfigurationLandscape>(configuration);
    assert(m_resourceAccessor != nullptr);
    
    m_quadTree = std::make_shared<CQuadTree>();
    m_quadTree->generate(m_mesh->getVertexBuffer(),
                         m_mesh->getIndexBuffer(),
                         m_mesh->getMaxBound(),
                         m_mesh->getMinBound(),
                         4,
                         m_width);
    
	IGameObject::listenRenderMgr(m_isNeedToRender);
    m_status |= E_LOADING_STATUS_TEMPLATE_LOADED;
}

i32 CLandscapeChunk::zOrder(void)
{
    return m_zOrder;
}

bool CLandscapeChunk::checkOcclusion(void)
{
    return IGameObject::checkOcclusion();
}

ui32 CLandscapeChunk::numTriangles(void)
{
    return m_mesh && m_mesh->isLoaded() ? m_numIndexesToRender / 3 : 0;
}

void CLandscapeChunk::onBind(const std::string& mode)
{
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        assert(m_materials.find(mode) != m_materials.end());
        IGameObject::onBind(mode);
    }
}

void CLandscapeChunk::onDraw(const std::string& mode)
{
    if((m_status & E_LOADING_STATUS_TEMPLATE_LOADED) && m_numIndexesToRender != 0)
    {
        assert(m_camera != nullptr);
        assert(m_materials.find(mode) != m_materials.end());
        
        std::shared_ptr<CMaterial> material = m_materials.find(mode)->second;
        assert(material->getShader() != nullptr);
        
        m_materialBindImposer(material);
        
        assert(m_mesh != nullptr);
        m_mesh->draw(m_numIndexesToRender);
    }
}

void CLandscapeChunk::onUnbind(const std::string& mode)
{
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        assert(m_materials.find(mode) != m_materials.end());
        IGameObject::onUnbind(mode);
    }
}

void CLandscapeChunk::onBatch(const std::string& mode)
{
    IGameObject::onBatch(mode);
}

CSharedVertexBuffer CLandscapeChunk::getCollisionVertexBuffer(void) const
{
    assert(m_mesh != nullptr);
    assert(m_mesh->getVertexBuffer() != nullptr);
    return m_mesh->getVertexBuffer();
}

CSharedIndexBuffer CLandscapeChunk::getCollisionIndexBuffer(void) const
{
    assert(m_mesh != nullptr);
    assert(m_mesh->getIndexBuffer() != nullptr);
    return m_mesh->getIndexBuffer();
}

