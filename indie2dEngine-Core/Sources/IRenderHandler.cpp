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
m_renderBindCommand(nullptr),
m_renderDrawCommand(nullptr),
m_renderUnbindCommand(nullptr)
{

}

CRenderCommands::~CRenderCommands(void)
{
    m_renderQueuePositionCommand = nullptr;
    m_renderBindCommand = nullptr;
    m_renderDrawCommand = nullptr;
    m_renderUnbindCommand = nullptr;
}

void CRenderCommands::_ConnectRenderQueuePositionCommand(const __RENDER_QUEUE_POSITION_COMMAND &_command)
{
    assert(_command != nullptr);
    m_renderQueuePositionCommand = _command;
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

i32 CRenderCommands::_ExecuteRenderQueuePositionCommand(void)
{
    assert(m_renderQueuePositionCommand != nullptr);
    return m_renderQueuePositionCommand();
}

void CRenderCommands::_ExecuteRenderBindCommand(const std::string &_command)
{
    assert(m_renderBindCommand != nullptr);
    m_renderBindCommand(_command);
}

void CRenderCommands::_ExecuteRenderDrawCommand(const std::string &_command)
{
    assert(m_renderDrawCommand != nullptr);
    m_renderDrawCommand(_command);
}

void CRenderCommands::_ExecuteRenderUnbindCommand(const std::string &_command)
{
    assert(m_renderUnbindCommand != nullptr);
    m_renderUnbindCommand(_command);
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
    m_commands._ConnectRenderQueuePositionCommand(std::bind(&IRenderHandler::OnQueuePosition, this));
    m_commands._ConnectRenderBindCommand(std::bind(&IRenderHandler::OnBind, this, std::placeholders::_1));
    m_commands._ConnectRenderDrawCommand(std::bind(&IRenderHandler::OnDraw, this, std::placeholders::_1));
    m_commands._ConnectRenderUnbindCommand(std::bind(&IRenderHandler::OnUnbind, this, std::placeholders::_1));
}
