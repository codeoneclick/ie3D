//
//  IResourceSerializer.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/16/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef IResourceSerializer_h
#define IResourceSerializer_h

#include "HCommon.h"
#include "HEnums.h"

class IResource;
class IResourceSerializer : public std::enable_shared_from_this<IResourceSerializer>
{
private:
    
protected:
    
    std::string m_guid;
    std::shared_ptr<IResource> m_resource;
    E_SERIALIZER_STATUS m_status;
    
    std::istream* _LoadData(const std::string &_filename);
    void _FreeData(std::istream* _stream);
    
public:
    
    IResourceSerializer(const std::string& _guid, std::shared_ptr<IResource> _resource);
    virtual ~IResourceSerializer(void);
    
    virtual void Serialize(void) = 0;
    
    inline std::string Get_Guid(void)
    {
        return m_guid;
    };
    
    inline E_SERIALIZER_STATUS Get_Status(void)
    {
        return m_status;
    };
};

#endif 
