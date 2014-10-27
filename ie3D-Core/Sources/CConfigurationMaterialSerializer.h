//
//  CConfigurationMaterialSerializer.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/29/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CConfigurationMaterialSerializer_h
#define CConfigurationMaterialSerializer_h

#include "IConfigurationSerializer.h"

class CConfigurationMaterialSerializer final : public IConfigurationSerializer
{
private:
    
protected:
    
public:
    
    CConfigurationMaterialSerializer(void);
    ~CConfigurationMaterialSerializer(void);
    
    ISharedConfiguration serialize(const std::string& filename);
    void deserialize(const std::string& filename,
                     ISharedConfigurationRef configuration);
    
    static void serializeGameObjectMaterialsNode(CSharedConfigurationGameObjectRef gameObjectConfiguration,
                                                 const pugi::xml_node& mainNode);
};

#endif
