//
//  CShaderLoader.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/17/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef __indie2dEngine__CShaderLoader__
#define __indie2dEngine__CShaderLoader__

#include "IResourceLoader.h"

class CShaderLoader final : public IResourceLoader
{
private:
    
protected:
    
public:
    
    CShaderLoader(void);
    ~CShaderLoader(void);
    
    std::shared_future<std::shared_ptr<IResource> > StartLoadOperation(const std::string& _vsFilename, const std::string& _fsFilename);
};

#endif
