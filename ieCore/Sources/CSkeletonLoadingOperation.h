//
//  CSkeletonLoadingOperation.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 8/13/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CSkeletonLoadingOperation_h
#define CSkeletonLoadingOperation_h

#include "HCommon.h"
#include "IResourceLoadingOperation.h"

class IResource;

class CSkeletonLoadingOperation final : public IResourceLoadingOperation
{
private:
    
protected:
    
    std::string m_filename;
    
public:
    
    CSkeletonLoadingOperation(const std::string& _filename, std::shared_ptr<IResource> _resource);
    ~CSkeletonLoadingOperation(void);
    
    void Serialize(void);
    void Commit(void);
    
};

#endif
