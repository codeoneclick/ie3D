//
//  CComponentConfigurationLoading.h
//  ie3D-Core
//
//  Created by sergey.sergeev on 1/22/15.
//  Copyright (c) 2015 Sergey Sergeev. All rights reserved.
//

#ifndef CComponentConfigurationLoading_h
#define CComponentConfigurationLoading_h

#include "IComponent.h"
#include "HDeclaration.h"

class CComponentConfigurationLoading : public IComponent
{
private:
    
protected:
    
public:
    
    CComponentConfigurationLoading(void);
    ~CComponentConfigurationLoading(void);
    
    void onConfigurationLoaded(ISharedConfigurationRef configuration, bool isSucces);
};

#endif
