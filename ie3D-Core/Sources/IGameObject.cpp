//
//  IGameObject.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/24/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "IGameObject.h"
#include "CCamera.h"
#include "CFrustum.h"
#include "CMesh.h"
#include "CShader.h"
#include "CTexture.h"
#include "CMaterial.h"
#include "IRenderTechniqueImporter.h"
#include "IRenderTechniqueAccessor.h"
#include "CSceneUpdateMgr.h"
#include "CResourceAccessor.h"
#include "CGlobalLightSource.h"
#include "CBatchingMgr.h"
#include "CBoundingBox.h"
#include "HShaders.h"
#include "CComponentTransformation.h"
#include "CConfigurationAccessor.h"

IGameObject::IGameObject(CSharedResourceAccessorRef resourceAccessor,
                         ISharedRenderTechniqueAccessorRef renderTechniqueAccessor) :
m_resourceAccessor(resourceAccessor),
m_zOrder(0),
m_mesh(nullptr),
m_configuration(nullptr),
m_camera(nullptr),
m_globalLightSource(nullptr),
m_boundingBox(nullptr),
m_boundingBoxMaterial(nullptr),
m_renderTechniqueAccessor(renderTechniqueAccessor),
m_renderTechniqueImporter(nullptr),
m_sceneUpdateMgr(nullptr),
m_materialBindImposer(nullptr),
m_isNeedToRender(false),
m_isNeedToUpdate(false),
m_isBatching(false),
m_occlusionQueryOngoing(false),
m_occlusionQueryVisible(true),
m_status(E_LOADING_STATUS_UNLOADED)
{
    m_materialBindImposer = [this](CSharedMaterialRef material)
    {
        bindBaseShaderUniforms(material);
        bindCustomShaderUniforms(material);
    };
    
    m_boundingBoxMaterialBindImposer = [this](CSharedMaterialRef material)
    {
        bindBaseShaderUniforms(material);
    };
    
    CSharedComponentTransformation transformationComponent = std::make_shared<CComponentTransformation>();
    IGameObject::addComponent(transformationComponent);
    
#if defined(__OCCLUSIOON_QUERY__)
    
#if defined(__OSX__)
    
    glGenQueries(1, &m_occlusionQueryHandler);
    
#elif defined(__IOS__)
    
    glGenQueriesEXT(1, &m_occlusionQueryHandler);
    
#endif
    
#endif
}

IGameObject::~IGameObject(void)
{
    
#if defined(__OCCLUSIOON_QUERY__)
    
#if defined(__OSX__)
    
    glDeleteQueries(1, &m_occlusionQueryHandler);
    
#elif defined(__IOS)
    
    glDeleteQueriesEXT(1, &m_occlusionQueryHandler);
    
#endif
    
#endif
    
    m_materials.clear();
}

void IGameObject::addComponent(ISharedComponentRef component)
{
    assert(component != nullptr && component->getClass() != E_COMPONENT_CLASS_UNDEFINED);
    m_components.insert(std::make_pair(component->getClass(), component));
}

void IGameObject::removeComponent(ISharedComponentRef component)
{
    assert(component != nullptr && component->getClass() != E_COMPONENT_CLASS_UNDEFINED);
    auto iterator = m_components.find(component->getClass());
    assert(iterator != m_components.end());
    m_components.erase(iterator);
}

bool IGameObject::isComponentExist(E_COMPONENT_CLASS componentClass) const
{
    assert(componentClass != E_COMPONENT_CLASS_UNDEFINED);
    auto iterator = m_components.find(componentClass);
    return iterator != m_components.end();
}

ISharedComponent IGameObject::getComponent(E_COMPONENT_CLASS componentClass) const
{
    assert(componentClass != E_COMPONENT_CLASS_UNDEFINED);
    auto iterator = m_components.find(componentClass);
    return iterator != m_components.end() ? iterator->second : nullptr;
}

bool IGameObject::getBoundingBox(void)
{
    if(m_mesh != nullptr && m_mesh->isLoaded() && m_boundingBox == nullptr && m_isNeedBoundingBox)
    {
        m_boundingBox = std::make_shared<CBoundingBox>(m_mesh->getMinBound(),
                                                       m_mesh->getMaxBound());
        
        m_boundingBoxMaterial = std::make_shared<CMaterial>();
        CSharedShader shader = CShader::constructCustomShader("boundingBox", ShaderBoundingBox_vert, ShaderBoundingBox_frag);
        assert(shader != nullptr);
        m_boundingBoxMaterial->setShader(shader);
        
        m_boundingBoxMaterial->setCulling(false);
        m_boundingBoxMaterial->setCullingMode(GL_BACK);
        
        m_boundingBoxMaterial->setBlending(false);
        m_boundingBoxMaterial->setBlendingFunctionSource(GL_SRC_ALPHA);
        m_boundingBoxMaterial->setBlendingFunctionDestination(GL_ONE);
        
        m_boundingBoxMaterial->setDepthTest(true);
        m_boundingBoxMaterial->setDepthMask(true);
        
        m_boundingBoxMaterial->setClipping(false);
        m_boundingBoxMaterial->setClippingPlane(glm::vec4(0.0, 0.0, 0.0, 0.0));
        
        m_boundingBoxMaterial->setReflecting(false);
        m_boundingBoxMaterial->setShadowing(false);
        m_boundingBoxMaterial->setDebugging(false);

        return true;
    }
    else if(m_mesh != nullptr && m_mesh->isLoaded() && m_boundingBox != nullptr && m_isNeedBoundingBox)
    {
        return true;
    }
    return false;
}

void IGameObject::onSceneUpdate(f32 deltatime)
{

}

void IGameObject::onResourceLoaded(ISharedResourceRef resource,
                                   bool success)
{
    IResourceLoadingHandler::onResourceLoaded(resource, success);
}

void IGameObject::onConfigurationLoaded(ISharedConfigurationRef configuration,
                                        bool success)
{
    m_configuration = configuration;
    std::shared_ptr<IConfigurationGameObject> configurationGameObject = std::static_pointer_cast<IConfigurationGameObject>(configuration);
    for(const auto& iterator : configurationGameObject->getMaterialsConfigurations())
    {
        std::shared_ptr<CConfigurationMaterial> configurationMaterial = std::static_pointer_cast<CConfigurationMaterial>(iterator);
        CSharedMaterial material =  CMaterial::constructCustomMaterial(configurationMaterial,
                                                                       m_resourceAccessor,
                                                                       m_renderTechniqueAccessor,
                                                                       shared_from_this());
        m_materials.insert(std::make_pair(configurationMaterial->getRenderOperationName(), material));
    }
}

i32 IGameObject::zOrder(void)
{
    return 0;
}

bool IGameObject::checkOcclusion(void)
{
    
#if defined(__OCCLUSIOON_QUERY__)
    
    return !m_occlusionQueryVisible;
    
#else
    
    assert(m_cameraFrustum != nullptr);
    glm::vec3 maxBound = IGameObject::getMaxBound() + IGameObject::getPosition();
    glm::vec3 minBound = IGameObject::getMinBound() + IGameObject::getPosition();
    return !m_cameraFrustum->isBoundBoxInFrustum(maxBound, minBound);
    
#endif
    
}

ui32 IGameObject::numTriangles(void)
{
    return m_mesh && m_mesh->isLoaded() ? m_mesh->getNumIndices() / 3 : 0;
}

void IGameObject::onDrawBoundingBox(void)
{
    if(m_isNeedBoundingBox && IGameObject::getBoundingBox())
    {
        m_boundingBoxMaterial->bind();
        m_boundingBox->bind(m_boundingBoxMaterial->getShader()->getAttributes(), true);
        m_boundingBoxMaterialBindImposer(m_boundingBoxMaterial);
        m_boundingBox->draw(true);
        m_boundingBoxMaterial->unbind();
        m_boundingBox->unbind(m_boundingBoxMaterial->getShader()->getAttributes(), true);
    }
}

void IGameObject::onBind(const std::string& techniqueName)
{
    assert(m_mesh != nullptr);
    assert(m_camera != nullptr);
    assert(m_globalLightSource != nullptr);
    assert(m_materials.find(techniqueName) != m_materials.end());
    
    auto iterator = m_materials.find(techniqueName);
    if(!m_isBatching &&
       iterator->second->getShader()->isLoaded())
    {
        iterator->second->bind();
        m_mesh->bind(iterator->second->getShader()->getAttributes());
        m_materialBindImposer(iterator->second);
    }
}

void IGameObject::onDraw(const std::string& techniqueName)
{
    assert(m_mesh != nullptr);
    assert(m_camera != nullptr);
    assert(m_globalLightSource != nullptr);
    assert(m_materials.find(techniqueName) != m_materials.end());
    
    auto iterator = m_materials.find(techniqueName);
    if(!m_isBatching &&
       iterator->second->getShader()->isLoaded())
    {
        m_mesh->draw();
    }
}

void IGameObject::onUnbind(const std::string& techniqueName)
{
    assert(m_mesh != nullptr);
    assert(m_camera != nullptr);
    assert(m_globalLightSource != nullptr);
    assert(m_materials.find(techniqueName) != m_materials.end());
    
    auto iterator = m_materials.find(techniqueName);
    if(!m_isBatching &&
       iterator->second->getShader()->isLoaded())
    {
        iterator->second->unbind();
        m_mesh->unbind(iterator->second->getShader()->getAttributes());
    }
}

void IGameObject::onBatch(const std::string& techniqueName)
{
    assert(m_materials.find(techniqueName) != m_materials.end());
    CSharedMaterial material = m_materials.find(techniqueName)->second;
    assert(material->getShader() != nullptr);
    if(m_mesh != nullptr &&
       m_mesh->isLoaded() &&
       m_isBatching)
    {
        m_renderTechniqueAccessor->getBatchingMgr()->batch(techniqueName,
                                                           m_zOrder,
                                                           m_mesh,
                                                           material,
                                                           m_materialBindImposer,
                                                           IGameObject::getTransformation());
    }
}

void IGameObject::bindBaseShaderUniforms(CSharedMaterialRef material)
{
    assert(material != nullptr);
    
    // base matrices
    material->getShader()->setMatrix4x4(m_camera->Get_ProjectionMatrix(), E_SHADER_UNIFORM_MATRIX_PROJECTION);
    material->getShader()->setMatrix4x4(!material->isReflecting() ? m_camera->Get_ViewMatrix() : m_camera->Get_ViewReflectionMatrix(), E_SHADER_UNIFORM_MATRIX_VIEW);
    material->getShader()->setMatrix4x4(m_camera->Get_MatrixNormal(), E_SHADER_UNIFORM_MATRIX_NORMAL);
    material->getShader()->setMatrix4x4(m_isBatching ? glm::mat4x4(1.0) : IGameObject::getTransformation(), E_SHADER_UNIFORM_MATRIX_WORLD);
    
    // camera base parameters
    material->getShader()->setVector3(m_camera->Get_Position(), E_SHADER_UNIFORM_VECTOR_CAMERA_POSITION);
    material->getShader()->setFloat(m_camera->Get_Near(), E_SHADER_UNIFORM_FLOAT_CAMERA_NEAR);
    material->getShader()->setFloat(m_camera->Get_Far(), E_SHADER_UNIFORM_FLOAT_CAMERA_FAR);
    material->getShader()->setVector4(material->getClippingPlane(), E_SHADER_UNIFORM_VECTOR_CLIP_PLANE);
    
    // global light parameters
    material->getShader()->setVector3(m_globalLightSource->getPosition(),
                                      E_SHADER_UNIFORM_VECTOR_GLOBAL_LIGHT_POSITION);
    material->getShader()->setMatrix4x4(m_globalLightSource->getProjectionMatrix(),
                                        E_SHADER_UNIFORM_MATRIX_GLOBAL_LIGHT_PROJECTION);
    material->getShader()->setMatrix4x4(m_globalLightSource->getViewMatrix(),
                                        E_SHADER_UNIFORM_MATRIX_GLOBAL_LIGHT_VIEW);
}

void IGameObject::bindCustomShaderUniforms(CSharedMaterialRef material)
{
    assert(material != nullptr);
    std::map<std::string, CSharedShaderUniform> customShaderUniforms = material->getCustomUniforms();
    CSharedShaderUniform currentUniform = nullptr;
    for(const auto& iterator : customShaderUniforms)
    {
        currentUniform = iterator.second;
        assert(currentUniform != nullptr);
        switch (currentUniform->getClass())
        {
            case E_UNIFORM_CLASS_MAT4X4:
            {
                material->getShader()->setMatrix4x4Custom(currentUniform->getMatrix4x4(), iterator.first);
            }
                break;
                
            case E_UNIFORM_CLASS_MAT3X3:
            {
                material->getShader()->setMatrix3x3Custom(currentUniform->getMatrix3x3(), iterator.first);
            }
                break;
                
            case E_UNIFORM_CLASS_VECTOR4:
            {
                material->getShader()->setVector4Custom(currentUniform->getVector4(), iterator.first);
            }
                break;
                
            case E_UNIFORM_CLASS_VECTOR3:
            {
                 material->getShader()->setVector3Custom(currentUniform->getVector3(), iterator.first);
            }
                break;
                
            case E_UNIFORM_CLASS_VECTOR2:
            {
                material->getShader()->setVector2Custom(currentUniform->getVector2(), iterator.first);
            }
                break;
                
            case E_UNIFORM_CLASS_FLOAT:
            {
                material->getShader()->setFloatCustom(currentUniform->getFloat(), iterator.first);
            }
                break;
                
            case E_UNIFORM_CLASS_INT:
            {
                material->getShader()->setIntCustom(currentUniform->getInt(), iterator.first);
            }
                break;
                
            default:
                break;
        }
    }
}

void IGameObject::setPosition(const glm::vec3& position)
{
    CSharedComponentTransformation component = std::static_pointer_cast<CComponentTransformation>(m_components.at(E_COMPONENT_CLASS_TRANSFORMATION));
    component->setPosition(position);
}

glm::vec3 IGameObject::getPosition(void) const
{
    CSharedComponentTransformation component = std::static_pointer_cast<CComponentTransformation>(m_components.at(E_COMPONENT_CLASS_TRANSFORMATION));
    return component->getPosition();
}

void IGameObject::setRotation(const glm::vec3& rotation)
{
    CSharedComponentTransformation component = std::static_pointer_cast<CComponentTransformation>(m_components.at(E_COMPONENT_CLASS_TRANSFORMATION));
    component->setRotation(rotation);
}

glm::vec3 IGameObject::getRotation(void) const
{
    CSharedComponentTransformation component = std::static_pointer_cast<CComponentTransformation>(m_components.at(E_COMPONENT_CLASS_TRANSFORMATION));
    return component->getRotation();
}

void IGameObject::setScale(const glm::vec3& scale)
{
    CSharedComponentTransformation component = std::static_pointer_cast<CComponentTransformation>(m_components.at(E_COMPONENT_CLASS_TRANSFORMATION));
    component->setScale(scale);
}

glm::vec3 IGameObject::getScale(void) const
{
    CSharedComponentTransformation component = std::static_pointer_cast<CComponentTransformation>(m_components.at(E_COMPONENT_CLASS_TRANSFORMATION));
    return component->getScale();
}

glm::mat4x4 IGameObject::getTransformation(void) const
{
    CSharedComponentTransformation component = std::static_pointer_cast<CComponentTransformation>(m_components.at(E_COMPONENT_CLASS_TRANSFORMATION));
    return component->getTransformation();
}

glm::vec3 IGameObject::getMaxBound(void) const
{
    return m_mesh && m_mesh->isLoaded() ? m_mesh->getMaxBound() * IGameObject::getScale() : glm::vec3(0.0f);
}

glm::vec3 IGameObject::getMinBound(void) const
{
    return m_mesh && m_mesh->isLoaded() ? m_mesh->getMinBound() * IGameObject::getScale() : glm::vec3(0.0f);
}

void IGameObject::setCamera(CSharedCameraRef camera)
{
    m_camera = camera;
}

void IGameObject::setCameraFrustum(CSharedFrustumRef frustum)
{
    m_cameraFrustum = frustum;
}

void IGameObject::setGlobalLightSource(CSharedGlobalLightSourceRef lightSource)
{
    m_globalLightSource = lightSource;
}

CSharedVertexBuffer IGameObject::getVertexBuffer(void) const
{
    return m_mesh && m_mesh->isCommited() ? m_mesh->getVertexBuffer() : nullptr;
}

CSharedIndexBuffer IGameObject::getIndexBuffer(void) const
{
    return m_mesh && m_mesh->isCommited() ? m_mesh->getIndexBuffer() : nullptr;
}

CSharedMaterial IGameObject::getMaterial(const std::string& renderTechique) const
{
    const auto& material = m_materials.find(renderTechique);
    assert(material != m_materials.end());
    return material->second;
}

void IGameObject::setCustomShaderUniform(const glm::mat4x4& matrix, const std::string& uniform, const std::string& renderTechnique)
{
    if(renderTechnique.length() != 0)
    {
        assert(m_materials.find(renderTechnique) != m_materials.end());
        auto iterator = m_materials.find(renderTechnique);
        iterator->second->setCustomShaderUniform(matrix, uniform);
    }
    else
    {
        for(const auto& iterator : m_materials)
        {
            iterator.second->setCustomShaderUniform(matrix, uniform);
        }
    }
}

void IGameObject::setCustomShaderUniform(const glm::mat3x3& matrix, const std::string& uniform, const std::string& renderTechnique)
{
    if(renderTechnique.length() != 0)
    {
        assert(m_materials.find(renderTechnique) != m_materials.end());
        auto iterator = m_materials.find(renderTechnique);
        iterator->second->setCustomShaderUniform(matrix, uniform);
    }
    else
    {
        for(const auto& iterator : m_materials)
        {
            iterator.second->setCustomShaderUniform(matrix, uniform);
        }
    }
}

void IGameObject::setCustomShaderUniform(const glm::vec4& vector, const std::string& uniform, const std::string& renderTechnique)
{
    if(renderTechnique.length() != 0)
    {
        assert(m_materials.find(renderTechnique) != m_materials.end());
        auto iterator = m_materials.find(renderTechnique);
        iterator->second->setCustomShaderUniform(vector, uniform);
    }
    else
    {
        for(const auto& iterator : m_materials)
        {
            iterator.second->setCustomShaderUniform(vector, uniform);
        }
    }
}

void IGameObject::setCustomShaderUniform(const glm::vec3& vector, const std::string& uniform, const std::string& renderTechnique)
{
    if(renderTechnique.length() != 0)
    {
        assert(m_materials.find(renderTechnique) != m_materials.end());
        auto iterator = m_materials.find(renderTechnique);
        iterator->second->setCustomShaderUniform(vector, uniform);
    }
    else
    {
        for(const auto& iterator : m_materials)
        {
            iterator.second->setCustomShaderUniform(vector, uniform);
        }
    }
}

void IGameObject::setCustomShaderUniform(const glm::vec2& vector, const std::string& uniform, const std::string& renderTechnique)
{
    if(renderTechnique.length() != 0)
    {
        assert(m_materials.find(renderTechnique) != m_materials.end());
        auto iterator = m_materials.find(renderTechnique);
        iterator->second->setCustomShaderUniform(vector, uniform);
    }
    else
    {
        for(const auto& iterator : m_materials)
        {
            iterator.second->setCustomShaderUniform(vector, uniform);
        }
    }
}

void IGameObject::setCustomShaderUniform(f32 value, const std::string& uniform, const std::string& renderTechnique)
{
    if(renderTechnique.length() != 0)
    {
        assert(m_materials.find(renderTechnique) != m_materials.end());
        auto iterator = m_materials.find(renderTechnique);
        iterator->second->setCustomShaderUniform(value, uniform);
    }
    else
    {
        for(const auto& iterator : m_materials)
        {
            iterator.second->setCustomShaderUniform(value, uniform);
        }
    }
}

void IGameObject::setCustomShaderUniform(i32 value, const std::string& uniform, const std::string& renderTechnique)
{
    if(renderTechnique.length() != 0)
    {
        assert(m_materials.find(renderTechnique) != m_materials.end());
        auto iterator = m_materials.find(renderTechnique);
        iterator->second->setCustomShaderUniform(value, uniform);
    }
    else
    {
        for(const auto& iterator : m_materials)
        {
            iterator.second->setCustomShaderUniform(value, uniform);
        }
    }
}

CSharedVertexBuffer IGameObject::getCollisionVertexBuffer(void) const
{
    return nullptr;
}

CSharedIndexBuffer IGameObject::getCollisionIndexBuffer(void) const
{
    return nullptr;
}

void IGameObject::setTexture(CSharedTextureRef texture,
                             E_SHADER_SAMPLER sampler,
                             const std::string& renderTechnique)
{
    if(renderTechnique.length() != 0)
    {
        assert(m_materials.find(renderTechnique) != m_materials.end());
        auto iterator = m_materials.find(renderTechnique);
        iterator->second->setTexture(texture, sampler);
    }
    else
    {
        for(const auto& iterator : m_materials)
        {
            iterator.second->setTexture(texture, sampler);
        }
    }
    texture->addLoadingHandler(shared_from_this());
}

void IGameObject::removeLoadingDependencies(void)
{
    for(const auto& iterator : m_materials)
    {
        for(ui32 i = E_SHADER_SAMPLER_01; i < E_SHADER_SAMPLER_MAX; ++i)
        {
            CSharedTexture texture = iterator.second->getTexture(static_cast<E_SHADER_SAMPLER>(i));
            if(texture != nullptr)
            {
                texture->removeLoadingHandler(shared_from_this());
            }
            
            CSharedShader shader = iterator.second->getShader();
            if(shader != nullptr)
            {
                shader->removeLoadingHandler(shared_from_this());
            }
        }
    }
    if(m_mesh != nullptr)
    {
        m_mesh->removeLoadingHandler(shared_from_this());
    }
}

void IGameObject::setClippingPlane(const glm::vec4& clippingPlane,
                      const std::string& renderTechnique)
{
    assert(m_materials.find(renderTechnique) != m_materials.end());
    auto iterator = m_materials.find(renderTechnique);
    iterator->second->setClippingPlane(clippingPlane);
}

void IGameObject::setRenderTechniqueImporter(ISharedRenderTechniqueImporterRef techniqueImporter)
{
    m_renderTechniqueImporter = techniqueImporter;
}

void IGameObject::setRenderTechniqueAccessor(ISharedRenderTechniqueAccessorRef techniqueAccessor)
{
    m_renderTechniqueAccessor = techniqueAccessor;
}

void IGameObject::setSceneUpdateMgr(CSharedSceneUpdateMgrRef sceneUpdateMgr)
{
    m_sceneUpdateMgr = sceneUpdateMgr;
}

void IGameObject::enableRender(bool value)
{
    for(const auto& iterator : m_materials)
    {
		if(m_renderTechniqueImporter != nullptr &&
           m_renderTechniqueImporter->isSupportingRenderTechnique(iterator.first))
		{
			value == true ? m_renderTechniqueImporter->addRenderTechniqueHandler(iterator.first, shared_from_this()) :
			m_renderTechniqueImporter->removeRenderTechniqueHandler(iterator.first, shared_from_this());
#if defined(__OCCLUSIOON_QUERY__)
            
            value == true ? m_renderTechniqueImporter->addToOcluddingQuery(iterator.first, shared_from_this()) :
            m_renderTechniqueImporter->removeFromOcluddingQuery(iterator.first, shared_from_this());
            
#endif
		}
    }
	m_isNeedToRender = value;
}

void IGameObject::enableUpdate(bool value)
{
    if(m_sceneUpdateMgr != nullptr)
    {
        value == true ? m_sceneUpdateMgr->RegisterSceneUpdateHandler(shared_from_this()) :
        m_sceneUpdateMgr->UnregisterSceneUpdateHandler(shared_from_this());
    }
    m_isNeedToUpdate = value;
}

void IGameObject::onOcclusionQueryDraw(CSharedMaterialRef material)
{
    
#if not defined(__OCCLUSIOON_QUERY__)
    
    assert(false);
    
#endif
    
    if(m_boundingBox != nullptr && !m_occlusionQueryOngoing)
    {
        assert(material != nullptr);
        
        material->getShader()->setMatrix4x4(m_camera->Get_ProjectionMatrix(), E_SHADER_UNIFORM_MATRIX_PROJECTION);
        material->getShader()->setMatrix4x4(!material->isReflecting() ? m_camera->Get_ViewMatrix() : m_camera->Get_ViewReflectionMatrix(), E_SHADER_UNIFORM_MATRIX_VIEW);
        material->getShader()->setMatrix4x4(m_camera->Get_MatrixNormal(), E_SHADER_UNIFORM_MATRIX_NORMAL);
        material->getShader()->setMatrix4x4(m_isBatching ? glm::mat4x4(1.0) : IGameObject::getTransformation(), E_SHADER_UNIFORM_MATRIX_WORLD);
        
        m_occlusionQueryOngoing = true;
        m_boundingBox->bind(material->getShader()->getAttributes(), false);
#if defined(__OSX__)
        
        glBeginQuery(GL_SAMPLES_PASSED, m_occlusionQueryHandler);
        
#elif defined(__IOS__)
        
        glBeginQueryEXT(GL_ANY_SAMPLES_PASSED_EXT, m_occlusionQueryHandler);
        
#endif
        m_boundingBox->draw(false);
#if defined(__OSX__)
        
        glEndQuery(GL_SAMPLES_PASSED);
        
#elif defined(__IOS__)
        
        glEndQueryEXT(GL_ANY_SAMPLES_PASSED_EXT);
        
#endif
        m_boundingBox->unbind(material->getShader()->getAttributes(), false);
    }
}

void IGameObject::onOcclusionQueryUpdate(void)
{
    
#if not defined(__OCCLUSIOON_QUERY__)
    
    assert(false);
    
#endif
    
    if(m_boundingBox != nullptr)
    {
#if defined(__OSX__)
        
        GLint available = GL_FALSE;
        glGetQueryObjectiv(m_occlusionQueryHandler, GL_QUERY_RESULT_AVAILABLE, &available);
        
#elif defined(__IOS__)
        
        GLuint available = GL_FALSE;
        glGetQueryObjectuivEXT(m_occlusionQueryHandler, GL_QUERY_RESULT_AVAILABLE_EXT, &available);
        
#endif
        if (available == GL_TRUE)
        {
            m_occlusionQueryOngoing = false;
            
#if defined(__OSX__)
            
            GLint samplesPassed = 0;
            glGetQueryObjectiv(m_occlusionQueryHandler, GL_QUERY_RESULT, &samplesPassed);
            
#elif defined(__IOS__)
            
            GLuint samplesPassed = 0;
            glGetQueryObjectuivEXT(m_occlusionQueryHandler, GL_QUERY_RESULT_EXT, &samplesPassed);
            
#endif
            if (samplesPassed > 0)
            {
                m_occlusionQueryVisible = true;
            }
            else
            {
                m_occlusionQueryVisible = false;
            }
        }
    }
}
