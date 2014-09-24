//
//  CAtmosphericScattering.cpp
//  ie3D-Core
//
//  Created by sergey.sergeev on 9/24/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#include "CAtmosphericScattering.h"
#include "CMaterial.h"
#include "CShader.h"
#include "CTexture.h"
#include "CCamera.h"
#include "CLight.h"
#include "CResourceAccessor.h"
#include "CConfigurationGameObjects.h"
#include "CMesh.h"
#include "CVertexBuffer.h"
#include "CIndexBuffer.h"

CAtmosphericScattering::CAtmosphericScattering(CSharedResourceAccessorRef resourceAccessor,
                                               ISharedRenderTechniqueAccessorRef renderTechniqueAccessor) :
IGameObject(resourceAccessor, renderTechniqueAccessor)
{
    m_zOrder = E_GAME_OBJECT_Z_ORDER_ATMOSPHERIC_SCATTERING;
}

CAtmosphericScattering::~CAtmosphericScattering(void)
{
    
}

void CAtmosphericScattering::onSceneUpdate(f32 deltatime)
{
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        IGameObject::setPosition(m_camera->Get_Position());
        IGameObject::onSceneUpdate(deltatime);
    }
}

void CAtmosphericScattering::onResourceLoaded(ISharedResourceRef resource, bool success)
{
    IGameObject::onResourceLoaded(resource, success);
}

void CAtmosphericScattering::onConfigurationLoaded(ISharedConfigurationRef configuration, bool success)
{
    IGameObject::onConfigurationLoaded(configuration, success);
    
    std::shared_ptr<CConfigurationSkyBox> skyBoxConfiguration = std::static_pointer_cast<CConfigurationSkyBox>(configuration);
    assert(m_resourceAccessor != nullptr);
    
    CSharedTexture xpositiveTexture = m_resourceAccessor->getTexture(skyBoxConfiguration->getXPositiveTextureFilename());
    CSharedTexture xnegativeTexture = m_resourceAccessor->getTexture(skyBoxConfiguration->getXNegativeTextureFilename());
    CSharedTexture ypositiveTexture = m_resourceAccessor->getTexture(skyBoxConfiguration->getYPositiveTextureFilename());
    CSharedTexture ynegativeTexture = m_resourceAccessor->getTexture(skyBoxConfiguration->getYNegativeTextureFilename());
    CSharedTexture zpositiveTexture = m_resourceAccessor->getTexture(skyBoxConfiguration->getZPositiveTextureFilename());
    CSharedTexture znegativeTexture = m_resourceAccessor->getTexture(skyBoxConfiguration->getZNegativeTextureFilename());
    
    CSharedCubemapTexture texture = CCubemapTexture::constructCustomCubemapTexture("skybox.cubemap.texture",
                                                                                   xpositiveTexture,
                                                                                   xnegativeTexture,
                                                                                   ypositiveTexture,
                                                                                   ynegativeTexture,
                                                                                   zpositiveTexture,
                                                                                   znegativeTexture);
    
    m_resourceAccessor->addCustomTexture("skybox.cubemap.texture", texture);
    
    for(const auto& iterator : m_materials)
    {
        CSharedMaterial material = iterator.second;
        material->setTexture(texture, E_SHADER_SAMPLER_01);
    }
    
    CSharedVertexBuffer vertexBuffer = std::make_shared<CVertexBuffer>(8, GL_STATIC_DRAW);
    SAttributeVertex* vertexData = vertexBuffer->lock();
    
    vertexData[0].m_position = glm::vec3(-1.0, -1.0,  1.0);
    vertexData[1].m_position = glm::vec3( 1.0, -1.0,  1.0);
    vertexData[2].m_position = glm::vec3( 1.0,  1.0,  1.0);
    vertexData[3].m_position = glm::vec3(-1.0,  1.0,  1.0);
    
    vertexData[4].m_position = glm::vec3(-1.0, -1.0, -1.0);
    vertexData[5].m_position = glm::vec3( 1.0, -1.0, -1.0);
    vertexData[6].m_position = glm::vec3( 1.0,  1.0, -1.0);
    vertexData[7].m_position = glm::vec3(-1.0,  1.0, -1.0);
    
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
    
    m_mesh = CMesh::constructCustomMesh("skyBox", vertexBuffer, indexBuffer,
                                        glm::vec3(4096.0), glm::vec3(4096.0));
    assert(m_mesh != nullptr);
    
    IGameObject::enableRender(m_isNeedToRender);
    IGameObject::enableUpdate(m_isNeedToUpdate);
    
    m_status |= E_LOADING_STATUS_TEMPLATE_LOADED;
}

i32 CSkyBox::zOrder(void)
{
    return m_zOrder;
}

bool CSkyBox::checkOcclusion(void)
{
    return false;
}

ui32 CSkyBox::numTriangles(void)
{
    return IGameObject::numTriangles();
}

void CSkyBox::onBind(const std::string& mode)
{
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        assert(m_materials.find(mode) != m_materials.end());
        IGameObject::onBind(mode);
    }
}

void CSkyBox::onDraw(const std::string& mode)
{
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        assert(m_camera != nullptr);
        assert(m_materials.find(mode) != m_materials.end());
        
        CSharedMaterial material = m_materials.find(mode)->second;
        assert(material->getShader() != nullptr);
        
        glm::vec3 currentRotation = m_rotation;
        if(material->isReflecting())
        {
            IGameObject::setRotation(glm::vec3(180.0, -glm::degrees(m_camera->Get_Rotation()) * 2.0, m_rotation.z));
            IGameObject::onSceneUpdate(0);
        }
        
        material->getShader()->setMatrix4x4(m_matrixWorld, E_SHADER_UNIFORM_MATRIX_WORLD);
        material->getShader()->setMatrix4x4(m_camera->Get_ProjectionMatrix(), E_SHADER_UNIFORM_MATRIX_PROJECTION);
        material->getShader()->setMatrix4x4(m_camera->Get_ViewMatrix(), E_SHADER_UNIFORM_MATRIX_VIEW);
        material->getShader()->setMatrix4x4(m_camera->Get_MatrixNormal(), E_SHADER_UNIFORM_MATRIX_NORMAL);
        
        material->getShader()->setVector3(m_camera->Get_Position(), E_SHADER_UNIFORM_VECTOR_CAMERA_POSITION);
        material->getShader()->setFloat(m_camera->Get_Near(), E_SHADER_UNIFORM_FLOAT_CAMERA_NEAR);
        material->getShader()->setFloat(m_camera->Get_Far(), E_SHADER_UNIFORM_FLOAT_CAMERA_FAR);
        
        IGameObject::onDraw(mode);
        
        if(material->isReflecting())
        {
            IGameObject::setRotation(currentRotation);
            IGameObject::onSceneUpdate(0);
        }
    }
}

void CSkyBox::onUnbind(const std::string& mode)
{
    if(m_status & E_LOADING_STATUS_TEMPLATE_LOADED)
    {
        assert(m_materials.find(mode) != m_materials.end());
        IGameObject::onUnbind(mode);
    }
}

void CSkyBox::onBatch(const std::string& mode)
{
    IGameObject::onBatch(mode);
}

CSkyDome::CSkyDome()
{
    m_LightRef = NULL;
    
    m_Cols = 32;
    m_Rows = 32;
    m_nSize = 8;
    m_nSamples = 2;
    m_fInnerRadius = 20.0f;
    
    m_fKr = 0.0025f;
    m_fKr4PI = m_fKr * 4.0f * D3DX_PI;
    m_fKm = 0.0010f;
    m_fKm4PI = m_fKm * 4.0f * D3DX_PI;
    m_fESun = 20.0f;
    m_fKrESun = m_fESun * m_fKr;
    m_fKmESun = m_fESun * m_fKm;
    m_fg = -0.990f;
    m_fg2 = m_fg * m_fg;
    m_fOuterRadius = m_fInnerRadius * 1.025f;
    m_fScale = 1.0f / (m_fOuterRadius - m_fInnerRadius);
    
    m_fRayleighScaleDepth = 0.25f;
    m_fScaleOverScaleDepth = m_fScale / m_fRayleighScaleDepth;
    m_vInvWavelength4.x = 1.0f / powf(0.650f, 4.0f);
    m_vInvWavelength4.y = 1.0f / powf(0.570f, 4.0f);
    m_vInvWavelength4.z = 1.0f / powf(0.475f, 4.0f);
    m_vHG.x = 1.5f * ( (1.0f - m_fg2) / (2.0f + m_fg2) );
    m_vHG.y = 1.0f + m_fg2;
    m_vHG.z = 2.0f * m_fg;
    m_vEye.x = 0.0f;
    m_vEye.y = m_fInnerRadius + 1.0e-6f;
    m_vEye.z = 0.0f;
    
    m_Name = "sky_dome";
}


void CSkyDome::Load(std::vector<SResource> _resource)
{
    m_LightRef = Game::GetEnviromentControllerInstance()->GetLightInstance();
    
    m_MeshList[m_Name] = new Core::CMesh();
    m_MeshList[m_Name]->m_VertexBuffer = Core::CGlobal::GetDevice()->CreateVertexBuffer();
    m_MeshList[m_Name]->m_IndexBuffer = Core::CGlobal::GetDevice()->CreateIndexBuffer();
    m_MeshList[m_Name]->m_TextureArray[0] = Core::CGlobal::GetDevice()->CreateTextureExt(m_nSize,m_nSize,Core::ITexture::BPP_16);
    
    SResource skyResource = *_resource.begin();
    m_MeshList[m_Name]->m_Shader = CResource::GetShaderControllerInstance()->Load(skyResource.m_ShaderFile);
    
    unsigned int numVertex = m_Cols * m_Rows;
    unsigned int numIndex = ( m_Cols - 1 ) * (m_Rows - 1) * 6;
    
    SVertex* vertexData = (SVertex*)m_MeshList[m_Name]->m_VertexBuffer->Load(numVertex,sizeof(SVertex),0);
    
    unsigned int index = 0;
    for( int i = 0; i < m_Cols; i++ )
    {
        const float moveXZ = cos( 1.0f ) * i / (m_Cols - 1) ;
        for( int j = 0; j < m_Rows; j++ )
        {
            const float moveY = (3.141592654f * 2.0f) * j / (m_Rows - 1) ;
            
            vertexData[index].m_vPosition.x = sin( moveXZ ) * cos( moveY  );
            vertexData[index].m_vPosition.y = cos( moveXZ );
            vertexData[index].m_vPosition.z = sin( moveXZ ) * sin( moveY  );
            vertexData[index].m_vPosition *= m_fOuterRadius;
            
            vertexData[index].m_vTexCoord.x = j / (m_Rows - 1.0f);
            vertexData[index].m_vTexCoord.y = i / (m_Cols - 1.0f);
            index++;
        }
    }
    m_MeshList[m_Name]->m_VertexBuffer->CommitToVRAM(0);
    
    
    unsigned int *indexData = m_MeshList[m_Name]->m_IndexBuffer->Load(numIndex);
    
    for( unsigned char i = 0; i < m_Rows - 1; i++)
    {
        for( unsigned char j = 0; j < m_Cols - 1; j++)
        {
            *(indexData++) = i * m_Rows + j;
            *(indexData++) = (i + 1) * m_Rows + j;
            *(indexData++) = (i + 1) * m_Rows + j + 1;
            
            *(indexData++) = (i + 1) * m_Rows + j + 1;
            *(indexData++) = i * m_Rows + j + 1;
            *(indexData++) = i * m_Rows + j;
        }
    }
    
    m_MeshList[m_Name]->m_IndexBuffer->CommitToVRAM();
    
    Core::IVertexBuffer::SVertexDeclaration declaration;
    declaration.m_Elements = new Core::IVertexBuffer::SElementDeclaration[2];
    
    declaration.m_Elements[0].m_stream = 0;
    declaration.m_Elements[0].m_index = 0;
    declaration.m_Elements[0].m_size = Core::IVertexBuffer::ELEMENT_FLOAT3;
    declaration.m_Elements[0].m_type = Core::IVertexBuffer::ELEMENT_POSITION;
    declaration.m_Elements[0].m_offset = 0;
    
    declaration.m_Elements[1].m_stream = 0;
    declaration.m_Elements[1].m_index = 0;
    declaration.m_Elements[1].m_size = Core::IVertexBuffer::ELEMENT_FLOAT2;
    declaration.m_Elements[1].m_type = Core::IVertexBuffer::ELEMENT_TEXCOORD;
    declaration.m_Elements[1].m_offset = 12;
    
    declaration.m_ElementCount = 2;
    
    m_MeshList[m_Name]->m_VertexBuffer->SetDeclaration(declaration);
}

math::Vector3d CSkyDome::expv( const math::Vector3d &v )
{
    return math::Vector3d( expf(v.x), expf(v.y), expf(v.z) );
}

float CSkyDome::scale(float fCos)
{
    float x = 1.0f - fCos;
    return m_fRayleighScaleDepth * exp(-0.00287f + x*(0.459f + x*(3.83f + x*(-6.80f + x*5.25f))));
}

void CSkyDome::RefreshAtmosphericTexture()
{
    D3DXVECTOR4_16F * pBuffer = (D3DXVECTOR4_16F *)m_MeshList[m_Name]->m_TextureArray[0]->Lock();
    unsigned int nIndex = 0;
    for( unsigned int x = 0; x < m_nSize; x++ )
    {
        const float fCosxz = cos( 1.0f ) * x / (float)(m_nSize - 1.0f);
        for( unsigned int y = 0; y < m_nSize; y++ )
        {
            const float fCosy = (D3DX_PI * 2.0f) * y / (float)(m_nSize - 1.0f);
            math::Vector3d vVecPos = math::Vector3d(0.0f, m_fInnerRadius + 1e-6f, 0.0f);
            
            vVecPos.x = sin( fCosxz ) * cos( fCosy  ) * m_fOuterRadius;
            vVecPos.y = cos( fCosxz ) * m_fOuterRadius;
            vVecPos.z = sin( fCosxz ) * sin( fCosy  ) * m_fOuterRadius;
            
            math::Vector3d v3Pos = vVecPos;
            math::Vector3d v3Ray = v3Pos - m_vEye;
            float fFar = v3Ray.length();
            v3Ray /= fFar;
            
            math::Vector3d v3Start = m_vEye;
            float fHeight = v3Start.length();
            float fDepth = exp(m_fScaleOverScaleDepth * (m_fInnerRadius - m_vEye.y));
            float fStartAngle = math::dot(v3Ray, v3Start)  / fHeight;
            float fStartOffset = fDepth * scale(fStartAngle);
            
            float fSampleLength = fFar / m_nSamples;
            float fScaledLength = fSampleLength * m_fScale;
            math::Vector3d v3SampleRay = v3Ray * fSampleLength;
            math::Vector3d v3SamplePoint = v3Start + v3SampleRay * 0.5f;
            
            math::Vector3d v3FrontColor = math::Vector3d(0.0f, 0.0f, 0.0f);
            for(unsigned int i=0; i<m_nSamples; i++)
            {
                float fHeight = v3SamplePoint.length();
                float fDepth = exp(m_fScaleOverScaleDepth * (m_fInnerRadius - fHeight));
                float fLightAngle = math::dot(m_LightRef->GetDirection(),v3SamplePoint) / fHeight;
                float fCameraAngle = math::dot(v3Ray, v3SamplePoint) / fHeight;
                float fScatter = (fStartOffset + fDepth*(scale(fLightAngle) - scale(fCameraAngle)));
                math::Vector3d v3Attenuate = expv((m_vInvWavelength4 * m_fKr4PI + math::Vector3d(m_fKm4PI, m_fKm4PI, m_fKm4PI)) * -fScatter);
                v3FrontColor += v3Attenuate * (fDepth * fScaledLength);
                v3SamplePoint += v3SampleRay;
            }
            
            if(x == m_nSize / 2)
                Game::GetEnviromentControllerInstance()->GetCameraInstance()->m_FogColor = v3FrontColor;
            
            pBuffer[nIndex].x = v3FrontColor.x;//min( v3FrontColor.x, 6.5519996e4f);
            pBuffer[nIndex].y = v3FrontColor.y;//min( v3FrontColor.y, 6.5519996e4f);
            pBuffer[nIndex].z = v3FrontColor.z;//min( v3FrontColor.z, 6.5519996e4f);
            pBuffer[nIndex].w = 0.0f;
            
            nIndex++;
        }
    }
    
    m_MeshList[m_Name]->m_TextureArray[0]->Unlock();
}

void CSkyDome::Update()
{
    Matrix();
    
    RefreshAtmosphericTexture();
    
    std::map<std::string,Core::CMesh*>::iterator meshIteratorBegin = m_MeshList.begin();
    std::map<std::string,Core::CMesh*>::iterator meshIteratorEnd = m_MeshList.end();
    
    while(meshIteratorBegin != meshIteratorEnd)
    {
        meshIteratorBegin->second->m_Shader->SetMatrix(m_mWorldViewProjection, "worldViewProj", Core::IShader::VS_SHADER);
        meshIteratorBegin->second->m_Shader->SetFloat(m_fKrESun , "fKrESun", Core::IShader::VS_SHADER );
        meshIteratorBegin->second->m_Shader->SetFloat( m_fKmESun, "fKmESun", Core::IShader::VS_SHADER );
        math::Vector3d temp = math::Vector3d( (float)m_nSize, 1.0f / m_nSize, 0.0f );
        meshIteratorBegin->second->m_Shader->SetVector( temp, "Tex", Core::IShader::VS_SHADER);
        meshIteratorBegin->second->m_Shader->SetVector( m_LightRef->GetDirection(),"vSunPos", Core::IShader::VS_SHADER);
        meshIteratorBegin->second->m_Shader->SetVector( m_vHG,"vHG", Core::IShader::VS_SHADER);
        meshIteratorBegin->second->m_Shader->SetVector( m_vInvWavelength4,"vInvWavelength", Core::IShader::VS_SHADER);
        meshIteratorBegin->second->m_Shader->SetVector( m_vEye, "vEye", Core::IShader::VS_SHADER);
        meshIteratorBegin->second->m_Shader->SetTexture(m_MeshList[m_Name]->m_TextureArray[0], "Texture_01" ,Core::IShader::VS_SHADER );
        meshIteratorBegin++;
    }
}

void CSkyDome::Render()
{
    Core::CGlobal::GetDevice()->SetCullFace(Core::IDevice::CULL_NONE);
    Core::CGlobal::GetDevice()->Ref()->SetRenderState(D3DRS_ZENABLE, FALSE);
    Core::CGlobal::GetDevice()->AlphaTestDisable();
    std::map<std::string,Core::CMesh*>::iterator meshIteratorBegin = m_MeshList.begin();
    std::map<std::string,Core::CMesh*>::iterator meshIteratorEnd = m_MeshList.end();
    
    while(meshIteratorBegin != meshIteratorEnd)
    {
        meshIteratorBegin->second->Draw();
        meshIteratorBegin++;
    }
    Core::CGlobal::GetDevice()->Ref()->SetRenderState(D3DRS_ZENABLE, TRUE);
}

