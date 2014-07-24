//
//  HDeclaration.h
//  indieEngineCore
//
//  Created by Sergey Sergeev on 4/7/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#ifndef indieEngineCore_HDeclaration_h
#define indieEngineCore_HDeclaration_h

#include "HCommon.h"

#if defined(__NDK__)

struct ANativeWindow;

#endif

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

class CSequence;
typedef std::shared_ptr<CSequence> CSharedSequence;
typedef const CSharedSequence& CSharedSequenceRef;

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

class CVertexBuffer;
struct SVertexData;
struct SAttributeVertex;
typedef std::shared_ptr<CVertexBuffer> CSharedVertexBuffer;
typedef const CSharedVertexBuffer& CSharedVertexBufferRef;

class CIndexBuffer;
typedef std::shared_ptr<CIndexBuffer> CSharedIndexBuffer;
typedef const CSharedIndexBuffer& CSharedIndexBufferRef;

class CBone;
struct SBoneData;
typedef std::shared_ptr<CBone> CSharedBone;
typedef const CSharedBone& CSharedBoneRef;

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

class CConfigurationTexture;
typedef std::shared_ptr<CConfigurationTexture> CSharedConfigurationTexture;
typedef const CSharedConfigurationTexture& CSharedConfigurationTextureRef;

class CConfigurationShader;
typedef std::shared_ptr<CConfigurationShader> CSharedConfigurationShader;
typedef const CSharedConfigurationShader& CSharedConfigurationShaderRef;

class CConfigurationMaterial;
typedef std::shared_ptr<CConfigurationMaterial> CSharedConfigurationMaterial;
typedef const CSharedConfigurationMaterial& CSharedConfigurationMaterialRef;

class CConfigurationGameObject;
typedef std::shared_ptr<CConfigurationGameObject> CSharedConfigurationGameObject;
typedef const CSharedConfigurationGameObject& CSharedConfigurationGameObjectRef;

class CConfigurationWSRenderOperation;
typedef std::shared_ptr<CConfigurationWSRenderOperation> CSharedConfigurationWSRenderOperation;
typedef const CSharedConfigurationWSRenderOperation& CSharedConfigurationWSRenderOperationRef;

class CConfigurationSSRenderOperation;
typedef std::shared_ptr<CConfigurationSSRenderOperation> CSharedConfigurationSSRenderOperation;
typedef const CSharedConfigurationSSRenderOperation& CSharedConfigurationSSRenderOperationRef;

class CConfigurationORenderOperation;
typedef std::shared_ptr<CConfigurationORenderOperation> CSharedConfigurationORenderOperation;
typedef const CSharedConfigurationORenderOperation& CSharedConfigurationORenderOperationRef;

class IConfigurationLoadingHandler;
typedef std::shared_ptr<IConfigurationLoadingHandler> ISharedConfigurationLoadingHandler;
typedef const ISharedConfigurationLoadingHandler& ISharedConfigurationLoadingHandlerRef;

class CConfigurationModelLoadingOperation;
typedef std::shared_ptr<CConfigurationModelLoadingOperation> CSharedConfigurationModelLoadingOperation;
typedef const CSharedConfigurationModelLoadingOperation& CSharedConfigurationModelLoadingOperationRef;

class CMaterial;
typedef std::shared_ptr<CMaterial> CSharedMaterial;
typedef const CSharedMaterial& CSharedMaterialRef;

class CMaterialCachedParameters;
typedef std::shared_ptr<CMaterialCachedParameters> CSharedMaterialCachedParameters;
typedef const CSharedMaterialCachedParameters& CSharedMaterialCachedParametersRef;

class CCamera;
typedef std::shared_ptr<CCamera> CSharedCamera;
typedef const CSharedCamera& CSharedCameraRef;

class CLightSource;
typedef std::shared_ptr<CLightSource> CSharedLightSource;
typedef const CSharedLightSource& CSharedLightSourceRef;

class CSceneUpdateMgr;
typedef std::shared_ptr<CSceneUpdateMgr> CSharedSceneUpdateMgr;
typedef const CSharedSceneUpdateMgr& CSharedSceneUpdateMgrRef;

class CResourceAccessor;
typedef std::shared_ptr<CResourceAccessor> CSharedResourceAccessor;
typedef const CSharedResourceAccessor& CSharedResourceAccessorRef;

class CBoundBox;
typedef std::shared_ptr<CBoundBox> CSharedBoundBox;
typedef const CSharedBoundBox& CSharedBoundBoxRef;

class CAnimationMixer;
typedef std::shared_ptr<CAnimationMixer> CSharedAnimationMixer;
typedef const CSharedAnimationMixer& CSharedAnimationMixerRef;

class CBatch;
typedef std::shared_ptr<CBatch> CSharedBatch;
typedef const CSharedBatch& CSharedBatchRef;

class CQuadTree;
typedef std::shared_ptr<CQuadTree> CSharedQuadTree;
typedef const CSharedQuadTree& CSharedQuadTreeRef;

class CFrustum;
typedef std::shared_ptr<CFrustum> CSharedFrustum;
typedef const CSharedFrustum& CSharedFrustumRef;

class CHeightmapProcessor;
typedef std::shared_ptr<CHeightmapProcessor> CSharedHeightmapProcessor;
typedef const CSharedHeightmapProcessor& CSharedHeightmapProcessorRef;

class CLandscapeChunk;
typedef std::shared_ptr<CLandscapeChunk> CSharedLandscapeChunk;
typedef const CSharedLandscapeChunk& CSharedLandscapeChunkRef;

class CLandscapeEdges;
typedef std::shared_ptr<CLandscapeEdges> CSharedLandscapeEdges;
typedef const CSharedLandscapeEdges& CSharedLandscapeEdgesRef;

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

class ICollisionHandler;
typedef std::shared_ptr<ICollisionHandler> ISharedCollisionHandler;
typedef const ISharedCollisionHandler& ISharedCollisionHandlerRef;

class CCubemapTexture;
typedef std::shared_ptr<CCubemapTexture> CSharedCubemapTexture;
typedef const CSharedCubemapTexture& CSharedCubemapTextureRef;

class CConfigurationGameObject;
typedef std::shared_ptr<CConfigurationGameObject> CSharedConfigurationGameObject;
typedef const CSharedConfigurationGameObject& CSharedConfigurationGameObjectRef;

class CHeightmapProcessingOperation;
typedef std::shared_ptr<CHeightmapProcessingOperation> CSharedHeightmapProcessingOperation;
typedef const CSharedHeightmapProcessingOperation& CSharedHeightmapProcessingOperationRef;

class CHeightmapData;
typedef std::shared_ptr<CHeightmapData> CSharedHeightmapData;
typedef const CSharedHeightmapData& CSharedHeightmapDataRef;

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

#endif
