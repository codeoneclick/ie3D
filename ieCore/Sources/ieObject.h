//
//  ieObject.h
//  ieCore
//
//  Created by sergey.sergeev on 4/7/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#ifndef __ieCore__ieObject__
#define __ieCore__ieObject__

#include "HCommon.h"
#include "ieDeclaration.h"

class ieObject : public std::enable_shared_from_this<ieObject>
{
private:
    
protected:
    
    std::string m_description;
    std::unordered_map<std::string, ieSharedObject> m_objects;
    std::unordered_map<std::string, ieSharedValue> m_values;
    
public:
    
    std::string getDescription(void) const;
    
    ieObject(void);
    virtual ~ieObject(void);
    
    void addObject(ieSharedObjectRef object, const std::string& key);
    void removeObject(const std::string& key);
    
    void setObject(ieSharedObjectRef object, const std::string& key);
    ieSharedObject getObject(const std::string& key) const;
    
    void addValue(ieSharedValueRef value, const std::string& key);
    void removeValue(const std::string& key);
    
    void setValue(ieSharedValueRef value, const std::string& key);
    ieSharedValue getValue(const std::string& key) const;
};


#endif
