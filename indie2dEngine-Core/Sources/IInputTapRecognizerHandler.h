//
//  IInputTapRecognizerHandler.h
//  iGaia-CartoonPanzers
//
//  Created by Sergey Sergeev on 3/27/13.
//
//

#ifndef IInputTapRecognizerHandler_h
#define IInputTapRecognizerHandler_h

#include "HCommon.h"

class CInputTapRecognizerCommands final
{

private:

    friend class IInputTapRecognizerHandler;
    friend class IInputContext;
    
    typedef std::function<void(const glm::ivec2&)> __TAP_RECOGNIZER_DID_PRESSED_COMMAND;
    typedef std::function<void(const glm::ivec2&)> __TAP_RECOGNIZER_DID_MOVED_COMMAND;
    typedef std::function<void(const glm::ivec2&)> __TAP_RECOGNIZER_DID_RELEASED_COMMAND;
    
    __TAP_RECOGNIZER_DID_PRESSED_COMMAND m_inputTapRecognizerDidPressedCommand;
    __TAP_RECOGNIZER_DID_MOVED_COMMAND m_inputTapRecognizerDidMovedCommand;
    __TAP_RECOGNIZER_DID_RELEASED_COMMAND m_inputTapRecognizerDidReleasedCommand;

protected:

    CInputTapRecognizerCommands(void);

    void _ConnectInputTapRecognizerDidPressedCommand(const __TAP_RECOGNIZER_DID_PRESSED_COMMAND& _command);
    void _ConnectInputTapRecognizerDidMovedCommand(const __TAP_RECOGNIZER_DID_MOVED_COMMAND& _command);
    void _ConnectInputTapRecognizerDidReleasedCommand(const __TAP_RECOGNIZER_DID_RELEASED_COMMAND& _command);
    
    void _ExecuteInputTapRecognizerDidPressedCommand(const glm::ivec2& _point);
    void _ExecuteInputTapRecognizerDidMovedCommand(const glm::ivec2& _point);
    void _ExecuteInputTapRecognizerDidReleasedCommand(const glm::ivec2& _point);
    
public:

    ~CInputTapRecognizerCommands(void);
};

class IInputTapRecognizerHandler
{
private:
    
    friend class CInputTapRecognizerCommands;
    friend class IInputContext;

    CInputTapRecognizerCommands m_commands;
    
    void _ConnectCommands(void);
    
    inline CInputTapRecognizerCommands _Get_Commands(void)
    {
        return m_commands;
    };

protected:

    IInputTapRecognizerHandler(void);

    virtual void _OnInputTapRecognizerPressed(const glm::ivec2& _point) = 0;
    virtual void _OnInputTapRecognizerMoved(const glm::ivec2& _point) = 0;
    virtual void _OnInputTapRecognizerReleased(const glm::ivec2& _point) = 0;

public:

    virtual ~IInputTapRecognizerHandler(void);
};


#endif 
