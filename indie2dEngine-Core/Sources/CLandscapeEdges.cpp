//
//  CLandscapeEdges.cpp
//  iGaia
//
//  Created by sergey sergeev on 7/11/12.
//
//

#include "CLandscapeEdges.h"

CLandscapeEdges::CLandscapeEdges(void)
{

}

CLandscapeEdges::~CLandscapeEdges(void)
{
    
}

void CLandscapeEdges::Load(CMaterial* _material, ui32 _width, ui32 _height, const glm::vec2 _heightBound)
{
    m_width = _width;
    m_height = _height;
    m_materials[E_RENDER_MODE_WORLD_SPACE_COMMON] = _material;
    
    std::unique_ptr<CVertexBuffer> vertexBuffer = std::unique_ptr<CVertexBuffer>(new CVertexBuffer(16, GL_STATIC_DRAW));
    SVertex* vertexData = vertexBuffer->Lock();

    glm::vec3 boundMin = glm::vec3(0.0f, _heightBound.x, 0.0f);
    glm::vec3 boundMax = glm::vec3(m_width, _heightBound.y, m_height);

    vertexData[0].m_position = glm::vec3(boundMin.x,  boundMin.y, boundMax.z);
    vertexData[1].m_position = glm::vec3(boundMax.x,  boundMin.y, boundMax.z);
    vertexData[2].m_position = glm::vec3(boundMax.x,  boundMax.y, boundMax.z);
    vertexData[3].m_position = glm::vec3(boundMin.x,  boundMax.y, boundMax.z);
    
    vertexData[4].m_position = glm::vec3(boundMin.x,  boundMin.y,  boundMin.z);
    vertexData[5].m_position = glm::vec3(boundMin.x,  boundMax.y,  boundMin.z);
    vertexData[6].m_position = glm::vec3(boundMax.x,  boundMax.y,  boundMin.z);
    vertexData[7].m_position = glm::vec3(boundMax.x,  boundMin.y,  boundMin.z);

    vertexData[8].m_position = glm::vec3(boundMax.x,  boundMin.y,   boundMax.z);
    vertexData[9].m_position = glm::vec3(boundMax.x,  boundMin.y,   boundMin.z);
    vertexData[10].m_position = glm::vec3(boundMax.x,  boundMax.y,  boundMin.z);
    vertexData[11].m_position = glm::vec3(boundMax.x,  boundMax.y,  boundMax.z);

    vertexData[12].m_position = glm::vec3(boundMin.x,  boundMin.y,  boundMin.z);
    vertexData[13].m_position = glm::vec3(boundMin.x,  boundMin.y,  boundMax.z);
    vertexData[14].m_position = glm::vec3(boundMin.x,  boundMax.y,  boundMax.z);
    vertexData[15].m_position = glm::vec3(boundMin.x,  boundMax.y,  boundMin.z);

    vertexData[0].m_texcoord = glm::vec2(0.0f, 1.0f / 4.0f);
    vertexData[1].m_texcoord = glm::vec2(1.0f - 0.001f, 1.0f / 4.0f);
    vertexData[2].m_texcoord = glm::vec2(1.0f - 0.001f, (1.0f / 4.0f) * 2.0f - 0.001f);
    vertexData[3].m_texcoord = glm::vec2(0.0f, (1.0f / 4.0f) * 2.0f - 0.001f);

    vertexData[4].m_texcoord = glm::vec2(1.0f - 0.001f, 0.0f);
    vertexData[5].m_texcoord = glm::vec2(1.0f - 0.001f, 1.0f / 4.0f - 0.001f);
    vertexData[6].m_texcoord = glm::vec2(0.0f, 1.0f / 4.0f - 0.001f);
    vertexData[7].m_texcoord = glm::vec2(0.0f, 0.0f);

    vertexData[8].m_texcoord =  glm::vec2(0.0f, (1.0f / 4.0f) * 3.0f);
    vertexData[9].m_texcoord =  glm::vec2(1.0f - 0.001f, (1.0f / 4.0f) * 3.0f);
    vertexData[10].m_texcoord = glm::vec2(1.0f - 0.001f, 1.0f - 0.001f);
    vertexData[11].m_texcoord = glm::vec2(0.0f, 1.0f - 0.001f);

    vertexData[12].m_texcoord = glm::vec2(0.0f, (1.0f / 4.0f) * 2.0f);
    vertexData[13].m_texcoord = glm::vec2(1.0f - 0.001f, (1.0f / 4.0f) * 2.0f);
    vertexData[14].m_texcoord = glm::vec2(1.0f - 0.001f, (1.0f / 4.0f) * 3.0f - 0.001f);
    vertexData[15].m_texcoord = glm::vec2(0.0f, (1.0f / 4.0f) * 3.0f - 0.001f);

    vertexBuffer->Unlock();

    std::unique_ptr<CIndexBuffer> indexBuffer = std::unique_ptr<CIndexBuffer>(new CIndexBuffer(24, GL_STATIC_DRAW));
    ui16* indexData = indexBuffer->Lock();

    indexData[0] = 0;
    indexData[1] = 1;
    indexData[2] = 2;
    indexData[3] = 0;
    indexData[4] = 2;
    indexData[5] = 3;

    indexData[6] = 4;
    indexData[7] = 5;
    indexData[8] = 6;
    indexData[9] = 4;
    indexData[10] = 6;
    indexData[11] = 7;

    indexData[12] = 8;
    indexData[13] = 9;
    indexData[14] = 10;
    indexData[15] = 8;
    indexData[16] = 10;
    indexData[17] = 11;

    indexData[18] = 12;
    indexData[19] = 13;
    indexData[20] = 14;
    indexData[21] = 12;
    indexData[22] = 14;
    indexData[23] = 15;

    indexBuffer->Unlock();

    m_mesh = new CMesh();
    m_mesh->Link(std::move(vertexBuffer), std::move(indexBuffer));
}

void CLandscapeEdges::OnResourceDidLoad(TSharedPtrResource _resource)
{
    CGameObject3d::OnResourceDidLoad(_resource);
}

void CLandscapeEdges::OnUpdate(f32 _deltatime)
{
    CGameObject3d::OnUpdate(_deltatime);
}

ui32 CLandscapeEdges::OnDrawIndex(void)
{
    return 1024;
}

void CLandscapeEdges::OnBind(E_RENDER_MODE_WORLD_SPACE _mode)
{
    CGameObject3d::OnBind(_mode);
}

void CLandscapeEdges::OnDraw(E_RENDER_MODE_WORLD_SPACE _mode)
{
    assert(m_materials[_mode] != nullptr);
    assert(m_camera != nullptr);
    assert(m_light != nullptr);

    switch (_mode)
    {
        case E_RENDER_MODE_WORLD_SPACE_COMMON:
        {
            assert(m_materials[_mode]->Get_Shader() != nullptr);
            m_materials[_mode]->Get_Shader()->Set_Matrix4x4(m_matrixWorld, E_SHADER_ATTRIBUTE_MATRIX_WORLD);
            m_materials[_mode]->Get_Shader()->Set_Matrix4x4(m_camera->Get_ProjectionMatrix(), E_SHADER_ATTRIBUTE_MATRIX_PROJECTION);
            m_materials[_mode]->Get_Shader()->Set_Matrix4x4(m_camera->Get_ViewMatrix(), E_SHADER_ATTRIBUTE_MATRIX_VIEW);

            m_materials[_mode]->Get_Shader()->Set_Vector3(m_camera->Get_Position(), E_SHADER_ATTRIBUTE_VECTOR_CAMERA_POSITION);
            m_materials[_mode]->Get_Shader()->Set_Vector3(m_light->Get_Position(), E_SHADER_ATTRIBUTE_VECTOR_LIGHT_POSITION);
        }
            break;
        case E_RENDER_MODE_WORLD_SPACE_REFLECTION:
        {
        }
            break;
        case E_RENDER_MODE_WORLD_SPACE_REFRACTION:
        {
        }
            break;
        default:
            break;
    }

    CGameObject3d::OnDraw(_mode);
}

void CLandscapeEdges::OnUnbind(E_RENDER_MODE_WORLD_SPACE _mode)
{
    CGameObject3d::OnUnbind(_mode);
}


