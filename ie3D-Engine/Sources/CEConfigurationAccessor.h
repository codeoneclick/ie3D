//
//  CEConfigurationAccessor.h
//  ie3D-Engine
//
//  Created by sergey.sergeev on 8/6/14.
//  Copyright (c) 2014 sergey.sergeev. All rights reserved.
//

#ifndef CEConfigurationAccessor_h
#define CEConfigurationAccessor_h

#include "CConfigurationAccessor.h"

class CEConfigurationAccessor : public CConfigurationAccessor
{
private:
    
protected:
    
public:
    
    CEConfigurationAccessor(void);
    ~CEConfigurationAccessor(void);
    
    void loadComplexModelConfiguration(const std::string& filename,
                                       ISharedConfigurationLoadingHandlerRef handler);
    
    void loadCustomModelConfiguration(const std::string& filename,
                                      ISharedConfigurationLoadingHandlerRef handler);
    
};

#endif
