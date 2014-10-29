//
//  CMEConfigurationAccessor.h
//  ie3D-Demo
//
//  Created by Sergey Sergeev on 6/7/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#ifndef CMEConfigurationAccessor_h
#define CMEConfigurationAccessor_h

#include "CEConfigurationAccessor.h"

class CMEConfigurationAccessor : public CEConfigurationAccessor
{
private:
    
protected:
    
public:
    
    CMEConfigurationAccessor(void);
    ~CMEConfigurationAccessor(void);
    
    void loadLandscapeBrushConfiguration(const std::string& filename,
                                         ISharedConfigurationLoadingHandlerRef handler);
    
    void loadModelBrushConfiguration(const std::string& filename,
                                     ISharedConfigurationLoadingHandlerRef handler);
    
};


#endif
