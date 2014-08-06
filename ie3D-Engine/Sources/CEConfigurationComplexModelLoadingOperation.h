//
//  CEConfigurationComplexModelLoadingOperation.h
//  ie3D-Engine
//
//  Created by sergey.sergeev on 8/6/14.
//  Copyright (c) 2014 sergey.sergeev. All rights reserved.
//

#ifndef CEConfigurationComplexModelLoadingOperation_h
#define CEConfigurationComplexModelLoadingOperation_h

#include "IConfigurationLoadingOperation.h"

class CEConfigurationComplexModelLoadingOperation : public IConfigurationLoadingOperation
{
private:
    
protected:
    
public:
    
    CEConfigurationComplexModelLoadingOperation(void);
    ~CEConfigurationComplexModelLoadingOperation(void);
    
    ISharedConfiguration serialize(const std::string& filename);
};

#endif
