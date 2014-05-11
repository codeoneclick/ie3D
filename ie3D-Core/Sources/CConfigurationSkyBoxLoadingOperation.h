//
//  CConfigurationSkyBoxLoadingOperation.h
//  ie3D-Core
//
//  Created by Sergey Sergeev on 5/6/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#ifndef CConfigurationSkyBoxLoadingOperation_h
#define CConfigurationSkyBoxLoadingOperation_h

#include "IConfigurationLoadingOperation.h"

class CConfigurationSkyBoxLoadingOperation : public IConfigurationLoadingOperation
{
private:
    
protected:
    
public:
    
    CConfigurationSkyBoxLoadingOperation(void);
    ~CConfigurationSkyBoxLoadingOperation(void);
    
    ISharedConfiguration serialize(const std::string& filename);
};

#endif
