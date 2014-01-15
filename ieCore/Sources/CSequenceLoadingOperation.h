//
//  CSequenceLoadingOperation.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 8/13/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CSequenceLoadingOperation_h
#define CSequenceLoadingOperation_h

#include "HCommon.h"
#include "IResourceLoadingOperation.h"

class IResource;

class CSequenceLoadingOperation final : public IResourceLoadingOperation
{
private:
    
protected:
    
    std::string m_filename;
    
public:
    
    CSequenceLoadingOperation(const std::string& _filename, std::shared_ptr<IResource> _resource);
    ~CSequenceLoadingOperation(void);
    
    void Serialize(void);
    void Commit(void);
    
};

#endif
