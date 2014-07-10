//
//  CMEUIToSceneCommands.cpp
//  ie3D-EMap
//
//  Created by sergey.sergeev on 7/8/14.
//
//

#include "CMEUIToSceneCommands.h"

CMEUIToSceneCommands::CMEUIToSceneCommands(void) :
m_setBrushSizeCommand(nullptr),
m_setBrushStrengthCommnand(nullptr),
m_setFalloffCoefficientCommand(nullptr),
m_setSmoothCoefficientCommand(nullptr)
{
    
}

CMEUIToSceneCommands::~CMEUIToSceneCommands(void)
{
    
}

void CMEUIToSceneCommands::connectSetBrushSizeCommand(const __SET_BRUSH_SIZE_COMMAND& command)
{
    m_setBrushSizeCommand = command;
}

void CMEUIToSceneCommands::executeSetBrushSizeCommand(ui32 value)
{
    if(m_setBrushSizeCommand != nullptr)
    {
        m_setBrushSizeCommand(value);
    }
}

void CMEUIToSceneCommands::connectSetBrushStrengthCommand(const __SET_BRUSH_STRENGTH_COMMAND& command)
{
    m_setBrushStrengthCommnand = command;
}

void CMEUIToSceneCommands::executeSetBrushStrengthCommand(ui32 value)
{
    if(m_setBrushStrengthCommnand != nullptr)
    {
        m_setBrushStrengthCommnand(value);
    }
}

void CMEUIToSceneCommands::connectSetFalloffCoefficientCommand(const __SET_FALLOFF_COEFFICIENT_COMMAND& command)
{
    m_setFalloffCoefficientCommand = command;
}

void CMEUIToSceneCommands::executeSetFalloffCoefficientCommand(ui32 value)
{
    if(m_setFalloffCoefficientCommand != nullptr)
    {
        m_setFalloffCoefficientCommand(value);
    }
}

void CMEUIToSceneCommands::connectSetSmoothCoefficientCommand(const __SET_SMOOTH_COEFFICIENT_COMMAND& command)
{
    m_setSmoothCoefficientCommand = command;
}

void CMEUIToSceneCommands::executeSetSmoothCoefficientCommand(ui32 value)
{
    if(m_setSmoothCoefficientCommand != nullptr)
    {
        m_setSmoothCoefficientCommand(value);
    }
}

void CMEUIToSceneCommands::connectSetTextureSamplerCommand(const __SET_TEXTURE_SAMPLER& command)
{
    m_setTextureSampler = command;
}

void CMEUIToSceneCommands::executeSetTextureSampler(const std::string& filename, E_SHADER_SAMPLER sampler)
{
    if(m_setTextureSampler != nullptr)
    {
        m_setTextureSampler(filename, sampler);
    }
}
