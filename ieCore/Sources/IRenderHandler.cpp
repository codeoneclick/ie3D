//
//  IRenderHandler.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/17/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "IRenderHandler.h"

CRenderCommands::CRenderCommands(void) :
m_renderQueuePositionCommand(nullptr),
m_renderOcclusionCommand(nullptr),
m_renderBindCommand(nullptr),
m_renderDrawCommand(nullptr),
m_renderUnbindCommand(nullptr),
m_renderDebugDrawCommand(nullptr),
m_renderGetNumTrianglesCommand(nullptr),
m_renderBatchCommand(nullptr)
{

}

CRenderCommands::~CRenderCommands(void)
{
    m_renderQueuePositionCommand = nullptr;
    m_renderOcclusionCommand = nullptr;
    m_renderBindCommand = nullptr;
    m_renderDrawCommand = nullptr;
    m_renderUnbindCommand = nullptr;
    m_renderDebugDrawCommand = nullptr;
    m_renderGetNumTrianglesCommand = nullptr;
    m_renderBatchCommand = nullptr;
}

void CRenderCommands::_ConnectRenderQueuePositionCommand(const __RENDER_QUEUE_POSITION_COMMAND &_command)
{
    assert(_command != nullptr);
    m_renderQueuePositionCommand = _command;
}

void CRenderCommands::_ConnectRenderOcclusionCommand(const __RENDER_QUEUE_POSITION_COMMAND &_command)
{
    assert(_command != nullptr);
    m_renderOcclusionCommand = _command;
}

void CRenderCommands::_ConnectRenderGetNumTrianglesCommand(const __RENDER_GET_NUM_TRIANGLES_COMMAND &_command)
{
    assert(_command != nullptr);
    m_renderGetNumTrianglesCommand = _command;
}

void CRenderCommands::_ConnectRenderBindCommand(const __RENDER_BIND_COMMAND &_command)
{
    assert(_command != nullptr);
    m_renderBindCommand = _command;
}

void CRenderCommands::_ConnectRenderDrawCommand(const __RENDER_DRAW_COMMAND &_command)
{
    assert(_command != nullptr);
    m_renderDrawCommand = _command;
}

void CRenderCommands::_ConnectRenderUnbindCommand(const __RENDER_UNBIND_COMMAND &_command)
{
    assert(_command != nullptr);
    m_renderUnbindCommand = _command;
}

void CRenderCommands::_ConnectRenderDebugDrawCommand(const __RENDER_DEBUG_DRAW_COMMAND &_command)
{
    assert(_command != nullptr);
    m_renderDebugDrawCommand = _command;
}

void CRenderCommands::_ConnectRenderBatchCommand(const __RENDER_BATCH_COMMAND &_command)
{
    assert(_command != nullptr);
    m_renderBatchCommand = _command;
}

i32 CRenderCommands::_ExecuteRenderQueuePositionCommand(void)
{
    assert(m_renderQueuePositionCommand != nullptr);
    return m_renderQueuePositionCommand();
}

bool CRenderCommands::_ExecuteRenderOcclusionCommand(void)
{
    assert(m_renderOcclusionCommand != nullptr);
    return m_renderOcclusionCommand();
}

ui32 CRenderCommands::_ExecuteRenderGetNumTrianglesCommand(void)
{
    assert(m_renderGetNumTrianglesCommand != nullptr);
    return m_renderGetNumTrianglesCommand();
}

void CRenderCommands::_ExecuteRenderBindCommand(const std::string &_mode)
{
    assert(m_renderBindCommand != nullptr);
    m_renderBindCommand(_mode);
}

void CRenderCommands::_ExecuteRenderDrawCommand(const std::string &_mode)
{
    assert(m_renderDrawCommand != nullptr);
    m_renderDrawCommand(_mode);
}

void CRenderCommands::_ExecuteRenderUnbindCommand(const std::string &_mode)
{
    assert(m_renderUnbindCommand != nullptr);
    m_renderUnbindCommand(_mode);
}

void CRenderCommands::_ExecuteRenderDebugDrawCommand(const std::string &_mode)
{
    assert(m_renderDebugDrawCommand != nullptr);
    m_renderDebugDrawCommand(_mode);
}

void CRenderCommands::_ExecuteRenderBatchCommand(const std::string &_mode)
{
    assert(m_renderBatchCommand != nullptr);
    m_renderBatchCommand(_mode);
}

IRenderHandler::IRenderHandler(void)
{
    IRenderHandler::_ConnectCommands();
}

IRenderHandler::~IRenderHandler(void)
{
    
}

void IRenderHandler::_ConnectCommands(void)
{
    m_commands._ConnectRenderQueuePositionCommand(std::bind(&IRenderHandler::_OnQueuePosition, this));
    m_commands._ConnectRenderOcclusionCommand(std::bind(&IRenderHandler::_OnOcclusion, this));
    m_commands._ConnectRenderGetNumTrianglesCommand(std::bind(&IRenderHandler::_OnGet_NumTriangles, this));
    m_commands._ConnectRenderBindCommand(std::bind(&IRenderHandler::_OnBind, this, std::placeholders::_1));
    m_commands._ConnectRenderDrawCommand(std::bind(&IRenderHandler::_OnDraw, this, std::placeholders::_1));
    m_commands._ConnectRenderUnbindCommand(std::bind(&IRenderHandler::_OnUnbind, this, std::placeholders::_1));
    m_commands._ConnectRenderDebugDrawCommand(std::bind(&IRenderHandler::_OnDebugDraw, this, std::placeholders::_1));
    m_commands._ConnectRenderBatchCommand(std::bind(&IRenderHandler::_OnBatch, this, std::placeholders::_1));
}
