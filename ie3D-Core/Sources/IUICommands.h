//
//  IUICommands.h
//  ie3D-Core
//
//  Created by sergey.sergeev on 2/12/15.
//  Copyright (c) 2015 Sergey Sergeev. All rights reserved.
//

#ifndef IUICommands_h
#define IUICommands_h

#include "HCommon.h"
#include "ICommand.h"
#include "HDeclaration.h"

class IUICommands
{
private:
    
protected:
    
    std::unordered_map<std::string, ISharedCommand> m_commands;
    
public:
    
    IUICommands(void) = default;
    ~IUICommands(void);
    
    void addCommand(const std::string& guid, ISharedCommandRef command);
    void removeCommand(const std::string& guid);
    
    template<class FUNCTION, typename... ARGS>
    void execute(const std::string& guid, ARGS... args)
    {
        auto command = m_commands.find(guid);
        if(command != m_commands.end())
        {
            std::shared_ptr<CCommand<FUNCTION>> function = std::static_pointer_cast<CCommand<FUNCTION>>(command->second);
            function->execute(args...);
        }
    };
};

#endif
