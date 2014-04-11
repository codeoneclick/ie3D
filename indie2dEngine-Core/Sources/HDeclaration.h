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

class IResourceLoader;
typedef std::shared_ptr<IResourceLoader> ISharedResourceLoader;
typedef const ISharedResourceLoader& ISharedResourceLoaderRef;

class CTextureLoader;
typedef std::shared_ptr<CTextureLoader> CSharedTextureLoader;
typedef const CSharedTextureLoader& CSharedTextureLoaderRef;

class CShaderLoader;
typedef std::shared_ptr<CShaderLoader> CSharedShaderLoader;
typedef const CSharedShaderLoader& CSharedShaderLoaderRef;

class CMeshLoader;
typedef std::shared_ptr<CMeshLoader> CSharedMeshLoader;
typedef const CSharedMeshLoader& CSharedMeshLoaderRef;

class CSkeletonLoader;
typedef std::shared_ptr<CSkeletonLoader> CSharedSkeletonLoader;
typedef const CSharedSkeletonLoader& CSharedSkeletonLoaderRef;

class CSequenceLoader;
typedef std::shared_ptr<CSequenceLoader> CSharedSequenceLoader;
typedef const CSharedSequenceLoader& CSharedSequenceLoaderRef;

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
struct SVertex;
typedef std::shared_ptr<CVertexBuffer> CSharedVertexBuffer;
typedef const CSharedVertexBuffer& CSharedVertexBufferRef;

class CIndexBuffer;
typedef std::shared_ptr<CIndexBuffer> CSharedIndexBuffer;
typedef const CSharedIndexBuffer& CSharedIndexBufferRef;

class CBone;
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

#endif
