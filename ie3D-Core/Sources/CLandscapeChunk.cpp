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
#include "CBatchingMgr.h"
#include "CMesh.h"
#include "CQuadTree.h"
#include "CVertexBuffer.h"
#include "CIndexBuffer.h"
#include "CConfigurationAccessor.h"

CLandscapeChunk::CLandscapeChunk(CSharedResourceAccessorRef resourceAccessor,
                                 ISharedRenderTechniqueAccessorRef renderTechniqueAccessor) :
IGameObject(resourceAccessor, renderTechniqueAccessor),
m_numPassedIndexes(0),
m_quadTree(nullptr),
m_currentLOD(E_LANDSCAPE_CHUNK_LOD_UNKNOWN),
m_inprogressLOD(E_LANDSCAPE_CHUNK_LOD_UNKNOWN),
m_size(0)
{
    for(ui32 i = 0; i < E_LANDSCAPE_SEAM_MAX; i++)
    {
        m_seamedLOD.at(i) = E_LANDSCAPE_CHUNK_LOD_UNKNOWN;
    }
    m_isNeedBoundingBox = true;
    m_zOrder = E_GAME_OBJECT_Z_ORDER_LANDSCAPE;
}

CLandscapeChunk::~CLandscapeChunk(void)
{
    m_materialBindImposer = nullptr;
}

void CLandscapeChunk::setMesh(CSharedMeshRef mesh)
{
    assert(mesh != nullptr);
    m_mesh = mesh;
    m_size = sqrt(mesh->getVertexBuffer()->getUsedSize()); // chunk must be quad
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

E_LANDSCAPE_CHUNK_LOD CLandscapeChunk::getSeamedLOD(E_LANDSCAPE_SEAM seamType) const
{
    return m_seamedLOD.at(seamType);
}

void CLandscapeChunk::setInprogressLOD(E_LANDSCAPE_CHUNK_LOD LOD)
{
    m_inprogressLOD = LOD;
}

void CLandscapeChunk::onSceneUpdate(f32 deltatime)
{
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        IGameObject::onSceneUpdate(deltatime);
        m_numPassedIndexes = m_quadTree != nullptr ? m_quadTree->update(m_cameraFrustum) : m_mesh->getIndexBuffer()->getUsedSize();
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
    return 0;
}

void CLandscapeChunk::onBind(const std::string& mode)
{

}

void CLandscapeChunk::onDraw(const std::string& mode)
{

}

void CLandscapeChunk::onUnbind(const std::string& mode)
{

}

void CLandscapeChunk::onBatch(const std::string& mode)
{
   
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

std::vector<SAttributeVertex> CLandscapeChunk::getSeamVerteces(E_LANDSCAPE_SEAM seamType) const
{
    std::vector<SAttributeVertex> seamVerteces;
    SAttributeVertex *vertexData = m_mesh->getVertexBuffer()->lock();
    std::function<ui32(ui32)> incrementFunction = CLandscapeChunk::getEdgeVertexIncrementFunction(seamType);
    for(ui32 i = 0; i < m_size; ++i)
    {
        ui32 index = incrementFunction(i);
        seamVerteces.push_back(vertexData[index]);
    }
    return seamVerteces;
}

void CLandscapeChunk::setSeamVerteces(const std::vector<SAttributeVertex>& verteces, E_LANDSCAPE_SEAM seamType)
{
    ui32 neighborEdgeLength = static_cast<ui32>(verteces.size());
    
    assert(neighborEdgeLength > 0 && m_size > 0);
    assert(neighborEdgeLength <= m_size);
    assert((m_size - 1) % (neighborEdgeLength - 1) == 0);
    
    ui32 edgesLengthDeltaStep = (m_size - 1) / (neighborEdgeLength - 1);
    
    std::function<ui32(ui32)> incrementFunction = CLandscapeChunk::getEdgeVertexIncrementFunction(seamType);
    std::function<f32(const glm::vec3&, const glm::vec3&, const glm::vec3&)> interpolationIntensityFunction = CLandscapeChunk::getInterpolationIntensityFunctionToSewSeams(seamType);
    SAttributeVertex *vertexData = m_mesh->getVertexBuffer()->lock();
    
    i32 neighborEdgeVertexIndex = 0;
    for(ui32 i = 0; i < m_size; ++i)
    {
        i32 index = incrementFunction(i);
        neighborEdgeVertexIndex = i / edgesLengthDeltaStep;
        
        ui32 neighborVertexIndex_01 = neighborEdgeVertexIndex;
        ui32 neighborVertexIndex_02 = neighborEdgeVertexIndex + 1 <= neighborEdgeLength - 1 ? neighborEdgeVertexIndex + 1 : neighborEdgeVertexIndex;
        
        f32 interpolationIntensity = interpolationIntensityFunction(verteces.at(neighborVertexIndex_01).m_position,
                                                                    verteces.at(neighborVertexIndex_02).m_position,
                                                                    vertexData[index].m_position);
        vertexData[index].m_position.y = glm::mix(verteces.at(neighborVertexIndex_01).m_position.y,
                                                  verteces.at(neighborVertexIndex_02).m_position.y,
                                                  interpolationIntensity);
    }
    m_mesh->getVertexBuffer()->unlock();
    m_seamedLOD.at(seamType) = m_currentLOD;
}

std::function<ui32(ui32)> CLandscapeChunk::getEdgeVertexIncrementFunction(E_LANDSCAPE_SEAM seamType) const
{
    std::function<ui32(ui32)> incrementFunction = nullptr;
    switch (seamType)
    {
        case E_LANDSCAPE_SEAM_X_MINUS:
        {
            incrementFunction = [this](ui32 index)
            {
                return index;
            };
        }
            break;
            
        case E_LANDSCAPE_SEAM_X_PLUS:
        {
            incrementFunction = [this](ui32 index)
            {
                ui32 newIndex = index + m_size * (m_size - 1);
                assert(newIndex >= 0 || newIndex < m_size);
                return newIndex;
            };
        }
            break;
            
        case E_LANDSCAPE_SEAM_Z_MINUS:
        {
            incrementFunction = [this](ui32 index)
            {
                ui32 newIndex = index + index * (m_size - 1);
                assert(newIndex >= 0 || newIndex < m_size);
                return newIndex;
            };
        }
            break;
            
        case E_LANDSCAPE_SEAM_Z_PLUS:
        {
            incrementFunction = [this](ui32 index)
            {
                ui32 newIndex =  index + (index + 1) * (m_size - 1);
                assert(newIndex >= 0 || newIndex < m_size);
                return newIndex;
            };
        }
            break;
            
        default:
            assert(false);
            break;
    }
    return incrementFunction;
}

std::function<f32(const glm::vec3&, const glm::vec3&, const glm::vec3&)> CLandscapeChunk::getInterpolationIntensityFunctionToSewSeams(E_LANDSCAPE_SEAM seamType)
{
    std::function<f32(const glm::vec3&, const glm::vec3&, const glm::vec3&)> interpolationIntensityFunction = nullptr;
    switch (seamType)
    {
        case E_LANDSCAPE_SEAM_X_MINUS:
        case E_LANDSCAPE_SEAM_X_PLUS:
        {
            interpolationIntensityFunction = [this](const glm::vec3& neighborVertexPosition_01, const glm::vec3& neighborVertexPosition_02, const glm::vec3& currentVertexPosition)
            {
                f32 distanceBetweenNeighborVertices = neighborVertexPosition_02.z - neighborVertexPosition_01.z;
                f32 distanceBetweenNeigborAndCurrentVertex = currentVertexPosition.z - neighborVertexPosition_01.z;
                assert(distanceBetweenNeighborVertices >= distanceBetweenNeigborAndCurrentVertex);
                assert(distanceBetweenNeighborVertices >= 0.0f);
                f32 interpolationIntensity = distanceBetweenNeighborVertices != 0.0f ? distanceBetweenNeigborAndCurrentVertex / distanceBetweenNeighborVertices : 0.0f;
                assert(interpolationIntensity >= 0.0f && interpolationIntensity <= 1.0f);
                return interpolationIntensity;
            };
        }
            break;
            
        case E_LANDSCAPE_SEAM_Z_MINUS:
        case E_LANDSCAPE_SEAM_Z_PLUS:
        {
            interpolationIntensityFunction = [this](const glm::vec3& neighborVertexPosition_01, const glm::vec3& neighborVertexPosition_02, const glm::vec3& currentVertexPosition)
            {
                f32 distanceBetweenNeighborVertices = neighborVertexPosition_02.x - neighborVertexPosition_01.x;
                f32 distanceBetweenNeigborAndCurrentVertex = currentVertexPosition.x - neighborVertexPosition_01.x;
                assert(distanceBetweenNeighborVertices >= distanceBetweenNeigborAndCurrentVertex);
                assert(distanceBetweenNeighborVertices >= 0.0f);
                f32 interpolationIntensity = distanceBetweenNeighborVertices != 0.0f ? distanceBetweenNeigborAndCurrentVertex / distanceBetweenNeighborVertices : 0.0f;
                assert(interpolationIntensity >= 0.0f && interpolationIntensity <= 1.0f);
                return interpolationIntensity;
            };
        }
            break;
            
        default:
            assert(false);
            break;
    }
    return interpolationIntensityFunction;
}

void CLandscapeChunk::resetSeams(void)
{
    for(ui32 i = 0; i < E_LANDSCAPE_SEAM_MAX; i++)
    {
        m_seamedLOD.at(i) = E_LANDSCAPE_CHUNK_LOD_UNKNOWN;
    }
}
