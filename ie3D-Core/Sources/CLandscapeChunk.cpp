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
#include "CMesh.h"
#include "CQuadTree.h"
#include "CVertexBuffer.h"
#include "CIndexBuffer.h"
#include "CConfigurationAccessor.h"
#include "CSceneUpdateMgr.h"

CLandscapeChunk::CLandscapeChunk(CSharedResourceAccessorRef resourceAccessor,
                                 ISharedRenderTechniqueAccessorRef renderTechniqueAccessor) :
IGameObject(resourceAccessor, renderTechniqueAccessor),
m_numPassedIndexes(0),
m_quadTree(nullptr),
m_currentLOD(E_LANDSCAPE_CHUNK_LOD_UNKNOWN),
m_inprogressLOD(E_LANDSCAPE_CHUNK_LOD_UNKNOWN),
m_size(0),
m_preprocessedSplattingDTexture(nullptr),
m_preprocessedSplattingNTexture(nullptr)
{

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

void CLandscapeChunk::setInprogressLOD(E_LANDSCAPE_CHUNK_LOD LOD)
{
    m_inprogressLOD = LOD;
}

void CLandscapeChunk::onSceneUpdate(f32 deltatime)
{
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        IGameObject::onSceneUpdate(deltatime);
        m_numPassedIndexes = m_quadTree != nullptr ? m_quadTree->update(m_cameraFrustum, IGameObject::getMMatrix()) : m_mesh->getIndexBuffer()->getUsedSize();
    }
}

void CLandscapeChunk::onResourceLoaded(ISharedResourceRef resource, bool success)
{
    IGameObject::onResourceLoaded(resource, success);
}

void CLandscapeChunk::onConfigurationLoaded(ISharedConfigurationRef configuration, bool success)
{
    if(m_sceneUpdateMgr)
    {
        m_sceneUpdateMgr->RegisterSceneUpdateHandler(shared_from_this());
    }
    m_status |= E_LOADING_STATUS_TEMPLATE_LOADED;
}

void CLandscapeChunk::setPreprocessedSplattingDTexture(CSharedTextureRef texture)
{
    m_preprocessedSplattingDTexture = texture;
}

const CSharedTexture CLandscapeChunk::getPreprocessedSplattingDTexture(void) const
{
    return m_preprocessedSplattingDTexture;
}

const CSharedTexture  CLandscapeChunk::getPreprocessedSplattingNTexture(void) const
{
    return m_preprocessedSplattingNTexture;
}

void CLandscapeChunk::setPreprocessedSplattingNTexture(CSharedTextureRef texture)
{
    m_preprocessedSplattingNTexture = texture;
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
