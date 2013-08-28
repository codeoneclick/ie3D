//
//  CTextureLoader.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/23/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CTextureLoader_h
#define CTextureLoader_h

#include "IResourceLoader.h"

class CTexture;
class IResourceLoadingHandler;
class CTextureLoader final : public IResourceLoader
{
private:
    
protected:
    
public:
    
    CTextureLoader(void);
    ~CTextureLoader(void);
    
    std::shared_ptr<CTexture> StartLoadOperation(const std::string& _filename);
};


#endif 
