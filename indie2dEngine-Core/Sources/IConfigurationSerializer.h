//
//  IConfigurationSerializer.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/29/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef IConfigurationSerializer_h
#define IConfigurationSerializer_h

#include "HCommon.h"
#include "HDeclaration.h"

class IConfigurationSerializer
{
private:
    
protected:

    pugi::xml_parse_result openXMLDocument(pugi::xml_document& document, const std::string& filename);
    
public:
    
    IConfigurationSerializer(void);
    virtual ~IConfigurationSerializer(void);
    
    virtual ISharedConfiguration serialize(const std::string& filename) = 0;
    virtual void deserialize(const std::string& filename,
                             ISharedConfigurationRef configuration) = 0;
};

#endif
