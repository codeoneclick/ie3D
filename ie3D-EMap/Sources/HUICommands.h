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
};

namespace UICommandGOEUpdateConfigurationMaterial
{
    typedef std::function<void(CSharedConfigurationMaterialRef)> COMMAND;
    const std::string GUID = "command_11";
};

namespace UICommandMSESetBrushSize
{
    typedef std::function<void(ui32)> COMMAND;
    const std::string GUID = "command_03";
};

namespace UICommandMSESetBrushStrength
{
    typedef std::function<void(ui32)> COMMAND;
    const std::string GUID = "command_04";
};

namespace UICommandMSESetFalloffCoefficient
{
    typedef std::function<void(ui32)> COMMAND;
    const std::string GUID = "command_05";
};

namespace UICommandMSESetSmoothCoefficient
{
    typedef std::function<void(ui32)> COMMAND;
    const std::string GUID = "command_06";
};

namespace UICommandMSESetTexture
{
    typedef std::function<void(CSharedTextureRef, E_SHADER_SAMPLER)> COMMAND;
    const std::string GUID = "command_07";
};

namespace UICommandMSESetTextureFilename
{
    typedef std::function<void(const std::string&, E_SHADER_SAMPLER)> COMMAND;
    const std::string GUID = "command_08";
};

namespace UICommandMSESetTillingTexcoord
{
    typedef std::function<void(f32, E_SHADER_SAMPLER)> COMMAND;
    const std::string GUID = "command_09";
};

namespace UICommandMSEGenerateHeightmap
{
    typedef std::function<void(const glm::ivec2& size, f32 frequency, i32 octaves, ui32 seed)> COMMAND;
    const std::string GUID = "command_10";
};

#endif
