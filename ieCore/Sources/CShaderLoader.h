//
//  CShaderLoader.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/17/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CShaderLoader_h
#define CShaderLoader_h

#include "IResourceLoader.h"

class IResourceLoadingHandler;
class CShader;
class CShaderLoader final : public IResourceLoader
{
private:
    
protected:
    
public:
    
    CShaderLoader(void);
    ~CShaderLoader(void);
    
    std::shared_ptr<CShader> StartLoadOperation(const std::string& _vsFilename, const std::string& _fsFilename);
};

#endif
