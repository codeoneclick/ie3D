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
#include "IRenderHandler.h"

class CBatch : public IRenderHandler
{
private:
    
protected:
    
    CSharedMaterial m_material;
    std::function<void(CSharedMaterialRef)> m_materialBindImposer;
    CSharedMesh m_mesh;
    
    std::string m_guid;
    std::string m_mode;
    
    std::vector<std::tuple<CSharedMesh, CSharedAnimationMixer>> m_models;
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
    void onBind(const std::string& mode);
    void onDraw(const std::string& mode);
    void onUnbind(const std::string& mode);
    void onBatch(const std::string& mode);
    
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
    std::string getMode(void) const;
    
    ui32 getNumUnlockedNumVertices(void) const;
    ui32 getNumUnlockedNumIndices(void) const;
    ui32 getNumUnlockedNumTransformations(void) const;
    
    void lock(void);
    void unlock(void);
    
    void batch(const std::tuple<CSharedMesh, CSharedAnimationMixer>& model,
               const glm::mat4x4& matrix);
};

#endif 
