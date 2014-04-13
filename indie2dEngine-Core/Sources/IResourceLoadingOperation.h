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
#include "HDeclaration.h"
#include "HEnums.h"

class IResourceLoadingOperation
{
private:
    
protected:
    
    ISharedResourceSerializer m_serializer;
    ISharedResourceCommiter m_commiter;
    ISharedResource m_resource;
    
    std::string m_guid;
    E_RESOURCE_LOADING_OPERATION_STATUS m_status;
    
    void onResourceLoaded(void);
    
public:
    
    IResourceLoadingOperation(const std::string& guid,
                              ISharedResourceRef resource);
    virtual ~IResourceLoadingOperation(void);
    
    virtual void serialize(void) = 0;
    virtual void commit(void) = 0;
    
    std::string getGuid(void) const;
    E_RESOURCE_LOADING_OPERATION_STATUS getStatus(void) const;
};

#endif 
