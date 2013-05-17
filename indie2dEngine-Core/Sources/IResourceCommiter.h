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
#include "HEnums.h"

class IResourceCommiter  : public std::enable_shared_from_this<IResourceCommiter>
{
private:
    
protected:
    
    std::string m_guid;
    E_COMMITER_STATUS m_status;
    
public:
    
    IResourceCommiter(const std::string& _guid);
    virtual ~IResourceCommiter(void);
    
    virtual void Commit(void) = 0;
    
    inline std::string Get_Guid(void)
    {
        return m_guid;
    };
    
    inline E_COMMITER_STATUS Get_Status(void)
    {
        return m_status;
    };
};

#endif 
