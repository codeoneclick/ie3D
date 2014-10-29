//
//  CEConfigurationCustomModelLoadingOperation.h
//  ie3D-Engine
//
//  Created by sergey.sergeev on 10/28/14.
//  Copyright (c) 2014 sergey.sergeev. All rights reserved.
//

#ifndef CEConfigurationCustomModelLoadingOperation_h
#define CEConfigurationCustomModelLoadingOperation_h

#include "IConfigurationLoadingOperation.h"

class CEConfigurationCustomModelLoadingOperation : public IConfigurationLoadingOperation
{
private:
    
protected:
    
public:
    
    CEConfigurationCustomModelLoadingOperation(void);
    ~CEConfigurationCustomModelLoadingOperation(void);
    
    ISharedConfiguration serialize(const std::string& filename);
};

#endif
