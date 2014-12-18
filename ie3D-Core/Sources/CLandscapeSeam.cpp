//
//  CLandscapeSeam.cpp
//  ie3D-Core
//
//  Created by sergey.sergeev on 10/17/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#include "CLandscapeSeam.h"
#include "CMesh.h"
#include "CVertexBuffer.h"
#include "CIndexBuffer.h"

CLandscapeSeam::CLandscapeSeam(CSharedResourceAccessorRef resourceAccessor,
                                 ISharedRenderTechniqueAccessorRef renderTechniqueAccessor) :
IGameObject(resourceAccessor, renderTechniqueAccessor)
{
    m_isNeedBoundingBox = false;
    m_zOrder = E_GAME_OBJECT_Z_ORDER_LANDSCAPE_SEAM;
}

CLandscapeSeam::~CLandscapeSeam(void)
{
    
}

void CLandscapeSeam::onSceneUpdate(f32 deltatime)
{
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        IGameObject::onSceneUpdate(deltatime);
    }
}

void CLandscapeSeam::onResourceLoaded(ISharedResourceRef resource, bool success)
{
    IGameObject::onResourceLoaded(resource, success);
}

void CLandscapeSeam::onConfigurationLoaded(ISharedConfigurationRef configuration, bool success)
{
    IGameObject::onConfigurationLoaded(configuration, success);
    
    m_seamMaxLength = 32;
    
    CSharedVertexBuffer vertexBuffer =std::make_shared<CVertexBuffer>(m_seamMaxLength * 2, GL_STATIC_DRAW);
    
    CSharedIndexBuffer indexBuffer = std::make_shared<CIndexBuffer>((m_seamMaxLength - 1) * 6, GL_STATIC_DRAW);
    
    m_mesh = CMesh::constructCustomMesh("landscape.seam", vertexBuffer, indexBuffer,
                                        glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    
    IGameObject::enableRender(m_isNeedToRender);
    IGameObject::enableUpdate(m_isNeedToUpdate);
    
    m_status |= E_LOADING_STATUS_TEMPLATE_LOADED;
}

i32  CLandscapeSeam::zOrder(void)
{
    return m_zOrder;
}

bool CLandscapeSeam::checkOcclusion(void)
{
    return IGameObject::checkOcclusion();
}

ui32 CLandscapeSeam::numTriangles(void)
{
    return IGameObject::numTriangles();
}

void CLandscapeSeam::onBind(const std::string& renderTechnique)
{
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        IGameObject::onBind(renderTechnique);
    }
}

void CLandscapeSeam::onDraw(const std::string& renderTechnique)
{
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        IGameObject::onDraw(renderTechnique);
    }
}

void CLandscapeSeam::onUnbind(const std::string& renderTechnique)
{
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        IGameObject::onUnbind(renderTechnique);
    }
}

void CLandscapeSeam::onBatch(const std::string& renderTechnique)
{
    
}

void CLandscapeSeam::onOcclusionQueryDraw(CSharedMaterialRef material)
{
    
}

void CLandscapeSeam::onOcclusionQueryUpdate(void)
{
    
}

void CLandscapeSeam::setVertexesToSewTogether(const std::vector<SAttributeVertex>& edge_01,
                                              const std::vector<SAttributeVertex>& edge_02)
{
    m_edge_01 = edge_01;
    m_edge_02 = edge_02;
    
    assert(m_edge_01.size() <= m_seamMaxLength);
    assert(m_edge_02.size() <= m_seamMaxLength);
    
    const std::vector<SAttributeVertex>& maxLODEdge = m_edge_01.size() >= m_edge_02.size() ? m_edge_01 : m_edge_02;
    const std::vector<SAttributeVertex>& minLODEdge = m_edge_01.size() < m_edge_02.size() ? m_edge_01 : m_edge_02;
    
    ui32 index = 0;
    SAttributeVertex *vertexData = m_mesh->getVertexBuffer()->lock();
    
    for(ui32 i = 0; i < maxLODEdge.size(); ++i)
    {
        vertexData[index] = maxLODEdge.at(i);
        ++index;
    }
    
    for(ui32 i = 0; i < minLODEdge.size(); ++i)
    {
        vertexData[index] = minLODEdge.at(i);
        ++index;
    }
}
