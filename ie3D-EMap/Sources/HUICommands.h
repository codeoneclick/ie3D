//
//  HUICommands.h
//  ie3D-EMap
//
//  Created by Sergey Sergeev on 2/12/15.
//
//

#ifndef HUICommands_h
#define HUICommands_h

#include "HCommon.h"
#include "HDeclaration.h"

namespace UICommandGOECreateConfiguration
{
    typedef std::function<void(const std::string&)> COMMAND;
    const std::string GUID = "command_01";
};

namespace UICommandGOEUpdateConfigurationsMaterials
{
    typedef std::function<void(std::vector<CSharedConfigurationMaterial>&)> COMMAND;
    const std::string GUID = "command_02";
}

#endif
