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

class CMesh;
class CQuad;
class CMaterial;
class CAnimationMixer;
class CBatch;
class CRenderMgr;

class CBatchingMgr
{
private:
    
protected:
    
    std::shared_ptr<CRenderMgr> m_renderMgr;
    std::map<std::string, std::shared_ptr<CBatch>> m_batches;
    
public:
    
    static const ui32 k_MAX_BATCHES_PER_MESH_TYPE;
    
    CBatchingMgr(const std::shared_ptr<CRenderMgr>& _renderMgr);
    ~CBatchingMgr(void);
    
    void Lock(const std::string& _mode);
    void Unlock(const std::string& _mode);
    void Batch(const std::string& _mode, ui32 _renderQueuePosition, const std::tuple<std::shared_ptr<CMesh>, std::shared_ptr<CAnimationMixer>>& _model, std::shared_ptr<CMaterial> _material, const std::function<void(std::shared_ptr<CMaterial>)>& _materialImposer, const glm::mat4x4& _worldMatrix);
	void Batch(const std::string& _mode, ui32 _renderQueuePosition, const std::shared_ptr<CQuad> _control, std::shared_ptr<CMaterial> _material, const std::function<void(std::shared_ptr<CMaterial>)>& _materialImposer, const glm::vec2& _position, const glm::vec2& _size);
};

#endif 
