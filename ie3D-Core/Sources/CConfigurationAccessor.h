//
//  CConfigurationAccessor.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/29/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CConfigurationAccessor_h
#define CConfigurationAccessor_h

#include "HCommon.h"
#include "HEnums.h"
#include "HDeclaration.h"

class CConfigurationAccessor
{
private:
    
protected:

public:
    
    CConfigurationAccessor(void);
    ~CConfigurationAccessor(void);
    
    void loadMaterialConfiguration(const std::string& filename,
                                   ISharedConfigurationLoadingHandlerRef handler);
    
    void loadModelConfiguration(const std::string& filename,
                                ISharedConfigurationLoadingHandlerRef handler);
    
    void loadOceanConfiguration(const std::string& filename,
                                ISharedConfigurationLoadingHandlerRef handler);
    
    void loadLandscapeConfiguration(const std::string& filename,
                                    ISharedConfigurationLoadingHandlerRef handler);
    
    void loadParticleEmitterConfiguration(const std::string& filename,
                                          ISharedConfigurationLoadingHandlerRef handler);
    
    void loadGameTransitionConfiguration(const std::string& filename,
                                         ISharedConfigurationLoadingHandlerRef handler);
    
    void loadSkyBoxConfiguration(const std::string& filename,
                                 ISharedConfigurationLoadingHandlerRef handler);
    
    void loadAtmosphericScatteringConfiguration(const std::string& filename,
                                                ISharedConfigurationLoadingHandlerRef handler);
};

#endif 
