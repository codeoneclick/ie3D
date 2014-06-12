//
//  CRenderMgr.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/17/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CRenderMgr_h
#define CRenderMgr_h

#include "IGameLoopHandler.h"
#include "IScreenSpaceRenderAccessor.h"

class IGraphicsContext;
class CMaterial;
class CTexture;
class CRenderOperationWorldSpace;
class CRenderOperationScreenSpace;
class CRenderOperationOutput;
class CBatchingMgr;
class IRenderHandler;

class CRenderMgr final : public IGameLoopHandler, public IScreenSpaceRenderAccessor
{
private:
    
    const std::shared_ptr<IGraphicsContext> m_graphicsContext;
    std::map<std::string, std::shared_ptr<CRenderOperationWorldSpace>> m_worldSpaceOperations;
    std::map<std::string, std::shared_ptr<CRenderOperationScreenSpace>> m_screenSpaceOperations;
    std::shared_ptr<CRenderOperationOutput> m_outputOperation;
    std::queue<std::shared_ptr<CRenderOperationScreenSpace> > m_customScreenSpaceOperationsQueue;
    ui32 m_numTriangles;
    
protected:
    
    std::shared_ptr<CBatchingMgr> m_batchingMgr;
    void _OnGameLoopUpdate(f32 _deltatime);
    
public:
    
    CRenderMgr(const std::shared_ptr<IGraphicsContext> graphicsContext);
    ~CRenderMgr(void);
    
    void RegisterOutputRenderOperation(std::shared_ptr<CMaterial> material);
    
    void RegisterWorldSpaceRenderOperation(const std::string& _mode, std::shared_ptr<CRenderOperationWorldSpace> _operation);
    void UnregisterWorldSpaceRenderOperation(const std::string& _mode);
    
    void RegisterScreenSpaceRenderOperation(const std::string& _mode, std::shared_ptr<CRenderOperationScreenSpace> _operation);
    void UnregisterScreenSpaceRenderOperation(const std::string& _mode);
    
    void RegisterWorldSpaceRenderHandler(const std::string& _mode, std::shared_ptr<IRenderHandler> _handler);
    void UnregisterWorldSpaceRenderHandler(const std::string& _mode, std::shared_ptr<IRenderHandler> _handler);
    
    CSharedTexture getSSOperationTexture(const std::string& mode);
    CSharedMaterial getSSOperationMaterial(const std::string& mode);
    CSharedTexture preprocessSSOperationTexture(CSharedMaterialRef material,
                                                ui32 width,
                                                ui32 height);
    
    inline void Set_BatchingMgr(const std::shared_ptr<CBatchingMgr>& _batchingMgr)
    {
        m_batchingMgr = _batchingMgr;
    };
    
    inline std::shared_ptr<CBatchingMgr> Get_BatchingMgr(void)
    {
        return m_batchingMgr;
    };
    
    inline ui32 Get_NumTriangles(void)
    {
        return m_numTriangles;
    };
    
    ui32 getWidth(void) const;
    ui32 getHeight(void) const;
};



#endif 
