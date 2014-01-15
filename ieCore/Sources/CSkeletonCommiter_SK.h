//
//  CSkeletonCommiter_SK.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 8/13/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CSkeletonCommiter_SK_h
#define CSkeletonCommiter_SK_h

#include "IResourceCommiter.h"

class CSkeletonCommiter_SK final : public IResourceCommiter
{
private:
    
protected:
    
public:
    
    CSkeletonCommiter_SK(const std::string& _guid, std::shared_ptr<IResource> _resource);
    ~CSkeletonCommiter_SK(void);
    
    void Commit(void);
};

#endif 
