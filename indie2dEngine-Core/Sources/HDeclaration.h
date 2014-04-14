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

class CTemplateAttribute;
typedef std::shared_ptr<CTemplateAttribute> CSharedTemplateAttribute;
typedef const CSharedTemplateAttribute& CSharedTemplateAttributeRef;

class CTemplateCommon;
typedef std::shared_ptr<CTemplateCommon> CSharedTemplateCommon;
typedef const CSharedTemplateCommon& CSharedTemplateCommonRef;

#endif
