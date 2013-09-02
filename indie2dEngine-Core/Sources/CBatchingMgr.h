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
class CMaterial;
class CBatch;
class CBatchingMgr
{
private:
    
protected:
    
    std::map<std::string, std::shared_ptr<CBatch>> m_batches;
    
public:
    
    CBatchingMgr(void);
    ~CBatchingMgr(void);
    
    void Lock(void);
    void Unlock(void);
    void Batch(const std::shared_ptr<CMesh>& _mesh, std::shared_ptr<CMaterial> _material, const glm::mat4x4& _matrix);
    void Draw(void);
};

#endif 
