//
//  CConfigurationMaterialLoadingOperation.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/29/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CConfigurationMaterialLoadingOperation_h
#define CConfigurationMaterialLoadingOperation_h

#include "IConfigurationLoadingOperation.h"

class CConfigurationMaterialLoadingOperation : public IConfigurationLoadingOperation
{
private:
    
protected:
    
public:
    
    CConfigurationMaterialLoadingOperation(void);
    ~CConfigurationMaterialLoadingOperation(void);
    
    ISharedConfiguration serialize(const std::string& filename);
    
    static void serializeGameObjectMaterialsConfigurations(CSharedConfigurationGameObjectRef gameObjectConfiguration);
};

#endif 
