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

class IOGLContext;
class CMaterial;
class CTexture;
class CRenderOperationWorldSpace;
class CRenderOperationScreenSpace;
class CRenderOperationOutput;
class IRenderHandler;

class CRenderMgr final : public IGameLoopHandler
{
private:
    
    const std::shared_ptr<IOGLContext> m_glContext;
    std::shared_ptr<CMaterial> m_outputRenderMaterial;
    std::map<std::string, std::shared_ptr<CRenderOperationWorldSpace> > m_worldSpaceOperations;
    std::map<std::string, std::shared_ptr<CRenderOperationScreenSpace> > m_screenSpaceOperations;
    std::shared_ptr<CRenderOperationOutput> m_outputOperation;
    
    std::queue<std::shared_ptr<CRenderOperationScreenSpace> > m_customScreenSpaceOperationsQueue;
    
protected:
    
    void _OnGameLoopUpdate(f32 _deltatime);
    
public:
    
    CRenderMgr(const std::shared_ptr<IOGLContext> _glContext, std::shared_ptr<CMaterial> _material);
    ~CRenderMgr(void);
    
    void RegisterWorldSpaceRenderOperation(const std::string& _mode, std::shared_ptr<CRenderOperationWorldSpace> _operation);
    void UnregisterWorldSpaceRenderOperation(const std::string& _mode);
    
    void RegisterScreenSpaceRenderOperation(const std::string& _mode, std::shared_ptr<CRenderOperationScreenSpace> _operation);
    void UnregisterScreenSpaceRenderOperation(const std::string& _mode);
    
    std::shared_ptr<CTexture> PushCustomScreenSpaceRenderOperation(std::shared_ptr<CMaterial> _material, ui32 _textureWidth, ui32 _textureHeight);
    
    void RegisterWorldSpaceRenderHandler(const std::string& _mode, std::shared_ptr<IRenderHandler> _handler);
    void UnregisterWorldSpaceRenderHandler(const std::string& _mode, std::shared_ptr<IRenderHandler> _handler);
    
    std::shared_ptr<CTexture> Get_WorldSpaceOperationTexture(const std::string& _mode);
    std::shared_ptr<CTexture> Get_ScreenSpaceOperationTexture(const std::string& _mode);
};



#endif 
