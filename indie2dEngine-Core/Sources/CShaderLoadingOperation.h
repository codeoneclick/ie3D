//
//  CShaderLoadingOperation.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/17/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CShaderLoadingOperation_h
#define CShaderLoadingOperation_h

#include "HCommon.h"
#include "IResourceLoadingOperation.h"

class IResource;

class CShaderLoadingOperation final : public IResourceLoadingOperation
{
private:
    
protected:
    
    std::string m_vsFilename;
    std::string m_fsFilename;
    
public:
    
    CShaderLoadingOperation(const std::string& _vsFilename, const std::string& _fsFilename, std::shared_ptr<IResource> _resource);
    ~CShaderLoadingOperation(void);
    
    void Serialize(void);
    void Commit(void);
    
};

#endif 
