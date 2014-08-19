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
#include "CBatchingMgr.h"
#include "CMesh.h"
#include "CQuadTree.h"

CLandscapeChunk::CLandscapeChunk(CSharedResourceAccessorRef resourceAccessor,
                                 ISharedRenderTechniqueAccessorRef renderTechniqueAccessor) :
IGameObject(resourceAccessor, renderTechniqueAccessor),
m_heightmapSize(glm::ivec2(0)),
m_numIndexesToRender(0),
m_prerenderedSplattingDiffuseTexture(nullptr),
m_prerenderedSplattingNormalTexture(nullptr),
m_quadTree(nullptr)
{
    m_zOrder = E_GAME_OBJECT_Z_ORDER_LANDSCAPE;
    
    m_materialBindImposer = [this](CSharedMaterialRef material)
    {
        material->getShader()->setMatrix4x4(m_isBatching ? glm::mat4x4(1.0f) : m_matrixWorld, E_SHADER_UNIFORM_MATRIX_WORLD);
        material->getShader()->setMatrix4x4(m_camera->Get_ProjectionMatrix(), E_SHADER_UNIFORM_MATRIX_PROJECTION);
        material->getShader()->setMatrix4x4(!material->isReflecting() ? m_camera->Get_ViewMatrix() : m_camera->Get_ViewReflectionMatrix(),
                                            E_SHADER_UNIFORM_MATRIX_VIEW);
        material->getShader()->setMatrix4x4(m_camera->Get_MatrixNormal(), E_SHADER_UNIFORM_MATRIX_NORMAL);
        
        material->getShader()->setVector3(m_camera->Get_Position(), E_SHADER_UNIFORM_VECTOR_CAMERA_POSITION);
        material->getShader()->setVector4(material->getClippingPlane(), E_SHADER_UNIFORM_VECTOR_CLIP_PLANE);
        material->getShader()->setFloat(m_camera->Get_Near(), E_SHADER_UNIFORM_FLOAT_CAMERA_NEAR);
        material->getShader()->setFloat(m_camera->Get_Far(), E_SHADER_UNIFORM_FLOAT_CAMERA_FAR);
        
#if defined(__OSX__)
        material->getShader()->setFloatCustom(m_tillingTexcoord[E_SHADER_SAMPLER_01], "IN_TillingTexcoordLayer_01");
        material->getShader()->setFloatCustom(m_tillingTexcoord[E_SHADER_SAMPLER_02], "IN_TillingTexcoordLayer_02");
        material->getShader()->setFloatCustom(m_tillingTexcoord[E_SHADER_SAMPLER_03], "IN_TillingTexcoordLayer_03");
#elif defined(__IOS__)
        if(g_highPerformancePlatforms.count(getPlatform()) != 0)
        {
            material->getShader()->setFloatCustom(m_tillingTexcoord[E_SHADER_SAMPLER_01], "IN_TillingTexcoordLayer_01");
            material->getShader()->setFloatCustom(m_tillingTexcoord[E_SHADER_SAMPLER_02], "IN_TillingTexcoordLayer_02");
            material->getShader()->setFloatCustom(m_tillingTexcoord[E_SHADER_SAMPLER_03], "IN_TillingTexcoordLayer_03");
        }
        else
        {
            if(m_prerenderedSplattingDiffuseTexture)
            {
                material->getShader()->setTexture(m_prerenderedSplattingDiffuseTexture, E_SHADER_SAMPLER_01);
            }
            if(m_prerenderedSplattingNormalTexture)
            {
                material->getShader()->setTexture(m_prerenderedSplattingNormalTexture, E_SHADER_SAMPLER_02);
            }
        }
#else
        material->getShader()->setFloatCustom(MAX_VALUE(m_heightmapSize.x, m_heightmapSize.y) / m_splattingTillingFactor,
                                              "IN_SplattingTillingFactor");
#endif
        material->getShader()->setFloatCustom(256.0, "IN_fogLinearStart");
        material->getShader()->setFloatCustom(512.0, "IN_fogLinearEnd");
    };
}

CLandscapeChunk::~CLandscapeChunk(void)
{
    if(m_quadTree != nullptr)
    {
        m_quadTree->destroy();
    }
    m_materialBindImposer = nullptr;
}

void CLandscapeChunk::setMesh(CSharedMeshRef mesh)
{
    assert(m_mesh == nullptr);
    assert(mesh != nullptr);
    m_mesh = mesh;
}

void CLandscapeChunk::setQuadTree(CSharedQuadTreeRef quadTree)
{
    assert(m_quadTree == nullptr);
    assert(quadTree != nullptr);
    m_quadTree = quadTree;
}

void CLandscapeChunk::setHeightmapSize(const glm::ivec2 &size)
{
    m_heightmapSize = size;
}

void CLandscapeChunk::setTillingTexcoord(f32 value, E_SHADER_SAMPLER sampler)
{
    m_tillingTexcoord[sampler] = value;
}

void CLandscapeChunk::setPrerenderedSplattingDiffuseTexture(CSharedTextureRef texture)
{
    m_prerenderedSplattingDiffuseTexture = texture;
}

void CLandscapeChunk::setPrerenderedSplattingNormalTexture(CSharedTextureRef texture)
{
    m_prerenderedSplattingNormalTexture = texture;
}

void CLandscapeChunk::setDiffuseTextureLayer_01(CSharedTextureRef texture)
{
    // #TODO:
}

void CLandscapeChunk::setDiffuseTextureLayer_02(CSharedTextureRef texture)
{
    // #TODO:
}

void CLandscapeChunk::setDiffuseTextureLayer_03(CSharedTextureRef texture)
{
    // #TODO:
}

void CLandscapeChunk::setSplattinMaskTexture(CSharedTextureRef texture)
{
    for(const auto& material : m_materials)
    {
        material.second->setTexture(texture, E_SHADER_SAMPLER_04);
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
    IResourceLoadingHandler::onResourceLoaded(resource, success);
}

void CLandscapeChunk::onConfigurationLoaded(ISharedConfigurationRef configuration, bool success)
{
    IGameObject::onConfigurationLoaded(configuration, success);
    
	IGameObject::enableRender(m_isNeedToRender);
    IGameObject::enableUpdate(m_isNeedToUpdate);
    
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

