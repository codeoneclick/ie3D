//
//  CMESceneToUICommands.cpp
//  ie3D-EMap
//
//  Created by sergey.sergeev on 7/8/14.
//
//

#include "CMESceneToUICommands.h"

CMESceneToUICommands::CMESceneToUICommands(void) :
m_setBrushSizeCommand(nullptr),
m_setBrushStrengthCommnand(nullptr),
m_setFalloffCoefficientCommand(nullptr),
m_setSmoothCoefficientCommand(nullptr),
m_setTextureSampler(nullptr),
m_setTillingTexcoordCommand(nullptr)
{
    
}

CMESceneToUICommands::~CMESceneToUICommands(void)
{
    
}

void CMESceneToUICommands::connectSetBrushSizeCommand(const __SET_BRUSH_SIZE_COMMAND& command)
{
    m_setBrushSizeCommand = command;
}

void CMESceneToUICommands::executeSetBrushSizeCommand(ui32 value)
{
    if(m_setBrushSizeCommand != nullptr)
    {
        m_setBrushSizeCommand(value);
    }
}

void CMESceneToUICommands::connectSetBrushStrengthCommand(const __SET_BRUSH_STRENGTH_COMMAND& command)
{
    m_setBrushStrengthCommnand = command;
}

void CMESceneToUICommands::executeSetBrushStrengthCommand(ui32 value)
{
    if(m_setBrushStrengthCommnand != nullptr)
    {
        m_setBrushStrengthCommnand(value);
    }
}

void CMESceneToUICommands::connectSetFalloffCoefficientCommand(const __SET_FALLOFF_COEFFICIENT_COMMAND& command)
{
    m_setFalloffCoefficientCommand = command;
}

void CMESceneToUICommands::executeSetFalloffCoefficientCommand(ui32 value)
{
    if(m_setFalloffCoefficientCommand != nullptr)
    {
        m_setFalloffCoefficientCommand(value);
    }
}

void CMESceneToUICommands::connectSetSmoothCoefficientCommand(const __SET_SMOOTH_COEFFICIENT_COMMAND& command)
{
    m_setSmoothCoefficientCommand = command;
}

void CMESceneToUICommands::executeSetSmoothCoefficientCommand(ui32 value)
{
    if(m_setSmoothCoefficientCommand != nullptr)
    {
        m_setSmoothCoefficientCommand(value);
    }
}

void CMESceneToUICommands::connectSetTextureSamplerCommand(const __SET_TEXTURE_SAMPLER& command)
{
    m_setTextureSampler = command;
}

void CMESceneToUICommands::executeSetTextureSampler(CSharedTextureRef texture, E_SHADER_SAMPLER sampler)
{
    if(m_setTextureSampler != nullptr)
    {
        m_setTextureSampler(texture, sampler);
    }
}

void CMESceneToUICommands::connectSetTillingTexcoordCommand(const __SET_TILLING_TEXCOORD& command)
{
    m_setTillingTexcoordCommand = command;
}

void CMESceneToUICommands::executeSetTillingTexcoordCommand(f32 value, E_SHADER_SAMPLER sampler)
{
    if(m_setTillingTexcoordCommand != nullptr)
    {
        m_setTillingTexcoordCommand(value, sampler);
    }
}
