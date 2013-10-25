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
#include "IRenderHandler.h"

class CMaterial;
class CMesh;
class CQuad;
class CAnimationMixer;

class CBatch : public IRenderHandler
{
private:
    
protected:
    
    std::shared_ptr<CMaterial> m_material;
    std::function<void(std::shared_ptr<CMaterial>)> m_materialImposer;
    std::shared_ptr<CMesh> m_mesh;
    std::string m_guid;
    std::string m_mode;
    
    std::vector<std::tuple<std::shared_ptr<CMesh>, std::shared_ptr<CAnimationMixer>>> m_meshes;
	std::vector<std::shared_ptr<CQuad>> m_controls;
    std::vector<glm::mat4x4> m_matrices;
	std::vector<glm::mat4x4> m_transformations;
	std::vector<glm::vec2> m_positions;
	std::vector<glm::vec2> m_sizes;
    
    ui8 m_locked;
    ui8 m_proccessed;
    ui8 m_unlocked;
    
    ui32 m_numIndices;
    ui32 m_numVertices;
    
    ui32 m_numPushedIndices;
    ui32 m_numPushedVertices;
    
    ui32 m_renderQueuePosition;
    
    ui32 m_numBatchedVertices;
    ui32 m_numBatchedIndices;
    ui32 m_numBatchedTransformations;

	E_BATCH_GEOMETRY_MODE m_geometryMode;

	std::function<void(void)> _UnlockModelsGeometry(void);
	std::function<void(void)> _UnlockControlsGeometry(void);
	std::function<void(void)> _UnlockPatriclesGeometry(void);
    
    i32 _OnQueuePosition(void);
    bool _OnOcclusion(void);
    ui32 _OnGet_NumTriangles(void);
    void _OnBind(const std::string& _mode);
    void _OnDraw(const std::string& _mode);
    void _OnUnbind(const std::string& _mode);
    void _OnDebugDraw(const std::string& _mode);
    void _OnBatch(const std::string& _mode);
    
public:
    
    CBatch(const std::string& _mode, E_BATCH_GEOMETRY_MODE _geometryMode, ui32 _renderQueuePosition, const std::shared_ptr<CMaterial> _material, const std::function<void(std::shared_ptr<CMaterial>)>& _materialImposer);
    ~CBatch(void);
    
    static const ui32 k_MAX_NUM_VERTICES;
    static const ui32 k_MAX_NUM_INDICES;
    static const ui32 k_MAX_NUM_TRANSFORMATION;
    
    inline std::string Get_Guid(void) const
    {
        return m_guid;
    };
    
    inline std::string Get_Mode(void) const
    {
        return m_mode;
    };
    
    inline ui32 Get_NumBatchedVertices(void)
    {
        return m_numBatchedVertices;
    };
    
    inline ui32 Get_NumBatchedIndices(void)
    {
        return m_numBatchedIndices;
    };
    
    inline ui32 Get_NumBatchedTransformations(void)
    {
        return m_numBatchedTransformations;
    };

    void Lock(void);
    void Unlock(void);
    
    void Batch(const std::tuple<std::shared_ptr<CMesh>, std::shared_ptr<CAnimationMixer>>& _model, const glm::mat4x4& _worldMatrix);
	void Batch(const std::shared_ptr<CQuad>& _control, const glm::vec2& _position, const glm::vec2& _size); 
};

#endif 
