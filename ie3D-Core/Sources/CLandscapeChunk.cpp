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
    for(ui32 i = 0; i < m_seams.size(); ++i)
    {
        m_seams.at(i) = nullptr;
    }
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

std::vector<SAttributeVertex> CLandscapeChunk::getSeamVerteces(E_LANDSCAPE_SEAM type) const
{
    std::vector<SAttributeVertex> seamVerteces;
    ui32 seamLength = sqrt(m_mesh->getVertexBuffer()->getUsedSize());
    SAttributeVertex *vertexData = m_mesh->getVertexBuffer()->lock();
    switch (type)
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
                seamVerteces.push_back(vertexData[i + seamLength * (seamLength - 1)]);
            }
        }
            break;
            
        case E_LANDSCAPE_SEAM_LEFT:
        {
            for(i32 i = 0; i < seamLength; ++i)
            {
                i32 index = i + i * (seamLength - 1);
                assert(index >= 0 || index < seamLength);
                seamVerteces.push_back(vertexData[index]);
                //std::cout<<"left seam vertex: "<<vertexData[index].m_position.x<<","<<vertexData[index].m_position.z<<std::endl;
            }
        }
            break;
            
        case E_LANDSCAPE_SEAM_RIGHT:
        {
            for(i32 i = 0; i < seamLength; ++i)
            {
                i32 index = i + (i + 1) * (seamLength - 1);
                assert(index >= 0 || index < seamLength);
                seamVerteces.push_back(vertexData[index]);
                //std::cout<<"right seam vertex: "<<vertexData[index].m_position.x<<","<<vertexData[index].m_position.z<<std::endl;
            }
        }
            break;
            
        default:
            assert(false);
            break;
    }
    return seamVerteces;
}

void CLandscapeChunk::setSeamVerteces(const std::vector<SAttributeVertex>& verteces, E_LANDSCAPE_SEAM type)
{
    ui32 currentEdgeLength = sqrt(m_mesh->getVertexBuffer()->getUsedSize());
    ui32 neighborEdgeLength = static_cast<ui32>(verteces.size());
    
    assert(neighborEdgeLength > 0 && currentEdgeLength > 0);
    assert(neighborEdgeLength < currentEdgeLength);
    assert((currentEdgeLength - 1) % (neighborEdgeLength - 1) == 0);
    
    ui32 edgesLengthDeltaStep = (currentEdgeLength - 1) / (neighborEdgeLength - 1);
    std::cout<<"setSeamVerteces with Type: "<<type<<std::endl;
    
    SAttributeVertex *vertexData = m_mesh->getVertexBuffer()->lock();
    
    switch (type)
    {
        case E_LANDSCAPE_SEAM_TOP:
        {
            for(ui32 i = 0; i < currentEdgeLength; ++i)
            {
                vertexData[i].m_position.y = 0.0;
            }
        }
            break;
            
        case E_LANDSCAPE_SEAM_BOTTOM:
        {
            ui32 neighborEdgeVertexIndex = 0;
            ui32 edgesLengthDeltaStep = 0;
            for(ui32 i = 0; i < currentEdgeLength; ++i)
            {
                i32 index = i + currentEdgeLength * (currentEdgeLength - 1);
                assert(index >= 0 || index < currentEdgeLength);
                
                /*if(edgesLengthDeltaStep == edgesLengthDelta || edgesLengthDeltaStep == 0)
                {
                    vertexData[index].m_position.y = verteces.at(neighborEdgeVertexIndex).m_position.y;
                }
                
                edgesLengthDeltaStep++;
                if(edgesLengthDeltaStep == edgesLengthDelta)
                {
                    neighborEdgeVertexIndex++;
                }*/
            }
        }
            break;
            
        case E_LANDSCAPE_SEAM_LEFT:
        {
            ui32 neighborEdgeVertexIndex = 0;
            ui32 edgesLengthDeltaStep = 0;
            for(ui32 i = 0; i < currentEdgeLength; ++i)
            {
                i32 index = i + i * (currentEdgeLength - 1);
                assert(index >= 0 || index < currentEdgeLength);
                
                //if(edgesLengthDeltaStep == edgesLengthDelta || edgesLengthDeltaStep == 0)
                {
                    vertexData[index].m_position.y = 0.0;//verteces.at(neighborEdgeVertexIndex).m_position.y;
                }
                
                //edgesLengthDeltaStep++;
                //if(edgesLengthDeltaStep == edgesLengthDelta)
                //{
                //    neighborEdgeVertexIndex++;
               // }
            }
        }
            break;
            
        case E_LANDSCAPE_SEAM_RIGHT:
        {
            i32 neighborEdgeVertexIndex = 0;
            for(ui32 i = 0; i < currentEdgeLength; ++i)
            {
                i32 index = i + (i + 1) * (currentEdgeLength - 1);
                assert(index >= 0 || index < currentEdgeLength);
                
                neighborEdgeVertexIndex = i / edgesLengthDeltaStep;
                
                glm::vec3 neighborVertexPosition_01 = verteces.at(neighborEdgeVertexIndex).m_position;
                glm::vec3 neighborVertexPosition_02 = verteces.at(neighborEdgeVertexIndex + 1 < neighborEdgeLength - 1 ? neighborEdgeVertexIndex + 1 : neighborEdgeVertexIndex).m_position;
                
                f32 distanceBetweenNeighborVertices = neighborVertexPosition_02.x - neighborVertexPosition_01.x;
                f32 distanceBetweenNeigborAndCurrentVertex = vertexData[index].m_position.x - neighborVertexPosition_01.x;
                assert(distanceBetweenNeighborVertices >= 0.0f);
                f32 interpolationIntensity = distanceBetweenNeighborVertices != 0.0f ? distanceBetweenNeigborAndCurrentVertex / distanceBetweenNeighborVertices : 0.0f;
                assert(interpolationIntensity >= 0.0f && interpolationIntensity <= 1.0f);
                vertexData[index].m_position.y = glm::mix(neighborVertexPosition_01.y, neighborVertexPosition_02.y, interpolationIntensity);
            }
        }
            break;
            
        default:
            assert(false);
            break;
    }
    m_mesh->getVertexBuffer()->unlock();
}

CSharedLandscapeSeam CLandscapeChunk::getSeam(E_LANDSCAPE_SEAM type) const
{
    return m_seams.at(type);
}

void CLandscapeChunk::setSeam(CSharedLandscapeSeamRef seam, E_LANDSCAPE_SEAM type)
{
    m_seams.at(type) = seam;
}

bool CLandscapeChunk::isMeshExist(void) const
{
    return m_mesh != nullptr;
}

