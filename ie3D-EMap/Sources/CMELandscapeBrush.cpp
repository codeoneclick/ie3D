//
//  CMELandscapeBrush.cpp
//  ie3D-EMap
//
//  Created by Sergey Sergeev on 4/30/14.
//
//

#include "CMELandscapeBrush.h"
#include "CVertexBuffer.h"
#include "CIndexBuffer.h"
#include "CMEConfigurationGameObjects.h"
#include "CLandscape.h"
#include "CMesh.h"
#include "CMaterial.h"
#include "CShader.h"
#include "CCamera.h"

CMELandscapeBrush::CMELandscapeBrush(CSharedResourceAccessorRef resourceAccessor,
                                     ISharedRenderTechniqueAccessorRef renderTechniqueAccessor) :
IGameObject(resourceAccessor, renderTechniqueAccessor),
m_size(4),
m_landscape(nullptr)
{
    m_zOrder = 6;
}

CMELandscapeBrush::~CMELandscapeBrush(void)
{
    
}

void CMELandscapeBrush::onSceneUpdate(f32 deltatime)
{
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        IGameObject::onSceneUpdate(deltatime);
    }
}

void CMELandscapeBrush::onResourceLoaded(ISharedResourceRef resource, bool success)
{
    IGameObject::onResourceLoaded(resource, success);
}

void CMELandscapeBrush::onConfigurationLoaded(ISharedConfigurationRef configuration, bool success)
{
    IGameObject::onConfigurationLoaded(configuration, success);
    
    std::shared_ptr<CMEConfigurationLandscapeBrush> landscapeBrushConfiguration = std::static_pointer_cast<CMEConfigurationLandscapeBrush>(configuration);
    assert(m_resourceAccessor != nullptr);
    
    CMELandscapeBrush::createMesh(m_size);
    
	IGameObject::enableRender(m_isNeedToRender);
    IGameObject::enableUpdate(m_isNeedToUpdate);
    
    m_status |= E_LOADING_STATUS_TEMPLATE_LOADED;
}

void CMELandscapeBrush::createMesh(f32 radius)
{
    ui32 sizeX = static_cast<ui32>(radius * 2);
    ui32 sizeZ = static_cast<ui32>(radius * 2);
    CSharedVertexBuffer vertexBuffer = std::make_shared<CVertexBuffer>(sizeX * sizeZ, GL_STREAM_DRAW);
    SAttributeVertex* vertexData = vertexBuffer->lock();
    
    ui32 index = 0;
    for(i32 i = -static_cast<i32>(sizeX) / 2; i < static_cast<i32>(sizeX) / 2; ++i)
    {
        for(i32 j = -static_cast<i32>(sizeZ) / 2; j < static_cast<i32>(sizeZ) / 2; ++j)
        {
            glm::vec3 position = glm::vec3(i + m_position.x, 0.0, j + m_position.z);
            position.y = m_landscape != nullptr &&
            position.x > 0.0 &&
            position.z > 0.0 &&
            position.x < m_landscape->getHeightmapSizeX() &&
            position.z < m_landscape->getHeightmapSizeZ() ?
            m_landscape->getHeight(position) : 0.0;
            vertexData[index].m_position = position;
            
            glm::u16vec2 texcoord = CVertexBuffer::compressVec2(glm::vec2(static_cast<f32>(i + static_cast<i32>(sizeX) / 2) / static_cast<f32>(sizeX),
                                                                          static_cast<f32>(j + static_cast<i32>(sizeZ) / 2) / static_cast<f32>(sizeZ)));
            vertexData[index].m_texcoord = texcoord;
            
            ++index;
        }
    }
    vertexBuffer->unlock();
    
    CSharedIndexBuffer indexBuffer = std::make_shared<CIndexBuffer>((sizeX - 1) * (sizeZ - 1) * 6, GL_STATIC_DRAW);
    ui16* indexData = indexBuffer->lock();
    
    index = 0;
    for(ui32 i = 0; i < (sizeX - 1); ++i)
    {
        for(ui32 j = 0; j < (sizeZ - 1); ++j)
        {
            indexData[index] = i + j * sizeX;
            index++;
            indexData[index] = i + (j + 1) * sizeX;
            index++;
            indexData[index] = i + 1 + j * sizeX;
            index++;
            
            indexData[index] = i + (j + 1) * sizeX;
            index++;
            indexData[index] = i + 1 + (j + 1) * sizeX;
            index++;
            indexData[index] = i + 1 + j * sizeX;
            index++;
        }
    }
    indexBuffer->unlock();
    
    m_mesh = CMesh::constructCustomMesh("landscape.brush", vertexBuffer, indexBuffer,
                                        glm::vec3(4096.0), glm::vec3(4096.0));
    assert(m_mesh != nullptr);
}

i32 CMELandscapeBrush::zOrder(void)
{
    return m_zOrder;
}

bool CMELandscapeBrush::checkOcclusion(void)
{
    return false;
}

ui32 CMELandscapeBrush::numTriangles(void)
{
    return IGameObject::numTriangles();
}

void CMELandscapeBrush::onBind(const std::string& mode)
{
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        assert(m_materials.find(mode) != m_materials.end());
        IGameObject::onBind(mode);
    }
}

void CMELandscapeBrush::onDraw(const std::string& mode)
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
        material->getShader()->setVector2Custom(glm::vec2(m_position.x, m_position.z), "IN_Center");
        material->getShader()->setFloatCustom(m_size * 0.75, "IN_Radius");
        
        IGameObject::onDraw(mode);
    }
}

void CMELandscapeBrush::onUnbind(const std::string& mode)
{
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        assert(m_materials.find(mode) != m_materials.end());
        IGameObject::onUnbind(mode);
    }
}

void CMELandscapeBrush::onBatch(const std::string& mode)
{
    IGameObject::onBatch(mode);
}

void CMELandscapeBrush::setLandscape(CSharedLandscapeRef landscape)
{
    assert(landscape != nullptr);
    m_landscape = landscape;
}

void CMELandscapeBrush::setPosition(const glm::vec3 &position)
{
    IGameObject::setPosition(position);
    if(m_landscape != nullptr &&
       m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        assert(m_size > 0);
        ui32 sizeX = static_cast<ui32>(m_size * 2);
        ui32 sizeZ = static_cast<ui32>(m_size * 2);
        
        SAttributeVertex* vertexData = m_mesh->getVertexBuffer()->lock();
        
        ui32 index = 0;
        for(i32 i = -static_cast<i32>(sizeX) / 2; i < static_cast<i32>(sizeX) / 2; ++i)
        {
            for(i32 j = -static_cast<i32>(sizeZ) / 2; j < static_cast<i32>(sizeZ) / 2; ++j)
            {
                glm::vec3 position = glm::vec3(i + m_position.x + 0.5, 0.0, j + m_position.z + 0.5);
                position.y = position.x > 0.0 &&
                position.z > 0.0 &&
                position.x < m_landscape->getHeightmapSizeX() &&
                position.z < m_landscape->getHeightmapSizeZ() ?
                m_landscape->getHeight(position) : 0.0;
                vertexData[index].m_position = position;
                ++index;
            }
        }
        m_mesh->getVertexBuffer()->unlock();
    }
}

void CMELandscapeBrush::setSize(f32 size)
{
    if(m_size != size)
    {
        CMELandscapeBrush::createMesh(size);
    }
    m_size = size;
}
