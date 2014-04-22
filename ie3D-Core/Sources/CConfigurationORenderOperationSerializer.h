//
//  CConfigurationORenderOperationSerializer.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 6/7/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CConfigurationORenderOperationSerializer_h
#define CConfigurationORenderOperationSerializer_h

#include "IConfigurationSerializer.h"

class CConfigurationORenderOperationSerializer  final : public IConfigurationSerializer
{
private:
    
protected:
    
public:
    
    CConfigurationORenderOperationSerializer(void);
    ~CConfigurationORenderOperationSerializer(void);
    
    ISharedConfiguration serialize(const std::string& filename);
    void deserialize(const std::string& filename,
                     ISharedConfigurationRef configuration);
};

#endif

