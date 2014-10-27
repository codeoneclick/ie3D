//
//  CMEPlacementModel.cpp
//  ie3D-EMap
//
//  Created by sergey.sergeev on 7/10/14.
//
//

#include "CMEPlacementModel.h"
#include "CMaterial.h"
#include "CShader.h"
#include "CMesh.h"
#include "CCamera.h"
#include "CVertexBuffer.h"
#include "CIndexBuffer.h"

CMEPlacementModel::CMEPlacementModel(CSharedResourceAccessorRef resourceAccessor,
                                     ISharedRenderTechniqueAccessorRef renderTechniqueAccessor) :
IGameObject(resourceAccessor, renderTechniqueAccessor),
m_landscape(nullptr),
m_model(nullptr)
{
    m_zOrder = 7;
    
    m_arrows.at(E_PLACEMENT_MODEL_ARROW_X) = nullptr;
    m_arrows.at(E_PLACEMENT_MODEL_ARROW_Y) = nullptr;
    m_arrows.at(E_PLACEMENT_MODEL_ARROW_Z) = nullptr;
}

CMEPlacementModel::~CMEPlacementModel(void)
{
    
}

void CMEPlacementModel::onSceneUpdate(f32 deltatime)
{
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        IGameObject::onSceneUpdate(deltatime);
    }
}

void CMEPlacementModel::onResourceLoaded(ISharedResourceRef resource, bool success)
{
    IGameObject::onResourceLoaded(resource, success);
}

void CMEPlacementModel::onConfigurationLoaded(ISharedConfigurationRef configuration, bool success)
{
    IGameObject::onConfigurationLoaded(configuration, success);
    
    m_arrows.at(E_PLACEMENT_MODEL_ARROW_X) = CMEPlacementModel::createArrowMesh(E_PLACEMENT_MODEL_ARROW_X);
    m_arrows.at(E_PLACEMENT_MODEL_ARROW_Y) = CMEPlacementModel::createArrowMesh(E_PLACEMENT_MODEL_ARROW_Y);
    m_arrows.at(E_PLACEMENT_MODEL_ARROW_Z) = CMEPlacementModel::createArrowMesh(E_PLACEMENT_MODEL_ARROW_Z);

    IGameObject::enableRender(m_isNeedToRender);
    IGameObject::enableUpdate(m_isNeedToUpdate);
    
    m_status |= E_LOADING_STATUS_TEMPLATE_LOADED;
}

CSharedMesh CMEPlacementModel::createArrowMesh(E_PLACEMENT_MODEL_ARROW arrow)
{
    glm::vec3 maxBound = glm::vec3(0.0);
    glm::vec3 minBound = glm::vec3(0.0);
    
    switch (arrow)
    {
        case E_PLACEMENT_MODEL_ARROW_X:
        {
            maxBound = glm::vec3(8.0, 2.0, 2.0);
        }
            break;
        case E_PLACEMENT_MODEL_ARROW_Y:
        {
            maxBound = glm::vec3(2.0, 8.0, 2.0);
        }
            break;
        case E_PLACEMENT_MODEL_ARROW_Z:
        {
            maxBound = glm::vec3(2.0, 2.0, 8.0);
        }
            break;
            
        default:
        {
            assert(false);
        }
            break;
    }
    
    CSharedVertexBuffer vertexBuffer = std::make_shared<CVertexBuffer>(8, GL_STATIC_DRAW);
    SAttributeVertex* vertexData = vertexBuffer->lock();
    
    vertexData[0].m_position = glm::vec3(minBound.x, minBound.y, maxBound.z);
    vertexData[1].m_position = glm::vec3(maxBound.x, minBound.y, maxBound.z);
    vertexData[2].m_position = glm::vec3(maxBound.x, maxBound.y, maxBound.z);
    vertexData[3].m_position = glm::vec3(minBound.x, maxBound.y, maxBound.z);
    
    vertexData[4].m_position = glm::vec3(minBound.x, minBound.y, minBound.z);
    vertexData[5].m_position = glm::vec3(maxBound.x, minBound.y, minBound.z);
    vertexData[6].m_position = glm::vec3(maxBound.x, maxBound.y, minBound.z);
    vertexData[7].m_position = glm::vec3(minBound.x, maxBound.y, minBound.z);
    
    vertexBuffer->unlock();
    
    CSharedIndexBuffer indexBuffer = std::make_shared<CIndexBuffer>(36, GL_STATIC_DRAW);
    ui16* indexData = indexBuffer->lock();
    
    indexData[0] = 0;
    indexData[1] = 1;
    indexData[2] = 2;
    indexData[3] = 2;
    indexData[4] = 3;
    indexData[5] = 0;
    
    indexData[6] = 3;
    indexData[7] = 2;
    indexData[8] = 6;
    indexData[9] = 6;
    indexData[10] = 7;
    indexData[11] = 3;
    
    indexData[12] = 7;
    indexData[13] = 6;
    indexData[14] = 5;
    indexData[15] = 5;
    indexData[16] = 4;
    indexData[17] = 7;
    
    indexData[18] = 4;
    indexData[19] = 5;
    indexData[20] = 1;
    indexData[21] = 1;
    indexData[22] = 0;
    indexData[23] = 4;
    
    indexData[24] = 4;
    indexData[25] = 0;
    indexData[26] = 3;
    indexData[27] = 3;
    indexData[28] = 7;
    indexData[29] = 4;
    
    indexData[30] = 1;
    indexData[31] = 5;
    indexData[32] = 6;
    indexData[33] = 6;
    indexData[34] = 2;
    indexData[35] = 1;
    
    indexBuffer->unlock();
    
    m_mesh = CMesh::constructCustomMesh("arrow", vertexBuffer, indexBuffer,
                                        glm::vec3(4096.0), glm::vec3(4096.0));
    assert(m_mesh != nullptr);
    return m_mesh;
}

i32 CMEPlacementModel::zOrder(void)
{
    return m_zOrder;
}

bool CMEPlacementModel::checkOcclusion(void)
{
    return false;
}

ui32 CMEPlacementModel::numTriangles(void)
{
    return IGameObject::numTriangles();
}

void CMEPlacementModel::onBind(const std::string& mode)
{
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        assert(m_materials.find(mode) != m_materials.end());
        IGameObject::onBind(mode);
    }
}

void CMEPlacementModel::onDraw(const std::string& mode)
{
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        assert(m_camera != nullptr);
        assert(m_materials.find(mode) != m_materials.end());
        
        std::shared_ptr<CMaterial> material = m_materials.find(mode)->second;
        assert(material->getShader() != nullptr);
        
        material->getShader()->setMatrix4x4(m_matrixWorld, E_SHADER_UNIFORM_MATRIX_WORLD);
        material->getShader()->setMatrix4x4(m_camera->Get_ProjectionMatrix(), E_SHADER_UNIFORM_MATRIX_PROJECTION);
        material->getShader()->setMatrix4x4(m_camera->Get_ViewMatrix(), E_SHADER_UNIFORM_MATRIX_VIEW);
        material->getShader()->setMatrix4x4(m_camera->Get_MatrixNormal(), E_SHADER_UNIFORM_MATRIX_NORMAL);

        IGameObject::onDraw(mode);
    }
}

void CMEPlacementModel::onUnbind(const std::string& mode)
{
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        assert(m_materials.find(mode) != m_materials.end());
        IGameObject::onUnbind(mode);
    }
}

void CMEPlacementModel::onBatch(const std::string& mode)
{
    IGameObject::onBatch(mode);
}

void CMEPlacementModel::setLandscape(CSharedLandscapeRef landscape)
{
    assert(landscape != nullptr);
    m_landscape = landscape;
}