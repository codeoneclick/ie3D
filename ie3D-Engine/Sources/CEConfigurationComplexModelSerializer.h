//
//  CEConfigurationComplexModelSerializer.h
//  ie3D-Engine
//
//  Created by sergey.sergeev on 8/6/14.
//  Copyright (c) 2014 sergey.sergeev. All rights reserved.
//

#ifndef CEConfigurationComplexModelSerializer_h
#define CEConfigurationComplexModelSerializer_h

#include "IConfigurationSerializer.h"

class CEConfigurationComplexModelSerializer final : public IConfigurationSerializer
{
private:
    
protected:
    
public:
    
    CEConfigurationComplexModelSerializer(void);
    ~CEConfigurationComplexModelSerializer(void);
    
    ISharedConfiguration serialize(const std::string& filename);
    void deserialize(const std::string& filename,
                     ISharedConfigurationRef configuration);
};

#endif
