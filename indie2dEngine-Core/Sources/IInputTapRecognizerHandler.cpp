//
//  IInputTapRecognizerHandler.cpp
//  iGaia-CartoonPanzers
//
//  Created by Sergey Sergeev on 3/27/13.
//
//

#include "IInputTapRecognizerHandler.h"

CInputTapRecognizerCommands::CInputTapRecognizerCommands(void) :
m_inputTapRecognizerDidPressedCommand(nullptr),
m_inputTapRecognizerDidMovedCommand(nullptr),
m_inputTapRecognizerDidReleasedCommand(nullptr)
{
    
}

CInputTapRecognizerCommands::~CInputTapRecognizerCommands(void)
{
    m_inputTapRecognizerDidPressedCommand = nullptr;
    m_inputTapRecognizerDidMovedCommand = nullptr;
    m_inputTapRecognizerDidReleasedCommand = nullptr;
}

void CInputTapRecognizerCommands::_ConnectInputTapRecognizerDidPressedCommand(const __TAP_RECOGNIZER_DID_PRESSED_COMMAND &_command)
{
    assert(_command != nullptr);
    m_inputTapRecognizerDidPressedCommand = _command;
}

void CInputTapRecognizerCommands::_ConnectInputTapRecognizerDidMovedCommand(const __TAP_RECOGNIZER_DID_MOVED_COMMAND &_command)
{
    assert(_command != nullptr);
    m_inputTapRecognizerDidMovedCommand = _command;
}

void CInputTapRecognizerCommands::_ConnectInputTapRecognizerDidReleasedCommand(const __TAP_RECOGNIZER_DID_RELEASED_COMMAND &_command)
{
    assert(_command != nullptr);
    m_inputTapRecognizerDidReleasedCommand = _command;
}

void CInputTapRecognizerCommands::_ExecuteInputTapRecognizerDidPressedCommand(const glm::ivec2 &_point)
{
    assert(m_inputTapRecognizerDidPressedCommand != nullptr);
    m_inputTapRecognizerDidPressedCommand(_point);
}

void CInputTapRecognizerCommands::_ExecuteInputTapRecognizerDidMovedCommand(const glm::ivec2 &_point)
{
    assert(m_inputTapRecognizerDidMovedCommand != nullptr);
    m_inputTapRecognizerDidMovedCommand(_point);
}

void CInputTapRecognizerCommands::_ExecuteInputTapRecognizerDidReleasedCommand(const glm::ivec2 &_point)
{
    assert(m_inputTapRecognizerDidReleasedCommand != nullptr);
    m_inputTapRecognizerDidReleasedCommand(_point);
}

IInputTapRecognizerHandler::IInputTapRecognizerHandler(void)
{
    IInputTapRecognizerHandler::_ConnectCommands();
}

IInputTapRecognizerHandler::~IInputTapRecognizerHandler(void)
{
    
}

void IInputTapRecognizerHandler::_ConnectCommands(void)
{
    m_commands._ConnectInputTapRecognizerDidPressedCommand(std::bind(&IInputTapRecognizerHandler::_OnInputTapRecognizerPressed, this, std::placeholders::_1));
    m_commands._ConnectInputTapRecognizerDidMovedCommand(std::bind(&IInputTapRecognizerHandler::_OnInputTapRecognizerMoved, this, std::placeholders::_1));
    m_commands._ConnectInputTapRecognizerDidReleasedCommand(std::bind(&IInputTapRecognizerHandler::_OnInputTapRecognizerReleased, this, std::placeholders::_1));
}


