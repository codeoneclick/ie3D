//
//  CMEConfigurationLandscapeBrushLoadingOperation.h
//  ie3D-Demo
//
//  Created by Sergey Sergeev on 6/7/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#ifndef CMEConfigurationLandscapeBrushLoadingOperation_h
#define CMEConfigurationLandscapeBrushLoadingOperation_h

#include "IConfigurationLoadingOperation.h"

class CMEConfigurationLandscapeBrushLoadingOperation : public IConfigurationLoadingOperation
{
private:
    
protected:
    
public:
    
    CMEConfigurationLandscapeBrushLoadingOperation(void);
    ~CMEConfigurationLandscapeBrushLoadingOperation(void);
    
    ISharedConfiguration serialize(const std::string& filename);
};

#endif
