//
//  CBatch.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 8/22/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CBatch_h
#define CBatch_h

#include "HCommon.h"
#include "HEnums.h"
#include "HDeclaration.h"
#include "IRenderTechniqueHandler.h"

class CBatch : public IRenderTechniqueHandler
{
private:
    
protected:
    
    CSharedMaterial m_material;
    std::function<void(CSharedMaterialRef)> m_materialBindImposer;
    CSharedMesh m_batchMesh;
    
    std::string m_guid;
    std::string m_mode;
    
    std::vector<CSharedMesh> m_meshes;
    std::vector<CSharedAnimationMixer> m_animationMixers;
    std::vector<glm::mat4x4> m_matrices;
	std::vector<glm::mat4x4> m_transformations;
    
    ui32 m_numUnlockedIndices;
    ui32 m_numUnlockedVertices;
    ui32 m_numUnlockedTransformations;
    
    ui32 m_numLockedIndices;
    ui32 m_numLockedVertices;
    ui32 m_numLockedTransformations;
    
    ui32 m_zOrder;
    
    i32  zOrder(void);
    bool checkOcclusion(void);
    ui32 numTriangles(void);
    void onBind(const std::string& techniqueName);
    void onDraw(const std::string& techniqueName);
    void onUnbind(const std::string& techniqueName);
    void onBatch(const std::string& techniqueName);
    
public:
    
    CBatch(const std::string& mode,
           ui32 renderQueuePosition,
           CSharedMaterialRef material,
           const std::function<void(CSharedMaterialRef)>& materialBindImposer);
    ~CBatch(void);
    
    static const ui32 k_MAX_NUM_VERTICES;
    static const ui32 k_MAX_NUM_INDICES;
    static const ui32 k_MAX_NUM_TRANSFORMATION;
    
    std::string getGuid(void) const;
    std::string getTechniqueName(void) const;
    
    ui32 getNumUnlockedVertices(void) const;
    ui32 getNumUnlockedIndices(void) const;
    ui32 getNumUnlockedTransformations(void) const;
    
    bool isAnimated(void) const;
    
    void lock(void);
    void unlock(void);
    
    void batch(CSharedMeshRef mesh,
               const glm::mat4x4& matrix);
    
    void batch(CSharedMeshRef mesh,
               CSharedAnimationMixerRef animationMixer,
               const glm::mat4x4& matrix);
};

#endif 
