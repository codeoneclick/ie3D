//
//  CLandscape.cpp
//  iGaia
//
//  Created by sergey.sergeev on 1/16/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "CLandscape.h"

CLandscape::CLandscape(void)
{
    m_heightmapProcessor = new CHeightmapProcessor();

    m_splattingDiffuseMaterial = nullptr;
    m_splattingNormalMaterial = nullptr;
    m_landscapeEdges = nullptr;
}

CLandscape::~CLandscape(void)
{

}

void CLandscape::Load(CResourceMgrsFacade* _resourceMgrsFacade, CShaderComposite* _shaderComposite, const std::string& _filename)
{
    assert(_resourceMgrsFacade != nullptr);
    SLandscapeSettings* settings = _resourceMgrsFacade->LoadLandscapeSettings(_filename);
    assert(settings != nullptr);

    assert(m_heightmapProcessor != nullptr);
    m_heightmapProcessor->Process(settings->m_heightmapDataFileName, glm::vec2(settings->m_width, settings->m_height), settings->m_splattingDataFileName, glm::vec2(settings->m_width, settings->m_height));

    m_splattingDiffuseMaterial = new CMaterial(_shaderComposite->Get_Shader(E_SHADER_SCREEN_PLANE_LANDSCAPE_SPLATTING_PREPROCESS));
    m_splattingNormalMaterial = new CMaterial(_shaderComposite->Get_Shader(E_SHADER_SCREEN_PLANE_LANDSCAPE_SPLATTING_PREPROCESS));
    
    std::vector<const SMaterialSettings*> m_materialsSettings = settings->m_materialsSettings;
    for(const SMaterialSettings* materialSettings : m_materialsSettings)
    {
        assert(materialSettings->m_renderMode < E_RENDER_MODE_WORLD_SPACE_MAX && materialSettings->m_renderMode >= 0);
        assert(m_materials[materialSettings->m_renderMode] == nullptr);

        const SShaderSettings* shaderSettings = materialSettings->m_shaderSettings;

        assert(shaderSettings->m_guid >= 0 && shaderSettings->m_guid < E_SHADER_MAX);

        CShader* shader = _shaderComposite->Get_Shader(static_cast<E_SHADER>(shaderSettings->m_guid));
        m_materials[materialSettings->m_renderMode] = new CMaterial(shader);
        m_materials[materialSettings->m_renderMode]->Set_RenderState(E_RENDER_STATE_CULL_MODE, materialSettings->m_isCullFace);
        m_materials[materialSettings->m_renderMode]->Set_RenderState(E_RENDER_STATE_DEPTH_TEST, materialSettings->m_isDepthTest);
        m_materials[materialSettings->m_renderMode]->Set_RenderState(E_RENDER_STATE_DEPTH_MASK, materialSettings->m_isDepthMask);
        m_materials[materialSettings->m_renderMode]->Set_RenderState(E_RENDER_STATE_BLEND_MODE, materialSettings->m_isBlend);

        m_materials[materialSettings->m_renderMode]->Set_CullFaceMode(materialSettings->m_cullFaceMode);
        m_materials[materialSettings->m_renderMode]->Set_BlendFunctionSource(materialSettings->m_blendFunctionSource);
        m_materials[materialSettings->m_renderMode]->Set_BlendFunctionDest(materialSettings->m_blendFunctionDestination);

        for(const STextureSettings* textureSettings : materialSettings->m_texturesSettings)
        {
            CTexture* texture = _resourceMgrsFacade->LoadTexture(textureSettings->m_name).get();
            texture->Set_Wrap(textureSettings->m_wrap);
            assert(texture != nullptr);
            assert(textureSettings->m_slot < E_TEXTURE_SLOT_MAX);
            m_materials[materialSettings->m_renderMode]->Set_Texture(texture, static_cast<E_TEXTURE_SLOT>(textureSettings->m_slot));

            if(E_RENDER_MODE_WORLD_SPACE_COMMON == materialSettings->m_renderMode)
            {
                if(textureSettings->m_slot < E_TEXTURE_SLOT_MAX / 2)
                {
                    assert(m_splattingDiffuseMaterial != nullptr);
                    m_splattingDiffuseMaterial->Set_Texture(texture, static_cast<E_TEXTURE_SLOT>(textureSettings->m_slot));
                }
                else if(textureSettings->m_slot >= E_TEXTURE_SLOT_MAX / 2)
                {
                    assert(m_splattingNormalMaterial != nullptr);
                    m_splattingNormalMaterial->Set_Texture(texture, static_cast<E_TEXTURE_SLOT>(textureSettings->m_slot - E_TEXTURE_SLOT_MAX / 2));
                }
            }
        }
    }

    m_numChunkRows = m_heightmapProcessor->Get_NumChunkRows();
    m_numChunkCells = m_heightmapProcessor->Get_NumChunkCells();
    
    ui32 chunkWidth = m_heightmapProcessor->Get_ChunkWidth();
    ui32 chunkHeight = m_heightmapProcessor->Get_ChunkHeight();
    
    m_landscapeContainer = new CLandscapeChunk*[m_numChunkRows * m_numChunkCells];
    
    for(ui32 i = 0; i < m_numChunkRows; ++i)
    {
        for(ui32 j = 0; j < m_numChunkCells; ++j)
        {
            CMesh* mesh = m_heightmapProcessor->Get_Chunk(i, j);
            m_landscapeContainer[i + j * m_numChunkRows] = new CLandscapeChunk();
            m_landscapeContainer[i + j * m_numChunkRows]->Load(mesh, m_materials, chunkWidth, chunkHeight);
        }
    }

    assert(m_heightmapProcessor != nullptr);
    
    m_heightmapProcessor->PreprocessSplattingTexture();
    m_heightmapProcessor->PreprocessHeightmapTexture();

    CreateLandscapeEdges(_resourceMgrsFacade, _shaderComposite, settings);
}

void CLandscape::CreateLandscapeEdges(CResourceMgrsFacade* _resourceMgrsFacade, CShaderComposite* _shaderComposite, SLandscapeSettings* _settings)
{
    assert(m_heightmapProcessor != nullptr);

    m_landscapeEdges = new CLandscapeEdges();

    CShader* shader = _shaderComposite->Get_Shader(E_SHADER_LANDSCAPE_EDGES);
    CMaterial* landscapeEdgesMaterial = new CMaterial(shader);

    CTexture* texture = _resourceMgrsFacade->LoadTexture(_settings->m_edgesTextureFileName).get();
    texture->Set_Wrap(GL_REPEAT);
    assert(texture != nullptr);
    landscapeEdgesMaterial->Set_Texture(texture, E_TEXTURE_SLOT_01);
    landscapeEdgesMaterial->Set_Texture(m_heightmapProcessor->PreprocessEdgesMaskTexture(), E_TEXTURE_SLOT_02);

    landscapeEdgesMaterial->Set_RenderState(E_RENDER_STATE_CULL_MODE, false);
    landscapeEdgesMaterial->Set_RenderState(E_RENDER_STATE_DEPTH_TEST, true);
    landscapeEdgesMaterial->Set_RenderState(E_RENDER_STATE_DEPTH_MASK, true);
    landscapeEdgesMaterial->Set_RenderState(E_RENDER_STATE_BLEND_MODE,true);

    landscapeEdgesMaterial->Set_CullFaceMode(GL_FRONT);
    landscapeEdgesMaterial->Set_BlendFunctionSource(GL_SRC_ALPHA);
    landscapeEdgesMaterial->Set_BlendFunctionDest(GL_ONE_MINUS_SRC_ALPHA);
    
    m_landscapeEdges->Load(landscapeEdgesMaterial, m_heightmapProcessor->Get_Width(), m_heightmapProcessor->Get_Height(), glm::vec2(-8.0f, 8.0f));
}

void CLandscape::Set_Camera(CCamera* _camera)
{
    for(ui32 i = 0; i < m_numChunkRows; ++i)
    {
        for(ui32 j = 0; j < m_numChunkCells; ++j)
        {
            assert(m_landscapeContainer != nullptr);
            assert(m_landscapeContainer[i + j * m_numChunkRows] != nullptr);
            m_landscapeContainer[i + j * m_numChunkRows]->Set_Camera(_camera);
        }
    }
    assert(m_landscapeEdges != nullptr);
    m_landscapeEdges->Set_Camera(_camera);
}

void CLandscape::Set_Light(CLight* _light)
{
    for(ui32 i = 0; i < m_numChunkRows; ++i)
    {
        for(ui32 j = 0; j < m_numChunkCells; ++j)
        {
            assert(m_landscapeContainer != nullptr);
            assert(m_landscapeContainer[i + j * m_numChunkRows] != nullptr);
            m_landscapeContainer[i + j * m_numChunkRows]->Set_Light(_light);
        }
    }
    assert(m_landscapeEdges != nullptr);
    m_landscapeEdges->Set_Light(_light);
}

void CLandscape::Set_RenderMgr(CRenderMgr* _renderMgr)
{
    assert(m_heightmapProcessor != nullptr);
    m_heightmapProcessor->Set_RenderMgr(_renderMgr);
       
    assert(m_splattingDiffuseMaterial != nullptr);
    assert(m_splattingNormalMaterial != nullptr);


    m_heightmapProcessor->PreprocessSplattingDiffuseTexture(m_splattingDiffuseMaterial);
    m_heightmapProcessor->PreprocessSplattingNormalTexture(m_splattingNormalMaterial);

    for(ui32 i = 0; i < E_RENDER_MODE_WORLD_SPACE_MAX; ++i)
    {
        if(m_materials[i] != nullptr)
        {
            m_materials[i]->Set_Texture(m_heightmapProcessor->Get_DiffuseTexture(), E_TEXTURE_SLOT_01);
            m_materials[i]->Set_Texture(m_heightmapProcessor->Get_NormalTexture(), E_TEXTURE_SLOT_02);
        }
    }

    for(ui32 i = 0; i < m_numChunkRows; ++i)
    {
        for(ui32 j = 0; j < m_numChunkCells; ++j)
        {
            assert(m_landscapeContainer != nullptr);
            assert(m_landscapeContainer[i + j * m_numChunkRows] != nullptr);
            m_landscapeContainer[i + j * m_numChunkRows]->Set_RenderMgr(_renderMgr);
        }
    }
    assert(m_landscapeEdges != nullptr);
    m_landscapeEdges->Set_RenderMgr(_renderMgr);
}

void CLandscape::Set_UpdateMgr(CSceneUpdateMgr* _updateMgr)
{
    for(ui32 i = 0; i < m_numChunkRows; ++i)
    {
        for(ui32 j = 0; j < m_numChunkCells; ++j)
        {
            assert(m_landscapeContainer != nullptr);
            assert(m_landscapeContainer[i + j * m_numChunkRows] != nullptr);
            m_landscapeContainer[i + j * m_numChunkRows]->Set_UpdateMgr(_updateMgr);
        }
    }
    assert(m_landscapeEdges != nullptr);
    m_landscapeEdges->Set_UpdateMgr(_updateMgr);
}

void CLandscape::ListenRenderMgr(bool _value)
{
    for(ui32 i = 0; i < m_numChunkRows; ++i)
    {
        for(ui32 j = 0; j < m_numChunkCells; ++j)
        {
            assert(m_landscapeContainer != nullptr);
            assert(m_landscapeContainer[i + j * m_numChunkRows] != nullptr);
            m_landscapeContainer[i + j * m_numChunkRows]->ListenRenderMgr(_value);
        }
    }
    assert(m_landscapeEdges != nullptr);
    m_landscapeEdges->ListenRenderMgr(_value);
}

void CLandscape::ListenUpdateMgr(bool _value)
{
    for(ui32 i = 0; i < m_numChunkRows; ++i)
    {
        for(ui32 j = 0; j < m_numChunkCells; ++j)
        {
            assert(m_landscapeContainer != nullptr);
            assert(m_landscapeContainer[i + j * m_numChunkRows] != nullptr);
            m_landscapeContainer[i + j * m_numChunkRows]->ListenUpdateMgr(_value);
        }
    }
    assert(m_landscapeEdges != nullptr);
    m_landscapeEdges->ListenUpdateMgr(_value);
}

void CLandscape::OnResourceDidLoad(TSharedPtrResource _resource)
{
    for(ui32 i = 0; i < m_numChunkRows; ++i)
    {
        for(ui32 j = 0; j < m_numChunkCells; ++j)
        {
            assert(m_landscapeContainer != nullptr);
            assert(m_landscapeContainer[i + j * m_numChunkRows] != nullptr);
            m_landscapeContainer[i + j * m_numChunkRows]->OnResourceDidLoad(_resource);
        }
    }
}

void CLandscape::OnUpdate(f32 _deltatime)
{
    assert(false);
}

ui32 CLandscape::OnDrawIndex(void)
{
    assert(false);
    return 0;
}

void CLandscape::OnBind(E_RENDER_MODE_WORLD_SPACE _mode)
{
    assert(false);
}

void CLandscape::OnDraw(E_RENDER_MODE_WORLD_SPACE _mode)
{
    assert(false);
}

void CLandscape::OnUnbind(E_RENDER_MODE_WORLD_SPACE _mode)
{
    assert(false);
}



