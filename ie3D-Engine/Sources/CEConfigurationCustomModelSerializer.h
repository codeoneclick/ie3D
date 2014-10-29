//
//  CEConfigurationCustomModelSerializer.h
//  ie3D-Engine
//
//  Created by sergey.sergeev on 10/28/14.
//  Copyright (c) 2014 sergey.sergeev. All rights reserved.
//

#ifndef CEConfigurationCustomModelSerializer_h
#define CEConfigurationCustomModelSerializer_h

#include "IConfigurationSerializer.h"

class CEConfigurationCustomModelSerializer final : public IConfigurationSerializer
{
private:
    
protected:
    
public:
    
    CEConfigurationCustomModelSerializer(void);
    ~CEConfigurationCustomModelSerializer(void);
    
    ISharedConfiguration serialize(const std::string& filename);
    void deserialize(const std::string& filename,
                     ISharedConfigurationRef configuration);
};


#endif
