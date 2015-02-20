//
//  HDeclarations.h
//  indieEngineCore
//
//  Created by Sergey Sergeev on 4/7/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#ifndef HDeclarations_h
#define HDeclarations_h

#include "HCommon.h"
#include "HConfigurationDeclarations.h"

class CTexture;
typedef std::shared_ptr<CTexture> CSharedTexture;
typedef const CSharedTexture& CSharedTextureRef;

class CShader;
typedef std::shared_ptr<CShader> CSharedShader;
typedef const CSharedShader& CSharedShaderRef;

class CMesh;
typedef std::shared_ptr<CMesh> CSharedMesh;
typedef const CSharedMesh& CSharedMeshRef;

class CSkeleton;
typedef std::shared_ptr<CSkeleton> CSharedSkeleton;
typedef const CSharedSkeleton& CSharedSkeletonRef;

class CAnimationSequence;
typedef std::shared_ptr<CAnimationSequence> CSharedAnimationSequence;
typedef const CSharedAnimationSequence& CSharedAnimationSequenceRef;

class CResourceLoader;
typedef std::shared_ptr<CResourceLoader> CSharedResourceLoader;
typedef const CSharedResourceLoader& CSharedResourceLoaderRef;

class CMeshData;
typedef std::shared_ptr<CMeshData> CSharedMeshData;
typedef const CSharedMeshData& CSharedMeshDataRef;

class CFrameData;
typedef std::shared_ptr<CFrameData> CSharedFrameData;
typedef const CSharedFrameData& CSharedFrameDataRef;

class CSequenceData;
typedef std::shared_ptr<CSequenceData> CSharedSequenceData;
typedef const CSharedSequenceData& CSharedSequenceDataRef;

struct SVertexData;
struct SAttributeVertex;

class CVertexBuffer;
typedef std::shared_ptr<CVertexBuffer> CSharedVertexBuffer;
typedef const CSharedVertexBuffer& CSharedVertexBufferRef;

class CIndexBuffer;
typedef std::shared_ptr<CIndexBuffer> CSharedIndexBuffer;
typedef const CSharedIndexBuffer& CSharedIndexBufferRef;

class CBone;
struct SBoneData;
typedef std::shared_ptr<CBone> CSharedBone;
typedef const CSharedBone& CSharedBoneRef;

class CBoneData;
typedef std::shared_ptr<CBoneData> CSharedBoneData;
typedef const CSharedBoneData& CSharedBoneDataRef;

class CSkeletonData;
typedef std::shared_ptr<CSkeletonData> CSharedSkeletonData;
typedef const CSharedSkeletonData& CSharedSkeletonDataRef;

class IResource;
typedef std::shared_ptr<IResource> ISharedResource;
typedef const ISharedResource& ISharedResourceRef;

class IResourceData;
typedef std::shared_ptr<IResourceData> ISharedResourceData;
typedef const ISharedResourceData& ISharedResourceDataRef;

class CTextureData;
typedef std::shared_ptr<CTextureData> CSharedTextureData;
typedef const CSharedTextureData& CSharedTextureDataRef;

class CShaderData;
typedef std::shared_ptr<CShaderData> CSharedShaderData;
typedef const CSharedShaderData& CSharedShaderDataRef;

class CShaderUniform;
typedef std::shared_ptr<CShaderUniform> CSharedShaderUniform;
typedef const CSharedShaderUniform& CSharedShaderUniformRef;

class IResourceCommiter;
typedef std::shared_ptr<IResourceCommiter> ISharedResourceCommiter;
typedef const ISharedResourceCommiter& ISharedResourceCommiterRef;

class IResourceSerializer;
typedef std::shared_ptr<IResourceSerializer> ISharedResourceSerializer;
typedef const ISharedResourceSerializer& ISharedResourceSerializerRef;

class IResourceLoadingOperation;
typedef std::shared_ptr<IResourceLoadingOperation> ISharedResourceLoadingOperation;
typedef const ISharedResourceLoadingOperation& ISharedResourceLoadingOperationRef;

class IResourceLoadingHandler;
typedef std::shared_ptr<IResourceLoadingHandler> ISharedResourceLoadingHandler;
typedef const ISharedResourceLoadingHandler& ISharedResourceLoadingHandlerRef;

class ISceneUpdateHandler;
typedef std::shared_ptr<ISceneUpdateHandler> ISharedSceneUpdateHandler;
typedef const ISharedSceneUpdateHandler& ISharedSceneUpdateHandlerRef;

class CConfigurationAttribute;
typedef std::shared_ptr<CConfigurationAttribute> CSharedConfigurationAttribute;
typedef const CSharedConfigurationAttribute& CSharedConfigurationAttributeRef;

class IConfiguration;
typedef std::shared_ptr<IConfiguration> ISharedConfiguration;
typedef const ISharedConfiguration& ISharedConfigurationRef;

class IConfigurationLoadingHandler;
typedef std::shared_ptr<IConfigurationLoadingHandler> ISharedConfigurationLoadingHandler;
typedef const ISharedConfigurationLoadingHandler& ISharedConfigurationLoadingHandlerRef;

class CMaterial;
typedef std::shared_ptr<CMaterial> CSharedMaterial;
typedef const CSharedMaterial& CSharedMaterialRef;

class CMaterialCachedParameters;
typedef std::shared_ptr<CMaterialCachedParameters> CSharedMaterialCachedParameters;
typedef const CSharedMaterialCachedParameters& CSharedMaterialCachedParametersRef;

class CCamera;
typedef std::shared_ptr<CCamera> CSharedCamera;
typedef const CSharedCamera& CSharedCameraRef;

class CGlobalLightSource;
typedef std::shared_ptr<CGlobalLightSource> CSharedGlobalLightSource;
typedef const CSharedGlobalLightSource& CSharedGlobalLightSourceRef;

class CSceneUpdateMgr;
typedef std::shared_ptr<CSceneUpdateMgr> CSharedSceneUpdateMgr;
typedef const CSharedSceneUpdateMgr& CSharedSceneUpdateMgrRef;

class CResourceAccessor;
typedef std::shared_ptr<CResourceAccessor> CSharedResourceAccessor;
typedef const CSharedResourceAccessor& CSharedResourceAccessorRef;

class CAnimationMixer;
typedef std::shared_ptr<CAnimationMixer> CSharedAnimationMixer;
typedef const CSharedAnimationMixer& CSharedAnimationMixerRef;

class CBatch;
typedef std::shared_ptr<CBatch> CSharedBatch;
typedef const CSharedBatch& CSharedBatchRef;

class CBatchingMgr;
typedef std::shared_ptr<CBatchingMgr> CSharedBatchingMgr;
typedef const CSharedBatchingMgr& CSharedBatchingMgrRef;

class CQuadTree;
typedef std::shared_ptr<CQuadTree> CSharedQuadTree;
typedef const CSharedQuadTree& CSharedQuadTreeRef;

class CFrustum;
typedef std::shared_ptr<CFrustum> CSharedFrustum;
typedef const CSharedFrustum& CSharedFrustumRef;

class CHeightmapGenerator;
typedef std::shared_ptr<CHeightmapGenerator> CSharedHeightmapGenerator;
typedef const CSharedHeightmapGenerator& CSharedHeightmapGeneratorRef;

class CLandscapeChunk;
typedef std::shared_ptr<CLandscapeChunk> CSharedLandscapeChunk;
typedef const CSharedLandscapeChunk& CSharedLandscapeChunkRef;

class IOGLWindow;
typedef std::shared_ptr<IOGLWindow> ISharedOGLWindow;
typedef const ISharedOGLWindow& ISharedOGLWindowRef;

class IInputContext;
typedef std::shared_ptr<IInputContext> ISharedInputContext;
typedef const ISharedInputContext& ISharedInputContextRef;

class IGestureRecognizerHandler;
typedef std::shared_ptr<IGestureRecognizerHandler> ISharedGestureRecognizerHandler;
typedef const ISharedGestureRecognizerHandler& ISharedGestureRecognizerHandlerRef;

class IGameObject;
typedef std::shared_ptr<IGameObject> ISharedGameObject;
typedef const ISharedGameObject& ISharedGameObjectRef;

class COcean;
typedef std::shared_ptr<COcean> CSharedOcean;
typedef const CSharedOcean& CSharedOceanRef;

class CLandscape;
typedef std::shared_ptr<CLandscape> CSharedLandscape;
typedef const CSharedLandscape& CSharedLandscapeRef;

class CLandscapeChunk;
typedef std::shared_ptr<CLandscapeChunk> CSharedLandscapeChunk;
typedef const CSharedLandscapeChunk& CSharedLandscapeChunkRef;

class CSkyBox;
typedef std::shared_ptr<CSkyBox> CSharedSkyBox;
typedef const CSharedSkyBox& CSharedSkyBoxRef;

class CAtmosphericScattering;
typedef std::shared_ptr<CAtmosphericScattering> CSharedAtmosphericScattering;
typedef const CSharedAtmosphericScattering& CSharedAtmosphericScatteringRef;

class ICollisionHandler;
typedef std::shared_ptr<ICollisionHandler> ISharedCollisionHandler;
typedef const ISharedCollisionHandler& ISharedCollisionHandlerRef;

class CCubemapTexture;
typedef std::shared_ptr<CCubemapTexture> CSharedCubemapTexture;
typedef const CSharedCubemapTexture& CSharedCubemapTextureRef;

class CHeightmap;
typedef std::shared_ptr<CHeightmap> CSharedHeightmap;
typedef const CSharedHeightmap& CSharedHeightmapRef;

class CModel;
typedef std::shared_ptr<CModel> CSharedModel;
typedef const CSharedModel& CSharedModelRef;

class CParticleEmitter;
typedef std::shared_ptr<CParticleEmitter> CSharedParticleEmitter;
typedef const CSharedParticleEmitter& CSharedParticleEmitterRef;

class CCollisionMgr;
typedef std::shared_ptr<CCollisionMgr> CSharedCollisionMgr;
typedef const CSharedCollisionMgr& CSharedCollisionMgrRef;

class IScene;
typedef std::shared_ptr<IScene> ISharedScene;
typedef const ISharedScene& ISharedSceneRef;

class CSceneGraph;
typedef std::shared_ptr<CSceneGraph> CSharedSceneGraph;
typedef const CSharedSceneGraph& CSharedSceneGraphRef;

class IGraphicsContext;
typedef std::shared_ptr<IGraphicsContext> ISharedGraphicsContext;
typedef const ISharedGraphicsContext& ISharedGraphicsContextRef;

class CConfigurationAccessor;
typedef std::shared_ptr<CConfigurationAccessor> CSharedConfigurationAccessor;
typedef const CSharedConfigurationAccessor& CSharedConfigurationAccessorRef;

class CSceneFabricator;
typedef std::shared_ptr<CSceneFabricator> CSharedSceneFabricator;
typedef const CSharedSceneFabricator& CSharedSceneFabricatorRef;

class CQuad;
typedef std::shared_ptr<CQuad> CSharedQuad;
typedef const CSharedQuad& CSharedQuadRef;

class CBoundingBox;
typedef std::shared_ptr<CBoundingBox> CSharedBoundingBox;
typedef const CSharedBoundingBox& CSharedBoundingBoxRef;

class IRenderTechniqueHandler;
typedef std::shared_ptr<IRenderTechniqueHandler> ISharedRenderTechniqueHandler;
typedef const ISharedRenderTechniqueHandler& ISharedRenderTechniqueHandlerRef;

class IRenderTechniqueBase;
typedef std::shared_ptr<IRenderTechniqueBase> ISharedRenderTechniqueBase;
typedef const ISharedRenderTechniqueBase& ISharedRenderTechniqueBaseRef;

class CRenderTechniqueMain;
typedef std::shared_ptr<CRenderTechniqueMain> CSharedRenderTechniqueMain;
typedef const CSharedRenderTechniqueMain& CSharedRenderTechniqueMainRef;

class CRenderTechniqueWorldSpace;
typedef std::shared_ptr<CRenderTechniqueWorldSpace> CSharedRenderTechniqueWorldSpace;
typedef const CSharedRenderTechniqueWorldSpace& CSharedRenderTechniqueWorldSpaceRef;

class CRenderTechniqueScreenSpace;
typedef std::shared_ptr<CRenderTechniqueScreenSpace> CSharedRenderTechniqueScreenSpace;
typedef const CSharedRenderTechniqueScreenSpace& CSharedRenderTechniqueScreenSpaceRef;

class CRenderPipeline;
typedef std::shared_ptr<CRenderPipeline> CSharedRenderPipeline;
typedef const CSharedRenderPipeline& CSharedRenderPipelineRef;

class IRenderTechniqueImporter;
typedef std::shared_ptr<IRenderTechniqueImporter> ISharedRenderTechniqueImporter;
typedef const ISharedRenderTechniqueImporter& ISharedRenderTechniqueImporterRef;

class IRenderTechniqueAccessor;
typedef std::shared_ptr<IRenderTechniqueAccessor> ISharedRenderTechniqueAccessor;
typedef const ISharedRenderTechniqueAccessor& ISharedRenderTechniqueAccessorRef;

class CRenderTarget;
typedef std::shared_ptr<CRenderTarget> CSharedRenderTarget;
typedef const CSharedRenderTarget& CSharedRenderTargetRef;

class CThreadOperation;
typedef std::shared_ptr<CThreadOperation> CSharedThreadOperation;
typedef const CSharedThreadOperation& CSharedThreadOperationRef;

class IGameTransition;
typedef std::shared_ptr<IGameTransition> ISharedGameTransition;
typedef const ISharedGameTransition& ISharedGameTransitionRef;

class IRenderTechniqueOperationTextureHandler;
typedef std::shared_ptr<IRenderTechniqueOperationTextureHandler> ISharedRenderTechniqueOperationTextureHandler;
typedef const ISharedRenderTechniqueOperationTextureHandler& ISharedRenderTechniqueOperationTextureHandlerRef;

class IOcclusionQueryHandler;
typedef std::shared_ptr<IOcclusionQueryHandler> ISharedOcclusionQueryHandler;
typedef const ISharedOcclusionQueryHandler& ISharedOcclusionQueryHandlerRef;

class COcclusionQueryManager;
typedef std::shared_ptr<COcclusionQueryManager> CSharedOcclusionQueryManager;
typedef const CSharedOcclusionQueryManager& CSharedOcclusionQueryManagerRef;

class IComponent;
typedef std::shared_ptr<IComponent> ISharedComponent;
typedef const ISharedComponent& ISharedComponentRef;

class CComponentTransformation;
typedef std::shared_ptr<CComponentTransformation> CSharedComponentTransformation;
typedef const CSharedComponentTransformation& CSharedComponentTransformationRef;

class CComponentRendering;
typedef std::shared_ptr<CComponentRendering> CSharedComponentRendering;
typedef const CSharedComponentRendering& CSharedComponentRenderingRef;

class CComponentDebugRendering;
typedef std::shared_ptr<CComponentDebugRendering> CSharedComponentDebugRendering;
typedef const CSharedComponentDebugRendering& CSharedComponentDebugRenderingRef;



class ICommand;
typedef std::shared_ptr<ICommand> ISharedCommand;
typedef const ISharedCommand& ISharedCommandRef;

class IUICommands;
typedef std::shared_ptr<IUICommands> ISharedUICommands;
typedef const ISharedUICommands& ISharedUICommandsRef;

class IConfigurationGameObject;
typedef std::shared_ptr<IConfigurationGameObject> ISharedConfigurationGameObject;
typedef const ISharedConfigurationGameObject& ISharedConfigurationGameObjectRef;

#endif
