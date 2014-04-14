//
//  ITemplateSerializer.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/29/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef ITemplateSerializer_h
#define ITemplateSerializer_h

#include "HCommon.h"
#include "HDeclaration.h"

class ITemplateSerializer
{
private:
    
protected:

    pugi::xml_parse_result openXMLDocument(pugi::xml_document& document, const std::string& filename);
    
public:
    
    ITemplateSerializer(void);
    virtual ~ITemplateSerializer(void);
    
    virtual CSharedTemplateCommon serialize(const std::string& filename) = 0;
    virtual void deserialize(const std::string& filename, CSharedTemplateCommonRef commonTemplate) = 0;
};

#endif
