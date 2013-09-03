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

class CMaterial;
class CMesh;
class CAnimationMixer;

class CBatch
{
private:
    
protected:
    
    
    std::shared_ptr<CMaterial> m_material;
    std::shared_ptr<CMesh> m_mesh;
    std::string m_guid;
    
    std::vector<std::tuple<std::shared_ptr<CMesh>, std::shared_ptr<CAnimationMixer>>> m_meshes;
    std::vector<glm::mat4x4> m_matrices;
    bool m_isUnlocked;
    bool m_isLocked;
    
    ui32 m_numIndices;
    ui32 m_numVertices;
    
    ui32 m_numUsingIndices;
    ui32 m_numUsingVertices;
    
public:
    
    CBatch(std::shared_ptr<CMaterial> _material);
    ~CBatch(void);
    
    inline std::string Get_Guid(void) const
    {
        return m_guid;
    };

    void Lock(void);
    void Unlock(void);
    
    void Batch(const std::tuple<std::shared_ptr<CMesh>, std::shared_ptr<CAnimationMixer>>& _mesh, const glm::mat4x4& _matrix);
    void Draw(void);
};

#endif 
