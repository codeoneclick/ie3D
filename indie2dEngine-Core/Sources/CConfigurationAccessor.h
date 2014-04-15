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
    
    void LoadModelConfiguration(const std::string& filename,
                                ISharedConfigurationLoadingHandlerRef handler);
    
    void LoadOceanConfiguration(const std::string& filename,
                                ISharedConfigurationLoadingHandlerRef handler);
    
    void LoadLandscapeConfiguration(const std::string& filename,
                                    ISharedConfigurationLoadingHandlerRef handler);
    
    void LoadParticleEmitterConfiguration(const std::string& filename,
                                          ISharedConfigurationLoadingHandlerRef handler);
    
    void LoadGameTransitionConfiguration(const std::string& filename,
                                         ISharedConfigurationLoadingHandlerRef handler);
};

#endif 
