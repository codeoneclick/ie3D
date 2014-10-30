//
//  CBatchingMgr.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 8/22/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CBatchingMgr_h
#define CBatchingMgr_h


#include "HCommon.h"
#include "HDeclaration.h"

class CBatchingMgr
{
private:
    
protected:
    
    ISharedRenderTechniqueImporter m_renderTechniqueImporter;
    std::unordered_map<std::string, CSharedBatch> m_batches;
    
public:
    
    static const ui32 k_MAX_BATCHES_PER_MODELTYPE;
    
    CBatchingMgr(ISharedRenderTechniqueImporterRef renderTechniqueImporter);
    ~CBatchingMgr(void);
    
    void lock(const std::string& mode);
    void unlock(const std::string& mode);
    void batch(const std::string& mode,
               ui32 renderQueuePosition,
               const std::tuple<CSharedMesh, CSharedAnimationMixer>& model,
               CSharedMaterialRef material,
               const std::function<void(CSharedMaterialRef)>& materialBindImposer,
               const glm::mat4x4& matrix);
};

#endif
