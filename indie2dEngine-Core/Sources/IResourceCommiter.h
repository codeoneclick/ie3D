//
//  IResourceCommiter.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/16/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef IResourceCommiter_h
#define IResourceCommiter_h

#include "HCommon.h"
#include "HDeclaration.h"
#include "HEnums.h"

class IResourceCommiter  : public std::enable_shared_from_this<IResourceCommiter>
{
private:
    
protected:
    
    std::string m_guid;
    ISharedResource m_resource;
    E_COMMITER_STATUS m_status;
    
    void onResourceDataCommitStatusChanged(ISharedResourceDataRef resourceData, E_RESOURCE_DATA_STATUS status);
    
public:
    
    IResourceCommiter(const std::string& guid, ISharedResourceRef resource);
    virtual ~IResourceCommiter(void);
    
    std::string getGuid(void) const;
    E_COMMITER_STATUS getStatus(void) const;
    
    virtual void commit(void) = 0;
    
};

#endif 
