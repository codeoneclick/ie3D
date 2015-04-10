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
#include "CMeshExtension.h"

CMEModelBrush::CMEModelBrush(CSharedResourceAccessorRef resourceAccessor,
                             ISharedRenderTechniqueAccessorRef renderTechniqueAccessor) :
IGameObject(resourceAccessor, renderTechniqueAccessor),
m_landscape(nullptr),
m_model(nullptr),
m_sphere(nullptr),
m_currentMode(E_GAMEOBJECT_BRUSH_MODE_TRANSLATE)
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
    
    f32 radius = 8.0;
    i32 rings = 12;
    i32 sectors = 24;
    
    CSharedVertexBuffer vbo = std::make_shared<CVertexBuffer>(8 * 6, GL_STATIC_DRAW);
    SAttributeVertex* vertices = vbo->lock();
    
    CSharedIndexBuffer ibo = std::make_shared<CIndexBuffer>(36 * 6, GL_STATIC_DRAW);
    ui16* indices = ibo->lock();
    
    ui32 verticesOffset = 0;
    ui32 indicesOffset = 0;
    
    for(ui32 i = 0; i < configurationModelBrush->getElementsConfigurations().size(); ++i)
    {
        CSharedMEConfigurationBrushElement configurationBrushElement = std::static_pointer_cast<CMEConfigurationBrushElement>(configurationModelBrush->getElementsConfigurations().at(i));
        std::string name = configurationBrushElement->getName();
        glm::vec2 size = glm::vec2(configurationBrushElement->getWidth(),
                                   configurationBrushElement->getHeight());
        glm::u8vec4 color = glm::u8vec4(configurationBrushElement->getColorR(),
                                        configurationBrushElement->getColorG(),
                                        configurationBrushElement->getColorB(),
                                        configurationBrushElement->getColorA());
        
        if(name == "arrowX")
        {
            m_arrows.at(E_MODEL_BRUSH_ARROW_X) = CMEModelBrush::createArrow(E_MODEL_BRUSH_ARROW_X, size, color,
                                                                            vertices, verticesOffset,
                                                                            indices, indicesOffset);
        }
        else if(name == "arrowY")
        {
            m_arrows.at(E_MODEL_BRUSH_ARROW_Y) = CMEModelBrush::createArrow(E_MODEL_BRUSH_ARROW_Y, size, color,
                                                                            vertices, verticesOffset,
                                                                            indices, indicesOffset);
        }
        else if(name == "arrowZ")
        {
            m_arrows.at(E_MODEL_BRUSH_ARROW_Z) = CMEModelBrush::createArrow(E_MODEL_BRUSH_ARROW_Z, size, color,
                                                                            vertices, verticesOffset,
                                                                            indices, indicesOffset);
        }
        else if(name == "planeX")
        {
            m_planes.at(E_MODEL_BRUSH_PLANE_YZ) = CMEModelBrush::createPlane(E_MODEL_BRUSH_PLANE_YZ, size, color,
                                                                             vertices, verticesOffset,
                                                                             indices, indicesOffset);
        }
        else if(name == "planeY")
        {
            m_planes.at(E_MODEL_BRUSH_PLANE_XZ) = CMEModelBrush::createPlane(E_MODEL_BRUSH_PLANE_XZ, size, color,
                                                                             vertices, verticesOffset,
                                                                             indices, indicesOffset);
        }
        else if(name == "planeZ")
        {
            m_planes.at(E_MODEL_BRUSH_PLANE_XY) = CMEModelBrush::createPlane(E_MODEL_BRUSH_PLANE_XY, size, color,
                                                                             vertices, verticesOffset,
                                                                             indices, indicesOffset);
        }
        else
        {
            assert(false);
        }
        verticesOffset += 8;
        indicesOffset += 36;
    }
    vbo->unlock();
    ibo->unlock();
    
    m_meshes[E_GAMEOBJECT_BRUSH_MODE_TRANSLATE] = CMesh::construct("gameobject.brush.translate", vbo, ibo);

    vbo = std::make_shared<CVertexBuffer>(rings * sectors * 3, GL_STATIC_DRAW);
    vertices = vbo->lock();
    
    ibo = std::make_shared<CIndexBuffer>((rings - 1) * (sectors - 1) * 6, GL_STATIC_DRAW);
    indices = ibo->lock();
    
    verticesOffset = 0;
    indicesOffset = 0;

    m_sphere = CMEModelBrush::createSphere(radius, rings, sectors, vertices, verticesOffset, indices, indicesOffset);
    vbo->unlock();
    ibo->unlock();
    
    m_meshes[E_GAMEOBJECT_BRUSH_MODE_ROTATE] = CMesh::construct("gameobject.brush.rotate", vbo, ibo);
    
    m_mesh = m_meshes[m_currentMode];
    
    IGameObject::onConfigurationLoaded(configuration, success);
    m_status |= E_LOADING_STATUS_TEMPLATE_LOADED;
}

CESharedCustomModel CMEModelBrush::createArrow(E_MODEL_BRUSH_ARROW arrow, const glm::vec2& size, const glm::u8vec4& color,
                                               SAttributeVertex *mainVertices, ui32 verticesOffset,
                                               ui16 *mainIndices, ui32 indicesOffset)
{
    glm::vec3 maxBound = glm::vec3(0.0);
    glm::vec3 minBound = glm::vec3(0.0);
    switch (arrow)
    {
        case E_MODEL_BRUSH_ARROW_X:
        {
            maxBound = glm::vec3(size.y, size.x, size.x);
            minBound = glm::vec3(size.x, 0.0, 0.0);
        }
            break;
        case E_MODEL_BRUSH_ARROW_Y:
        {
            maxBound = glm::vec3(size.x, size.y, size.x);
            minBound = glm::vec3(0.0, 0.0, 0.0);
        }
            break;
        case E_MODEL_BRUSH_ARROW_Z:
        {
            maxBound = glm::vec3(size.x, size.x, size.y);
            minBound = glm::vec3(0.0, 0.0, size.x);
        }
            break;
            
        default:
        {
            assert(false);
        }
            break;
    }
    
    CSharedMesh mesh = CMeshExtension::createBox(minBound, maxBound, color);
    SAttributeVertex *vertices = mesh->getVertexBuffer()->lock();
    ui16 *indices = mesh->getIndexBuffer()->lock();
    
    for(ui32 i = 0; i < 8; ++i)
    {
        mainVertices[verticesOffset + i] = vertices[i];
    }
    for(ui32 i = 0; i < 36; ++i)
    {
        mainIndices[indicesOffset + i] = indices[i] + verticesOffset;
    }
    
    CESharedCustomModel arrowM = std::make_shared<CECustomModel>(m_resourceAccessor, m_renderTechniqueAccessor);
    arrowM->setCamera(m_camera);
    arrowM->setMesh(mesh);
    return arrowM;
}

CESharedCustomModel CMEModelBrush::createPlane(E_MODEL_BRUSH_PLANE plane, const glm::vec2& size, const glm::u8vec4& color,
                                               SAttributeVertex *mainVertices, ui32 verticesOffset,
                                               ui16 *mainIndices, ui32 indicesOffset)
{
    glm::vec3 maxBound = glm::vec3(0.0);
    glm::vec3 minBound = glm::vec3(0.0);
    switch (plane)
    {
        case E_MODEL_BRUSH_PLANE_YZ:
        {
            maxBound = glm::vec3(size.x, size.y, 0.0);
            minBound = glm::vec3(1.0, 1.0, 0.0);
        }
            break;
        case E_MODEL_BRUSH_PLANE_XZ:
        {
            maxBound = glm::vec3(size.x, 0.0, size.y);
            minBound = glm::vec3(1.0, 0.0, 1.0);
        }
            break;
        case E_MODEL_BRUSH_PLANE_XY:
        {
            maxBound = glm::vec3(0.0, size.x, size.y);
            minBound = glm::vec3(0.0, 1.0, 1.0);
        }
            break;
            
        default:
        {
            assert(false);
        }
            break;
    }
    
    CSharedMesh mesh = CMeshExtension::createBox(minBound, maxBound, color);
    SAttributeVertex *vertices = mesh->getVertexBuffer()->lock();
    ui16 *indices = mesh->getIndexBuffer()->lock();
    
    for(ui32 i = 0; i < 8; ++i)
    {
        mainVertices[verticesOffset + i] = vertices[i];
    }
    for(ui32 i = 0; i < 36; ++i)
    {
        mainIndices[indicesOffset + i] = indices[i] + verticesOffset;
    }
    
    CESharedCustomModel planeM = std::make_shared<CECustomModel>(m_resourceAccessor, m_renderTechniqueAccessor);
    planeM->setCamera(m_camera);
    planeM->setMesh(mesh);

    return planeM;
}

CESharedCustomModel CMEModelBrush::createSphere(f32 radius, i32 rings, i32 sectors,
                                                SAttributeVertex *mainVertices, ui32 verticesOffset,
                                                ui16 *mainIndices, ui32 indicesOffset)
{
    CSharedMesh mesh = CMeshExtension::createSphere(radius, rings, sectors, glm::u8vec4(0, 128, 0, 128));
    SAttributeVertex *vertices = mesh->getVertexBuffer()->lock();
    ui16 *indices = mesh->getIndexBuffer()->lock();
    
    for(i32 i = 0; i < rings * sectors * 3; ++i)
    {
        mainVertices[verticesOffset + i] = vertices[i];
    }
    for(i32 i = 0; i < (rings - 1) * (sectors - 1) * 6; ++i)
    {
        mainIndices[indicesOffset + i] = indices[i] + verticesOffset;
    }
    
    CESharedCustomModel sphere = std::make_shared<CECustomModel>(m_resourceAccessor, m_renderTechniqueAccessor);
    sphere->setCamera(m_camera);
    sphere->setMesh(mesh);
    return sphere;
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
        m_sphere->setPosition(position);
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
        m_sphere->setRotation(rotation);
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
        m_sphere->setScale(scale);
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
        m_sphere->setCamera(camera);
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

const CESharedCustomModel CMEModelBrush::getSphere(void) const
{
    return m_sphere;
}

void CMEModelBrush::setMode(E_GAMEOBJECT_BRUSH_MODE mode)
{
    m_currentMode = mode;
    m_mesh = m_meshes[m_currentMode];
}

E_GAMEOBJECT_BRUSH_MODE CMEModelBrush::getMode(void) const
{
    return m_currentMode;
}
