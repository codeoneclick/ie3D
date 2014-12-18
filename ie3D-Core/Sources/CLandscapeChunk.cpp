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
#include "CGlobalLightSource.h"
#include "CResourceAccessor.h"
#include "CConfigurationGameObjects.h"
#include "CBatchingMgr.h"
#include "CMesh.h"
#include "CQuadTree.h"
#include "CVertexBuffer.h"
#include "CIndexBuffer.h"

CLandscapeChunk::CLandscapeChunk(CSharedResourceAccessorRef resourceAccessor,
                                 ISharedRenderTechniqueAccessorRef renderTechniqueAccessor) :
IGameObject(resourceAccessor, renderTechniqueAccessor),
m_heightmapSize(glm::ivec2(0)),
m_numIndexesToRender(0),
m_prerenderedSplattingDiffuseTexture(nullptr),
m_prerenderedSplattingNormalTexture(nullptr),
m_quadTree(nullptr),
m_currentLOD(E_LANDSCAPE_CHUNK_LOD_UNKNOWN),
m_inprogressLOD(E_LANDSCAPE_CHUNK_LOD_UNKNOWN)
{
    m_isNeedBoundingBox = true;
    m_zOrder = E_GAME_OBJECT_Z_ORDER_LANDSCAPE;
}

CLandscapeChunk::~CLandscapeChunk(void)
{
    m_materialBindImposer = nullptr;
}

void CLandscapeChunk::setMesh(CSharedMeshRef mesh)
{
    m_mesh = mesh;
}

void CLandscapeChunk::setQuadTree(CSharedQuadTreeRef quadTree, E_LANDSCAPE_CHUNK_LOD LOD)
{
    m_quadTree = quadTree;
    m_currentLOD = LOD;
}

E_LANDSCAPE_CHUNK_LOD CLandscapeChunk::getCurrentLOD(void) const
{
    return m_currentLOD;
}

E_LANDSCAPE_CHUNK_LOD CLandscapeChunk::getInprogressLOD(void) const
{
    return m_inprogressLOD;
}

void CLandscapeChunk::setInprogressLOD(E_LANDSCAPE_CHUNK_LOD LOD)
{
    m_inprogressLOD = LOD;
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
        m_numIndexesToRender = m_quadTree != nullptr ? m_quadTree->update(m_cameraFrustum) : m_mesh->getIndexBuffer()->getUsedSize();
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
        IGameObject::onBind(mode);
    }
}

void CLandscapeChunk::onDraw(const std::string& mode)
{
    if((m_status & E_LOADING_STATUS_TEMPLATE_LOADED) && m_numIndexesToRender != 0)
    {
        assert(m_mesh != nullptr);
        assert(m_camera != nullptr);
        assert(m_globalLightSource != nullptr);
        assert(m_materials.find(mode) != m_materials.end());
        
        m_mesh->draw(m_numIndexesToRender);
    }
}

void CLandscapeChunk::onUnbind(const std::string& mode)
{
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        IGameObject::onUnbind(mode);
    }
}

void CLandscapeChunk::onBatch(const std::string& mode)
{
   
}

void CLandscapeChunk::bindCustomShaderUniforms(CSharedMaterialRef material)
{
    IGameObject::bindCustomShaderUniforms(material);
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
    glm::mat4x4 matrixViewInverse = glm::inverse(m_camera->Get_ViewMatrix());
    material->getShader()->setMatrix4x4Custom(matrixViewInverse, "u_matrixViewInverse");
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

std::vector<SAttributeVertex> CLandscapeChunk::getSeamVertexes(E_LANDSCAPE_SEAM seam)
{
    std::vector<SAttributeVertex> seamVerteces;
    ui32 seamLength = m_mesh->getVertexBuffer()->getUsedSize() / 2;
    SAttributeVertex *vertexData = m_mesh->getVertexBuffer()->lock();
    switch (seam)
    {
        case E_LANDSCAPE_SEAM_TOP:
        {
            for(ui32 i = 0; i < seamLength; ++i)
            {
                seamVerteces.push_back(vertexData[i]);
            }
        }
            break;
            
        case E_LANDSCAPE_SEAM_BOTTOM:
        {
            for(ui32 i = 0; i < seamLength; ++i)
            {
                seamVerteces.push_back(vertexData[i + seamLength * (seamLength -1)]);
            }

        }
            break;

            
        case E_LANDSCAPE_SEAM_LEFT:
        {
            for(ui32 i = 0; i < seamLength; ++i)
            {
                seamVerteces.push_back(vertexData[i + i * (seamLength - 1)]);
            }
        }
            break;

            
        case E_LANDSCAPE_SEAM_RIGHT:
        {
            for(ui32 i = 0; i < seamLength; ++i)
            {
                seamVerteces.push_back(vertexData[(seamLength - 1) + i * (seamLength - 1)]);
            }
        }
            break;
            
        default:
            assert(false);
            break;
    }
    return seamVerteces;
}

