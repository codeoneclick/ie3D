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
#include "CMEConfigurationAccessor.h"

CMEModelBrush::CMEModelBrush(CSharedResourceAccessorRef resourceAccessor,
                             ISharedRenderTechniqueAccessorRef renderTechniqueAccessor) :
IGameObject(resourceAccessor, renderTechniqueAccessor),
m_landscape(nullptr),
m_model(nullptr)
{
    m_arrows.at(E_MODEL_BRUSH_ARROW_X) = nullptr;
    m_arrows.at(E_MODEL_BRUSH_ARROW_Y) = nullptr;
    m_arrows.at(E_MODEL_BRUSH_ARROW_Z) = nullptr;
    
    m_planes.at(E_MODEL_BRUSH_PLANE_X) = nullptr;
    m_planes.at(E_MODEL_BRUSH_PLANE_Y) = nullptr;
    m_planes.at(E_MODEL_BRUSH_PLANE_Z) = nullptr;
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

void CMEModelBrush::onConfigurationLoaded(ISharedConfigurationRef configuration, bool)
{
    CSharedMEConfigurationModelBrush configurationModelBrush = std::static_pointer_cast<CMEConfigurationModelBrush>(configuration);
    
    for(ui32 i = 0; i < configurationModelBrush->getElementsConfigurations().size(); ++i)
    {
        CSharedMEConfigurationBrushElement configurationBrushElement = std::static_pointer_cast<CMEConfigurationBrushElement>(configurationModelBrush->getElementsConfigurations().at(i));
        std::string name = configurationBrushElement->getName();
        if(name == "arrowX")
        {
             m_arrows.at(E_MODEL_BRUSH_ARROW_X) = CMEModelBrush::createArrowModel(E_MODEL_BRUSH_ARROW_X,
                                                                                  configuration);
        }
        else if(name == "arrowY")
        {
            m_arrows.at(E_MODEL_BRUSH_ARROW_Y) = CMEModelBrush::createArrowModel(E_MODEL_BRUSH_ARROW_Y,
                                                                                 configuration);
        }
        else if(name == "arrowZ")
        {
            m_arrows.at(E_MODEL_BRUSH_ARROW_Z) = CMEModelBrush::createArrowModel(E_MODEL_BRUSH_ARROW_Z,
                                                                                 configuration);
        }
        else if(name == "planeX")
        {
            m_planes.at(E_MODEL_BRUSH_PLANE_X) = CMEModelBrush::createPlaneModel(E_MODEL_BRUSH_PLANE_X,
                                                                                 configuration);
        }
        else if(name == "planeY")
        {
            m_planes.at(E_MODEL_BRUSH_PLANE_Y) = CMEModelBrush::createPlaneModel(E_MODEL_BRUSH_PLANE_Y,
                                                                                 configuration);
        }
        else if(name == "planeZ")
        {
            m_planes.at(E_MODEL_BRUSH_PLANE_Z) = CMEModelBrush::createPlaneModel(E_MODEL_BRUSH_PLANE_Z,
                                                                                 configuration);
        }
        else
        {
            assert(false);
        }
    }
    
    m_status |= E_LOADING_STATUS_TEMPLATE_LOADED;
}

CESharedCustomModel CMEModelBrush::createArrowModel(E_MODEL_BRUSH_ARROW arrow, ISharedConfigurationRef configuration)
{
    glm::vec3 maxBound = glm::vec3(0.0);
    glm::vec3 minBound = glm::vec3(0.0);
    glm::u8vec4 color = glm::u8vec4(255);
    switch (arrow)
    {
        case E_MODEL_BRUSH_ARROW_X:
        {
            maxBound = glm::vec3(12.0, 1.0, 1.0);
            minBound = glm::vec3(1.0, 0.0, 0.0);
            color = glm::vec4(255, 0, 0, 255);
        }
            break;
        case E_MODEL_BRUSH_ARROW_Y:
        {
            maxBound = glm::vec3(1.0, 12.0, 1.0);
            minBound = glm::vec3(0.0, 0.0, 0.0);
            color = glm::vec4(0, 255, 0, 255);
        }
            break;
        case E_MODEL_BRUSH_ARROW_Z:
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
    arrowModel->setCameraFrustum(m_cameraFrustum);
    arrowModel->setGlobalLightSource(m_globalLightSource);
    arrowModel->setMesh(arrowMesh);
    
    if(m_renderTechniqueImporter && m_sceneUpdateMgr)
    {
        arrowModel->onAddedToScene(m_renderTechniqueImporter,
                                   m_sceneUpdateMgr);
    }
    ISharedConfigurationLoadingHandler handler = std::static_pointer_cast<IConfigurationLoadingHandler>(arrowModel);
    handler->onConfigurationLoaded(configuration, true);
    
    return arrowModel;
}

CESharedCustomModel CMEModelBrush::createPlaneModel(E_MODEL_BRUSH_PLANE plane, ISharedConfigurationRef configuration)
{
    glm::vec3 maxBound = glm::vec3(0.0);
    glm::vec3 minBound = glm::vec3(0.0);
    glm::u8vec4 color = glm::u8vec4(255, 255, 0, 192);
    switch (plane)
    {
        case E_MODEL_BRUSH_PLANE_X:
        {
            maxBound = glm::vec3(8.0, 8.0, 0.75);
            minBound = glm::vec3(1.0, 1.0, 0.25);
        }
            break;
        case E_MODEL_BRUSH_PLANE_Y:
        {
            maxBound = glm::vec3(0.75, 8.0, 8.0);
            minBound = glm::vec3(0.25, 1.0, 1.0);
        }
            break;
        case E_MODEL_BRUSH_PLANE_Z:
        {
            maxBound = glm::vec3(8.0, 0.75, 8.0);
            minBound = glm::vec3(1.0, 0.25, 1.0);
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
    
    CSharedMesh planeMesh = CMesh::constructCustomMesh("plane", vertexBuffer, indexBuffer,
                                                       glm::vec3(4096.0), glm::vec3(-4096.0));
    assert(planeMesh != nullptr);
    
    CESharedCustomModel planeModel = std::make_shared<CECustomModel>(m_resourceAccessor, m_renderTechniqueAccessor);
    planeModel->setCamera(m_camera);
    planeModel->setCameraFrustum(m_cameraFrustum);
    planeModel->setGlobalLightSource(m_globalLightSource);
    planeModel->setMesh(planeMesh);
    
    if(m_renderTechniqueImporter && m_sceneUpdateMgr)
    {
        planeModel->onAddedToScene(m_renderTechniqueImporter,
                                   m_sceneUpdateMgr);
    }
    
    ISharedConfigurationLoadingHandler handler = std::static_pointer_cast<IConfigurationLoadingHandler>(planeModel);
    handler->onConfigurationLoaded(configuration, true);
    
    return planeModel;
}

bool CMEModelBrush::isInCameraFrustum(CSharedFrustumRef)
{
    return true;
}

void CMEModelBrush::onDraw(CSharedMaterialRef)
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
        for(const auto& iterator : m_planes)
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
        for(const auto& iterator : m_planes)
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
        for(const auto& iterator : m_planes)
        {
            assert(iterator != nullptr);
            iterator->setScale(scale);
        }
    }
}

void CMEModelBrush::setVisible(bool value)
{
    IGameObject::setVisible(value);
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        for(const auto& iterator : m_arrows)
        {
            assert(iterator != nullptr);
            iterator->setVisible(value);
        }
        for(const auto& iterator : m_planes)
        {
            assert(iterator != nullptr);
            iterator->setVisible(value);
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
        for(const auto& iterator : m_planes)
        {
            assert(iterator != nullptr);
            iterator->setCamera(camera);
        }
    }
}

void CMEModelBrush::setCameraFrustum(CSharedFrustumRef frustum)
{
    IGameObject::setCameraFrustum(frustum);
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        for(const auto& iterator : m_arrows)
        {
            assert(iterator != nullptr);
            iterator->setCameraFrustum(frustum);
        }
        for(const auto& iterator : m_planes)
        {
            assert(iterator != nullptr);
            iterator->setCameraFrustum(frustum);
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
        for(const auto& iterator : m_planes)
        {
            assert(iterator != nullptr);
            iterator->setGlobalLightSource(lightSource);
        }
    }
}

void CMEModelBrush::onAddedToScene(ISharedRenderTechniqueImporterRef techniqueImporter,
                                   CSharedSceneUpdateMgrRef sceneUpdateMgr)
{
    IGameObject::onAddedToScene(techniqueImporter, sceneUpdateMgr);
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        for(const auto& iterator : m_arrows)
        {
            assert(iterator != nullptr);
            iterator->onAddedToScene(techniqueImporter, sceneUpdateMgr);
        }
        for(const auto& iterator : m_planes)
        {
            assert(iterator != nullptr);
            iterator->onAddedToScene(techniqueImporter, sceneUpdateMgr);
        }
    }
}

void CMEModelBrush::onRemovedFromScene(void)
{
    IGameObject::onRemovedFromScene();
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        for(const auto& iterator : m_arrows)
        {
            assert(iterator != nullptr);
            iterator->onRemovedFromScene();
        }
        for(const auto& iterator : m_planes)
        {
            assert(iterator != nullptr);
            iterator->onRemovedFromScene();
        }
    }
}