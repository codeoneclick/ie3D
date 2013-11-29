//
//  CBatch.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 8/22/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CBatch.h"
#include "CMaterial.h"
#include "CMesh.h"
#include "CQuad.h"
#include "CShader.h"
#include "CAnimationMixer.h"
#include "CCommonOS.h"

const ui32 CBatch::k_MAX_NUM_VERTICES = 65535 / 4; // 16k vertices
const ui32 CBatch::k_MAX_NUM_INDICES = 65535 / 2;  // 32k indices
const ui32 CBatch::k_MAX_NUM_TRANSFORMATION = 255;

CBatch::CBatch(const std::string& _mode, E_BATCH_GEOMETRY_MODE _geometryMode, ui32 _renderQueuePosition, std::shared_ptr<CMaterial> _material, const std::function<void(std::shared_ptr<CMaterial>)>& _materialImposer) :
m_material(_material),
m_renderQueuePosition(_renderQueuePosition),
m_materialImposer(_materialImposer),
m_mode(_mode),
m_geometryMode(_geometryMode),
m_numVertices(0),
m_numIndices(0),
m_numPushedVertices(0),
m_numPushedIndices(0),
m_locked(0),
m_proccessed(0),
m_unlocked(0)
{
    assert(m_material != nullptr);
    assert(m_material->Get_Shader() != nullptr);
    m_guid = m_material->Get_Shader()->Get_Guid() + ".batch";
    
    std::shared_ptr<CVertexBuffer> vertexBuffer = std::make_shared<CVertexBuffer>(k_MAX_NUM_VERTICES, GL_DYNAMIC_DRAW);
    SHardwareVertex* vertexData = vertexBuffer->Lock();
    memset(vertexData, 0x0, k_MAX_NUM_VERTICES * sizeof(SHardwareVertex));
    vertexBuffer->Unlock();
   
    std::shared_ptr<CIndexBuffer> indexBuffer = std::make_shared<CIndexBuffer>(k_MAX_NUM_INDICES, GL_DYNAMIC_DRAW);
    ui16* indexData = indexBuffer->Lock();
    memset(indexData, 0x0, k_MAX_NUM_INDICES * sizeof(ui16));
    indexBuffer->Unlock();
    
    m_mesh = std::make_shared<CMesh>(m_guid, vertexBuffer, indexBuffer);
    assert(m_mesh != nullptr);
}

CBatch::~CBatch(void)
{
    
}

void CBatch::Lock(void)
{
    if(m_locked == 0 && m_proccessed == 0)
    {
        m_meshes.clear();
        m_matrices.clear();
        m_transformations.clear();

		m_controls.clear();
		m_positions.clear();
		m_sizes.clear();
        
        m_numBatchedVertices = 0;
        m_numBatchedIndices = 0;
        m_numBatchedTransformations = 0;
    }
    m_locked = 1;
}

std::function<void(void)> CBatch::_UnlockModelsGeometry()
{
	return [this]()
        {
            ui32 numVertices = 0;
            ui32 numIndices = 0;
            ui32 numTransformations = 0;
            
            for(ui32 i = 0; i < m_meshes.size(); ++i)
            {
                std::shared_ptr<CMesh> mesh = std::get<0>(m_meshes[i]);
                std::shared_ptr<CAnimationMixer> mixer = std::get<1>(m_meshes[i]);
                glm::mat4x4& matrix = m_matrices[i];
                
                ui16* indexData_01 = m_mesh->Get_IndexBuffer()->Lock();
                ui16* indexData_02 = mesh->Get_IndexBuffer()->Lock();
                
                for(ui32 j = 0; j < mesh->Get_NumIndexes(); ++j)
                {
                    indexData_01[numIndices + j] = indexData_02[j] + numVertices;
                }
                
                SHardwareVertex* vertexData_01 = m_mesh->Get_VertexBuffer()->Lock();
                SHardwareVertex* vertexData_02 = mesh->Get_VertexBuffer()->Lock();
                for(ui32 j = 0; j < mesh->Get_NumVertexes(); ++j)
                {
                    glm::vec3 position = glm::vec3(0.0f);
                    glm::vec3 normal = glm::vec3(0.0f);
                    glm::vec3 tangent = glm::vec3(0.0f);
                    
                    for(ui32 k = 0; k < 4; ++k)
                    {
                        f32 weight = static_cast<f32>(vertexData_02[j].m_extra[k]) / 255.0f;
                        position += glm::transform(vertexData_02[j].m_position, m_transformations[vertexData_02[j].m_color[k] + numTransformations]) * weight;
                        normal += glm::transform(CVertexBuffer::UncompressU8Vec4(vertexData_02[j].m_normal), m_transformations[vertexData_02[j].m_color[k] + numTransformations]) * weight;
                        tangent += glm::transform(CVertexBuffer::UncompressU8Vec4(vertexData_02[j].m_tangent), m_transformations[vertexData_02[j].m_color[k] + numTransformations]) * weight;
                    }
                    
                    vertexData_01[numVertices + j] = vertexData_02[j];
                    vertexData_01[numVertices + j].m_position = glm::transform(position, matrix);
                    vertexData_01[numVertices + j].m_normal = CVertexBuffer::CompressVec3(glm::transform(normal, matrix));
                    vertexData_01[numVertices + j].m_tangent = CVertexBuffer::CompressVec3(glm::transform(tangent, matrix));
                }
                
                numVertices += mesh->Get_NumVertexes();
                numIndices += mesh->Get_NumIndexes();
                numTransformations += mixer->Get_TransformationSize();
                assert(numTransformations < k_MAX_NUM_TRANSFORMATION);
            }
            
            m_numPushedVertices = numVertices;
            m_numPushedIndices = numIndices;
            m_proccessed = 0;
            m_locked = 0;
            m_unlocked = 1;
        };
}

std::function<void(void)> CBatch::_UnlockControlsGeometry()
{
	return [this]()
	{
		ui32 numVertices = 0;
        ui32 numIndices = 0;
            
		for(ui32 i = 0; i < m_controls.size(); ++i)
        {
			std::shared_ptr<CQuad> quad = m_controls[i];
				
			glm::vec2 position = m_positions[i];
			glm::vec2 size = m_sizes[i];
                
            ui16* indexData_01 = m_mesh->Get_IndexBuffer()->Lock();
            ui16* indexData_02 = quad->Get_IndexBuffer()->Lock();
                
            for(ui32 j = 0; j < quad->Get_NumIndexes(); ++j)
            {
                indexData_01[numIndices + j] = indexData_02[j] + numVertices;
            }
                
            SHardwareVertex* vertexData_01 = m_mesh->Get_VertexBuffer()->Lock();

			f32 screenWidth = Get_ScreenWidth();
			f32 screenHeight = Get_ScreenHeight();

			glm::vec4 frame(0.0f);
			frame.x = (position.x / screenWidth) * 2.0f - 1.0f;
			frame.y = ((screenHeight - position.y) / screenHeight) * 2.0f - 1.0f;
			frame.z = ((position.x + size.x) / screenWidth) * 2.0f - 1.0f;
			frame.w = ((screenHeight - (position.y + size.y)) / screenHeight) * 2.0f - 1.0f;

			vertexData_01[numVertices + 0].m_position = glm::vec3(frame.x, frame.y, 0.0f);
			vertexData_01[numVertices + 0].m_texcoord = CVertexBuffer::CompressVec2(glm::vec2(0.0f, 0.0f));
			vertexData_01[numVertices + 1].m_position = glm::vec3(frame.x, frame.w, 0.0f);
			vertexData_01[numVertices + 1].m_texcoord = CVertexBuffer::CompressVec2(glm::vec2(0.0f, 1.0f));
			vertexData_01[numVertices + 2].m_position = glm::vec3(frame.z, frame.y, 0.0f);
			vertexData_01[numVertices + 2].m_texcoord = CVertexBuffer::CompressVec2(glm::vec2(1.0f, 0.0f));
			vertexData_01[numVertices + 3].m_position = glm::vec3(frame.z, frame.w, 0.0f);
			vertexData_01[numVertices + 3].m_texcoord = CVertexBuffer::CompressVec2(glm::vec2(1.0f, 1.0f));

            numVertices += quad->Get_NumVertexes();
            numIndices += quad->Get_NumIndexes();
		}
            
        m_numPushedVertices = numVertices;
        m_numPushedIndices = numIndices;
        m_proccessed = 0;
        m_locked = 0;
        m_unlocked = 1;
	};
}

std::function<void(void)> CBatch::_UnlockPatriclesGeometry()
{
	return nullptr;
}

void CBatch::Unlock(void)
{
    if(m_locked == 1 && m_proccessed == 0 && m_unlocked == 0)
    {
        m_proccessed = 1;

		std::function<void(void)> unlock = nullptr;
		switch (m_geometryMode)
		{
		case E_BATCH_GEOMETRY_MODE_NONE:
			{
				assert(false);
			}
			break;
		case E_BATCH_GEOMETRY_MODE_MODELS:
			{
				unlock = _UnlockModelsGeometry();
				assert(m_matrices.size() == m_meshes.size());
			}
			break;
		case E_BATCH_GEOMETRY_MODE_CONTROLS:
			{
				unlock = _UnlockControlsGeometry();
				assert(m_positions.size() == m_sizes.size());
				assert(m_positions.size() == m_controls.size());
			}
			break;
		case E_BATCH_GEOMETRY_MODE_PARTICLES:
			{

			}
			break;
		default:
			{
				assert(false);
			}
			break;
		}

		assert(unlock != nullptr);
        
#if defined(__USE_GCDPP__)
        //gcdpp::impl::DispatchAsync(gcdpp::queue::GetGlobalQueue(gcdpp::queue::GCDPP_DISPATCH_QUEUE_PRIORITY_LOW), unlock);
        unlock();
#else
        unlock();
		//std::async(std::launch::async, unlock);
#endif
    }
}

void CBatch::Batch(const std::tuple<std::shared_ptr<CMesh>, std::shared_ptr<CAnimationMixer>>& _mesh, const glm::mat4x4 &_matrix)
{
    if(m_locked == 1 && m_proccessed == 0)
    {
        m_meshes.push_back(_mesh);
        m_matrices.push_back(_matrix);
        
        m_numBatchedVertices += std::get<0>(_mesh)->Get_NumVertexes();
        m_numBatchedIndices += std::get<0>(_mesh)->Get_NumIndexes();
        m_numBatchedTransformations += std::get<1>(_mesh)->Get_TransformationSize();
        
        std::shared_ptr<CAnimationMixer> mixer = std::get<1>(_mesh);
        for(ui32 i = 0; i < mixer->Get_TransformationSize(); ++i)
        {
            m_transformations.push_back(mixer->Get_Transformations()[i]);
        }
    }
}

void CBatch::Batch(const std::shared_ptr<CQuad>& _control, const glm::vec2& _position, const glm::vec2& _size)
{ 
	if(m_locked == 1 && m_proccessed == 0)
    {
		m_controls.push_back(_control);
		m_positions.push_back(_position);
		m_sizes.push_back(_size);
        
        m_numBatchedVertices += _control->Get_NumVertexes();
        m_numBatchedIndices += _control->Get_NumIndexes();
    }
}

i32 CBatch::_OnQueuePosition(void)
{
    return m_renderQueuePosition;
}

bool CBatch::_OnOcclusion(void)
{
    return false;
}

ui32 CBatch::_OnGet_NumTriangles(void)
{
    return 0;
}

void CBatch::_OnBind(const std::string& _mode)
{
    
}

void CBatch::_OnDraw(const std::string& _mode)
{
    assert(m_mesh != nullptr);
    assert(m_material != nullptr);
    assert(m_material->Get_Shader() != nullptr);
    
    if(m_unlocked == 1)
    {
        m_unlocked = 0;
        m_numVertices = m_numPushedVertices;
        m_numIndices = m_numPushedIndices;
        m_mesh->Get_VertexBuffer()->Unlock(m_numVertices);
        m_mesh->Get_IndexBuffer()->Unlock(m_numIndices);
    }
    
    if(m_numIndices != 0 && m_numVertices != 0)
    {
        m_material->Bind();
		m_materialImposer(m_material);
        
        m_mesh->Bind(m_material->Get_Shader()->Get_Attributes());
        m_mesh->Draw(m_numIndices);
        m_mesh->Unbind(m_material->Get_Shader()->Get_Attributes());
        
        m_material->Unbind();
    }
}

void CBatch::_OnUnbind(const std::string& _mode)
{
    
}

void CBatch::_OnDebugDraw(const std::string& _mode)
{
    
}

void CBatch::_OnBatch(const std::string& _mode)
{
    
}
