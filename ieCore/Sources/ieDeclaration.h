//
//  ieDeclaration.h
//  ieCore
//
//  Created by sergey.sergeev on 4/7/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#ifndef ieCore_ieDeclaration_h
#define ieCore_ieDeclaration_h

#include "HCommon.h"

class ieValue;
typedef std::shared_ptr<ieValue> ieSharedValue;
typedef const ieSharedValue& ieSharedValueRef;

class ieObject;
typedef std::shared_ptr<ieObject> ieSharedObject;
typedef const ieSharedObject& ieSharedObjectRef;

class ieEvent;
typedef std::shared_ptr<ieEvent> ieSharedEvent;
typedef const ieSharedEvent& ieSharedEventRef;

class ieEventDispatcher;
typedef std::shared_ptr<ieEventDispatcher> ieSharedEventDispatcher;
typedef const ieSharedEventDispatcher& ieSharedEventDispatcherRef;

class ieVertexBuffer;
typedef std::shared_ptr<ieVertexBuffer> ieSharedVertexBuffer;
typedef const ieSharedVertexBuffer& ieSharedVertexBufferRef;

class ieIndexBuffer;
typedef std::shared_ptr<ieIndexBuffer> ieSharedIndexBuffer;
typedef const ieSharedIndexBuffer& ieSharedIndexBufferRef;

class ieMesh;
typedef std::shared_ptr<ieMesh> ieSharedMesh;
typedef const ieSharedMesh& ieSharedMeshRef;

class ieQuad;
typedef std::shared_ptr<ieQuad> ieSharedQuad;
typedef const ieSharedQuad& ieSharedQuadRef;

class ieResource;
typedef std::shared_ptr<ieResource> ieSharedResource;
typedef const ieSharedResource& ieSharedResourceRef;

class ieTexture;
typedef std::shared_ptr<ieTexture> ieSharedTexture;
typedef const ieSharedTexture& ieSharedTextureRef;

class ieShader;
typedef std::shared_ptr<ieShader> ieSharedShader;
typedef const ieSharedShader& ieSharedShaderRef;

class ieBone;
typedef std::shared_ptr<ieBone> ieSharedBone;
typedef const ieSharedBone& ieSharedBoneRef;

class ieSkeleton;
typedef std::shared_ptr<ieSkeleton> ieSharedSkeleton;
typedef const ieSharedSkeleton& ieSharedSkeletonRef;

class ieSequence;
typedef std::shared_ptr<ieSequence> ieSharedSequence;
typedef const ieSharedSequence& ieSharedSequenceRef;

class ieMaterial;
typedef std::shared_ptr<ieMaterial> ieSharedMaterial;
typedef const ieSharedMaterial& ieSharedMaterialRef;

class ieCamera;
typedef std::shared_ptr<ieCamera> ieSharedCamera;
typedef const ieSharedCamera& ieSharedCameraRef;

class ieLightSource;
typedef std::shared_ptr<ieLightSource> ieSharedLightSource;
typedef const ieSharedLightSource& ieSharedLightSourceRef;

class ieRenderManager;
typedef std::shared_ptr<ieRenderManager> ieSharedRenderManager;
typedef const ieSharedRenderManager& ieSharedRenderManagerRef;

class ieResourceAccessor;
typedef std::shared_ptr<ieResourceAccessor> ieSharedResourceAccessor;
typedef const ieSharedResourceAccessor& ieSharedResourceAccessorRef;

class ieScreenSpaceTextureAccessor;
typedef std::shared_ptr<ieScreenSpaceTextureAccessor> ieSharedScreenSpaceTextureAccessor;
typedef const ieSharedScreenSpaceTextureAccessor& ieSharedScreenSpaceTextureAccessorRef;

class ieDisplayObject;
typedef std::shared_ptr<ieDisplayObject> ieSharedDisplayObject;
typedef const ieSharedDisplayObject& ieSharedDisplayObjectRef;

class ieDisplayObjectContainer;
typedef std::shared_ptr<ieDisplayObjectContainer> ieSharedDisplayObjectContainer;
typedef const ieSharedDisplayObjectContainer& ieSharedDisplayObjectContainerRef;

class ieStage;
typedef std::shared_ptr<ieStage> ieSharedStage;
typedef const ieSharedStage& ieSharedStageRef;

#endif
