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
    
    m_planes.at(E_MODEL_BRUSH_PLANE_YZ) = nullptr;
    m_planes.at(E_MODEL_BRUSH_PLANE_XZ) = nullptr;
    m_planes.at(E_MODEL_BRUSH_PLANE_XY) = nullptr;
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
    CSharedMEConfigurationModelBrush configurationModelBrush = std::static_pointer_cast<CMEConfigurationModelBrush>(configuration);
    
    CSharedVertexBuffer vertexBuffer = std::make_shared<CVertexBuffer>(8 * 6, GL_STATIC_DRAW);
    SAttributeVertex* vertexData = vertexBuffer->lock();
    
    CSharedIndexBuffer indexBuffer = std::make_shared<CIndexBuffer>(36 * 6, GL_STATIC_DRAW);
    ui16* indexData = indexBuffer->lock();
    
    ui32 verticesOffset = 0;
    ui32 indicesOffset = 0;
    
    for(ui32 i = 0; i < configurationModelBrush->getElementsConfigurations().size(); ++i)
    {
        CSharedMEConfigurationBrushElement configurationBrushElement = std::static_pointer_cast<CMEConfigurationBrushElement>(configurationModelBrush->getElementsConfigurations().at(i));
        std::string name = configurationBrushElement->getName();
        if(name == "arrowX")
        {
             m_arrows.at(E_MODEL_BRUSH_ARROW_X) = CMEModelBrush::createArrowModel(E_MODEL_BRUSH_ARROW_X,
                                                                                  vertexData, verticesOffset,
                                                                                  indexData, indicesOffset);
        }
        else if(name == "arrowY")
        {
            m_arrows.at(E_MODEL_BRUSH_ARROW_Y) = CMEModelBrush::createArrowModel(E_MODEL_BRUSH_ARROW_Y,
                                                                                 vertexData, verticesOffset,
                                                                                 indexData, indicesOffset);
        }
        else if(name == "arrowZ")
        {
            m_arrows.at(E_MODEL_BRUSH_ARROW_Z) = CMEModelBrush::createArrowModel(E_MODEL_BRUSH_ARROW_Z,
                                                                                 vertexData, verticesOffset,
                                                                                 indexData, indicesOffset);
        }
        else if(name == "planeX")
        {
            m_planes.at(E_MODEL_BRUSH_PLANE_YZ) = CMEModelBrush::createPlaneModel(E_MODEL_BRUSH_PLANE_YZ,
                                                                                  vertexData, verticesOffset,
                                                                                  indexData, indicesOffset);
        }
        else if(name == "planeY")
        {
            m_planes.at(E_MODEL_BRUSH_PLANE_XZ) = CMEModelBrush::createPlaneModel(E_MODEL_BRUSH_PLANE_XZ,
                                                                                  vertexData, verticesOffset,
                                                                                  indexData, indicesOffset);
        }
        else if(name == "planeZ")
        {
            m_planes.at(E_MODEL_BRUSH_PLANE_XY) = CMEModelBrush::createPlaneModel(E_MODEL_BRUSH_PLANE_XY,
                                                                                  vertexData, verticesOffset,
                                                                                  indexData, indicesOffset);
        }
        else
        {
            assert(false);
        }
        verticesOffset += 8;
        indicesOffset += 36;
    }
    
    vertexBuffer->unlock();
    indexBuffer->unlock();
    
    m_mesh = CMesh::constructCustomMesh("gameobject.brush", vertexBuffer, indexBuffer,
                                        glm::vec3(4096.0), glm::vec3(-4096.0));
    m_mesh->updateBounds();
    
    IGameObject::onConfigurationLoaded(configuration, success);
    m_status |= E_LOADING_STATUS_TEMPLATE_LOADED;
}

CESharedCustomModel CMEModelBrush::createArrowModel(E_MODEL_BRUSH_ARROW arrow,
                                                    SAttributeVertex *mainVertexData, ui32 verticesOffset,
                                                    ui16 *mainIndexData, ui32 indicesOffset)
{
    glm::vec3 maxBound = glm::vec3(0.0);
    glm::vec3 minBound = glm::vec3(0.0);
    glm::u8vec4 color = glm::u8vec4(255);
    switch (arrow)
    {
        case E_MODEL_BRUSH_ARROW_X:
        {
            maxBound = glm::vec3(12.0, 0.5, 0.5);
            minBound = glm::vec3(0.25, 0.0, 0.0);
            color = glm::vec4(255, 0, 0, 128);
        }
            break;
        case E_MODEL_BRUSH_ARROW_Y:
        {
            maxBound = glm::vec3(0.5, 12.0, 0.5);
            minBound = glm::vec3(0.0, 0.0, 0.0);
            color = glm::vec4(0, 255, 0, 128);
        }
            break;
        case E_MODEL_BRUSH_ARROW_Z:
        {
            maxBound = glm::vec3(0.5, 0.5, 12.0);
            minBound = glm::vec3(0.0, 0.0, 0.25);
            color = glm::vec4(0, 0, 255, 128);
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
    
    for(ui32 i = 0; i < 8; ++i)
    {
        mainVertexData[verticesOffset + i] = vertexData[i];
    }
    for(ui32 i = 0; i < 36; ++i)
    {
        mainIndexData[indicesOffset + i] = indexData[i] + verticesOffset;
    }
    
    CSharedMesh arrowMesh = CMesh::constructCustomMesh("arrow", vertexBuffer, indexBuffer,
                                                       glm::vec3(4096.0), glm::vec3(-4096.0));
    arrowMesh->updateBounds();
    
    CESharedCustomModel arrowModel = std::make_shared<CECustomModel>(m_resourceAccessor, m_renderTechniqueAccessor);
    arrowModel->setCamera(m_camera);
    arrowModel->setMesh(arrowMesh);
    
    return arrowModel;
}

CESharedCustomModel CMEModelBrush::createPlaneModel(E_MODEL_BRUSH_PLANE plane,
                                                    SAttributeVertex *mainVertexData, ui32 verticesOffset,
                                                    ui16 *mainIndexData, ui32 indicesOffset)
{
    glm::vec3 maxBound = glm::vec3(0.0);
    glm::vec3 minBound = glm::vec3(0.0);
    glm::u8vec4 color = glm::u8vec4(255, 255, 0, 64);
    switch (plane)
    {
        case E_MODEL_BRUSH_PLANE_YZ:
        {
            maxBound = glm::vec3(8.0, 8.0, 0.25);
            minBound = glm::vec3(1.0, 1.0, 0.4);
        }
            break;
        case E_MODEL_BRUSH_PLANE_XZ:
        {
            maxBound = glm::vec3(8.0, 0.25, 8.0);
            minBound = glm::vec3(1.0, 0.4, 1.0);
        }
            break;
        case E_MODEL_BRUSH_PLANE_XY:
        {
            maxBound = glm::vec3(0.25, 8.0, 8.0);
            minBound = glm::vec3(0.4, 1.0, 1.0);
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
    
    for(ui32 i = 0; i < 8; ++i)
    {
        mainVertexData[verticesOffset + i] = vertexData[i];
    }
    for(ui32 i = 0; i < 36; ++i)
    {
        mainIndexData[indicesOffset + i] = indexData[i] + verticesOffset;
    }
    
    CSharedMesh planeMesh = CMesh::constructCustomMesh("plane", vertexBuffer, indexBuffer,
                                                       glm::vec3(4096.0), glm::vec3(-4096.0));
    planeMesh->updateBounds();
    
    CESharedCustomModel planeModel = std::make_shared<CECustomModel>(m_resourceAccessor, m_renderTechniqueAccessor);
    planeModel->setCamera(m_camera);
    planeModel->setMesh(planeMesh);

    return planeModel;
}

bool CMEModelBrush::isInCameraFrustum(CSharedFrustumRef)
{
    return m_isVisible;
}

void CMEModelBrush::onDraw(CSharedMaterialRef material)
{
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        IGameObject::onDraw(material);
    }
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
}

void CMEModelBrush::setGlobalLightSource(CSharedGlobalLightSourceRef lightSource)
{
    IGameObject::setGlobalLightSource(lightSource);
}

void CMEModelBrush::onAddedToScene(ISharedRenderTechniqueImporterRef techniqueImporter,
                                   CSharedSceneUpdateMgrRef sceneUpdateMgr)
{
    IGameObject::onAddedToScene(techniqueImporter, sceneUpdateMgr);
}

void CMEModelBrush::onRemovedFromScene(void)
{
    IGameObject::onRemovedFromScene();
}

const std::array<CESharedCustomModel, E_MODEL_BRUSH_ARROW_MAX>&  CMEModelBrush::getArrows(void) const
{
    return m_arrows;
}

const std::array<CESharedCustomModel, E_MODEL_BRUSH_PLANE_MAX>&  CMEModelBrush::getPlanes(void) const
{
    return m_planes;
}