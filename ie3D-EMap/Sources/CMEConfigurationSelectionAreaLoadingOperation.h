//
//  CMEConfigurationSelectionAreaLoadingOperation.h
//  ie3D-Demo
//
//  Created by Sergey Sergeev on 6/7/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#ifndef CMEConfigurationSelectionAreaLoadingOperation_h
#define CMEConfigurationSelectionAreaLoadingOperation_h

#include "IConfigurationLoadingOperation.h"

class CMEConfigurationSelectionAreaLoadingOperation : public IConfigurationLoadingOperation
{
private:
    
protected:
    
public:
    
    CMEConfigurationSelectionAreaLoadingOperation(void);
    ~CMEConfigurationSelectionAreaLoadingOperation(void);
    
    ISharedConfiguration serialize(const std::string& filename);
};

#endif
