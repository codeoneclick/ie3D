//
//  CMEUIToSceneCommands.h
//  ie3D-EMap
//
//  Created by sergey.sergeev on 7/8/14.
//
//

#ifndef CMEUIToSceneCommands_h
#define CMEUIToSceneCommands_h

#include "HCommon.h"
#include "HEnums.h"

class CMEUIToSceneCommands
{
private:
    
    typedef std::function<void(ui32)> __SET_BRUSH_SIZE_COMMAND;
    typedef std::function<void(ui32)> __SET_BRUSH_STRENGTH_COMMAND;
    typedef std::function<void(ui32)> __SET_FALLOFF_COEFFICIENT_COMMAND;
    typedef std::function<void(ui32)> __SET_SMOOTH_COEFFICIENT_COMMAND;
    typedef std::function<void(const std::string&, E_SHADER_SAMPLER)> __SET_TEXTURE_SAMPLER;
    
    __SET_BRUSH_SIZE_COMMAND m_setBrushSizeCommand;
    __SET_BRUSH_STRENGTH_COMMAND m_setBrushStrengthCommnand;
    __SET_FALLOFF_COEFFICIENT_COMMAND m_setFalloffCoefficientCommand;
    __SET_SMOOTH_COEFFICIENT_COMMAND m_setSmoothCoefficientCommand;
    __SET_TEXTURE_SAMPLER m_setTextureSampler;
    
protected:
    
public:
    
    CMEUIToSceneCommands(void);
    ~CMEUIToSceneCommands(void);
    
    void connectSetBrushSizeCommand(const __SET_BRUSH_SIZE_COMMAND& command);
    void executeSetBrushSizeCommand(ui32 value);
    
    void connectSetBrushStrengthCommand(const __SET_BRUSH_STRENGTH_COMMAND& command);
    void executeSetBrushStrengthCommand(ui32 value);
    
    void connectSetFalloffCoefficientCommand(const __SET_FALLOFF_COEFFICIENT_COMMAND& command);
    void executeSetFalloffCoefficientCommand(ui32 value);
    
    void connectSetSmoothCoefficientCommand(const __SET_SMOOTH_COEFFICIENT_COMMAND& command);
    void executeSetSmoothCoefficientCommand(ui32 value);
    
    void connectSetTextureSamplerCommand(const __SET_TEXTURE_SAMPLER& command);
    void executeSetTextureSampler(const std::string& filename, E_SHADER_SAMPLER sampler);
};

#endif
