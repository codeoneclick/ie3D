//
//  IResourceLoadingOperation.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/17/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef IResourceLoadingOperation_h
#define IResourceLoadingOperation_h

#include "HCommon.h"
#include "HEnums.h"

class IResourceSerializer;
class IResourceCommiter;
class IResource;

class IResourceLoadingOperation
{
private:
    
protected:
    
    std::shared_ptr<IResourceSerializer> m_serializer;
    std::shared_ptr<IResourceCommiter> m_commiter;
    
    std::string m_guid;
    E_RESOURCE_LOADING_OPERATION_STATUS m_status;
    
public:
    
    IResourceLoadingOperation(const std::string& _guid);
    ~IResourceLoadingOperation(void);
    
    virtual std::shared_ptr<IResource> Start(void) = 0;
    
    inline std::string Get_Guid(void)
    {
        return m_guid;
    };
    
    inline E_RESOURCE_LOADING_OPERATION_STATUS Get_Status(void)
    {
        return m_status;
    };
};

#endif 
