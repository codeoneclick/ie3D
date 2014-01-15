//
//  IRenderHandler.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/17/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef IRenderHandler_h
#define IRenderHandler_h

#include "HCommon.h"

class CRenderCommands final
{
private:
    
    typedef std::function<i32(void)> __RENDER_QUEUE_POSITION_COMMAND;
    typedef std::function<bool(void)> __RENDER_OCCLUSION_COMMAND;
    typedef std::function<ui32(void)> __RENDER_GET_NUM_TRIANGLES_COMMAND;
    typedef std::function<void(const std::string&)> __RENDER_BIND_COMMAND;
    typedef std::function<void(const std::string&)> __RENDER_DRAW_COMMAND;
    typedef std::function<void(const std::string&)> __RENDER_UNBIND_COMMAND;
    typedef std::function<void(const std::string&)> __RENDER_DEBUG_DRAW_COMMAND;
    typedef std::function<void(const std::string&)> __RENDER_BATCH_COMMAND;
    
    friend class IRenderHandler;
    friend class CRenderOperationWorldSpace;
    friend class CRenderOperationScreenSpace;
    friend class CRenderOperationOutput;
    
    __RENDER_QUEUE_POSITION_COMMAND m_renderQueuePositionCommand;
    __RENDER_OCCLUSION_COMMAND m_renderOcclusionCommand;
    __RENDER_GET_NUM_TRIANGLES_COMMAND m_renderGetNumTrianglesCommand;
    __RENDER_BIND_COMMAND m_renderBindCommand;
    __RENDER_DRAW_COMMAND m_renderDrawCommand;
    __RENDER_UNBIND_COMMAND m_renderUnbindCommand;
    __RENDER_DEBUG_DRAW_COMMAND m_renderDebugDrawCommand;
    __RENDER_BATCH_COMMAND m_renderBatchCommand;
    
protected:
    
    CRenderCommands(void);
    
    void _ConnectRenderQueuePositionCommand(const __RENDER_QUEUE_POSITION_COMMAND& _command);
    void _ConnectRenderOcclusionCommand(const __RENDER_QUEUE_POSITION_COMMAND& _command);
    void _ConnectRenderGetNumTrianglesCommand(const __RENDER_GET_NUM_TRIANGLES_COMMAND& _command);
    void _ConnectRenderBindCommand(const __RENDER_BIND_COMMAND& _command);
    void _ConnectRenderDrawCommand(const __RENDER_DRAW_COMMAND& _command);
    void _ConnectRenderUnbindCommand(const __RENDER_UNBIND_COMMAND& _command);
    void _ConnectRenderDebugDrawCommand(const __RENDER_DEBUG_DRAW_COMMAND& _command);
    void _ConnectRenderBatchCommand(const __RENDER_BATCH_COMMAND& _command);
    
    i32 _ExecuteRenderQueuePositionCommand(void);
    bool _ExecuteRenderOcclusionCommand(void);
    ui32 _ExecuteRenderGetNumTrianglesCommand(void);
    void _ExecuteRenderBindCommand(const std::string& _mode);
    void _ExecuteRenderDrawCommand(const std::string& _mode);
    void _ExecuteRenderUnbindCommand(const std::string& _mode);
    void _ExecuteRenderDebugDrawCommand(const std::string& _mode);
    void _ExecuteRenderBatchCommand(const std::string& _mode);
    
public:
    
    ~CRenderCommands(void);
};

class IRenderHandler
{
private:
    
    friend class CRenderOperationWorldSpace;
    friend class CRenderOperationScreenSpace;
    friend class CRenderOperationOutput;
    
    CRenderCommands m_commands;
    
    void _ConnectCommands(void);
    
    inline CRenderCommands _Get_Commands(void)
    {
        return m_commands;
    };
    
protected:
    
    IRenderHandler(void);
    
    virtual i32 _OnQueuePosition(void) = 0;
    virtual bool _OnOcclusion(void) = 0;
    virtual ui32 _OnGet_NumTriangles(void) = 0;
    virtual void _OnBind(const std::string& _mode) = 0;
    virtual void _OnDraw(const std::string& _mode) = 0;
    virtual void _OnUnbind(const std::string& _mode) = 0;
    virtual void _OnDebugDraw(const std::string& _mode) = 0;
    virtual void _OnBatch(const std::string& _mode) = 0;
    
public:
    
    virtual ~IRenderHandler(void);
};


#endif 
