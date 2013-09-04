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
#include "IRenderHandler.h"

class CMaterial;
class CMesh;
class CAnimationMixer;

class CBatch : public IRenderHandler
{
private:
    
protected:
    
    
    std::shared_ptr<CMaterial> m_material;
    std::function<void(std::shared_ptr<CMaterial>)> m_bind;
    std::shared_ptr<CMesh> m_mesh;
    std::string m_guid;
    std::string m_mode;
    
    std::vector<std::tuple<std::shared_ptr<CMesh>, std::shared_ptr<CAnimationMixer>>> m_meshes;
    std::vector<glm::mat4x4> m_matrices;
    std::vector<glm::mat4x4> m_transformations;
    bool m_isUnlocked;
    bool m_isLocked;
    
    ui32 m_numIndices;
    ui32 m_numVertices;
    
    ui32 m_numUsingIndices;
    ui32 m_numUsingVertices;
    
    ui32 m_renderQueuePosition;
    
    i32 _OnQueuePosition(void);
    bool _OnOcclusion(void);
    ui32 _OnGet_NumTriangles(void);
    void _OnBind(const std::string& _mode);
    void _OnDraw(const std::string& _mode);
    void _OnUnbind(const std::string& _mode);
    void _OnDebugDraw(const std::string& _mode);
    void _OnBatch(const std::string& _mode);
    
public:
    
    CBatch(const std::string& _mode, ui32 _renderQueuePosition, const std::shared_ptr<CMaterial> _material, const std::function<void(std::shared_ptr<CMaterial>)>& _bind);
    ~CBatch(void);
    
    inline std::string Get_Guid(void) const
    {
        return m_guid;
    };
    
    inline std::string Get_Mode(void) const
    {
        return m_mode;
    };

    void Lock(void);
    void Unlock(void);
    
    void Batch(const std::tuple<std::shared_ptr<CMesh>, std::shared_ptr<CAnimationMixer>>& _mesh, const glm::mat4x4& _matrix);
};

#endif 
