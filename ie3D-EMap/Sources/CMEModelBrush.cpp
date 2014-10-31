//
//  CMEModelBrush.cpp
//  ie3D-EMap
//
//  Created by sergey.sergeev on 7/10/14.
//
//

#include "CMEModelBrush.h"
#include "CMaterial.h"
#include "CShader.h"
#include "CMesh.h"
#include "CCamera.h"
#include "CVertexBuffer.h"
#include "CIndexBuffer.h"
#include "CECustomModel.h"

CMEModelBrush::CMEModelBrush(CSharedResourceAccessorRef resourceAccessor,
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

CMEModelBrush::~CMEModelBrush(void)
{
    
}

void CMEModelBrush::onSceneUpdate(f32)
{
    
}

void CMEModelBrush::onResourceLoaded(ISharedResourceRef, bool)
{

}

void CMEModelBrush::onConfigurationLoaded(ISharedConfigurationRef configuration, bool success)
{
    IGameObject::onConfigurationLoaded(configuration, success);
    
    m_arrows.at(E_PLACEMENT_MODEL_ARROW_X) = CMEModelBrush::createArrowModel(E_PLACEMENT_MODEL_ARROW_X);
    m_arrows.at(E_PLACEMENT_MODEL_ARROW_Y) = CMEModelBrush::createArrowModel(E_PLACEMENT_MODEL_ARROW_Y);
    m_arrows.at(E_PLACEMENT_MODEL_ARROW_Z) = CMEModelBrush::createArrowModel(E_PLACEMENT_MODEL_ARROW_Z);

    IGameObject::enableRender(m_isNeedToRender);
    IGameObject::enableUpdate(m_isNeedToUpdate);
    
    m_status |= E_LOADING_STATUS_TEMPLATE_LOADED;
}

CESharedCustomModel CMEModelBrush::createArrowModel(E_PLACEMENT_MODEL_ARROW arrow)
{
    glm::vec3 maxBound = glm::vec3(0.0);
    glm::vec3 minBound = glm::vec3(0.0);
    glm::u8vec4 color = glm::u8vec4(255);
    switch (arrow)
    {
        case E_PLACEMENT_MODEL_ARROW_X:
        {
            maxBound = glm::vec3(12.0, 1.0, 1.0);
            minBound = glm::vec3(1.0, 0.0, 0.0);
            color = glm::vec4(255, 0, 0, 255);
        }
            break;
        case E_PLACEMENT_MODEL_ARROW_Y:
        {
            maxBound = glm::vec3(1.0, 12.0, 1.0);
            minBound = glm::vec3(0.0, 1.0, 0.0);
            color = glm::vec4(0, 255, 0, 255);
        }
            break;
        case E_PLACEMENT_MODEL_ARROW_Z:
        {
            maxBound = glm::vec3(1.0, 1.0, 12.0);
            minBound = glm::vec3(0.0, 0.0, 1.0);
            color = glm::vec4(0, 0, 255, 255);
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
    
    for(ui32 i = 0; i < 8; ++i)
    {
        vertexData[i].m_color = color;
    }
    
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
    
    CSharedMesh arrowMesh = CMesh::constructCustomMesh("arrow", vertexBuffer, indexBuffer,
                                                       glm::vec3(4096.0), glm::vec3(-4096.0));
    assert(arrowMesh != nullptr);
    
    CESharedCustomModel arrowModel = std::make_shared<CECustomModel>(m_resourceAccessor, m_renderTechniqueAccessor);
    arrowModel->setCamera(m_camera);
    arrowModel->setGlobalLightSource(m_globalLightSource);
    arrowModel->setMesh(arrowMesh);

    arrowModel->setRenderTechniqueImporter(m_renderTechniqueImporter);
    arrowModel->setRenderTechniqueAccessor(m_renderTechniqueAccessor);
    arrowModel->setSceneUpdateMgr(m_sceneUpdateMgr);
    
    arrowModel->enableRender(m_isNeedToRender);
    arrowModel->enableUpdate(m_isNeedToUpdate);
    
    ISharedConfigurationLoadingHandler handler = std::static_pointer_cast<IConfigurationLoadingHandler>(arrowModel);
    handler->onConfigurationLoaded(m_configuration, true);
    
    return arrowModel;
}

i32 CMEModelBrush::zOrder(void)
{
    return -1;
}

bool CMEModelBrush::checkOcclusion(void)
{
    return false;
}

ui32 CMEModelBrush::numTriangles(void)
{
    return 0;
}

void CMEModelBrush::onBind(const std::string&)
{

}

void CMEModelBrush::onDraw(const std::string&)
{

}

void CMEModelBrush::onUnbind(const std::string&)
{

}

void CMEModelBrush::onBatch(const std::string&)
{

}

void CMEModelBrush::setLandscape(CSharedLandscapeRef landscape)
{
    assert(landscape != nullptr);
    m_landscape = landscape;
}

void CMEModelBrush::setPosition(const glm::vec3 &position)
{
    IGameObject::setPosition(position);
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        for(const auto& iterator : m_arrows)
        {
            assert(iterator != nullptr);
            iterator->setPosition(position);
        }
    }
}

void CMEModelBrush::setRotation(const glm::vec3 &rotation)
{
    IGameObject::setRotation(rotation);
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        for(const auto& iterator : m_arrows)
        {
            assert(iterator != nullptr);
            iterator->setRotation(rotation);
        }
    }
}

void CMEModelBrush::setScale(const glm::vec3& scale)
{
    IGameObject::setScale(scale);
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        for(const auto& iterator : m_arrows)
        {
            assert(iterator != nullptr);
            iterator->setScale(scale);
        }
    }
}
void CMEModelBrush::setCamera(CSharedCameraRef camera)
{
    IGameObject::setCamera(camera);
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        for(const auto& iterator : m_arrows)
        {
            assert(iterator != nullptr);
            iterator->setCamera(camera);
        }
    }
}

void CMEModelBrush::setGlobalLightSource(CSharedGlobalLightSourceRef lightSource)
{
    IGameObject::setGlobalLightSource(lightSource);
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        for(const auto& iterator : m_arrows)
        {
            assert(iterator != nullptr);
            iterator->setGlobalLightSource(lightSource);
        }
    }
}

void CMEModelBrush::setRenderTechniqueImporter(ISharedRenderTechniqueImporterRef techniqueImporter)
{
    IGameObject::setRenderTechniqueImporter(techniqueImporter);
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        for(const auto& iterator : m_arrows)
        {
            assert(iterator != nullptr);
            iterator->setRenderTechniqueImporter(techniqueImporter);
        }
    }
}

void CMEModelBrush::setRenderTechniqueAccessor(ISharedRenderTechniqueAccessorRef techniqueAccessor)
{
    IGameObject::setRenderTechniqueAccessor(techniqueAccessor);
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        for(const auto& iterator : m_arrows)
        {
            assert(iterator != nullptr);
            iterator->setRenderTechniqueAccessor(techniqueAccessor);
        }
    }
}

void CMEModelBrush::setSceneUpdateMgr(CSharedSceneUpdateMgrRef sceneUpdateMgr)
{
    IGameObject::setSceneUpdateMgr(sceneUpdateMgr);
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        for(const auto& iterator : m_arrows)
        {
            assert(iterator != nullptr);
            iterator->setSceneUpdateMgr(sceneUpdateMgr);
        }
    }
}

void CMEModelBrush::enableRender(bool value)
{
    IGameObject::enableRender(value);
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        for(const auto& iterator : m_arrows)
        {
            assert(iterator != nullptr);
            iterator->enableRender(value);
        }
    }
}

void CMEModelBrush::enableUpdate(bool value)
{
    IGameObject::enableUpdate(value);
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        for(const auto& iterator : m_arrows)
        {
            assert(iterator != nullptr);
            iterator->enableUpdate(value);
        }
    }
}
